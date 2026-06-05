#include "../include/Allocator.h"
#include "../include/Allocator_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>




block_t *head=NULL;
block_t *tail=NULL;

// For statistics

size_t total_allocations = 0;
size_t total_frees = 0;
size_t total_requested_bytes = 0;


static block_t * request_space(size_t size){
    block_t * block;

    block = sbrk(size+sizeof(block_t));
    if(block==(void*)-1){
        return NULL;
    }

    block->size=size;
    block->free=false;

    block->prev=tail;
    block->next=NULL;

    block->magic=MAGIC;

    if(tail){
        tail->next=block;
    }

    tail=block;

    if(head==NULL){
        head=block;
    }

    return block;

}

// To find the free block

static block_t * find_free_block(size_t size){

    block_t* curr=head;

    while(curr!=NULL){
        if(curr->free && (curr->size)>=size){
            return curr;
        }
        curr=curr->next;
    }

    return NULL;

}

// splitting the block

static void split_block(block_t * block,size_t size){

    size_t remaining;
    remaining=block->size-size;

    if(remaining<sizeof(block_t)+8){
        return;
    }

    block_t* new_block;
    
    new_block=(block_t*)((char*)(block+1)+size);

    new_block->size=remaining-sizeof(block_t);

    new_block->free=true;
    new_block->magic=MAGIC;

    new_block->next=block->next;
    new_block->prev=block;

    if(block->next){
        block->next->prev=new_block;
    }

    block->next=new_block;

    if(tail==block){
        tail=new_block;
    }

    block->size=size;

}



// Implementing my own malloc function Here

void * malloc_clone(size_t size){

    if(size==0){
        return NULL;
    }

    size_t requested_size=size;

    // For alignment of size like 5,6,7-->8 and 9,12,14,15-->16 For faster memory access

    size=(size+7) & ~7;

    block_t* block;
    block=find_free_block(size);

    if(block){

        // split the block of required size if block is available
        split_block(block,size);

        block->free=false;

        total_allocations++;
        total_requested_bytes+=requested_size;

        return (block+1);
    }

    block=request_space(size);

    if(block==NULL){
        return NULL;
    }

    total_allocations++;
    total_requested_bytes+=requested_size;

    return (block+1);

}

// coalesce :  To merge the two empty free block

static void coalesce(block_t *block){

    while(block->next  && block->next->free && block->next->magic==MAGIC ){

        block_t *next=block->next;
        block->size+=sizeof(block_t)+next->size;

        block->next=next->next;

        if(next->next){
            next->next->prev=block;
        }

        if(tail==next){
            tail=block;
        }

    }
}



void free_clone(void *ptr){

    if(ptr==NULL) return;

    block_t *block;

    block=((block_t*)ptr)-1;

    // For magic number validation that is it valid block which is allocaated or reading garbage memory
    if(block->magic!=MAGIC){
        fprintf(stderr,
        "[ALLOCATOR ERROR] Invalid pointer or heap corruption\n");

        return;
    }

    if(block->free)
    {
        fprintf(stderr,
                "ERROR: Double free detected at %p\n",
                ptr);
        return;
    }

    block->free=true;

    total_frees++;

    // For mergeing next empty free block
    coalesce(block);

    // For mergeing next empty free block

    if(block->prev && block->prev->free){
        coalesce(block->prev);
    }

}


// calloc implementation

void* calloc_clone(size_t n,size_t size){

    if(size!=0 && n > SIZE_MAX/size){
        return NULL;
    }

    size_t total=n*size;

    void *ptr=malloc_clone(total);

    if(ptr==NULL){
        return NULL;
    }

    memset(ptr,0,total);  // memset(pr,value,bytes)

    return ptr;

}

void* realloc_clone(void *ptr,size_t size){

    if(ptr==NULL){
        return malloc_clone(size);
    }

    if(size==0){
        free_clone(ptr);
        return NULL;
    }

    size=((size+7) & ~7);


    block_t *block;

    block=((block_t *)ptr)-1;

    if(block->magic != MAGIC){

        fprintf(stderr,
            "[ALLOCATOR ERROR] Invalid pointer passed to realloc\n");
        return NULL;
        
    }
    
    if(block->size>=size){
        return ptr;
    }

    // else we have to allocate new block and copy the content to that new block

    void *new_ptr;

    new_ptr=malloc_clone(size);

    if(new_ptr==NULL) return NULL;

    memcpy(new_ptr,ptr,block->size);

    free_clone(ptr);

    return new_ptr;


}

