#pragma once

#include <stdlib.h>

#include "shared_data.h"

/*
    --------------------------------
    Anonymous data argument managing
    --------------------------------

    Anonymous data arguments are arguments that are not linked to any argument option, 
    for example:
    > program_name some_arg some_arg2 -f file.txt

    Then {some_arg, some_arg2} are anonymous arguments while {file.txt} is not an anonymous argument
    because it is linked to the argument option 'f'
*/


void _cargs_push_list_node(
    _cargs_anonymous_node** first, _cargs_anonymous_node** last, const char** data_pointer, const uint32_t count
);

bool _cargs_delete_list_head(_cargs_anonymous_node** node, _cargs_anonymous_node** last);

const ArgPackage* _cargs_get_list_package(_cargs_anonymous_node* first_node, uint32_t position);

bool _cargs_free_data_list(_cargs_data_storage_list* list);