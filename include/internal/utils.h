#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t position;
    _redundant_queue* next;
} _redundant_queue;

/*

*/
inline _redundant_queue* _create_redundant_queue();


/*
    Removes redundant option letters
*/
void remove_redundancies(
    const char* _non_reductible_args, const uint64_t* _non_reductible_args_count, 
    char* _reductible_args, uint64_t* _redutcible_arg_count
);