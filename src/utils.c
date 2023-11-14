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


//-----------------------------------------------------------------------------------------------------------

extern inline _cargs_argument* _find_argument_option(const char _character)
{
    if(_character < INVALID_CHARS) return NULL;
    return _cargs_valid_arg_options[_character - INVALID_CHARS];
}

_cargs_argument* _find_extended_argument(const char* ext_arg)
{
    if(_cargs_ext_arg_count == 0 || ext_arg == NULL) return 0;

    _cargs_argument* arg_ptr = _find_argument_option(ext_arg[0]);
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
        arg_ptr = _find_argument_option(i);
        if(arg_ptr && _cargs_streq(ext_arg, arg_ptr->extended_version))
        {
            return arg_ptr;
        }
    }

    return NULL;
}

uint32_t _add_argument_data(const int _remaining_argc, const char** _updated_argv, _cargs_argument* _actual_arg)
{
    //TODO: Remember
    //1. If data pieces exceed the maximum or do not reach the minimum, keep them added but push the error
    //Always return the data piece offset count

    //Loop over the data will find argument or interrupt if expose limits

    bool is_extended = (ext_arg_position == NULL ? false : true);
    uint32_t associated_option = 
        (!is_extended ? _get_actual_checkpoint() -1 : _cargs_ext_args[*ext_arg_position].associated_opt);
    if(_cargs_configure_and_store_equals_operator_data(argv[*index], associated_option)) return true;

    const uint32_t pointer_offset = (*index) +1;
    const char** data_pointer = argv + pointer_offset;
    uint32_t count = 0;
    while(count + pointer_offset < (uint32_t)argc && data_pointer[count][0] != _arg_id) 
    {
        if(count == (uint32_t)_cargs_maximum_data[associated_option] && _cargs_maximum_data[associated_option] > 0) break;
        count++;
    }

    //Check that the data count reaches the minimum data required
    if(count < (uint32_t)_cargs_minimum_data[associated_option])
    {
        _cargs_declare_error(argv[(*index)] + (is_extended ? 2 : 1), is_extended, CARGS_NOT_ENOUGH_DATA);
        return false;
    }

    if(_cargs_get_bit(_cargs_data_bit_vec, associated_option))
    {
        if(_cargs_treat_repeated_args_as_errors)
        {
            _cargs_declare_error(argv[*index], is_extended, CARGS_REDUNDANT_ARGUMENT);
            return false;
        }
        else if(count > 0)
            _cargs_push_list_node(
                &(_cargs_redundant_opt_data[associated_option].first_node),
                &(_cargs_redundant_opt_data[associated_option].last_node), 
                data_pointer, count
            );
    }
    else    //No redundant and new arg so add data
    {
        _cargs_data_packs[associated_option].count = count;
        _cargs_data_packs[associated_option].values = (count == 0 ? NULL : (char**)data_pointer);
        _cargs_set_bit(_cargs_data_bit_vec, associated_option, true);
    }

    //Set offset to argument iterator
    (*index) += count;
    return true;
}

bool _read_non_extended_argument(const int argc, const char* argv[], uint32_t* index)
{
    for(uint32_t j=1; argv[(*index)][j] != '\0'; j++)
    {
        if(_find_argument_char(argv[(*index)][j]))
        {
            if( //It is repeated and is treated as an error
                _cargs_treat_repeated_args_as_errors
                && _cargs_get_bit(
                    (_get_actual_read_point() == _cargs_bool_args ? _cargs_bool_bit_vec : _cargs_data_bit_vec),
                    _get_actual_checkpoint() -1
                )
            ) {
                _cargs_declare_error(argv[*index] +j, false, CARGS_REDUNDANT_ARGUMENT);
                return false;
            }

            //Check if data argument and write data
            if(_get_actual_read_point() == _cargs_data_args)
            {
                if(j == 1 && (argv[*index][2] == '=' || argv[*index][2] == '\0'))
                {
                    if(!_add_argument_data(argc, argv, index, NULL)) return false;
                    else
                    {
                        _cargs_set_bit(_cargs_data_bit_vec, _get_actual_checkpoint() -1, true);
                        return true;
                    }
                }
                _cargs_set_bit(_cargs_data_bit_vec, _get_actual_checkpoint() -1, true);
            }
            else _cargs_set_bit(_cargs_bool_bit_vec, _get_actual_checkpoint() -1, true);
        }
        else
        {
            _cargs_declare_error(argv[(*index)] +j, 0, CARGS_NON_EXISTENT);
            return false;
        }
    }

    return true;
}

