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

inline void _obtain_read_point()
{
    _read_point = _bool_args;
    if(_read_point == NULL) _read_point = _data_args;
    return _read_point;
}

inline void _swap_read_point()
{
    if(_bool_args == NULL) _read_point = _data_args;
    else if(_data_args == NULL) _read_point = _bool_args;
    else    //Both not null
    {
        if(_read_point == _bool_args) _read_point = _data_args;
        else //is _data_args
            _read_point = _bool_args;
    }
}

inline char* _get_actual_read_point() { return _read_point; }

inline uint32_t _get_actual_checkpoint() { return _checkpoint; }

inline void _reset_finders() { _checkpoint = 0; _read_point = NULL; }

/*
    -----------------------
    Error control utilities
    -----------------------
*/

inline enum cargs_error _is_argument_wrong(const char* actual_argument)
{
    if(actual_argument[0] != _arg_id)
    {
        _cargs_error_argument = actual_argument;
        _cargs_is_extended = 1; //true to print the whole wrong argument
        return CARGS_WRONG_ID;
    }
    return CARGS_NO_ERROR;
}

inline enum cargs_error _does_extended_arg_exist(const char* actual_argument)
{
    if(_find_extended_argument(actual_argument + 2) == 0)
    {
        _cargs_error_argument = actual_argument;
        _cargs_is_extended = 1;
        return CARGS_NON_EXISTENT;
    }
    return CARGS_NO_ERROR;
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

uint32_t _find_argument_char(const char argument_char)
{
    if(_read_point == NULL)
    {
        _obtain_read_point();
        //If read_point still null, cannot find anything
        if(_read_point == NULL) return 0;
    }

    uint32_t j = _checkpoint;
    char* checkpoint_read_point = _read_point;
    while(1)
    {
        //Swap read_point beacause finished actual
        if(_read_point[j] == '\0')
        {
            j = 0;
            _swap_read_point();
        }

        if(_read_point[j] == argument_char) //Finally found
        {
            _checkpoint = j+1;
            checkpoint_read_point = _read_point;
            return _checkpoint;
        }
        //Not found, and if looked everywhere, return
        else if(j == _checkpoint -1 && checkpoint_read_point == _read_point) 
        {
            _reset_finders();
            return _checkpoint;
        }
        
        j++;
    }
}

uint8_t _find_extended_argument(const char* ext_arg)
{
    if(_extended_args.size == 0 || ext_arg == NULL) return 0;

    uint32_t j = _checkpoint;
    while(1)
    {
        if(j == _extended_args.size -1) j = 0;

        if(strcmp(_extended_args.args[j].name, ext_arg) == 0)
        {
            _checkpoint = j+1;
            return 1;
        }
        //Not found, and if we're done with all the vector, reset and return
        else if(j == _checkpoint -1)
        {
            _checkpoint = 0;
            return 0;
        }

        j++;
    }
}

void _add_argument_data(const char** argv, uint32_t* index, const uint32_t ext_arg_position)
{
    const char** data_pointer = argv + (*index) + 1;
    uint32_t count = 0;
    while(data_pointer[count][0] != _arg_id) count++;
    _data_packs.packages[_extended_args.args[ext_arg_position].associated_opt].count = count;
    _data_packs.packages[_extended_args.args[ext_arg_position].associated_opt].values = (count == 0 ? NULL : data_pointer);

    //Set offset to argument iterator
    (*index) += count;
}