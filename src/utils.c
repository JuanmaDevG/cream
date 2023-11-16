#include "utils.h"

inline bool _cargs_streq(const char* _str1, const char* _str2)
{
    if(!(_str1 && _str2)) return false;

    uint32_t i=0;
    while(_str1[i] == _str2[i])
    {
        if(
            _str1[i+1] == '\0' && 
            _str2[i+1] == '\0'
        ) {
            return true;
        }
        i++;
    }
    return false;
}

inline bool _cargs_check_option(_cargs_argument* _option_ptr, const char* _option_location, const bool _is_option_extended)
{
    if(!_option_ptr)
    {
        _cargs_declare_error(CARGS_NON_EXISTENT, (char*)_option_location, _is_option_extended, NULL);
        return false;
    }
    if(_option_ptr->has_been_used_already && !(_option_ptr->has_permission_to_be_repeated))
    {
        _cargs_declare_error(CARGS_REDUNDANT_ARGUMENT, (char*)_option_location, _is_option_extended, NULL);
        return false;
    }
    _option_ptr->has_been_used_already = true;
    return true;
}


//-----------------------------------------------------------------------------------------------------------

extern inline _cargs_argument* _cargs_find_argument_option(const char _character)
{
    if(_character < INVALID_CHARS || _character >= INVALID_CHARS + ASCII_TABLE_SIZE) return NULL;
    return _cargs_valid_arg_options[_character - INVALID_CHARS];
}

_cargs_argument* _cargs_find_extended_argument(const char* ext_arg)
{
    if(_cargs_ext_arg_count == 0 || ext_arg == NULL) return 0;

    _cargs_argument* arg_ptr = _cargs_find_argument_option(ext_arg[0]);
    if(arg_ptr)
    {
        char first_character = tolower(ext_arg[0]);
        if(
            first_character == arg_ptr->extended_version[0] &&
            (
               _cargs_promise_first_ext_arg_char_is_ext_arg  || 
               _cargs_streq(arg_ptr->extended_version +1, ext_arg +1) 
            )
        ) 
            return arg_ptr;
    }

    //Not constant time found, so iterate thorough the whole buffer
    for(char i=0; i < ASCII_TABLE_SIZE; i++)
    {
        arg_ptr = _cargs_find_argument_option(i);
        if(arg_ptr && _cargs_streq(ext_arg, arg_ptr->extended_version))
        {
            return arg_ptr;
        }
    }

    return NULL;
}

uint32_t _cargs_add_argument_data(const int _remaining_argc, const char** _updated_argv, _cargs_argument* _actual_arg, const bool _is_it_extended)
{
    uint32_t data_count = 0;

    //Looking for inline data or equals opeartor data and add it
    if(!_cargs_enable_multiple_opts_per_arg)
    {
        if(_actual_arg->data_container->minimum_data_count > 1) //Invalid?
        {
            _cargs_declare_error(CARGS_NOT_ENOUGH_DATA, _updated_argv[0], _is_it_extended, NULL);
            return 1;
        }

        if(!_is_it_extended && _updated_argv[0][2] != '\0' && _updated_argv[0][2] != '=') //Inline data?
        {
            _cargs_data_package pack = {1, (char**)_updated_argv, 2};
            _stack_push_block(&(_actual_arg->data_container->data), &pack, sizeof(_cargs_data_package));
        }

        for(uint8_t i=2; _updated_argv[0][i] != '\0'; i++) //Equals Operator?
        {
            if(_updated_argv[0][i] == '=' && _updated_argv[0][i+1] != '\0')
            {
                _cargs_data_package pack = {1, (char**)_updated_argv, i+1};
                _stack_push_block(&(_actual_arg->data_container->data), &pack, sizeof(_cargs_data_package));
                _actual_arg->data_container->actual_data_count += 1;
            }
        }
    }

    while(data_count < _remaining_argc && _updated_argv[data_count +1/*opt is not arg*/][0] != _arg_id)
    {
        data_count++;
        if(data_count == _actual_arg->data_container->maximum_data_count) break;
    }

    _cargs_data_package pack = {data_count, (char**)_updated_argv +1, 0};
    _actual_arg->data_container->actual_data_count += data_count;
    _stack_push_block(&(_actual_arg->data_container->data), &pack, 0, sizeof(_cargs_data_package));

    if(data_count < _actual_arg->data_container->minimum_data_count)
        _cargs_declare_error(CARGS_NOT_ENOUGH_DATA, _updated_argv[0], _is_it_extended, NULL);
    
    return data_count +1; /*plus the arg option offset*/
}

uint32_t _cargs_read_argument(const int _updated_argc, const char** _updated_argv)
{
    _cargs_argument* arg_ptr = NULL;
    bool is_option_extended = false;
    if(_updated_argv[0][0] == _arg_id) //Argument should be an option
    {
        if(_updated_argv[0][1] == _arg_id) //It is extended
        {
            is_option_extended = true; 
            arg_ptr = _cargs_find_extended_argument(_updated_argv[0] +2);
        }
        else arg_ptr = _cargs_find_argument_option(_updated_argv[0][1]);
    }
    else    //Argument is anonymous
    {
        uint32_t arg_offset = 1;
        while(arg_offset < _updated_argc && _updated_argv[arg_offset][0] != _arg_id)
            arg_offset++;
        if(_cargs_treat_anonymous_args_as_errors) return arg_offset;

        _cargs_data_package pack = {arg_offset, (char**)_updated_argv};
        _stack_push_block(&_cargs_anonymous_args, &pack, sizeof(_cargs_data_package));
        return 1;
    }

    if( //Is argument invalid?
        (   !_cargs_check_option(arg_ptr, _updated_argv[0], is_option_extended) && 
            is_option_extended
        ) || _updated_argv[0][1] == '\0' //Just a lonely arg id
    ) {
        return 1;
    }

    //Can we loop without having in count the equals operator or inline data?
    if(_cargs_enable_multiple_opts_per_arg && !is_option_extended)
    {
        for(uint32_t i=2; _updated_argv[0][i] != '\0'; i++)
        {
            arg_ptr = _cargs_find_argument_option(_updated_argv[0][i]);
            _cargs_check_option(arg_ptr, _updated_argv[0] +i, false);
        }
        return 1;
    }

    if(arg_ptr && arg_ptr->data_container) 
        return _cargs_add_argument_data(_updated_argc, _updated_argv, arg_ptr, is_option_extended);
    return 1;
}

extern inline void _cargs_set_data_limit(const char* _options_array, va_list _arg_limits, uint8_t _config_type)
{
    //TODO: Change this function to make it work
    _cargs_argument* current_opt;
    for(uint32_t i=0; _options_array[i] != '\0'; i++)
    {
        current_opt = _cargs_find_argument_option(_options_array[i]);
        if(current_opt && current_opt->data_container)
        {
            if(_config_type == CARGS_DATA_LIMIT_MAX) 
                current_opt->data_container->maximum_data_count = (uint8_t)va_arg(_arg_limits, int);
            else 
                current_opt->data_container->minimum_data_count = (uint8_t)va_arg(_arg_limits, int);
        }
    }
}

void _cargs_check_mandatory_arguments()
{
    for(uint32_t i=0; i < _cargs_option_count; i++)
    {
        if(
            _cargs_declared_arg_options[i].is_mandatory &&
            !_cargs_declared_arg_options[i].has_been_used_already
        ) {
            _cargs_declare_error(CARGS_MANDATORY, _cargs_declared_arg_options +i, false, NULL);
        }
    }
}