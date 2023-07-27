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
        if(potential_error) return (uint32_t)potential_error;

        if(argv[i][1] == _arg_id)   //Is extended
        {
            potential_error = _does_extended_arg_exist(argv[i]);
            if(potential_error) return (uint32_t)potential_error;

            uint32_t pos = _get_actual_ext_checkpoint() -1;
            //Check the argument into the buffer
            _extended_args.args[pos].read_point[_extended_args.args[pos].associated_opt] = '\\';
            //If is data args, fill data pointers to _data_packs
            if(_extended_args.args[pos].read_point == _data_args) _add_argument_data(argv, i, pos);
        }
        else                        //Is not extended
        {
            //Loop that supports multiple boolean argument values like -abcdf but if there is any data argument
            //into the multi-boolean argument string, it will notify and throw an error
            for(uint32_t j=1; argv[i][j] != '\0'; j++)
            {
                if(_find_argument_char(argv[i][j]))
                {
                    //TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                else
                {
                    _cargs_error_argument = argv[i] + j;
                    _cargs_is_extended = 1;
                    return (uint32_t)CARGS_NON_EXISTENT;
                }
            }
            //Search the arg
            //Confirm with a backslash (con read_point y checkpoint -1)
        }
        //If is data_arg, configure the ExtArgVec position
        //Make sure the argument contains data
    }
    _reset_ext_finders();
    _reset_finders();
    return (uint32_t)CARGS_NO_ERROR;
}

const char* cargs_get_error(uint32_t err_code)
{
    size_t char_count = strlen(_cargs_error_strings[err_code]);
    uint32_t null_location = 1 + (uint32_t)char_count;  //First null location is where to place the argument

    size_t err_arg_length = (_cargs_is_extended ? strlen(_cargs_error_argument) : /*Non extended means just one char opt*/ 1 );
    char_count += err_arg_length +1;
    if(_cargs_error_buffer_str != NULL) free(_cargs_error_buffer_str);
    _cargs_error_buffer_str = (char*)malloc(char_count);
    
    //Copy the error till the first null char
    memcpy(_cargs_error_buffer_str, _cargs_error_strings[err_code], null_location);
    //Copy the error content into the null character position
    if(_cargs_is_extended)
        memcpy(_cargs_error_buffer_str + null_location, _cargs_error_argument, err_arg_length);
    else
    {
        char* offset_pointer = _cargs_error_buffer_str + null_location;
        offset_pointer[0] = _cargs_error_argument[0]; //Just write the option character
    }
    //Copy the rest of the error message
    strcpy(_cargs_error_buffer_str + null_location + err_arg_length, _cargs_error_strings[err_code] + null_location +1);
    return (const char*)_cargs_error_buffer_str;
}