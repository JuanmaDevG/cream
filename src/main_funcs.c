#include "main_funcs.h"

inline bool cargs_check_bool_arg(const uint32_t __arg_index)
{
    return _cargs_get_bit(_cargs_bool_bit_vec, __arg_index);
}

inline bool cargs_check_data_arg(const uint32_t __arg_index)
{
    return _cargs_get_bit(_cargs_data_bit_vec, __arg_index);
}

inline uint32_t cargs_get_data(const uint32_t __arg_index, char** __data_dst)
{
    //return the argument count with the vector data package reallocated if redundant
}