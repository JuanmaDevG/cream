#include "arg_options.h"

extern inline bool _cream_find_argument_id(const char arg_id)
{
    return (arg_id >= _cream_arg_id_back_limit && arg_id <= _cream_arg_id_front_limit ? _cream_get_bit(_cream_arg_id_bit_vec, (size_t)arg_id) : false);
}

extern inline _cream_argument_option* _cream_get_arg_option(const char _character)
{
    if(_character < _cream_back_arg_opt_limit || _character > _cream_front_arg_opt_limit) return NULL;
    return _cream_declared_arg_options + (_character - _cream_back_arg_opt_limit);
}

_cream_argument_option* _cream_find_ext_arg_option(const char* ext_arg)
{
    //TODO: implement again using the trie
}