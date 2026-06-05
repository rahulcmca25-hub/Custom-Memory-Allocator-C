#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include<stddef.h>

// Allocation APIs

void* malloc_clone(size_t size);
void* calloc_clone(size_t n,size_t size);
void* realloc_clone(void *ptr,size_t size);
void  free_clone(void *ptr);

// Debug APIs

void print_heap(void);
void print_stats(void);
void heap_check(void);
void detect_leaks(void);

#endif