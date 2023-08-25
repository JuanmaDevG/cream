#include "bit_vec_ops.h"

inline size_t _cargs_get_byte_size(const size_t __bit_count)
{
    return (__bit_count >> 3) + ((__bit_count & 0b111) == 0 ? 0 : 1);
}

inline bool _cargs_get_bit(const uint8_t* _bit_vector, const size_t _position)
{
    return ((_bit_vector[_position >> 3]) >> (_position & 0b111)) & 1;
}

inline void _cargs_set_bit(uint8_t* _bit_vector, const size_t _position, const bool _value)
{
    size_t _actual_byte = _position >> 3;
    _bit_vector[_actual_byte] = (_value ? 
        _bit_vector[_actual_byte] | ((uint8_t)1 << (_position & 0b111))     //value is true -> set bit to 1
        : _bit_vector[_actual_byte] & ~((uint8_t)1 << (_position & 0b111))
    );
}