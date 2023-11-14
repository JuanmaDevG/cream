#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "shared_data.h"
#include "error_system.h"
#include "bit_vec_ops.h"


/*
    Simply returns true if the argument character has been declared on the available arguments 
    pointer vector (the arg has been configured and is part of the application).
*/
_cargs_argument* _find_argument_option(const char option_char);

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
_cargs_argument* _find_extended_argument(const char* ext_arg);

/*
    Adds a data package to the selected argument data package buffer starting from the argv starting read point.

    Returns the number of data pieces found.

    WARNING:
    This function stores errors into the error system buffers if the cargs state machine is configured to do it.
*/
uint32_t _add_argument_data(const int remaining_argc, const char** updated_argv, _cargs_argument* dst_arg);

/*
    Scans a non-extended argument string

    Supports multiple boolean argument values like -abcdf but if there is any data argument
    into the multi-boolean argument string, it will notify and throw a cargs_error

    Data arguments must come alone. If an example data argument option is a, the correct usage is:
    > program_name -a data1 data2 ...
*/
bool _read_non_extended_argument(const int argc, const char** argv, uint32_t* index);

/*
    Writes the argument limits (maximum or minimum depending on the write_point) into the 
    _cargs_maximum_data or _cargs_minimum_data, or any custom buffer.

    The limits are position dependent, so the numbers will be placed the same position as 
    the argument buffer.
*/
void _cargs_set_data_limit(const char* data_arg_string, va_list arg_limits, uint8_t* write_point);

/*
    Stores an anonymous argument package on the anonymous arguments linked list.
    If The anonymous arguments are separated by another argument (boolean or data arg with limits)
    cargs will store a new node in the anonymous argument linked list.

    EXAMPLE:
    Let's assume that "program-name" has an option -f that only allows one argument

    > program-name some-anonymous some-other -f file.txt boo foo bar
    The anonymous linked list will store:
    (some-anonymous, some-other)->(boo, foo, bar)
*/
void _cargs_store_anonymous_arguments(const int argc, const char** argv, uint32_t* arg_index);

/*
    Checks that mandatory arguments have been written within the program input.

    If any mandatory argument has not been checked, this function will return false and 
    declare a cargs_error in the cargs_error_code and it's parameters
*/
bool _cargs_check_mandatory_arguments();

/*
    Returns the position of the data after the equals operator found in an argument 
    option.

    If the opeartor is not found returns zero
*/
uint32_t _cargs_search_equals_operator(const char* argument_pointer);

/*
    Stores the equals operator data with it's associated option.

    WARNING:
    This is made overloading the _cargs_equals_operator_pointer_bank so when the 
    bank is full the function will return FALSE and no data will be added.

    It is recommended to use the argument with equals just once per defined argument:
    > program_name -f=file.txt -I=include_dir\ (DON't) -f=another_different_file.txt
*/
bool _cargs_store_equals_operator_data(const char* data_pointer, const uint32_t associated_option);

/*
    Searches the equals operator in the argument given and if found, stores the data into the argument 
    package vector using the pointer bank as nexus to directly point to the argument data.
*/
bool _cargs_configure_and_store_equals_operator_data(const char* arg_option, const uint32_t associated_option);

/*
    Free the memory blocks that have been relocated during getters 
    or free redundant arg data linked lists.

    WARNING:
    Does not set any pointer to NULL
*/
void _cargs_remove_redundant_args_linked_lists();

/*
    Frees the extended argument buffers and sets the pointers 
    NULL to let them be reused
*/
void _cargs_reset_ext_arg_buffers();

/*
    Frees the mandatory argument buffers and sets the pointers 
    to NULL to let them be reused
*/
void _cargs_reset_mandatory_arg_buffers();

/*
    Frees the error argument buffers and sets the pointers 
    to NULL to let them be reused
*/
void _cargs_reset_error_buffers();

/*
    Removes all the anonymous argument buffers cached after 
    any argument load
*/
void _cargs_remove_anonymous_arguments();