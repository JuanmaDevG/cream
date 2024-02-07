#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/*
    Calculates the number of bytes that should occupy the given number of bits
*/
size_t _cream_get_byte_size(const uint64_t bit_count);

/*
    Returns true is the bit into the bit vector is set to 1
*/
bool _cream_get_bit(const uint8_t* bit_vec, size_t position);

/*
    Sets the desired bit position of the bit vector to true(1) or false(0)
*/
void _cream_set_bit(uint8_t* _bit_vector, const size_t _bit_position, const bool _value);