/*
    ----------------------------------
    Anonymous data argument management
    ----------------------------------

    Anonymous data arguments are arguments that are not linked to any argument option, 
    for example:
    > program_name some_arg some_arg2 -f file.txt

    Then {some_arg, some_arg2} are anonymous arguments while {file.txt} is not an anonymous argument
    because it is linked to the argument option 'f'
*/

#pragma once

#include "shared_data.h"
#include <stdlib.h>

typedef struct _cargs_anonymous_node {
    ArgPackage package;
    struct _cargs_anonymous_node* next;
    struct _cargs_anonymous_node* previous;
} _cargs_anonymous_node;

typedef struct _cargs_data_storage_list {
    _cargs_anonymous_node* first_node;
    _cargs_anonymous_node* last_node;
} _cargs_data_storage_list;

void _cargs_push_list_node(
    _cargs_anonymous_node** first, _cargs_anonymous_node** last, const char** data_pointer, const uint32_t count
);

bool _cargs_delete_list_head(_cargs_anonymous_node** node, _cargs_anonymous_node** last);

const ArgPackage* _cargs_get_list_package(_cargs_anonymous_node* first_node, uint32_t position);

bool _cargs_free_data_list(_cargs_data_storage_list* list);