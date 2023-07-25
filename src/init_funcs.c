#include "init_funcs.h"


void cargs_set_identificator(const char new_id) { arg_id = new_id; }

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
    char* read_point = _obtain_read_point(); //To be _bool_args or _data_args
    if(read_point == NULL) return;

    size_t length = strlen(arg_characters);
    va_list arg_l;
    va_start(arg_l, length);

    //Extended args vector setting up
    _extended_args.size = length;
    _extended_args.args = (ExtArg*)calloc(length, sizeof(ExtArg));

    //Loops over the argument buffers finding the argument letters to associate
    uint32_t checkpoint = 0;
    for(uint32_t i=0; i < length; i++)
    {
        if(_find_argument_letter(arg_characters[i], read_point, &checkpoint) == (uint8_t)1)
            _push_extended_argument(va_arg(arg_l, char*), checkpoint -1, read_point, i);
    }

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
    //Get the argument characters positions

    //Trigger those vector positions to be confirmed while using cargs_load_args
}

uint32_t cargs_load_args(const int argc, const char** argv)
{
    for(uint32_t i=1; i < argc; i++)
    {
        //Look if extended (double argument identificator) or not

        //Loop with find functions and previously declare read_points

        //Check the boolean arguments or add the data pointers to data argument vector
    }
}

const char* cargs_get_error(uint32_t err_code) {}