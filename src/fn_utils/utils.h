#pragma once

#include <stdarg.h>
#include <ctype.h>

#include "shared_data.h"
#include "error_system.h"
#include "bit_vec.h"

//TODO: this might get deleted
enum _cream_data_limit_config { CREAM_DATA_LIMIT_MAX, CREAM_DATA_LIMIT_MIN };

//TODO: probably refactor almost all the following functions

/*
    Adds a data package to the selected argument data package buffer starting from the point where the argument option was found

    Returns the number of data pieces found.

    This function stores errors into the error system buffers if the cream state machine is configured to do it.
*/
uint32_t _cream_add_argument_data(const int remaining_argc, const char** updated_argv, _cream_argument_option* dst_arg, const bool is_it_extended);

/*
    Scans a string looking for an argument option.
    If this option is a data option (non boolean) adds the data.

    This function will register the necessary errors following the state machine constraints configured by the user 
    of this library.

    Returns the number of argument values registered respect the updated_argv pointer (as an offset). Minimum returns one.
*/
uint32_t _cream_read_argument(const int updated_argc, const char** updated_argv);

/*
    Sets the data limit to the given options in the char array.

    Uses a _cream_data_limit_config to select maximum or minimum.
    If an option does not exist, cream will ignore it.
*/
void _cream_set_data_limit(const char* options_array, va_list arg_limits, uint8_t config_type);

/*
    Checks that mandatory arguments have been written within the program input.

    If any mandatory option is not present, it declares the error to the error system.
*/
void _cream_check_mandatory_arguments();

/*
    Checks that the data option arguments have not surpassed it's own data limits.

    Declares errors if needed so.
*/
void _cream_check_surpassed_data_bounds();