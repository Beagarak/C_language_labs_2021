#include <stdio.h>
#include <stdlib.h>

#include "mem_debug.h"

void test_1(void* heap){
    const char* description = "Normal successful memory allocation";
    printf("ТEST 1: %s\n", description);
    void* block = _malloc(25);
    debug_heap(stdout, heap);
    free(block);
    printf("ТEST 1: Finished\n");
}

void test_2(void* heap){
    const char* description = "Freeing one block from several allocated ones";
    printf("ТEST 2: %s\n", description);
    void* block1 = _malloc(56);
    void* block2 = _malloc(20);
    _free(block1);
    debug_heap(stdout, heap);
    _free(block2);
    printf("ТEST 2: Finished\n");
}

void test_3(void* heap){
    const char* description = "Freeing two blocks from several allocated ones";
    printf("ТEST 3: %s\n", description);
    void* block1 = _malloc(500);
    void* block2 = _malloc(600);
    void* block3 = _malloc(700);
    _free(block1);
    _free(block2);
    debug_heap(stdout, heap);
    printf("ТEST 3: Finished\n");
}

void test_4(void* heap){
    const char* description = "The memory has run out, the new memory region expands the old one.";
    printf("ТEST 4: %s\n", description);
    void* block = _malloc(14000);
    debug_heap(stdout, heap);
    _free(block);
    debug_heap(stdout, heap);
    printf("ТEST 4: Finished\n");
}

void test_5(void* heap){
    const char* description = "The memory has run out, the new region is allocated elsewhere.";
    printf("ТEST 5: %s\n", description);
    _malloc(1000000000);
    _malloc(100);
    debug_heap(stdout, heap);
    printf("ТEST 5: Finished\n");
}
