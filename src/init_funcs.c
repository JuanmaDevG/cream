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
        cargs_clean();
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

    if(_cargs_declared_option_chars)
    {
        free(_cargs_declared_option_chars);     //Buffer start point
        _cargs_declared_option_chars = NULL;
        _cargs_option_count = 0;
        _cargs_declared_arg_options = NULL;
        _cargs_declared_options_data = NULL;
        _cargs_ext_arg_count = 0;
    }

    if(!_cargs_anonymous_relocated_args)
    {
        free(_cargs_anonymous_relocated_args);
        _cargs_anonymous_relocated_args = NULL;
    }
    else
        _stack_free_expandable(&_cargs_anonymous_args);
}

void cargs_associate_extended(const char* arg_characters, ...) {
    va_list arg_l;
    va_start(arg_l, arg_characters);

    _cargs_argument* p_arg;
    for(uint32_t i=0; arg_characters[i] != '\0'; i++)
    {
        p_arg = _cargs_find_argument_option(arg_characters[i]);
        if(p_arg)
            p_arg->extended_version = va_arg(arg_l, char*);
    }

    va_end(arg_l);
}

void cargs_make_mandatory(const char* arg_characters)
{
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
    va_list arg_limits;
    va_start(arg_limits, data_arg_string);
    _cargs_set_data_limit(data_arg_string, arg_limits, CARGS_DATA_LIMIT_MIN);
    va_end(arg_limits);
}

extern inline void cargs_set_maximum_data(const char* data_arg_string, ...)
{
    va_list arg_limits;
    va_start(arg_limits, data_arg_string);
    _cargs_set_data_limit(data_arg_string, arg_limits, CARGS_DATA_LIMIT_MAX);
    va_end(arg_limits);
}

extern inline void cargs_treat_anonymous_args_as_errors(const bool _value) { _cargs_treat_anonymous_args_as_errors = _value; }

extern inline void cargs_disable_options_repetition(const char* _arg_options)
{
    _cargs_argument* p_arg;
    for(uint32_t i=0; _arg_options[i] != '\0'; i++)
    {
        p_arg = _cargs_find_argument_option(_arg_options[i]);
        if(p_arg)
            p_arg->cannot_be_repeated = true;
    }
}

extern inline void cargs_include_argument_zero(const bool _value) { _cargs_include_argument_zero = _value; }

void cargs_load_args(const int argc, const char** argv)
{
    for(uint32_t i=(_cargs_include_argument_zero ? 0 : 1); i < (uint32_t)argc; i++)
    {
        //TODO
    }
    _cargs_check_mandatory_arguments();
}

void cargs_cancel_argument_loads()
{
    //TODO
}

const char* cargs_get_error()
{
    if(cargs_error_code == CARGS_NO_ERROR) return NULL;
    if(_cargs_error_buffer_str) { free(_cargs_error_buffer_str); _cargs_error_buffer_str = NULL; }

    const uint8_t message_offset = 11; /*(Initial error string:) The argument ...*/
    size_t 
        arg_length = (_cargs_is_error_extended ? strlen(_cargs_error_argument) : 1), 
        err_message_length = strlen(_cargs_error_strings[cargs_error_code]);
    
    _cargs_error_buffer_str = (char*)malloc(message_offset + arg_length +2/*space + null last char*/ + err_message_length);
    char* write_point = _cargs_error_buffer_str;

    memcpy(write_point, "The option ", message_offset);
    write_point += message_offset;
    memcpy(write_point, _cargs_error_argument, arg_length);
    write_point += arg_length;
    write_point[0] = ' ';
    write_point++;
    memcpy(write_point, _cargs_error_strings[cargs_error_code], err_message_length +1);

    return (const char*)_cargs_error_buffer_str;
}