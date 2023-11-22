#include "main_funcs.h"


extern inline bool cargs_check_option(const char _arg_option)
{
    _cargs_argument* p_arg = _cargs_find_argument_option(_arg_option);
    if(p_arg)
        return p_arg->has_been_used_already;
    return false;
}

extern inline uint32_t cargs_get_data_count(const char _arg_option)
{
    _cargs_argument* p_arg = _cargs_find_argument_option(_arg_option);
    if(p_arg && p_arg->data_container)
        p_arg->data_container->actual_data_count;

    return 0;
}

extern inline char** cargs_get_data(const char _arg_option)
{
    _cargs_argument* p_arg = _cargs_find_argument_option(_arg_option);
    if(!(p_arg && p_arg->data_container && p_arg->data_container->data.byte_count)) return NULL;
    if(p_arg->data_container->data_relocation_buffer)
    {
        free(p_arg->data_container->data_relocation_buffer);
        p_arg->data_container->data_relocation_buffer = NULL;
    }

    _cargs_data_package pack;
    if(
        p_arg->data_container->data.byte_count == sizeof(_cargs_data_package)) //Just one package means no need to relocate
    {
        memcpy(&pack, p_arg->data_container->data.main_block, sizeof(_cargs_data_package));
        if(!pack.inline_data_offset)
            return (char**)(*(p_arg->data_container->data.main_block + 4)); /*4 bytes offset to skip the count property*/
    }

    //Extract data packages and allocate and copy all the pointers
    p_arg->data_container->data_relocation_buffer = (char**)malloc(p_arg->data_container->actual_data_count * sizeof(char*));
    char** write_point = p_arg->data_container->data_relocation_buffer;

    uint32_t confirmed_pointers = 0;
    for(size_t buf_offset = 0; buf_offset < p_arg->data_container->data.byte_count; buf_offset += sizeof(_cargs_data_package))
    {
        _stack_copy_cached_block(&pack, &(p_arg->data_container->data), buf_offset, sizeof(_cargs_data_package));
        if(pack.inline_data_offset) //Just one inlined data pointer
        {
            write_point[confirmed_pointers] = (pack.values[0] + pack.inline_data_offset);
            confirmed_pointers++;
            continue;
        }

        for(uint32_t i=0; i < pack.count; i++) //Maybe more than one data pointer
        {
            write_point[confirmed_pointers] = pack.values[i];
            confirmed_pointers++;
        }
    }

    return write_point;
}

extern inline uint32_t cargs_get_anonymous_arg_count()
{
    return _cargs_anon_arg_count;
}

extern inline char** cargs_get_anonymous_data()
{
    if(_cargs_anonymous_relocated_args)
    {
        free(_cargs_anonymous_relocated_args);
        _cargs_anonymous_relocated_args = NULL;
    }

    //Only one package means just passing the pointer
    if(_cargs_anonymous_args.byte_count == sizeof(_cargs_data_package))
    {
        return (char**)(*(_cargs_anonymous_args.main_block + 4)); /* +4 skips the count package attribute */
    }

    //Decompressing packages
    _cargs_data_package pack;
    uint32_t confirmed_pointers = 0;
    _cargs_anonymous_relocated_args = (char**)malloc(_cargs_anon_arg_count * sizeof(char*));
    for(size_t package_offset = 0; package_offset < _cargs_anonymous_args.byte_count; package_offset += sizeof(_cargs_data_package))
    {
        _stack_copy_cached_block(&pack, &(_cargs_anonymous_args), package_offset, sizeof(_cargs_data_package));
        for(uint32_t i=0; i < pack.count; i++)
        {
            _cargs_anonymous_relocated_args[confirmed_pointers] = pack.values[i];
            confirmed_pointers++;
        }
    }

    return _cargs_anonymous_relocated_args;
}