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
    _cargs_argument* p_args = _cargs_declared_option_chars + _cargs_option_count;
    //Boolean arguments
    //Data arguments

}

bool cargs_clean()
{
    if(!(_cargs_bool_args || _cargs_data_args || _cargs_ext_args || _cargs_mandatory_args)) 
        return false; //No buffers
    _cargs_remove_redundant_args_linked_lists();
    _cargs_remove_anonymous_arguments();

    //General purpose main buffer
    bool bool_args_free = false;
    if(_cargs_bool_args) 
    {
        free(_cargs_bool_args); _cargs_bool_args = NULL; _cargs_bool_args_count = 0;
        _cargs_bool_bit_vec = NULL; bool_args_free = true;
    }
    if(!bool_args_free && _cargs_data_args) free(_cargs_data_args);

    if(_cargs_data_args) //There was data -> set to NULL associated buffer information
    {
        _cargs_data_args = NULL; _cargs_data_args_count = 0; _cargs_data_packs = NULL;
        _cargs_data_bit_vec = NULL;
        _cargs_bank_stack_pointer = 0; _cargs_equals_operator_pointer_bank = NULL;

        //Anonymous arguments were reset by the specific function...
        _cargs_maximum_data = NULL; _cargs_minimum_data = NULL;
        
        _reset_ext_finders();
        _cargs_redundant_opt_data = NULL;
        _cargs_is_data_relocated_bit_vec = NULL;
    }
    _reset_finders();
    _cargs_reset_ext_arg_buffers();
    _cargs_reset_mandatory_arg_buffers();
    _cargs_reset_error_buffers();
    _stack_free_expandable(&_cargs_general_buffer);

    return true;
}

void cargs_associate_extended(const char* arg_characters, ...) {
    if(!(arg_characters && _obtain_read_point())) return;
    va_list arg_l;
    va_start(arg_l, arg_characters);

    uint64_t count = 0;
    ExtArg actual_arg;
    for(uint64_t i=0; arg_characters[i] != '\0'; i++)
    {
        if(_find_argument_char(arg_characters[i]))
        {
            count++;
            actual_arg.name = va_arg(arg_l, const char*);
            actual_arg.read_point = _get_actual_read_point();
            actual_arg.associated_opt = _get_actual_checkpoint() -1;
            _stack_push_block(&_cargs_general_buffer, &actual_arg, sizeof(ExtArg));
        }
    }
    _reset_finders();

    //If there is mem allocated and not enough, realloc
    if(_cargs_ext_args)
    {
        if(count > _cargs_ext_arg_count) 
            _cargs_ext_args = (ExtArg*)realloc(_cargs_ext_args, count * sizeof(ExtArg));
    }
    else
    {
        _cargs_ext_args = (ExtArg*)malloc(count * sizeof(ExtArg));
    }

    /*
        The order of the extended arg vector is not relevant because each exttended arg is bound to the main char option
        and so the raw memory can be copied literally
    */
    _cargs_ext_arg_count = count;
    _stack_copy_cached_block(_cargs_ext_args, &_cargs_general_buffer, 0, 0);
    _stack_free_expandable(&_cargs_general_buffer);
    va_end(arg_l);
}

void cargs_make_mandatory(const char* arg_characters)
{
    const size_t length = (arg_characters ? strlen(arg_characters) : 0);
    if(length == 0)
    {
        free(_cargs_mandatory_args);
        _cargs_mandatory_args = NULL;
        _cargs_mandatory_arg_count = 0;
        return;
    }

    if(_cargs_mandatory_args) //Is there allocated memory?
    {
        size_t buf_size = length * sizeof(_cargs_buffer_position);
        if(length > _cargs_mandatory_arg_count)     //Not enough -> reallocate
            _cargs_mandatory_args = realloc(_cargs_mandatory_args, buf_size);
        memset(_cargs_mandatory_args, 0, (buf_size < _cargs_mandatory_arg_count ? _cargs_mandatory_arg_count : buf_size));
    }
    else //No -> allocate
        _cargs_mandatory_args = (_cargs_buffer_position*)calloc(length, sizeof(_cargs_buffer_position));
    _cargs_mandatory_arg_count = (uint32_t)length;

    //The fail offset avoids void gaps locating the index correctly when an argument char is not found
    size_t fail_offset = 0;
    for(size_t i=0; i < length; i++)
    {
        if(!_find_argument_char(arg_characters[i])) { fail_offset++; _cargs_mandatory_arg_count--; }
        else
        {
            _cargs_mandatory_args[i - fail_offset].position = _get_actual_checkpoint() -1;
            _cargs_mandatory_args[i - fail_offset].read_point = (_get_actual_read_point() == _cargs_bool_args ? _cargs_bool_bit_vec : _cargs_data_bit_vec);
        }
    }
    _reset_finders();
}

