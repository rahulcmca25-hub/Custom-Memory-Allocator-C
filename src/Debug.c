#include<stdio.h>
#include "../include/Allocator_internal.h"

// print heap for debugging purpose

void print_heap(){
    block_t *curr=head;

    while(curr){
        printf("Block: %p , Size: %zu , Free:%d\n",curr,curr->size,curr->free);
        curr=curr->next;
    }
}



void print_stats()
{
    printf("\n===== HEAP STATS =====\n");

    printf("Allocations       : %zu\n",
           total_allocations);

    printf("Frees             : %zu\n",
           total_frees);

    printf("Requested Bytes   : %zu\n",
           total_requested_bytes);

    printf("======================\n");
}

// Heap Integrity check 

void heap_check()
{
    block_t *curr=head;

    while(curr)
    {
        if(curr->magic!=MAGIC)
        {
            printf("Heap Corruption Found\n");
            return;
        }

        curr=curr->next;
    }

    printf("Heap Integrity OK\n");
}



// Memory Leak Detection

void detect_leaks(){
    block_t *curr=head;

    while(curr){

        if(!curr->free){
            printf(
                "Memory Leak: Block %p, Size %zu bytes\n",
                 curr,
                 curr->size
            );
        }
        curr=curr->next;

    }
}
