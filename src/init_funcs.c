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

void cargs_associate_extended(const char* arg_letters, ...) {
    char* read_point = _obtain_read_point(); //To be _bool_args or _data_args
    if(read_point == NULL) return;

    size_t length = strlen(arg_letters);
    va_list arg_l;
    va_start(arg_l, length);

    //Extended args vector setting up
    _extended_args.size = length;
    _extended_args.args = (ExtArg*)calloc(length, sizeof(ExtArg));

    //Loops over the argument buffers finding the argument letters to associate
    uint32_t checkpoint = 0;
    for(uint32_t i=0; i < length; i++)
    {
        uint8_t found = 0;
        uint32_t j = checkpoint;
        char* checkpoint_read_point = read_point;
        while(found == 0)
        {
            //Swap read_point beacause finished actual
            if(read_point[j] == '\0')
            {
                j = 0;
                _swap_read_point(read_point);
            }

            //Associated argument letter found
            if(read_point[j] == arg_letters[i])
            {
                found = 1;
                checkpoint = j+1;
                checkpoint_read_point = read_point;
                _push_extended_argument(va_arg(arg_l, char*), j, read_point, i);
            }

            //Found or not, we're done with the two argument strings so continue
            else if(j == checkpoint -1 && checkpoint_read_point == read_point) 
                found = 1;
            
            //Increment in case not found, otherwise it will reset to checkpoint
            j++;
        }
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

void cargs_load_args(const int argc, const char** argv)
{
    for(uint32_t i=1; i < argc; i++)
    {
        
    }
}