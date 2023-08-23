#pragma once

#include <stdint.h>
#include <stdbool.h>


/*
    Calculates the number of bytes that should occupy the given number of bits
*/
extern inline size_t _cargs_get_byte_size(const uint64_t bit_count);

/*
    Returns true is the bit into the bit vector is set to 1
*/
extern inline bool _cargs_get_bit(const uint8_t* bit_vec, size_t position);