void cargs_set_minimum_data(const char* data_arg_string, ...)
{
    if(!_cargs_minimum_data) return;
    va_list arg_limits;
    va_start(arg_limits, data_arg_string);

    _cargs_set_data_limit(data_arg_string, arg_limits, _cargs_minimum_data);
    va_end(arg_limits);
    _reset_finders();
}

void cargs_set_maximum_data(const char* data_arg_string, ...)
{
    if(!_cargs_maximum_data) return;
    va_list arg_limits;
    va_start(arg_limits, data_arg_string);

    _cargs_set_data_limit(data_arg_string, arg_limits, _cargs_maximum_data);
    va_end(arg_limits);
    _reset_finders();
}

void cargs_treat_anonymous_args_as_errors(const bool _value) { _cargs_treat_anonymous_args_as_errors = _value; }

void cargs_treat_repeated_args_as_errors(const bool _value) { _cargs_treat_repeated_args_as_errors = _value; }

void cargs_include_argument_zero(const bool _value) { _cargs_include_argument_zero = _value; }

void cargs_load_args(const int argc, const char** argv)
{
    for(uint32_t i=(_cargs_include_argument_zero ? 0 : 1); i < (uint32_t)argc; i++)
    {
        if(argv[i][0] != _arg_id)
        {
            if(_cargs_treat_anonymous_args_as_errors) {
                _cargs_declare_error(argv[i], 1, CARGS_NON_EXISTENT);
                return;
            }
            //Do not treat as errors
            _cargs_store_anonymous_arguments(argc, argv, &i);
            continue;
        }

        if(argv[i][1] == _arg_id)   //Is extended
        {
            if(!_find_extended_argument(argv[i] +2))
            {
                _cargs_declare_error(argv[i] +2, 1, CARGS_NON_EXISTENT);
                return;
            }

            const uint32_t pos = _get_actual_ext_checkpoint() -1;
            if(_cargs_ext_args[pos].read_point == _cargs_data_args) //Is data arg option
            {
                if(!_add_argument_data(argc, argv, &i, &pos)) return;
            }
            else
            {
                if(
                    _cargs_treat_repeated_args_as_errors
                    && _cargs_get_bit(_cargs_bool_bit_vec, _cargs_ext_args[pos].associated_opt)
                ) {
                    _cargs_declare_error(_cargs_ext_args[pos].name, true, CARGS_REDUNDANT_ARGUMENT);
                    return;
                }
                _cargs_set_bit(_cargs_bool_bit_vec, _cargs_ext_args[pos].associated_opt, true);
            }
        }
        else if(!_read_non_extended_argument(argc, argv, &i)) return;
    }
    _reset_ext_finders();
    _reset_finders();
    _cargs_check_mandatory_arguments();
}

void cargs_cancel_argument_loads()
{
    _cargs_remove_redundant_args_linked_lists();
    memset(_cargs_bool_bit_vec, 0, _cargs_get_byte_size(_cargs_bool_args_count));
    //Set 0 one after data packages (_cargs_maximum_data addr) to actually remove data packages information
    memset(_cargs_data_bit_vec, 0, (size_t)(((uint8_t*)_cargs_maximum_data) - ((uint8_t*)_cargs_data_bit_vec)));
    _cargs_bank_stack_pointer = 0;
    //memset del bit vec de las relocations de las linked lists
    memset(_cargs_is_data_relocated_bit_vec, 0, _cargs_get_byte_size(_cargs_data_args_count));
    _cargs_remove_anonymous_arguments();
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