extern inline void _cargs_set_data_limit(const char* data_arg_string, va_list arg_limits, uint8_t* write_point)
{
    if(!write_point) return;
    for(uint32_t i=0; data_arg_string[i] != '\0'; i++)
    {
        if(_find_argument_char(data_arg_string[i]))
            write_point[_get_actual_checkpoint() -1] = (uint8_t)va_arg(arg_limits, int);
    }
}

void _cargs_store_anonymous_arguments(const int argc, const char* argv[], uint32_t* arg_index)
{
    uint32_t count = 1;                         //We know this position contains anonymous arg
    const uint32_t anon_arg_pack_position = (*arg_index);  //Store the first position for the anonymous list node
    (*arg_index)++;
    while((*arg_index) < (uint32_t)argc && argv[(*arg_index)][0] != _arg_id) //Count anonymous arguments
    {
        count++;
        (*arg_index)++;
    }
    (*arg_index)--;     //Next iteration, the index variable will increment
    _cargs_anon_arg_count += count;
    _cargs_push_list_node(&_cargs_anon_args, &_cargs_anon_last, argv + anon_arg_pack_position, count);
}

bool _cargs_check_mandatory_arguments()
{
    for(uint32_t i=0; i < _cargs_mandatory_arg_count; i++)
    {
        if(!_cargs_mandatory_args[i].read_point) break; //NULL read point means no more mandatory args
        if(!_cargs_get_bit(_cargs_mandatory_args[i].read_point, _cargs_mandatory_args[i].position))
        {
            _cargs_declare_error(
                (_cargs_mandatory_args[i].read_point == _cargs_bool_bit_vec ? _cargs_bool_args : _cargs_data_args) + _cargs_mandatory_args[i].position,
                0, CARGS_MANDATORY
            );
            return false;
        }
    }

    return true;
}

extern inline uint32_t _cargs_search_equals_operator(const char* argument_pointer)
{
    for(uint32_t i=0; argument_pointer[i] != '\0'; i++)
        if(argument_pointer[i] == '=') return i+1;
    return 0;
}

extern inline bool _cargs_store_equals_operator_data(const char* data_pointer, const uint32_t associated_option)
{
    if(_cargs_bank_stack_pointer == _cargs_data_args_count) return false; //The pointer bank is full

    _cargs_data_packs[associated_option].count = 1;
    _cargs_equals_operator_pointer_bank[_cargs_bank_stack_pointer] = (char*)data_pointer;
    _cargs_data_packs[associated_option].values = _cargs_equals_operator_pointer_bank + _cargs_bank_stack_pointer;
    _cargs_bank_stack_pointer++;
    return true;
}

extern inline bool _cargs_configure_and_store_equals_operator_data(const char* arg_option, const uint32_t associated_option)
{
    const uint32_t cur_data_location = _cargs_search_equals_operator(arg_option);
    if(cur_data_location != 0)
    {
        _cargs_set_bit(_cargs_data_bit_vec, associated_option, true);
        _cargs_store_equals_operator_data(arg_option + cur_data_location, associated_option);
        return true;
    }

    return false;
}

extern inline void _cargs_remove_redundant_args_linked_lists()
{
    for(uint32_t i=0; i < _cargs_data_args_count; i++)
    {
        if(_cargs_get_bit(_cargs_is_data_relocated_bit_vec, i))
            free(_cargs_data_packs[i].values);
        else //data getter was never used so deallocate linked list
            _cargs_free_data_list(_cargs_redundant_opt_data + i);
    }
}

extern inline void _cargs_reset_ext_arg_buffers()
{
    if(_cargs_ext_args)
    {
        free(_cargs_ext_args);
        _cargs_ext_args = NULL;
    }
}

extern inline void _cargs_reset_mandatory_arg_buffers()
{
    if(_cargs_mandatory_args) 
    { 
        free(_cargs_mandatory_args); 
        _cargs_mandatory_args = NULL; 
        _cargs_mandatory_arg_count = 0; 
    }
}

extern inline void _cargs_reset_error_buffers()
{
    if(_cargs_error_argument)
    {
        _cargs_error_argument = NULL;
        free(_cargs_error_buffer_str); _cargs_error_buffer_str = NULL;
        cargs_error_code = CARGS_NO_ERROR;
    }
}

extern inline void _cargs_remove_anonymous_arguments()
{
    if(_cargs_anonymous_relocated_args) 
    {
        free(_cargs_anonymous_relocated_args); 
        _cargs_anonymous_relocated_args = NULL; 
        _cargs_anonymous_relocated_buf_size = 0;
    }

    while(_cargs_anon_args) _cargs_delete_list_head(&_cargs_anon_args, &_cargs_anon_last);
    _cargs_anon_arg_count = 0;
}