#include "anon_arg_list.h"

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

inline void _cargs_push_list_node(
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

inline bool _cargs_delete_list_head(_cargs_anonymous_node** node, _cargs_anonymous_node** last)
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

inline const ArgPackage* _cargs_get_list_package(_cargs_anonymous_node* first_node, uint32_t position)
{
    if(!first_node) return NULL;
    _cargs_anonymous_node* iterator =  first_node;

    while(position > 0)
    {
        iterator = iterator->next;
        if(!iterator) return NULL;
        position--;
    }

    return (const ArgPackage*) &(iterator->package);
}

inline bool _cargs_free_data_list(_cargs_data_storage_list* list)
{
    if(!list) return false;
    if(!list->first_node) return false;

    while(list->first_node) _cargs_delete_list_head(&(list->first_node), &(list->last_node));
    return true;
}