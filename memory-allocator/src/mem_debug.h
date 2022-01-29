#ifndef _MEM_DEBUG_H
#define _MEM_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include "mem_internals.h"
#include "mem.h"


void debug_struct_info( FILE* f, void const* addr );
void debug_heap( FILE* f,  void const* ptr );
void debug_block(struct block_header* b, const char* fmt, ... );
void debug(const char* fmt, ... );

#endif
