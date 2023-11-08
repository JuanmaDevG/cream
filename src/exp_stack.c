#include "exp_stack.h"


/*
    Adds a new node and locates the pointer to the stack top and the remaining block bytes
*/
static void add_node(_expandable_stack* _exp_stack)
{
    _linked_mem_block* block = (_linked_mem_block*)malloc(sizeof(_linked_mem_block));
    //Linking
    block->next = NULL;
    if(_exp_stack->last)
    {
        block->previous = _exp_stack->last;
        _exp_stack->last->next = block;
    }
    else //Empty
    {
        _exp_stack->first = block;
        block->previous = NULL;
    }
    _exp_stack->last = block;
    //Reloading buffer
    _exp_stack->p_stack_top = _exp_stack->last->block;
    _exp_stack->block_bytes_left = MEM_BLOCK_SIZE;
}

static void copy_from_node(void* mem_dst, _linked_mem_block* actual_block, uint32_t inblock_position, const size_t max_copy_size)
{

}

// ---------------------------------------------------------------------------------------------------

_expandable_stack* _create_expandable_stack()
{
    _expandable_stack* target = (_expandable_stack*)malloc(sizeof(_expandable_stack));
    target->p_stack_top = target->main_block;
    target->first = NULL; target->last = NULL;
    target->byte_count = 0;
    target->block_bytes_left = MEM_BLOCK_SIZE;
}

void _push_block(_expandable_stack* _exp_stack, const void* _mem_src, size_t _block_size)
{
    uint32_t mem_src_offset = 0;
    while(_block_size > 0)
    {
        if(_exp_stack->block_bytes_left == 0) add_node(_exp_stack);

        //Client block bigger, so copy as much memory as possible and wait for new blocks
        if(_block_size > _exp_stack->block_bytes_left)
        {
            memcpy(_exp_stack->p_stack_top, (uint8_t*)_mem_src + mem_src_offset, _exp_stack->block_bytes_left);
            _exp_stack->byte_count += _exp_stack->block_bytes_left;
            mem_src_offset += _exp_stack->block_bytes_left;

            _block_size -= _exp_stack->block_bytes_left;
            _exp_stack->block_bytes_left = 0;   //no remaining mem
        }
        //Client block smaller or equal, copy the whole client block
        else
        {
            memcpy(_exp_stack->p_stack_top, (uint8_t*)_mem_src + mem_src_offset, _block_size);
            _exp_stack->byte_count += _block_size;
            
            _exp_stack->block_bytes_left -= _block_size;
            _block_size = 0;    //nothing more to copy
        }
    }

    if(_exp_stack->block_bytes_left == 0) _exp_stack->p_stack_top = NULL; //No bytes left
}

void _copy_cache(void* _mem_dst, const _expandable_stack* _exp_stack, const size_t _offset, const size_t _size_limit)
{ 
    if(_offset > 0)
    {
        if(_offset >= _exp_stack->byte_count) return; //Offset out of bounds -> nothing to copy

        _linked_mem_block* actual_block = NULL;
        uint32_t inblock_position = _offset % MEM_BLOCK_SIZE;
        if(_offset >= MEM_BLOCK_SIZE);
        {
            for(uint32_t block_id = _offset / MEM_BLOCK_SIZE; block_id > 0 && actual_block; block_id--)
                actual_block = actual_block->next;
            if(!actual_block) return;
        }
    }

    //TODO: copy the actual memory iterating through blocks
}