#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t position;
    _redundant_queue* next;
} _redundant_queue;


/*
    Removes redundant option letters
*/
void remove_redundancies(
    char* _bool_args, 
    uint64_t* _bool_args_count, 
    char* _data_args, 
    uint64_t* _data_args_count
);