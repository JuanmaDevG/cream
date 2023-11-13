#include "exp_stack.h"


/*
    Adds a new node and locates the pointer to the stack top and the remaining block bytes
*/
void add_node(_expandable_stack* _exp_stack)
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

/*
    Gets the memory bloc location based on the offset.
    If returns NULL, the offset is located in the first block (the main block).

    WARNING:
    Make sure that the offset is in byte count bounds, otherwise will also return NULL;
*/
inline _linked_mem_block* get_block(const _expandable_stack* _exp_stack, const size_t _offset)
{
    if(_offset >= _exp_stack->byte_count) return NULL;

    _linked_mem_block* block = NULL;
    if(_offset > MEM_BLOCK_SIZE)
    {
        block = _exp_stack->first;
        for(uint32_t block_id = (uint32_t)(_offset / MEM_BLOCK_SIZE); block_id > 1; block_id--)
            block = block->next;
    }
    return block;
}


// ---------------------------------------------------------------------------------------------------

_expandable_stack* _stack_create_expandable()
{
    _expandable_stack* target = (_expandable_stack*)malloc(sizeof(_expandable_stack));
    target->p_stack_top = target->main_block;
    target->first = NULL; target->last = NULL;
    target->byte_count = 0;
    target->block_bytes_left = MEM_BLOCK_SIZE;
}

void _stack_push_block(_expandable_stack* _exp_stack, const void* _mem_src, size_t _block_size)
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

void _stack_copy_cache(void* _mem_dst, const _expandable_stack* _exp_stack, const size_t _offset, size_t _size_limit)
{
    if( //Are bytes unreachable?
        _exp_stack->byte_count == 0 || 
        _offset >= _exp_stack->byte_count ||
        _offset + _size_limit > _exp_stack->byte_count
    ) return;

    _linked_mem_block* actual_block = get_block(_exp_stack, _offset);
    size_t 
        inblock_position = (_offset % MEM_BLOCK_SIZE), 
        confirmed_bytes;
    bool read_whole_mem = (!_size_limit);

    //First copy is made based on the first block to start reading and inblock_position
    confirmed_bytes = (read_whole_mem || _size_limit > MEM_BLOCK_SIZE - inblock_position ? MEM_BLOCK_SIZE - inblock_position : _size_limit);
    memcpy(
        _mem_dst, (actual_block ? actual_block->block : _exp_stack->main_block) + inblock_position, confirmed_bytes 
    );
    //If we're not reading the whole memory and no more memory to read, done
    if(!read_whole_mem)
    {
        _size_limit -= confirmed_bytes;
        if(_size_limit == 0) return;
    }
    
    //Locate the following memory block
    if(actual_block) actual_block = actual_block->next;
    else actual_block = _exp_stack->first;

    bool enough_mem_to_read = true;
    size_t block_to_read = 0;
    while((_size_limit > 0 || confirmed_bytes < _exp_stack->byte_count) && enough_mem_to_read)
    {
        if(_size_limit > MEM_BLOCK_SIZE)  //Size limit is defined
            block_to_read = MEM_BLOCK_SIZE;
        else if(read_whole_mem) //No size limit, so look to byte count
        {
            if(confirmed_bytes + MEM_BLOCK_SIZE > _exp_stack->byte_count)
                block_to_read = MEM_BLOCK_SIZE - (confirmed_bytes + MEM_BLOCK_SIZE - _exp_stack->byte_count);
            else block_to_read = MEM_BLOCK_SIZE;
        }
        else if(confirmed_bytes + _size_limit > _exp_stack->byte_count) //Not enough writable memory
        {
            block_to_read = _exp_stack->byte_count - confirmed_bytes;
            enough_mem_to_read = false;
        }
        else block_to_read = _size_limit;
        memcpy((uint8_t)_mem_dst + confirmed_bytes, actual_block->block, block_to_read);
        _size_limit -= block_to_read;
    }
}

void _stack_free_expandable(_expandable_stack* _exp_stack)
{
    _exp_stack->byte_count = 0; 
    _exp_stack->block_bytes_left = MEM_BLOCK_SIZE;
    _exp_stack->p_stack_top = _exp_stack->main_block;
    for(_linked_mem_block* block = _exp_stack->first; block; block = block->next)
    {
        _exp_stack->first = block->next;
        free(block);
        block = _exp_stack->first;
    }
    _exp_stack->first = NULL;
    _exp_stack->last = NULL;
}