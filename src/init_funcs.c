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
    _mandatory_arguments = (char*)malloc(length);
    memcpy(_mandatory_arguments, arg_characters, length);
}

uint32_t cargs_load_args(const int argc, const char** argv)
{
    enum cargs_error potential_error = CARGS_NO_ERROR;
    for(uint32_t i=1; i < argc; i++)
    {
        potential_error = _is_argument_wrong(argv[i]);
        if(potential_error != CARGS_NO_ERROR) return (uint32_t)potential_error;

        if(argv[i][1] == _arg_id)   //Is extended
        {
            potential_error = _does_extended_arg_exist(argv[i]);
            if(potential_error != CARGS_NO_ERROR) return (uint32_t)potential_error;

            uint32_t pos = _get_actual_checkpoint() -1;
            _reset_finders();
            _extended_args.args[pos].read_point[_extended_args.args[pos].associated_opt] = '\\';

            if(_extended_args.args[pos].read_point == _data_args) _add_argument_data(argv, i, pos);
        }
        else                        //Is not extended
        {
            //Search the arg
            //Confirm with a backslash (con read_point y checkpoint -1)
        }
        //If is data_arg, configure the ExtArgVec position
        //Make sure the argument contains data
    }
    _reset_finders();
    return (uint32_t)CARGS_NO_ERROR;
}

const char* cargs_get_error(uint32_t err_code) 
{
    uint32_t null_location = 0; //First null location is where to place the argument
    size_t char_count = 0;

    for(uint32_t i=0; _cargs_error_strings[err_code][i] != '\0'; i++)
    {
        char_count++;
        if(null_location == 0 && _cargs_error_strings[err_code][i+1] == '\0')
        {
            null_location == i+1;
            i++;
        }
    }

    size_t err_arg_length = strlen(_cargs_error_argument);
    char_count += err_arg_length +1;
    if(_cargs_error_buffer_str != NULL) free(_cargs_error_buffer_str);
    _cargs_error_buffer_str = (char*)malloc(char_count);
    
    //Copy the error with it's offsets to write the argument that caused the error
    memcpy(_cargs_error_buffer_str, _cargs_error_strings[err_code], null_location);
    memcpy(_cargs_error_buffer_str + null_location, _cargs_error_argument, err_arg_length);
    strcpy(_cargs_error_buffer_str + null_location + err_arg_length, _cargs_error_strings[err_code] + null_location +1);
}