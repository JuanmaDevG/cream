#include "exp_stack.h"

enum _stack_process_block_mode {PROCESS_MODE_COPY, PROCESS_MODE_COMPARE, PROCESS_MODE_REPLACE, PROCESS_MODE_POP};

typedef struct {
    uint8_t confirmed_bytes;
    _linked_mem_block* next_block;
} _stack_block_info;


// ---------------------------------------------------------------------------------------------------
//                                  Internal linkage functions
// ---------------------------------------------------------------------------------------------------

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

/*
    Gets the memory bloc location based on the offset.
    It returns NULL, the offset is located in the first block (the main block).

    WARNING:
    Make sure that the offset is in byte count bounds, use are_bytes_unreachable() function
*/
static inline _linked_mem_block* get_block(const _expandable_stack* _exp_stack, const size_t _offset)
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

static inline bool are_bytes_unreachable(const _expandable_stack* _exp_stack, const size_t _offset, const size_t _block_size)
{
    if(
        _exp_stack->byte_count == 0 || 
        _offset >= _exp_stack->byte_count ||
        _offset + _block_size > _exp_stack->byte_count
    ) {
        return true;
    }
    return false;
}

/*
    Makes the process operation defined between the client block and the pointer to raw mem block.
    Requires the _mem_size to be precalculated to fit with the memory block, otherwise will crash.

    Returns false if something fails
*/
static inline bool process_block(void* _client_block, uint8_t* _p_raw_block, const size_t _mem_size, const uint8_t process_mode)
{
    switch(process_mode)
    {
    case PROCESS_MODE_COPY: //From stack to client
        memcpy(_client_block, _p_raw_block, _mem_size);
        break;
    case PROCESS_MODE_COMPARE:
        if(memcmp(_client_block, _p_raw_block, _mem_size) != 0) return false;
        break;
    case PROCESS_MODE_REPLACE: //Client replaces stack mem
        memcpy(_p_raw_block, _client_block, _mem_size);
        break;
    default:
        return false;
    }
    return true;
}

static inline uint8_t/*TODO: stack process finish state enum*/ process_blocks()
{
    //TODO
}

/*
    Returns the second block to process after processing the first block, 
    or returns NULL if there is no more memory to read.

    WARNING:
    Bytes must be reachable or will crash, first run are_bytes_unreachable() function
*/
static _stack_block_info process_first_block(void* _client_block, _expandable_stack* _exp_stack, size_t _stack_offset, size_t _client_block_size, const uint8_t _process_mode)
{
    _linked_mem_block* block = get_block(_exp_stack, _stack_offset);
    uint8_t* stack_mem_read_point = (block ? block->block : _exp_stack->main_block);
    if(block)
    {
        stack_mem_read_point = block->block;
        block = block->next;
    }
    else
    {
        stack_mem_read_point = _exp_stack->main_block;
        block = _exp_stack->first;
    }
    bool finished = false;

    //Inblock offset
    _stack_offset = (_stack_offset % MEM_BLOCK_SIZE);
    //Block size to read
    if(_client_block_size > MEM_BLOCK_SIZE - _stack_offset)
        _client_block_size = MEM_BLOCK_SIZE - _stack_offset;
    else finished = true;

    _stack_block_info info;
    if(
        !process_block(_client_block, stack_mem_read_point + _stack_offset, _client_block_size, _process_mode) || 
        finished
    ) {
        info.confirmed_bytes = 0;
        info.next_block = NULL;
    } 
    else
    {
        info.confirmed_bytes = (uint8_t)_client_block_size;
        info.next_block = block;
    }
    return info;
}

// ---------------------------------------------------------------------------------------------------
//                                  External linkage functions
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

bool _stack_replace_block(void* _client_block, _expandable_stack* _exp_stack, const size_t _offset, const size_t _block_size)
{
    if(are_bytes_unreachable(_exp_stack, _offset, _block_size)) return false;

    //TODO: make the generic function process_blocks() and remove all redundant code

    _linked_mem_block* block; size_t confirmed_bytes;
    {
        _stack_block_info info = process_first_block(_client_block, _exp_stack, _offset, _block_size, PROCESS_MODE_REPLACE);
        block = info.next_block;
        confirmed_bytes = info.confirmed_bytes;
    }
    if(!block) return true;

    //Do not continue here for the moment

    return true;
}

void _stack_copy_cached_block(void* _mem_dst, const _expandable_stack* _exp_stack, const size_t _offset, size_t _size_limit)
{
    if(!_mem_dst || are_bytes_unreachable(_exp_stack, _offset, _size_limit)) return;
    if(!_size_limit) 
        _size_limit = _exp_stack->byte_count;

    _linked_mem_block* actual_block; size_t confirmed_bytes;
    {
        _stack_block_info result = process_first_block(_mem_dst, _exp_stack, _offset, _size_limit, PROCESS_MODE_COPY);
        actual_block = result.next_block;
        confirmed_bytes = (size_t)result.confirmed_bytes;
    }
    if(!actual_block) return; //Finished

    _size_limit -= confirmed_bytes;
    size_t block_to_read = 0;
    while(_size_limit > 0)
    {
        if(_size_limit > MEM_BLOCK_SIZE)
            block_to_read = MEM_BLOCK_SIZE;
        else block_to_read = _size_limit;
        memcpy((uint8_t*)_mem_dst + confirmed_bytes, actual_block->block, block_to_read);
        _size_limit -= block_to_read;
    }
}

void _stack_free_expandable(_expandable_stack* _exp_stack)
{
    if(!_exp_stack) return;
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

bool _stack_memcmp(const void* _client_block, const _expandable_stack* _exp_stack, const size_t _offset, size_t _block_size)
{
    if(_exp_stack->byte_count == 0 && _block_size == 0) return true;
    if(are_bytes_unreachable(_exp_stack, _offset, _block_size)) return false;

    _linked_mem_block* block;
    size_t confirmed_bytes;
    {
        _stack_block_info info = process_first_block(_client_block, _exp_stack, _offset, _block_size, PROCESS_MODE_COMPARE);
        block = info.next_block;
        confirmed_bytes = (size_t)info.confirmed_bytes;
    }
    if(!confirmed_bytes) return false;


    while(block && confirmed_bytes < _block_size)
    {
        if(_block_size - confirmed_bytes > MEM_BLOCK_SIZE)
        {
            if(
                !process_block((uint8_t*)_client_block + confirmed_bytes, block->block, MEM_BLOCK_SIZE, PROCESS_MODE_COMPARE)
            ) return false;
            confirmed_bytes += MEM_BLOCK_SIZE;
        }
        else
        {
            if(
                !process_block((uint8_t*)_client_block + confirmed_bytes, block->block, _block_size - confirmed_bytes, PROCESS_MODE_COMPARE)
            ) return false;
            confirmed_bytes = _block_size; //Finished confirmed bytes
        }
        block = block->next;
    }

    return true;
}