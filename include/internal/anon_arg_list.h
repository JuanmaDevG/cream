#pragma once

#include "shared_data.h"

/*
    Pushes a new node to the anonymous argument linked list.
    Each node can store a data argument location in argv with the number 
    of anonymous arguments that contains that package.

    EXAMPLE:
    program_name anon_1 anon_2 anon_3 -f file.txt
    anon_1 to anon_3 will be stored in the first anonymous list node
*/
extern inline void _cargs_push_anon_node(char** data_location, const uint32_t argument_count);

/*
    Returns the argument package const pointer into the linked list location 
    given as parameter.
*/
extern inline const ArgPackage* _cargs_get_anon_package(uint32_t position);

/*
    Deletes the head node and if it is the last one, sets everything to NULL
*/
extern inline void _cargs_delete_head_node();

/*
    Deletes all the linked list nodes directly
*/
extern inline void _cargs_free_anon_list();