#include "init_funcs.h"

/*
    User hided functions
*/

void _push_extended_argument(
    const char* argument, const uint32_t associated_opt, const char* read_point, 
    const uint32_t vec_pos
) {
    _extended_args.args->associated_opt = associated_opt;
    _extended_args.args->read_point = read_point;

    _extended_args.args[vec_pos].name = (char*)malloc(strlen(argument) +1);
    strcpy(_extended_args.args[vec_pos].name, argument);
}

//Configure read point (first go bool arguments)
inline char* _obtain_read_point()
{
    char* read_point = _bool_args;
    if(read_point == NULL) read_point = _data_args;
    return read_point;
}

inline void _swap_read_point(char* read_point)
{
    if(_bool_args == NULL) read_point = _data_args;
    else if(_data_args == NULL) read_point = _bool_args;
    else    //Both not null
    {
        if(read_point == _bool_args) read_point = _data_args;
        else //is _data_args
            read_point = _bool_args;
    }
}

/*
    User visible functions
*/

//
void cargs_set_identificator(const char new_id) { arg_id = new_id; }

void cargs_set_boolean_args(const char* arg_letters) {
    if(arg_letters == NULL) return;
    _bool_args_count = strlen(arg_letters);

    _bool_args = (char*)malloc(_bool_args_count +1);
    memcpy(_bool_args, arg_letters, (_bool_args_count * sizeof(char)) +1);
    remove_redundancies(_data_args, &(_data_packs.size), _bool_args, &_bool_args_count);

    //Calculating the bits for bit boolean vector
    _bool_args_bit_vec = (uint8_t*)calloc(
        (_bool_args_count >> 3) + (size_t)(_bool_args_count & (size_t)0b111 == 0 ? 0 : 1),
        sizeof(uint8_t)
    );
}

void cargs_associate_extended(const char* arg_letters, ...) {
    char* read_point = _obtain_read_point(); //To be _bool_args or _data_args
    if(read_point == NULL) return;

    size_t length = strlen(arg_letters);
    va_list arg_l;
    va_start(arg_l, length);
    _extended_args.size = length;
    _extended_args.args = (ExtArg*)calloc(length, sizeof(ExtArg));

    uint32_t checkpoint = 0;
    for(uint32_t i=0; i < length; i++)
    {
        uint8_t found = 0;
        uint32_t j = checkpoint;
        char* checkpoint_read_point = read_point;
        while(found == 0)
        {
            if(read_point[j] == '\0')
            {
                j = 0;
                _swap_read_point(read_point);
            }

            //Change the read point
            if(read_point[j] == arg_letters[i])
            {
                found = 1;
                checkpoint = j+1;
                checkpoint_read_point = read_point;
                _push_extended_argument(va_arg(arg_l, char*), j, read_point, i);
            }
            if(j == checkpoint -1 && checkpoint_read_point == read_point) 
                found = 1; //Found or not, we've done the two argument strings
            
            //Increment in case not found, otherwise it will reset to checkpoint
            j++;
        }
    }
}

void cargs_set_data_args(const char* arg_letters) {
    if(!arg_letters) return;
    _data_packs.size = strlen(arg_letters);

    _data_args = (char*)malloc(_data_packs.size +1);
    memcpy(_data_args, arg_letters, _data_packs.size +1);
    remove_redundancies(_bool_args, _bool_args_count,_data_args, &(_data_packs.size));
    _data_packs.packages = (ArgPackage*)calloc(_data_packs.size, sizeof(ArgPackage));

    _data_args_bit_vec = (uint8_t*)calloc(
        (_data_packs.size >> 3) + (size_t)(_data_packs.size & (size_t)0b111 == 0 ? 0 : 1),
        sizeof(uint8_t)
    );
}
