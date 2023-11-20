#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "shared_data.h"
#include "error_system.h"
#include "bit_vec_ops.h"

enum _cargs_data_limit_config { CARGS_DATA_LIMIT_MAX, CARGS_DATA_LIMIT_MIN };


/*
    Returns true if the character is configured as an argument identificator
*/
bool _cargs_find_argument_id(const char arg_id);

/*
    Returns the pointer to the argument option data structure if it is available, 
    otherwise returns NULL
*/
_cargs_argument* _cargs_find_argument_option(const char option_char);

/*
    Sets the given argument pointer to the cargs valid options
*/
bool _cargs_set_option_pointer(const char option_char, const _cargs_argument* p_arg);

/*
    Finds the extended argument.
    The char pointer location must be the argument pointer plus the double 
    argument identificator offset, like:
    
    --some-extended-arg -> (pass this) some-extended-arg (resulting argument without double id)

    It is highly recommended for performance that the string first character is equal (first char not case sensitive) 
    to the extended arg's first character, otherwise the finder will have to look into the entire extended arguments 
    buffer.

    Returns true if found, otherwise false
*/
_cargs_argument* _cargs_find_extended_argument(const char* ext_arg);

/*
    Adds a data package to the selected argument data package buffer starting from the point where the argument option was found

    Returns the number of data pieces found.

    This function stores errors into the error system buffers if the cargs state machine is configured to do it.
*/
uint32_t _cargs_add_argument_data(const int remaining_argc, const char** updated_argv, _cargs_argument* dst_arg, const bool is_it_extended);

/*
    Scans a string looking for an argument option.
    If this option is a data option (non boolean) adds the data.

    This function will register the necessary errors following the state machine constraints configured by the user 
    of this library.

    Returns the number of argument values registered respect the updated_argv pointer (as an offset). Minimum returns one.
*/
uint32_t _cargs_read_argument(const int updated_argc, const char** updated_argv);

/*
    Sets the data limit to the given options in the char array.

    Uses a _cargs_data_limit_config to select maximum or minimum.
    If an option does not exist, cargs will ignore it.
*/
void _cargs_set_data_limit(const char* options_array, va_list arg_limits, uint8_t config_type);

/*
    Checks that mandatory arguments have been written within the program input.

    If any mandatory option is not present, it declares the error to the error system.
*/
void _cargs_check_mandatory_arguments();