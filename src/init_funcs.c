#include "init_funcs.h"


void cargs_set_identificator(const char new_id) { _arg_id = new_id; }

void cargs_set_args(const char* bool_args, const char* data_args)
{
    cargs_clean();
    if(!(bool_args || data_args)) return;

    //Measures
    if(bool_args) _cargs_bool_args_count = strlen(bool_args);
    if(data_args) _cargs_data_args_count = strlen(data_args);

    size_t buf_length = 
        _cargs_bool_args_count                                          /*boolean argument characters*/
        + _cargs_get_byte_size(_cargs_bool_args_count)                  /*boolean arg bit vector existence confirmator*/
        + _cargs_data_args_count                                        /*data argument characters*/ 
        + (bool_args && data_args ? 2 : 1)                              /*If both buffers (bool and data args), two null characters*/
        + _cargs_get_byte_size(_cargs_data_args_count)                  /*data arg bit vector existence confirmator*/
        + (_cargs_data_args_count * sizeof(char*))                      /*equals operator pointer bank*/
        + (_cargs_data_args_count * sizeof(_cargs_data_storage_list))   /*redundant argument option data bank*/
        + (_cargs_data_args_count * sizeof(ArgPackage))                 /*argument data packages for non redundant (no linked list)*/
        + _cargs_data_args_count                                        /*for maximum data per argument limits*/
        + _cargs_data_args_count                                        /*for minimum data per argument limits*/
        + _cargs_get_byte_size(_cargs_data_args_count);                 /*for the is_data_relocated bit vector*/

    //Cache friendly single buffer with multi-pointer support
    _cargs_bool_args = (char*) malloc(buf_length);
    memset(_cargs_bool_args, 0, buf_length);

    if(bool_args)
    {
        memcpy(_cargs_bool_args, bool_args, _cargs_bool_args_count +1);
        _cargs_bool_bit_vec = (uint8_t*)(_cargs_bool_args + _cargs_bool_args_count +1);
    }
    if(data_args)
    {
        //Data arguments pointer offset
        _cargs_data_args = 
            (bool_args ? (char*)_cargs_bool_bit_vec + _cargs_get_byte_size(_cargs_bool_args_count) : _cargs_bool_args);
        if(!bool_args) _cargs_bool_args = NULL;
        memcpy(_cargs_data_args, data_args, _cargs_data_args_count + 1);
        _remove_redundancies(REMOVE_DATA_REDUNDANCIES);
        _cargs_data_bit_vec = (uint8_t*)(_cargs_data_args + _cargs_data_args_count +1);
        //Equals operator pointer bank to point to the specific argument determined by equals sign
        _cargs_equals_operator_pointer_bank = (char**)(_cargs_data_bit_vec + _cargs_get_byte_size(_cargs_data_args_count));
        //Redundant argument options data pointers storage
        _cargs_redundant_opt_data = (_cargs_data_storage_list*)(_cargs_equals_operator_pointer_bank + _cargs_data_args_count);
        //Non redundant option data packages storage
        _cargs_data_packs = (ArgPackage*)(_cargs_redundant_opt_data + _cargs_data_args_count);
        //Allocation of maximum and minimum data required argument counts
        _cargs_maximum_data = (uint8_t*)(_cargs_data_packs + _cargs_data_args_count);
        _cargs_minimum_data = (uint8_t*)(_cargs_maximum_data + _cargs_data_args_count);
        _cargs_is_data_relocated_bit_vec = _cargs_minimum_data + _cargs_data_args_count;
    }
}

bool cargs_clean()
{
    if(!(_cargs_bool_args || _cargs_data_args || _extended_args.args || _cargs_mandatory_args)) 
        return false; //No buffers
    _cargs_remove_redundant_args_linked_lists();

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

        _cargs_anon_arg_count = 0; _cargs_anon_args = NULL; _cargs_anon_last = NULL;
        _cargs_maximum_data = NULL; _cargs_minimum_data = NULL;
        
        _reset_finders(); _reset_ext_finders();
        _cargs_redundant_opt_data = NULL;
        _cargs_is_data_relocated_bit_vec = NULL;
    }
    _cargs_reset_ext_arg_buffers();
    _cargs_reset_mandatory_arg_buffers();
    _cargs_reset_error_buffers();

    return true;
}

void cargs_associate_extended(const char* arg_characters, ...) {
    if(!arg_characters) return;
    _obtain_read_point();
    if(_get_actual_read_point() == NULL) return;
    const size_t length = strlen(arg_characters); 
    if(!length) return;
    va_list arg_l;
    va_start(arg_l, arg_characters);

    if(_extended_args.args)
    {
        for(size_t i=0; i < _extended_args.size; i++) free(_extended_args.args[i].name);
        memset(_extended_args.args, 0, _extended_args.size * sizeof(ExtArg));
    }
    
    if(length != _extended_args.size) //If need ext_arg reallocation
    {
        free(_extended_args.args);
        _extended_args.args = (ExtArg*)calloc(length, sizeof(ExtArg));
    }
    _extended_args.size = length;

    //Loops over the argument buffers finding the argument characters to associate
    for(uint32_t i=0; i < length; i++)
    {
        if(_find_argument_char(arg_characters[i]))
            _cargs_push_extended_argument(
                va_arg(arg_l, const char*), /*argument string*/
                _get_actual_checkpoint() -1 /*associated option*/, 
                _get_actual_read_point(), i
            );
    }
    _reset_finders();

    va_end(arg_l);
}

void cargs_make_mandatory(const char* arg_characters)
{
    const size_t length = (arg_characters ? strlen(arg_characters) : 0);
    if(length == 0)
    {
        free(_cargs_mandatory_args); _cargs_mandatory_args = NULL;
        _cargs_mandatory_arg_count = 0;
        return;
    }

    if(_cargs_mandatory_args) //Is there allocated memory?
    {
        size_t buf_size = length * sizeof(_cargs_buffer_position);
        if(length > _cargs_mandatory_arg_count)     //Not enough -> reallocate
            realloc(_cargs_mandatory_args, buf_size);
        memset(_cargs_mandatory_args, 0, (buf_size < _cargs_mandatory_arg_count ? _cargs_mandatory_arg_count : buf_size));
    }
    else //No -> allocate
        _cargs_mandatory_args = (_cargs_buffer_position*)calloc(length, sizeof(_cargs_buffer_position));
    _cargs_mandatory_arg_count = (uint32_t)length;

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

inline void cargs_treat_anonymous_args_as_errors(const bool _value) { _cargs_treat_anonymous_args_as_errors = _value; }

inline void cargs_treat_repeated_args_as_errors(const bool _value) { _cargs_treat_repeated_args_as_errors = _value; }

inline void cargs_include_argument_zero(const bool _value) { _cargs_include_argument_zero = _value; }

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
            if(_extended_args.args[pos].read_point == _cargs_data_args) //Is data arg option
            {
                if(!_add_argument_data(argc, argv, &i, &pos)) return;
            }
            else
            {
                if(
                    _cargs_treat_repeated_args_as_errors
                    && _cargs_get_bit(_cargs_bool_bit_vec, _extended_args.args[pos].associated_opt)
                ) {
                    _cargs_declare_error(_extended_args.args[pos].name, true, CARGS_REDUNDANT_ARGUMENT);
                    return;
                }
                _cargs_set_bit(_cargs_bool_bit_vec, _extended_args.args[pos].associated_opt, true);
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