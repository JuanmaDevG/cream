#include "arg_options.h"

static inline bool _cream_checkout_option(_cream_argument_option* _option_ptr, const char* _option_location, const bool _is_option_extended)
{
    if(!_option_ptr)
    {
        _cream_declare_error(CREAM_NON_EXISTENT, (char*)_option_location, _is_option_extended, NULL);
        return false;
    }
    if(_option_ptr->has_been_used_already && _option_ptr->cannot_be_repeated)
    {
        _cream_declare_error(CREAM_REDUNDANT_ARGUMENT, (char*)_option_location, _is_option_extended, NULL);
        return false;
    }
    _option_ptr->has_been_used_already = true;
    return true;
}

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

uint32_t _cream_add_argument_data(const int _remaining_argc, const char** _updated_argv, _cream_argument_option* _actual_arg, const bool _is_it_extended)
{
    if(!_cream_enable_multiple_opts_per_arg) //Can read inline data?
    {
        bool inline_data_found = false;

        _cream_data_package pack = {1, (char*)_updated_argv, 2};
        if(!_is_it_extended && _updated_argv[0][2] != '\0' && _updated_argv[0][2] != '=') //Inline data?
        {
            inline_data_found = true;
        }
        else
        {
            for(uint8_t i=2; _updated_argv[0][i] != '\0' && !inline_data_found; i++) //Equals Operator?
            {
                switch(_updated_argv[0][i])
                {
                case '\0':
                    break;
                case '=':
                    pack.inline_data_offset = i+1;
                    inline_data_found = true;
                    break;
                default:
                    pack.inline_data_offset = i;
                    inline_data_found = true;
                }
            }
        }

        if(inline_data_found)
        {
            _stack_push_block(&(_actual_arg->data_container->data), &pack, sizeof(_cream_data_package));
            _actual_arg->data_container->actual_data_count += 1;
            return 1;
        }
    }

    uint32_t data_count = 0; //+1 because the option is not treated like data
    while(data_count +1 < _remaining_argc && _updated_argv[data_count +1][0] != _arg_id)
    {
        data_count++;
        if(data_count == _actual_arg->data_container->maximum_data_count) break;
    }

    _cream_data_package pack = {data_count, (char**)_updated_argv +1, 0};
    _actual_arg->data_container->actual_data_count += data_count;
    _stack_push_block(&(_actual_arg->data_container->data), &pack, sizeof(_cream_data_package));

    return data_count +1; /*plus the arg option offset*/
}

uint32_t _cream_read_argument(const int _updated_argc, const char** _updated_argv)
{
    _cream_argument_option* arg_ptr = NULL;
    bool is_option_extended = false;
    if(_updated_argv[0][0] == _arg_id) //Argument should be an option
    {
        if(_updated_argv[0][1] == _arg_id) //It is extended
        {
            is_option_extended = true; 
            arg_ptr = _cream_find_ext_arg_option(_updated_argv[0] +2);
        }
        else arg_ptr = _cream_get_arg_option(_updated_argv[0][1]);
    }
    else    //Argument is anonymous
    {
        uint32_t arg_offset = 1;
        while(arg_offset < _updated_argc && _updated_argv[arg_offset][0] != _arg_id)
            arg_offset++;

        if(_cream_treat_anonymous_args_as_errors)
        {
            _cream_declare_error(CREAM_ARGUMENTS_NOT_BOUND_TO_ANY_OPTION, _updated_argv[0], true, NULL);
            return arg_offset;
        }

        _cream_anon_arg_count += arg_offset;
        _cream_data_package pack = {arg_offset, (char**)_updated_argv, 0};
        _stack_push_block(&_cream_anonymous_args, &pack, sizeof(_cream_data_package));
        return arg_offset;
    }

    if( //Is argument invalid?
        (   !_cream_checkout_option(arg_ptr, _updated_argv[0], is_option_extended) && 
            is_option_extended
        ) || _updated_argv[0][1] == '\0' //Just a lonely arg id
    ) {
        return 1;
    }

    if(_cream_enable_multiple_opts_per_arg && !is_option_extended)
    {
        for(uint32_t i=2; _updated_argv[0][i] != '\0'; i++)
        {
            arg_ptr = _cream_get_arg_option(_updated_argv[0][i]);
            _cream_checkout_option(arg_ptr, _updated_argv[0] +i, false);
        }
        //If more than one option inlined, do not add data
        if(_updated_argv[0][2] != '\0') return 1;
    }

    if(arg_ptr && arg_ptr->data_container) 
        return _cream_add_argument_data(_updated_argc, _updated_argv, arg_ptr, is_option_extended);
    return 1;
}

extern inline void _cream_set_data_limit(const char* _options_array, va_list _arg_limits, uint8_t _config_type)
{
    if(!_options_array) return;
    _cream_argument_option* current_opt;
    for(uint32_t i=0; _options_array[i] != '\0'; i++)
    {
        current_opt = _cream_get_arg_option(_options_array[i]);
        if(current_opt && current_opt->data_container)
        {
            if(_config_type == CREAM_DATA_LIMIT_MAX) 
                current_opt->data_container->maximum_data_count = (uint8_t)va_arg(_arg_limits, int);
            else 
                current_opt->data_container->minimum_data_count = (uint8_t)va_arg(_arg_limits, int);
        }
    }
}

void _cream_check_mandatory_arguments()
{
    for(uint32_t i=0; i < _cream_option_count; i++)
    {
        if(
            _cream_declared_arg_options[i].is_mandatory &&
            !_cream_declared_arg_options[i].has_been_used_already
        ) {
            _cream_declare_error(CREAM_MANDATORY, _cream_declared_arg_options +i, false, NULL);
        }
    }
}

void _cream_check_surpassed_data_bounds()
{
    _cream_option_data* _actual_opt;
    for(uint32_t i=0; i < _cream_option_count; i++)
    {
        _actual_opt = _cream_get_arg_option(_cream_declared_option_chars[i])->data_container;
        if(!_actual_opt) continue;

        if(
            _actual_opt->maximum_data_count && 
            _actual_opt->actual_data_count > (uint32_t)_actual_opt->maximum_data_count
        )
            _cream_declare_error(CREAM_TOO_MUCH_DATA, _cream_declared_option_chars + i, true, NULL);
        if(_actual_opt->actual_data_count < _actual_opt->minimum_data_count)
            _cream_declare_error(CREAM_NOT_ENOUGH_DATA, _cream_declared_option_chars +i, true, NULL);
    }
}