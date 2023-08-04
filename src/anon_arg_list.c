#include "anon_arg_list.h"


inline _cargs_anonymous_list* fill_node(
    _cargs_anonymous_list* previous, _cargs_anonymous_list* next, const uint32_t argument_count, char** data_location
) {
    _cargs_anonymous_list* target = (_cargs_anonymous_list*)malloc(sizeof(_cargs_anonymous_list));
    target->previous = previous;
    target->next = next;
    target->package.count = argument_count;
    target->package.values = data_location;
}

//------------------------------------------------------------------------------------------

inline void _cargs_push_anon_node(char** data_location, const uint32_t argument_count)
{
    //is a new list
    if(!_cargs_anon_args)
    {
        _cargs_anon_args = fill_node(NULL, NULL, argument_count, data_location);
        _cargs_anon_last = _cargs_anon_args;
        return;
    }

    _cargs_anon_last->next = fill_node(_cargs_anon_last, NULL, argument_count, data_location);
    _cargs_anon_last = _cargs_anon_last->next;
}

inline const ArgPackage* _cargs_get_anon_package(uint32_t position)
{
    if(!_cargs_anon_args) return NULL;
    _cargs_anonymous_list* iterator = _cargs_anon_args;

    while(position > 0)
    {
        iterator = iterator->next;
        position--;
    }

    return (const ArgPackage*) &(iterator->package);
}

inline void _cargs_delete_head_node()
{
    if(!_cargs_anon_args) return;

    _cargs_anonymous_list* next = _cargs_anon_args->next;
    free(_cargs_anon_args);
    _cargs_anon_args = next;
    if(_cargs_anon_args) _cargs_anon_args->previous = NULL;
    else _cargs_anon_last = NULL;
}

inline void _cargs_free_anon_list()
{
    while(_cargs_anon_args) _cargs_delete_head_node();
}