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


inline void _cargs_push_list_node(
    _cargs_anonymous_node** first, _cargs_anonymous_node** last, const char** data_pointer, const uint32_t count
);

inline bool _cargs_delete_list_head(_cargs_anonymous_node** node, _cargs_anonymous_node** last);

inline ArgPackage* _cargs_get_list_package(_cargs_anonymous_node* first_node, uint32_t position);