#pragma once

#include "shared_data.h"

/**
 * Tells if the given character is set as an argument option identificator
*/
bool _cream_find_argument_id(const char arg_id);

/**
 * Returns the pointer to the argument option data structure if it is available, 
 * otherwise returns NULL
*/
_cream_argument_option* _cream_get_arg_option(const char option_char);

/*
    Finds the extended argument option.
    The char pointer location must be the argument pointer plus the double 
    argument identificator offset, like:
    
    --some-extended-arg -> (pass this) some-extended-arg (resulting argument without double id)

    Returns true if found, otherwise false
*/
_cream_argument_option* _cream_find_ext_arg_option(const char* ext_arg);