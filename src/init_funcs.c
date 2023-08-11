#include "init_funcs.h"


void cargs_set_identificator(const char new_id) { _arg_id = new_id; }

void cargs_set_args(const char* bool_args, const char* data_args)
{
    //Cleanup
    if(_bool_args) { free(_bool_args); _bool_args = NULL; }
    if(_data_args) { free(_data_args); _data_args = NULL; }

    //Measures
    if(bool_args) _bool_args_count = strlen(bool_args);
    if(data_args) _data_packs.size = strlen(data_args);
    size_t buf_length = 
        _bool_args_count                                            /*boolean argument characters*/
        + _data_packs.size                                          /*data argument characters*/ 
        + (bool_args && data_args ? 2 : 1)                          /*If both buffers (bool and data args), two null characters*/
        + (_data_packs.size * sizeof(char*))                        /*equals operator pointer bank*/ 
        + (_data_packs.size * sizeof(_cargs_redundant_data_storage))/*redundant argument option data bank*/
        + (_data_packs.size * sizeof(ArgPackage))                   /*argument data packages for non redundant (no linked list)*/
        + _data_packs.size                                          /*for maximum data per argument limits*/
        + _data_packs.size;                                         /*for minimum data per argument limits*/
    if(buf_length == 0) return;

    //Cache friendly single buffer with multi-pointer support
    _bool_args = (char*) malloc(buf_length);

    if(bool_args) memcpy(_bool_args, bool_args, _bool_args_count +1);
    if(data_args)
    {
        //Data arguments pointer offset
        _data_args = _bool_args + _bool_args_count + (bool_args ? 1 : 0);
        if(!bool_args) _bool_args = NULL;
        memcpy(_data_args, data_args, _data_packs.size + 1);
        _remove_redundancies(REMOVE_DATA_REDUNDANCIES);
        //Equals operator pointer bank to point to the specific argument determined by equals sign
        _cargs_equals_operator_pointer_bank = (char**)(_data_args + _data_packs.size +1);
        //Redundant argument options data pointers storage
        _cargs_redundant_arguments = (_cargs_redundant_data_storage*)(_cargs_equals_operator_pointer_bank + _data_packs.size);
        memset(_cargs_redundant_arguments, 0, _data_packs.size * sizeof(_cargs_redundant_data_storage));
        //Non redundant option data packages storage
        _data_packs.packages = (ArgPackage*)(_cargs_redundant_arguments + _data_packs.size);
        memset(_data_packs.packages, 0, _data_packs.size * sizeof(ArgPackage));
        //Allocation of maximum and minimum data required argument counts
        _cargs_maximum_data = (uint8_t*)(_data_packs.packages + _data_packs.size);
        _cargs_minimum_data = (uint8_t*)(_cargs_maximum_data + _data_packs.size);
        memset(_cargs_maximum_data, 0, _data_packs.size *2);
    }
}

void cargs_associate_extended(const char* arg_characters, ...) {
    _obtain_read_point();
    if(_get_actual_read_point() == NULL) return;

    size_t length = strlen(arg_characters);
    va_list arg_l;
    va_start(arg_l, arg_characters);

    //Extended args vector setting up
    _extended_args.size = length;
    _extended_args.args = (ExtArg*)calloc(length, sizeof(ExtArg));

    //Loops over the argument buffers finding the argument characters to associate
    for(uint32_t i=0; i < length; i++)
    {
        if(_find_argument_char(arg_characters[i]))
            _cargs_push_extended_argument(va_arg(arg_l, const char*), _get_actual_checkpoint() -1, _get_actual_read_point(), i);
    }
    _reset_finders();

    va_end(arg_l);
}

void cargs_make_mandatory(const char* arg_characters)
{
    size_t length = strlen(arg_characters) +1;
    _cargs_mandatory_arg_count = length;
    _cargs_mandatory_args = (_cargs_mandatory_position*)calloc(length, sizeof(_cargs_mandatory_position));
    for(uint32_t i=0; i < length; i++)
    {
        if(_find_argument_char(arg_characters[i]) == 0)
        {
            _cargs_mandatory_args[i].position = UINT32_MAX;
            _cargs_mandatory_args[i].read_point = NULL;
        }
        else
        {
            _cargs_mandatory_args[i].position = _get_actual_checkpoint() -1;
            _cargs_mandatory_args[i].read_point = _get_actual_read_point();
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

inline void cargs_treat_anonymous_args_as_errors(const bool value) { _cargs_treat_anonymous_args_as_errors = value; }

inline void cargs_treat_repeated_args_as_errors(const bool value) { _cargs_treat_repeated_args_as_errors = value; }

void cargs_load_args(const int argc, const char** argv)
{
    for(uint32_t i=1; i < (uint32_t)argc; i++)
    {
        if(argv[i][0] != _arg_id)
        {
            if(_cargs_treat_anonymous_args_as_errors) {
                _cargs_declare_error(argv[i], 1, CARGS_WRONG_ID);
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
            //If is data args, fill data pointers to _data_packs
            if(_extended_args.args[pos].read_point == _data_args && !_add_argument_data(argc, argv, &i, &pos)) return;
            //Check the argument storage
            _extended_args.args[pos].read_point[_extended_args.args[pos].associated_opt] = '\\';
        }
        else if(!_read_non_extended_argument(argc, argv, &i)) return;
    }
    _reset_ext_finders();
    _reset_finders();
    _cargs_check_mandatory_arguments();
}

const char* cargs_get_error()
{
    if(cargs_error_code == CARGS_NO_ERROR) return NULL;

    const uint8_t message_offset = 13; /*(Initial error string:) The argument ...*/
    size_t 
        arg_length = (_cargs_is_extended ? strlen(_cargs_error_argument) : 1), 
        err_message_length = strlen(_cargs_error_strings[cargs_error_code]);
    
    _cargs_error_buffer_str = (char*)malloc(message_offset + arg_length +2/*space + null last char*/ + err_message_length);
    char* write_point = _cargs_error_buffer_str;

    memcpy(write_point, "The argument ", message_offset);
    write_point += message_offset;
    memcpy(write_point, _cargs_error_argument, arg_length);
    write_point += arg_length;
    write_point[0] = ' ';
    write_point++;
    memcpy(write_point, _cargs_error_strings[cargs_error_code], err_message_length +1);

    return (const char*)_cargs_error_buffer_str;
}