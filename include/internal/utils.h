#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t value;
    _cargs_queue* next;
} _cargs_queue;

/*
    Creates a new queue node
*/
inline _cargs_queue* _create_cargs_queue();

/*
    Pushes a node to the head of the queue
*/
inline void _push_node(_cargs_queue* _last_q_pos, const uint32_t elem);

/*
    Pops a node from the last position (first insertion)
*/
inline void _pop_node(_cargs_queue* q);

/*
    Returns 1 if empty, otherwise 0
*/
inline uint8_t _q_is_empty(_cargs_queue* q);


/*
    Removes redundant option letters
*/
void remove_redundancies(
    const char* _non_reductible_args, const uint64_t* _non_reductible_args_count, 
    char* _reductible_args, uint64_t* _redutcible_arg_count
);