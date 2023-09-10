#include "main_funcs.h"
#include <stdio.h> //MOD


inline bool __cargs_data_unready(const uint32_t __arg_index)
{
    return
        !_cargs_data_args 
        || __arg_index >= _cargs_data_args_count
        || _cargs_data_packs[__arg_index].count == 0;
}

void _cargs_update_data_buffer(const uint32_t _arg_index)
{
    if(!_cargs_redundant_opt_data[_arg_index].first_node) return;
    _cargs_set_bit(_cargs_is_data_relocated_bit_vec, _arg_index, true);

    //Redundant input data arguments involve rescaling the package and removing the linked list
    //This is to avoid the need of the user to manually free the the pointers offered by the library
    const uint32_t _cargs_old_count = _cargs_data_packs[_arg_index].count;
    //Empty brackets to dellocate stack memory
    {
        _cargs_anonymous_node* _cargs_ptr = _cargs_redundant_opt_data[_arg_index].first_node;
        while(_cargs_ptr)
        {
            _cargs_data_packs[_arg_index].count += _cargs_ptr->package.count;
            _cargs_ptr = _cargs_ptr->next;
        }

        char** _cargs_old_value_pointers = _cargs_data_packs[_arg_index].values;
        _cargs_data_packs[_arg_index].values = (char**)malloc(sizeof(char*) * _cargs_data_packs[_arg_index].count);
        memcpy(_cargs_data_packs[_arg_index].values, _cargs_old_value_pointers, sizeof(char*) * _cargs_old_count);
    }

    for(uint32_t copy_offset_point = _cargs_old_count; _cargs_redundant_opt_data[_arg_index].first_node;)
    {
        memcpy(
            _cargs_data_packs[_arg_index].values + copy_offset_point, 
            _cargs_redundant_opt_data[_arg_index].first_node->package.values,
            sizeof(char*) * _cargs_redundant_opt_data[_arg_index].first_node->package.count
        );
        copy_offset_point += _cargs_redundant_opt_data[_arg_index].first_node->package.count;
        _cargs_delete_list_head(&(_cargs_redundant_opt_data[_arg_index].first_node), &(_cargs_redundant_opt_data[_arg_index].last_node));
    }
}

//-------------------------------------------------------------

inline bool cargs_check_bool(const uint32_t __arg_index)
{
    return _cargs_get_bit(_cargs_bool_bit_vec, __arg_index);
}

inline bool cargs_check_data(const uint32_t __arg_index)
{
    return _cargs_get_bit(_cargs_data_bit_vec, __arg_index);
}

inline uint32_t cargs_get_data_count(const uint32_t __arg_index)
{
    if(__cargs_data_unready(__arg_index)) return 0;
    _cargs_update_data_buffer(__arg_index);
    return _cargs_data_packs[__arg_index].count;
}

inline char** cargs_get_data(const uint32_t __arg_index)
{
    if(__cargs_data_unready(__arg_index)) { return NULL; }
    _cargs_update_data_buffer(__arg_index);
    return _cargs_data_packs[__arg_index].values;
}