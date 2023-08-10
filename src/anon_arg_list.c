#include "anon_arg_list.h"
#include <stdio.h> //MOD

static inline _cargs_anonymous_node* fill_node(
    _cargs_anonymous_node* previous, _cargs_anonymous_node* next, const uint32_t argument_count, const char** data_location
) {
    _cargs_anonymous_node* target = (_cargs_anonymous_node*)malloc(sizeof(_cargs_anonymous_node));
    target->previous = previous;
    target->next = next;
    target->package.count = argument_count;
    target->package.values = (char**)data_location;

    return target;
}

static inline bool delete_head_node(_cargs_anonymous_node** node, _cargs_anonymous_node** last)
{
    if(!(node && last)) return false;
    _cargs_anonymous_node* next = (*node)->next;
    free(*node);
    (*node) = next;
    if(*node) (*node)->previous = NULL;
    else //Not node
        (*last) = NULL;
    return true;
}

static inline void push_node(
    _cargs_anonymous_node** first, _cargs_anonymous_node** last, const char** data_pointer, const uint32_t count
) {
    //Is a new list
    if(!(*first))
    {
        (*first) = fill_node(NULL, NULL, count, data_pointer);
        (*last) = (*first);
        return;
    }

    (*last)->next = fill_node((*last), NULL, count, data_pointer);
    (*last) = (*last)->next;
}


//------------------------------------------------------------------------------------------
// Anonymous arguments managing functions
//------------------------------------------------------------------------------------------

inline void _cargs_push_anon_node(const char** data_location, const uint32_t anon_argument_count)
{
    _cargs_anon_arg_count += anon_argument_count;
    push_node(&_cargs_anon_args, &_cargs_anon_last, data_location, anon_argument_count);
}

inline const ArgPackage* _cargs_get_anon_package(uint32_t position)
{
    if(!_cargs_anon_args) return NULL;
    _cargs_anonymous_node* iterator = _cargs_anon_args;

    while(position > 0)
    {
        iterator = iterator->next;
        if(iterator == NULL) return NULL;
        position--;
    }

    return (const ArgPackage*) &(iterator->package);
}

inline void _cargs_pop_anon_head_node()
{
    _cargs_anon_arg_count -= (_cargs_anon_args ? _cargs_anon_args->package.count : 0);
    delete_head_node(&_cargs_anon_args, &_cargs_anon_last);
}

inline void _cargs_free_anon_list()
{
    while(_cargs_anon_args) _cargs_pop_anon_head_node();
}


//------------------------------------------------------------------------------------------
// Redundant data argument managing functions
//------------------------------------------------------------------------------------------

inline void _cargs_push_redundant_argument(const uint32_t associated_option, const char** data_pointer, const uint32_t count)
{
    push_node(
        &(_cargs_redundant_arguments[associated_option].first_node), 
        &(_cargs_redundant_arguments[associated_option].last_node), 
        data_pointer, 
        count
    );
}

inline bool _cargs_pop_redundant_head_node(const uint32_t associated_option)
{
    return delete_head_node(
        &(_cargs_redundant_arguments[associated_option].first_node),
        &(_cargs_redundant_arguments[associated_option].last_node)
    );
}