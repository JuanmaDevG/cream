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


/*
    Pushes a new node to the anonymous argument linked list.
    Each node can store a data argument location in argv with the number 
    of anonymous arguments that contains that package.

    EXAMPLE:
    program_name anon_1 anon_2 anon_3 -f file.txt
    anon_1 to anon_3 will be stored in the first anonymous list node
*/
extern inline void _cargs_push_anon_node(const char** data_location, const uint32_t anon_argument_count);

/*
    Returns the argument package const pointer into the linked list location 
    given as parameter.
*/
extern inline const ArgPackage* _cargs_get_anon_package(uint32_t position);

/*
    Deletes the head node and if it is the last one, sets everything to NULL
*/
extern inline void _cargs_pop_anon_head_node();

/*
    Deletes all the linked list nodes directly
*/
extern inline void _cargs_free_anon_list();


/*
    ------------------------------------------
    Redundant argument data managing functions
    ------------------------------------------
*/

extern inline void _cargs_push_redundant_argument(const uint32_t associated_option, const char** data_pointer, const uint32_t count);

extern inline bool _cargs_pop_redundant_head_node(const uint32_t associated_option);