#include "init_funcs.h"


extern inline void cream_set_identificator(const char new_id) { _arg_id = new_id; }

void cream_set_args(const uint32_t bool_option_count, const char* bool_options, const uint32_t data_option_count, const char* data_options)
{
    cream_clean();
    if(!(bool_options || data_options) || !(bool_option_count || data_option_count)) return;

    //TODO: Treat redundancies as errors
    _cream_option_count == bool_option_count + data_option_count;
    size_t buf_length = 
        _cream_option_count +                               //Where the argument options are written to check them
        (_cream_option_count * sizeof(_cream_argument)) +   //Where arguments options are stored and configured
        (data_option_count * sizeof(_cream_option_data));   //Where data argument options store their associated data
    _cream_declared_option_chars = calloc(1, buf_length);   //And the general buffer where everything is placed

    if(!_cream_declared_option_chars)
    {
        _cream_declare_error(CREAM_NOT_ENOUGH_MEMORY, NULL, false, NULL);
        return;
    }

    if(bool_option_count)
        memcpy(_cream_declared_option_chars, bool_options, bool_option_count);
    if(data_option_count)
        memcpy(_cream_declared_option_chars + bool_option_count, data_options, data_option_count);

    //Attach pointers
    _cream_argument* p_arg = (_cream_argument*)(_cream_declared_option_chars + _cream_option_count);
    for(uint32_t i=0; i < _cream_option_count; i++)
    {
        _cream_set_option_pointer(_cream_declared_option_chars, p_arg);
        p_arg++;
    }
    //Associate data mem regions with the actual arguments
    _cream_option_data* p_data = (_cream_option_data*)((_cream_argument*)(_cream_declared_option_chars + _cream_option_count) + _cream_option_count);
    for(uint32_t i=0; i < data_option_count; i++)
    {
        p_arg = _cream_find_argument_option(data_options[i]);
        if(p_arg)
        {
            p_arg->data_container = p_data;
            p_data++;
        }
    }
}

void cream_clean()
{
    _stack_free_expandable(&_cream_general_buffer);
    memset(_cream_valid_arg_options, 0, ASCII_TABLE_SIZE);
    cream_cancel_argument_loads();

    if(_cream_declared_option_chars)
    {
        free(_cream_declared_option_chars);     //Buffer start point
        _cream_declared_option_chars = NULL;
        _cream_option_count = 0;
        _cream_declared_arg_options = NULL;
        _cream_declared_options_data = NULL;
        _cream_ext_arg_count = 0;
    }

    //Clean error system
    _stack_free_expandable(&_cream_error_buffer);
    if(_cream_out_error_msg)
    {
        free(_cream_out_error_msg);
        _cream_out_error_msg = NULL;
    }
}

void cream_associate_extended(const char* arg_characters, ...) {
    if(!_cream_declared_option_chars) return;
    va_list arg_l;
    va_start(arg_l, arg_characters);

    _cream_argument* p_arg;
    for(uint32_t i=0; arg_characters[i] != '\0'; i++)
    {
        p_arg = _cream_find_argument_option(arg_characters[i]);
        if(p_arg)
        {
            p_arg->extended_version = va_arg(arg_l, char*);
            _cream_ext_arg_count++;
        }
    }

    va_end(arg_l);
}

void cream_make_mandatory(const char* arg_characters)
{
    if(!_cream_declared_option_chars) return;
    _cream_argument* p_arg;
    for(uint32_t i=0; arg_characters[i] != '\0'; i++)
    {
        p_arg = _cream_find_argument_option(arg_characters[i]);
        if(p_arg)
            p_arg->is_mandatory = true;
    }
}

extern inline void cream_set_minimum_data(const char* data_arg_string, ...)
{
    if(!_cream_declared_option_chars) return;
    va_list arg_limits;
    va_start(arg_limits, data_arg_string);
    _cream_set_data_limit(data_arg_string, arg_limits, CREAM_DATA_LIMIT_MIN);
    va_end(arg_limits);
}

extern inline void cream_set_maximum_data(const char* data_arg_string, ...)
{
    if(!_cream_declared_option_chars) return;
    va_list arg_limits;
    va_start(arg_limits, data_arg_string);
    _cream_set_data_limit(data_arg_string, arg_limits, CREAM_DATA_LIMIT_MAX);
    va_end(arg_limits);
}

extern inline void cream_treat_anonymous_args_as_errors(const bool _value) { _cream_treat_anonymous_args_as_errors = _value; }

extern inline void cream_treat_repeated_options_as_errors(const char* _arg_options)
{
    if(!_cream_declared_option_chars) return;
    _cream_argument* p_arg;
    for(uint32_t i=0; _arg_options[i] != '\0'; i++)
    {
        p_arg = _cream_find_argument_option(_arg_options[i]);
        if(p_arg)
            p_arg->cannot_be_repeated = true;
    }
}

extern inline void cream_include_argument_zero(const bool _value) { _cream_include_argument_zero = _value; }

extern inline void cream_load_args(const int argc, const char** argv)
{
    if(!_cream_declared_option_chars) return;

    int i = (_cream_include_argument_zero ? 0 : 1);
    while(i < argc)
        i += _cream_read_argument(argc - i, argv + i);
    _cream_check_mandatory_arguments();
    _cream_check_surpassed_data_bounds();
}

void cream_cancel_argument_loads()
{
    if(!_cream_declared_option_chars) return;

    //Reset usage flag and clean data pointers
    _cream_argument* p_arg;
    for(uint32_t i=0; i < _cream_option_count; i++)
    {
        p_arg = _cream_find_argument_option(_cream_declared_option_chars[i]);
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
    if(!_cream_anonymous_relocated_args)
    {
        free(_cream_anonymous_relocated_args);
        _cream_anonymous_relocated_args = NULL;
    }
    _stack_free_expandable(&_cream_anonymous_args);
    _cream_reset_error_system();
}

const char* cream_get_errors()
{
    if(_cream_error_buffer.byte_count == 0) return NULL;
    if(_cream_out_error_msg)
    {
        free(_cream_out_error_msg);
        _cream_out_error_msg = NULL;
    }

    //Store generated strings into the general buffer
    cream_error err;
    for(size_t confirmed_bytes = 0; confirmed_bytes < _cream_error_buffer.byte_count; confirmed_bytes += sizeof(cream_error))
    {
        _stack_copy_cached_block(&err, &_cream_error_buffer, confirmed_bytes, sizeof(cream_error));
        _stack_push_block(&_cream_general_buffer, "ERROR: ", 6);
        if(err.err_type == CREAM_NOT_ENOUGH_MEMORY)
        {
            _stack_push_block(&_cream_general_buffer, _cream_error_strings[err.err_type], _cream_error_lengths[err.err_type]);
            break;
        }

        _stack_push_block(&_cream_general_buffer, "the argument ", 13);
        _stack_push_block(&_cream_general_buffer, err.arg_option, (err.is_extended ? strlen(err.arg_option) : 1));
        _stack_push_block(&_cream_general_buffer, " ", 1);
        _stack_push_block(&_cream_general_buffer, _cream_error_strings[err.err_type], _cream_error_lengths[err.err_type]);
        _stack_push_block(&_cream_general_buffer, "\n", 1);
    }

    _cream_out_error_msg = malloc(_cream_general_buffer.byte_count +1);
    _stack_copy_cached_block(_cream_out_error_msg, &_cream_general_buffer, 0, _cream_general_buffer.byte_count);
    _cream_out_error_msg[_cream_general_buffer.byte_count] = '\0';
    _stack_free_expandable(&_cream_general_buffer);

    return (const char*)_cream_out_error_msg;
}