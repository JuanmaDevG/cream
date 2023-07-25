#include "utils.h"

/*
    ---------------------------
    Extended argument utilities
    ---------------------------
*/

void _push_extended_argument(
    const char* argument, const uint32_t associated_opt, const char* read_point, 
    const uint32_t vec_pos
) {
    _extended_args.args->associated_opt = associated_opt;
    _extended_args.args->read_point = read_point;

    size_t length = strlen(argument) +1;
    _extended_args.args[vec_pos].name = (char*)malloc(length);
    memcpy(_extended_args.args[vec_pos].name, argument, length);
}

/*
    ------------------------------
    Readpoint/writepoint utilities
    ------------------------------
*/

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
    -------------------------
    General purpose utilities
    -------------------------
*/

void _remove_redundancies(const enum _redundancy_remove_mode mode)
{
    if(_bool_args == NULL || _data_args == NULL) return;
    char const* read_point = NULL;
    size_t const* read_length = NULL;
    char* write_point = NULL;
    size_t* write_length = NULL;

    if(mode == REMOVE_BOOL_REDUNDANCIES)
    {
        read_point = _data_args;
        read_length = &(_data_packs.size);
        write_point = _bool_args;
        write_length = &_bool_args_count;
    }
    else    //REMOVE_DATA_REDUNDANCIES
    {
        read_point = _bool_args;
        read_length = &_bool_args_count;
        write_point = _data_args;
        write_length = &(_data_packs.size);
    }

    //Look for redundancies and remove them
    for(uint64_t i=0; i <= *read_length; i++)
    {
        for(uint64_t j=0; j <= *write_length; j++)
            if(read_point[i] == write_point[j]) 
                memcpy(write_point, write_point+1, (*write_length) - j);
    }

    //Does not reallocate because the null terminating character is copied and can be a waste of time
    //It is expected so, that the user does not write a lot of redundancies
}

uint8_t _find_argument_letter(const char argument_char, char const* read_point, uint32_t* checkpoint)
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

        if(read_point[j] == argument_char) //Finally found
        {
            found = 1;
            (*checkpoint) = j+1;
            checkpoint_read_point = read_point;
        }
        //Not found, and if looked everywhere, return
        else if(j == checkpoint -1 && checkpoint_read_point == read_point)
        {
            (*checkpoint) = 0;
            return found;
        }
        
        j++;
    }
}