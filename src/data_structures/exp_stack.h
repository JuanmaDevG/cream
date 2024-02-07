/*
    EXPANDABLE STACK

    It is what the name says, a stack of memory that groups an expandable list of memory blocks for general use 
    This blocks grow or decrease automatically depending on the needs from memory of the library.

    This is made to reduce to the minimum the amout of allocations per function call if memory is needed to store 
    certain values.
*/
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#define MEM_BLOCK_SIZE 64

typedef struct _linked_mem_block {
    uint8_t block[MEM_BLOCK_SIZE];
    _linked_mem_block* next;
    _linked_mem_block* previous;
} _linked_mem_block;

typedef struct _expandable_stack {
    uint8_t main_block[MEM_BLOCK_SIZE]; //The first block to be created
    char* p_stack_top;                  //Pointer to the stack top to fill it with memory
    uint8_t block_bytes_left;           //Number of available bytes on the actual block
    size_t byte_count;                  //Bytes that have been written, independent of the number of blocks
    _linked_mem_block* first;
    _linked_mem_block* last;
} _expandable_stack;


/*
    Allocates an expandable stack object with the correct default values
*/
_expandable_stack* _stack_create_expandable();

/*
    Frees all the extra memory blocks and restarts the stack pointer and the byte count

    WARNING
    Does not free the stack object itself, just it's internal structure
*/
void _stack_free_expandable(_expandable_stack*);

/*
    Pushes an entire block of memory defined by 
*/
void _stack_push_block(_expandable_stack* dst, const void* client_block, size_t client_block_size);

/*
    As descriptive as it is, replaces a stack mem block with the given client block.

    Will do nothing and return false if the mem block to write + the stack offset is out of bounds.
*/
bool _stack_replace_block(void* client_block, _expandable_stack* exp_stack, const size_t in_stack_offset, size_t client_block_size);


/*
    Copies the data to a contiguous memory block.
    If the block size limit is zero, copies all the data.

    WARNING:
    If the block size to read + the offset to take are bigger than the byte count, nothing 
    will be read.
*/
bool _stack_copy_cached_block(void* dst, const _expandable_stack* src, const size_t in_stack_offset, size_t block_size_limit);

/*
    Tells if the client block is equal to the expandable stack memory.

    Client block does not need to allocate the same memory as available on the stack
*/
bool _stack_memcmp(const void* block, const _expandable_stack* exp_stack, const size_t in_stack_offset, size_t block_size);

/*
    True if both expandable stacks are equal in byte count and content
*/
bool _stack_memcmp_stack(const _expandable_stack* exp_stack1, const _expandable_stack* exp_stack2);