#include "init_funcs.h"


void cargs_set_identificator(const char new_id) { _arg_id = new_id; }

/*
    Sets the boolean arguments to be detected
*/
void cargs_set_boolean_args(const char* arg_letters) {
    if(arg_letters == NULL) return;
    if(_bool_args != NULL) free(_bool_args);
    _bool_args_count = strlen(arg_letters);

    _bool_args = (char*)malloc(_bool_args_count +1);
    memcpy(_bool_args, arg_letters, _bool_args_count +1);
    _remove_redundancies(REMOVE_BOOL_REDUNDANCIES);
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

void cargs_set_data_args(const char* arg_letters) {
    if(!arg_letters) return;
    if(_data_args != NULL) free(_data_args);
    _data_packs.size = strlen(arg_letters);

    _data_args = (char*)malloc(_data_packs.size +1);
    memcpy(_data_args, arg_letters, _data_packs.size +1);
    _remove_redundancies(REMOVE_DATA_REDUNDANCIES);
    _data_packs.packages = (ArgPackage*)calloc(_data_packs.size, sizeof(ArgPackage));
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
}

void cargs_load_args(const int argc, const char** argv)
{
    for(uint32_t i=1; i < argc; i++)
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
            if(_extended_args.args[pos].read_point == _data_args && !_add_argument_data(argv, &i, &pos)) return;
        }
        else if(!_read_non_extended_argument(argv, &i)) return;
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
                    _mandatory_args[i].read_point[_mandatory_args[i].position],
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