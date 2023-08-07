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
    _extended_args.args[vec_pos].associated_opt = associated_opt;
    _extended_args.args[vec_pos].read_point = (char*)read_point;

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

inline uint32_t _get_actual_ext_checkpoint() { return _extended_checkpoint; }

inline void _reset_finders() { _checkpoint = 0; _read_point = NULL; }

inline void _reset_ext_finders() { _extended_checkpoint = 0; }

/*
    -----------------------
    Error control utilities
    -----------------------
*/

inline void _cargs_declare_error(const char* error_arg, const uint8_t is_extended, const uint32_t error_code)
{
    _cargs_error_argument = (char*)error_arg;
    _cargs_is_extended = is_extended;
    cargs_error_code = error_code;
}


/*
    -------------------------
    General purpose utilities
    -------------------------
*/

void _remove_redundancies(const uint32_t mode)
{
    if(_bool_args == NULL || _data_args == NULL) return;
    char const* read_point = NULL;
    size_t* read_length = NULL;
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
            {
                memcpy(write_point +j, write_point +j +1, (*write_length) - j);
                (*write_length)--;
                j--;
            }
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
        if(
            (j == _checkpoint -1 && checkpoint_read_point == _read_point) ||                        //Checkpoint is not zero
            (_checkpoint == 0 && _read_point[j+1] == '\0' && _read_point != checkpoint_read_point)  //Checkpoint is zero
        ) {
            _reset_finders();
            return _checkpoint;
        }
        
        j++;
    }
}

uint8_t _find_extended_argument(const char* ext_arg)
{
    if(_extended_args.size == 0 || ext_arg == NULL) return 0;

    uint32_t j = _extended_checkpoint;
    while(1)
    {
        if(j == _extended_args.size) j = 0; //When j surpasses the vector limit

        if(_extended_args.args[j].name != NULL && strcmp(_extended_args.args[j].name, ext_arg) == 0)
        {
            _extended_checkpoint = j+1;
            return 1;
        }
        
        //Not found, and if we're done with all the vector, reset and return
        if(
            j == _extended_checkpoint -1 ||
            (_extended_checkpoint == 0 && j == _extended_args.size -1)
        ) {
            _reset_ext_finders();
            return 0;
        }

        j++;
    }
}

bool _add_argument_data(const int argc, const char* argv[], uint32_t* index, const uint32_t* ext_arg_position)
{
    uint8_t is_extended = (ext_arg_position == NULL ? 0 : 1);
    const uint32_t pointer_offset = (*index) +1;

    const char** data_pointer = argv + pointer_offset;
    uint32_t count = 0;
    uint32_t associated_option = 
        (!is_extended ? _get_actual_checkpoint() -1 : _extended_args.args[*ext_arg_position].associated_opt);

    while(
        count + pointer_offset < (uint32_t)argc && data_pointer[count][0] != _arg_id
    ) {
        if(_cargs_maximum_data[associated_option] > 0 && count == (uint32_t)_cargs_maximum_data[associated_option]) break;
        count++;
    }

    if(count == 0 || count < _cargs_minimum_data[associated_option])
    {
        _cargs_declare_error(argv[(*index)] + (is_extended ? 2 : 1), is_extended, CARGS_NOT_ENOUGH_DATA);
        return false;
    }

    //There is no read_point association because _data_packs belong to _data_args read point
    _data_packs.packages[associated_option].count = count;
    _data_packs.packages[associated_option].values = (count == 0 ? NULL : (char**)data_pointer);

    //Set offset to argument iterator
    (*index) += count;

    return true;
}

uint8_t _read_non_extended_argument(const int argc, const char* argv[], uint32_t* index)
{
    for(uint32_t j=1; argv[(*index)][j] != '\0'; j++)
    {
        if(_find_argument_char(argv[(*index)][j]))
        {
            if(_get_actual_read_point() == _data_args)
            {
                if(j == 1 && argv[(*index)][2] == '\0')
                {
                    if(!_add_argument_data(argc, argv, index, NULL)) return 0;
                }
                else
                {
                    _cargs_declare_error(argv[(*index)] +j, 0, CARGS_MULTI_BOOL_ARG_ISSUE);
                    return 0;
                } 
            }

            _get_actual_read_point()[_get_actual_checkpoint() -1] = '\\';
        }
        else
        {
            _cargs_declare_error(argv[(*index)] +j, 0, CARGS_NON_EXISTENT);
            return 0;
        }
    }

    return 1;
}

inline void _cargs_set_data_limit(const char* data_arg_string, const uint32_t length, va_list arg_limits, uint8_t* write_point)
{
    for(uint32_t i=0; i < length; i++)
    {
        if(_find_argument_char(data_arg_string[i])) 
            write_point[i] = (uint8_t)va_arg(arg_limits, int);
    }
}

void _cargs_store_anonymous_arguments(const int argc, const char* argv[], uint32_t* arg_index)
{
    uint32_t count = 1;                         //We know this position contains anonymous arg
    const uint32_t anon_arg_pack_position = (*arg_index);  //Store the first position for the anonymous list node
    (*arg_index)++;
    while((*arg_index) < (uint32_t)argc && argv[(*arg_index)][0] != _arg_id) //Count anonymous arguments
    {
        count++;
        (*arg_index)++;
    }
    (*arg_index)--;     //Next iteration, i variable will increment
    _cargs_push_anon_node(argv + anon_arg_pack_position, count);
}

bool _cargs_check_mandatory_arguments()
{
    for(uint32_t i=0; i < _mandatory_arg_count; i++)
    {
        if(_mandatory_args[i].read_point != NULL)
            if(_mandatory_args[i].read_point[_mandatory_args[i].position] != '\\')
            {
                _cargs_declare_error(
                    _mandatory_args[i].read_point + _mandatory_args[i].position,
                    0, CARGS_MANDATORY
                );
                return false;
            }
    }

    return true;
}