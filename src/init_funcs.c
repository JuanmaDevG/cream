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
    size_t buf_length = _bool_args_count + _data_packs.size;
    if(buf_length == 0) return;

    //Cache friendly single buffer with dual pointer
    _bool_args = (char*) malloc(buf_length + (bool_args && data_args ? 2 : 1) /*If both buffers, two null characters*/);

    if(bool_args) memcpy(_bool_args, bool_args, _bool_args_count +1);
    if(data_args)
    {
        //Data arguments pointer offset
        _data_args = _bool_args + _bool_args_count +1;
        memcpy(_data_args, data_args, _data_packs.size + 1);
        _remove_redundancies(REMOVE_DATA_REDUNDANCIES);
        _data_packs.packages = (ArgPackage*)calloc(_data_packs.size, sizeof(ArgPackage));

        //Allocation of maximum and minimum data required argument counts
        _cargs_maximum_data = (uint8_t*)calloc(_data_packs.size, sizeof(uint8_t));
        _cargs_minimum_data = (uint8_t*)calloc(_data_packs.size, sizeof(uint8_t));
    }
}

void cargs_associate_extended(const char* arg_characters, ...) {
    _obtain_read_point();
    if(_get_actual_read_point() == NULL) return;

    size_t length = strlen(arg_characters);
    va_list arg_l;
    va_start(arg_l, length);

    //Extended args vector setting up
    _extended_args.size = length;
    _extended_args.args = (ExtArg*)calloc(length, sizeof(ExtArg));

    //Loops over the argument buffers finding the argument characters to associate
    for(uint32_t i=0; i < length; i++)
    {
        if(_find_argument_char(arg_characters[i]) != 0)
            _push_extended_argument(va_arg(arg_l, char*), _get_actual_checkpoint() -1, _get_actual_read_point(), i);
    }
    _reset_finders();

    va_end(arg_l);
}

void cargs_make_mandatory(const char* arg_characters)
{
    size_t length = strlen(arg_characters) +1;
    _mandatory_arg_count = length;
    _mandatory_args = (_cargs_mandatory_position*)malloc(length * sizeof(_cargs_mandatory_position));
    for(uint32_t i=0; i < length; i++)
    {
        if(_find_argument_char(arg_characters[i]) == 0)
        {
            _mandatory_args[i].position = UINT32_MAX;
            _mandatory_args[i].read_point = NULL;
        }
        else
        {
            _mandatory_args[i].position = _get_actual_checkpoint() -1;
            _mandatory_args[i].read_point = _get_actual_read_point();
        }
    }

    _reset_finders();
}

void cargs_set_minimum_data(const char* data_arg_string, ...)
{
    uint32_t length = (uint32_t)strlen(data_arg_string);
    va_list arg_limits;

    va_start(arg_limits, length);
    for(uint32_t i=0; i < length; i++)
    {
        if(_find_argument_char(data_arg_string[i]))
        {
            //TODO
        }
    }
}

void cargs_set_maximum_data(const char* data_arg_string, ...)
{
    //Code more and more...
}

inline void cargs_treat_anonymous_args_as_errors(const bool value) { _cargs_treat_anonymous_args_as_errors = value; }

void cargs_load_args(const int argc, const char** argv)
{
    for(uint32_t i=1; i < (uint32_t)argc; i++)
    {
        if(argv[i][0] != _arg_id)
        {
            _cargs_declare_error(argv[i], 1, CARGS_WRONG_ID);
            return;
        }

        if(argv[i][1] == _arg_id)   //Is extended
        {
            if(!_find_extended_argument(argv[i] +2))
            {
                _cargs_declare_error(argv[i] +2, 1, CARGS_NON_EXISTENT);
            }

            uint32_t pos = _get_actual_ext_checkpoint() -1;
            //Check the argument into the buffer
            _extended_args.args[pos].read_point[_extended_args.args[pos].associated_opt] = '\\';
            //If is data args, fill data pointers to _data_packs
            if(_extended_args.args[pos].read_point == _data_args && !_add_argument_data(argc, argv, &i, &pos)) return;
        }
        else if(!_read_non_extended_argument(argc, argv, &i)) return;
    }
    _reset_ext_finders();
    _reset_finders();

    //Checking that mandatory arguments have been written within the program input
    for(uint32_t i=0; i < _mandatory_arg_count; i++)
    {
        if(_mandatory_args[i].read_point != NULL)
            if(_mandatory_args[i].read_point[_mandatory_args[i].position] != '\\')
            {
                _cargs_declare_error(
                    _mandatory_args[i].read_point + _mandatory_args[i].position,
                    0, CARGS_MANDATORY
                );
                return;
            }
    }
}

const char* cargs_get_error()
{
    if(cargs_error_code == 0) return NULL;

    size_t char_count = strlen(_cargs_error_strings[cargs_error_code]);
    uint32_t null_location = 1 + (uint32_t)char_count;      //First null location is where to place the argument

    size_t err_arg_length = (_cargs_is_extended ? strlen(_cargs_error_argument) : /*Non extended means just one char opt*/ 1 );
    char_count += err_arg_length +1;
    if(_cargs_error_buffer_str != NULL) free(_cargs_error_buffer_str);
    _cargs_error_buffer_str = (char*)malloc(char_count);
    
    //Copy the error till the first null char
    memcpy(_cargs_error_buffer_str, _cargs_error_strings[cargs_error_code], null_location);
    //Copy the error content into the null character position
    if(_cargs_is_extended)
        memcpy(_cargs_error_buffer_str + null_location, _cargs_error_argument, err_arg_length);
    else
    {
        char* offset_pointer = _cargs_error_buffer_str + null_location;
        offset_pointer[0] = _cargs_error_argument[0]; //Just write the option character
    }
    //Copy the rest of the error message
    strcpy(_cargs_error_buffer_str + null_location + err_arg_length, _cargs_error_strings[cargs_error_code] + null_location +1);
    return (const char*)_cargs_error_buffer_str;
}