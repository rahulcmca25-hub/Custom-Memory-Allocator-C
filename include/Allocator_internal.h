#ifndef ALLOCATOR_INTERNAL_H
#define ALLOCATOR_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>

#define MAGIC 0xDEADBEEF

typedef struct block
{
    unsigned int magic;

    size_t size;
    bool free;

    struct block *next;
    struct block *prev;

} block_t;

extern block_t *head;
extern block_t *tail;

extern size_t total_allocations;
extern size_t total_frees;
extern size_t total_requested_bytes;

#endif