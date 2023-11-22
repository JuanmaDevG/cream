#include "init_funcs.h"


extern inline void cargs_set_identificator(const char new_id) { _arg_id = new_id; }

void cargs_set_args(const uint32_t bool_option_count, const char* bool_options, const uint32_t data_option_count, const char* data_options)
{
    cargs_clean();
    if(!(bool_options || data_options) || !(bool_option_count || data_option_count)) return;

    //TODO: Treat redundancies as errors
    _cargs_option_count == bool_option_count + data_option_count;
    size_t buf_length = 
        _cargs_option_count +                               //Where the argument options are written to check them
        (_cargs_option_count * sizeof(_cargs_argument)) +   //Where arguments options are stored and configured
        (data_option_count * sizeof(_cargs_option_data));   //Where data argument options store their associated data
    _cargs_declared_option_chars = calloc(1, buf_length);   //And the general buffer where everything is placed

    if(!_cargs_declared_option_chars)
    {
        _cargs_declare_error(CARGS_NOT_ENOUGH_MEMORY, NULL, false, NULL);
        return;
    }

    if(bool_option_count)
        memcpy(_cargs_declared_option_chars, bool_options, bool_option_count);
    if(data_option_count)
        memcpy(_cargs_declared_option_chars + bool_option_count, data_options, data_option_count);

    //Attach pointers
    _cargs_argument* p_arg = (_cargs_argument*)(_cargs_declared_option_chars + _cargs_option_count);
    for(uint32_t i=0; i < _cargs_option_count; i++)
    {
        _cargs_set_option_pointer(_cargs_declared_option_chars, p_arg);
        p_arg++;
    }
    //Associate data mem regions with the actual arguments
    _cargs_option_data* p_data = (_cargs_option_data*)((_cargs_argument*)(_cargs_declared_option_chars + _cargs_option_count) + _cargs_option_count);
    for(uint32_t i=0; i < data_option_count; i++)
    {
        p_arg = _cargs_find_argument_option(data_options[i]);
        if(p_arg)
        {
            p_arg->data_container = p_data;
            p_data++;
        }
    }
}

void cargs_clean()
{
    _stack_free_expandable(&_cargs_general_buffer);
    memset(_cargs_valid_arg_options, 0, ASCII_TABLE_SIZE);
    cargs_cancel_argument_loads();

    if(_cargs_declared_option_chars)
    {
        free(_cargs_declared_option_chars);     //Buffer start point
        _cargs_declared_option_chars = NULL;
        _cargs_option_count = 0;
        _cargs_declared_arg_options = NULL;
        _cargs_declared_options_data = NULL;
        _cargs_ext_arg_count = 0;
    }

    //Clean error system
    _stack_free_expandable(&_cargs_error_buffer);
    if(_cargs_out_error_msg)
    {
        free(_cargs_out_error_msg);
        _cargs_out_error_msg = NULL;
    }
}

void cargs_associate_extended(const char* arg_characters, ...) {
    if(!_cargs_declared_option_chars) return;
    va_list arg_l;
    va_start(arg_l, arg_characters);

    _cargs_argument* p_arg;
    for(uint32_t i=0; arg_characters[i] != '\0'; i++)
    {
        p_arg = _cargs_find_argument_option(arg_characters[i]);
        if(p_arg)
        {
            p_arg->extended_version = va_arg(arg_l, char*);
            _cargs_ext_arg_count++;
        }
    }

    va_end(arg_l);
}

void cargs_make_mandatory(const char* arg_characters)
{
    if(!_cargs_declared_option_chars) return;
    _cargs_argument* p_arg;
    for(uint32_t i=0; arg_characters[i] != '\0'; i++)
    {
        p_arg = _cargs_find_argument_option(arg_characters[i]);
        if(p_arg)
            p_arg->is_mandatory = true;
    }
}

extern inline void cargs_set_minimum_data(const char* data_arg_string, ...)
{
    if(!_cargs_declared_option_chars) return;
    va_list arg_limits;
    va_start(arg_limits, data_arg_string);
    _cargs_set_data_limit(data_arg_string, arg_limits, CARGS_DATA_LIMIT_MIN);
    va_end(arg_limits);
}

