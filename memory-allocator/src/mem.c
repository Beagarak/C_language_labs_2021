#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define _DEFAULT_SOURCE

#include "mem_internals.h"
#include "mem.h"
#include "util.h"

void debug_block (struct block_header* b, const char* fmt, ... );
void debug (const char* fmt, ... );

extern inline block_size size_from_capacity(block_capacity cap);
extern inline block_capacity capacity_from_size(block_size sz);

static bool block_is_big_enough (size_t query, struct block_header* block){
    return block->capacity.bytes >= query;
}

static size_t pages_count (size_t mem){
    return mem / getpagesize() + ((mem % getpagesize()) > 0);
}

static size_t round_pages (size_t mem){
    return getpagesize() * pages_count( mem );
}

static void block_init (void* restrict addr, block_size block_sz, void* restrict next){
    *((struct block_header*)addr) = (struct block_header) {
            .next = next,
            .capacity = capacity_from_size(block_sz),
            .is_free = true
    };
}

static size_t region_actual_size(size_t query){
    return size_max( round_pages( query ), REGION_MIN_SIZE );
}

extern inline bool region_is_invalid(const struct region* r);

static void* map_pages (void const* addr, size_t length, int additional_flags){
    return mmap((void*) addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | additional_flags , 0, 0);
}

/*  аллоцировать регион памяти и инициализировать его блоком */
static struct region alloc_region(void const * addr, size_t query) {
    query = region_actual_size(query);
    struct region allocated_region;
    void* next_address = map_pages(addr, query, MAP_FIXED);
    if (next_address == MAP_FAILED){
        next_address = map_pages(addr, query,0);
        allocated_region = (struct region){next_address, query, false};
    }
    else {allocated_region = (struct region){next_address, query, true};}

    block_init(next_address, (block_size){ query }, NULL);
    return allocated_region;
}

static void* block_after(struct block_header const* block);

void* heap_init( size_t initial ) {
    const struct region region = alloc_region( HEAP_START, initial );
    if ( region_is_invalid(&region) ) {return NULL;}
    return region.addr;
}

#define BLOCK_MIN_CAPACITY 24
/*  --- Разделение блоков (если найденный свободный блок слишком большой )--- */
static bool block_splittable (struct block_header* restrict block, size_t query){
    return block-> is_free && query + offsetof( struct block_header, contents ) + BLOCK_MIN_CAPACITY <= block->capacity.bytes;
}

static bool split_if_too_big(struct block_header* block, size_t query){
    if (block && block_splittable(block, query)){
        struct block_header* const  header = (struct block_header*)(block->contents + query);
        block_init((void*)header,(block_size) {.bytes = block->capacity.bytes - query},(void*)block->next);
        block->next = header;
        block->capacity = (block_capacity) {query};
        return true;
    }
    return false;
}


/*  --- Слияние соседних свободных блоков --- */
static void* block_after(struct block_header const* block){
    return  (void*) (block->contents + block->capacity.bytes);
}
static bool blocks_continuous(struct block_header const* fst, struct block_header const* snd){
    return (void*)snd == block_after(fst);
}

static bool mergeable(struct block_header const* restrict fst, struct block_header const* restrict snd){
    return fst->is_free && snd->is_free && blocks_continuous( fst, snd ) ;
}

static bool try_merge_with_next(struct block_header* block){
    struct block_header* next_block = block->next;
    if (next_block && mergeable(block, next_block)){
        block->capacity.bytes += offsetof(struct block_header, contents) + next_block->capacity.bytes;
        block->next = next_block->next;
        return true;
    }
    return false;
}

/*  --- ... ecли размера кучи хватает --- */
struct block_search_result{
    enum {BSR_FOUND_GOOD_BLOCK, BSR_REACHED_END_NOT_FOUND, BSR_CORRUPTED} type;
    struct block_header* block;
};

static struct block_search_result find_good_or_last(struct block_header* restrict block, size_t sz){
    struct block_header* current = block;
    struct block_search_result result = {BSR_FOUND_GOOD_BLOCK, NULL};
    if (!current) {
        result.type = BSR_CORRUPTED;
        return result;
    }

    while (current->next) {
        if (current->is_free && block_is_big_enough(sz, current)){
            result.type = BSR_FOUND_GOOD_BLOCK;
            result.block = current;
            return result;
        }
        if(!try_merge_with_next(current)) {current = current->next;}
    }

    if (current->is_free && block_is_big_enough(sz, current)){
        result.type = BSR_FOUND_GOOD_BLOCK;
        result.block = current;
        return result;
    }

    result.type = BSR_REACHED_END_NOT_FOUND;
    result.block = current;
    return result;
}



/*  Попробовать выделить память в куче начиная с блока `block` не пытаясь расширить кучу
 Можно переиспользовать как только кучу расширили. */
static struct block_search_result try_memalloc_existing( size_t query, struct block_header* block ) {
    struct block_search_result res = find_good_or_last(block, query);
    if (res.type == BSR_FOUND_GOOD_BLOCK) {
        split_if_too_big(res.block, query);
        res.block->is_free = false;
    }
    return  res;
}


static struct block_header* grow_heap(struct block_header* restrict last, size_t query){
    const struct region region = alloc_region(block_after(last), query);
    last->next = (struct block_header*) region.addr;
    try_merge_with_next(last);
    return region.addr;
}

/*  Реализует основную логику malloc и возвращает заголовок выделенного блока */

/* Перебираем блоки пока не находим "хороший". Хороший блок — такой, в который можно уместить n байт.
Если хорошего блока не нашлось, то см. второе приближение. Хороший блок может быть слишком большим,
тогда мы разделяем блок на две части. Адрес содержимого этого блока и вернёт malloc. */

static struct block_header* memalloc(size_t query, struct block_header* heap_start){
    query = size_max(BLOCK_MIN_CAPACITY, query);
    struct block_search_result res = try_memalloc_existing(query, heap_start);

    switch (res.type) {
        case BSR_FOUND_GOOD_BLOCK:
            return res.block;
        case BSR_REACHED_END_NOT_FOUND:
            while (res.type == BSR_REACHED_END_NOT_FOUND){
                grow_heap(res.block, query);
                res = try_memalloc_existing(query, res.block);
            }
            return res.block;
        default: return  NULL;
    }
}


void* _malloc (size_t query){
    struct block_header* const addr = memalloc( query, (struct block_header*) HEAP_START );
    if (addr) {return addr->contents;}
    else {return NULL;}
}

static struct block_header* block_get_header (void* contents){
    return (struct block_header*) (((uint8_t*)contents)-offsetof(struct block_header, contents));
}

/* При освобождении блока можно объединить его со всеми следующими за ним свободными блоками */
void _free (void* mem){
    if (!mem) return ;
    struct block_header* header = block_get_header( mem );
    header->is_free = true;
    while(header->next){
        if(!try_merge_with_next(header)){break;}
    }
}