extern inline void cargs_set_maximum_data(const char* data_arg_string, ...)
{
    if(!_cargs_declared_option_chars) return;
    va_list arg_limits;
    va_start(arg_limits, data_arg_string);
    _cargs_set_data_limit(data_arg_string, arg_limits, CARGS_DATA_LIMIT_MAX);
    va_end(arg_limits);
}

extern inline void cargs_treat_anonymous_args_as_errors(const bool _value) { _cargs_treat_anonymous_args_as_errors = _value; }

extern inline void cargs_treat_repeated_options_as_errors(const char* _arg_options)
{
    if(!_cargs_declared_option_chars) return;
    _cargs_argument* p_arg;
    for(uint32_t i=0; _arg_options[i] != '\0'; i++)
    {
        p_arg = _cargs_find_argument_option(_arg_options[i]);
        if(p_arg)
            p_arg->cannot_be_repeated = true;
    }
}

extern inline void cargs_include_argument_zero(const bool _value) { _cargs_include_argument_zero = _value; }

extern inline void cargs_load_args(const int argc, const char** argv)
{
    if(!_cargs_declared_option_chars) return;

    int i = (_cargs_include_argument_zero ? 0 : 1);
    while(i < argc)
        i += _cargs_read_argument(argc - i, argv + i);
    _cargs_check_mandatory_arguments();
    _cargs_check_surpassed_data_bounds();
}

void cargs_cancel_argument_loads()
{
    if(!_cargs_declared_option_chars) return;

    //Reset usage flag and clean data pointers
    _cargs_argument* p_arg;
    for(uint32_t i=0; i < _cargs_option_count; i++)
    {
        p_arg = _cargs_find_argument_option(_cargs_declared_option_chars[i]);
        p_arg->has_been_used_already = false;
        if(p_arg->data_container)
        {
            p_arg->data_container->actual_data_count = 0;
            if(p_arg->data_container->data_relocation_buffer)
            {
                free(p_arg->data_container->data_relocation_buffer);
                p_arg->data_container->data_relocation_buffer = NULL;
            }
            _stack_free_expandable(&(p_arg->data_container->data));
        }
    }

    //Clean anonymous arguments
    if(!_cargs_anonymous_relocated_args)
    {
        free(_cargs_anonymous_relocated_args);
        _cargs_anonymous_relocated_args = NULL;
    }
    _stack_free_expandable(&_cargs_anonymous_args);
    _cargs_reset_error_system();
}

const char* cargs_get_errors()
{
    if(_cargs_error_buffer.byte_count == 0) return NULL;
    if(_cargs_out_error_msg)
    {
        free(_cargs_out_error_msg);
        _cargs_out_error_msg = NULL;
    }

    //Store generated strings into the general buffer
    cargs_error err;
    for(size_t confirmed_bytes = 0; confirmed_bytes < _cargs_error_buffer.byte_count; confirmed_bytes += sizeof(cargs_error))
    {
        _stack_copy_cached_block(&err, &_cargs_error_buffer, confirmed_bytes, sizeof(cargs_error));
        _stack_push_block(&_cargs_general_buffer, "ERROR: ", 6);
        if(err.err_type == CARGS_NOT_ENOUGH_MEMORY)
        {
            _stack_push_block(&_cargs_general_buffer, _cargs_error_strings[err.err_type], _cargs_error_lengths[err.err_type]);
            break;
        }

        _stack_push_block(&_cargs_general_buffer, "the argument ", 13);
        _stack_push_block(&_cargs_general_buffer, err.arg_option, (err.is_extended ? strlen(err.arg_option) : 1));
        _stack_push_block(&_cargs_general_buffer, " ", 1);
        _stack_push_block(&_cargs_general_buffer, _cargs_error_strings[err.err_type], _cargs_error_lengths[err.err_type]);
        _stack_push_block(&_cargs_general_buffer, "\n", 1);
    }

    _cargs_out_error_msg = malloc(_cargs_general_buffer.byte_count +1);
    _stack_copy_cached_block(_cargs_out_error_msg, &_cargs_general_buffer, 0, _cargs_general_buffer.byte_count);
    _cargs_out_error_msg[_cargs_general_buffer.byte_count] = '\0';
    _stack_free_expandable(&_cargs_general_buffer);

    return (const char*)_cargs_out_error_msg;
}