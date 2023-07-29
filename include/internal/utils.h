#ifndef CARGS_UTILS_H
#define CARGS_UTILS_H

#include <stdlib.h>
#include <string.h>

#include "shared_data.h"

enum _reduncancy_remove_mode { REMOVE_BOOL_REDUNDANCIES, REMOVE_DATA_REDUNDANCIES, REMOVE_REDUNDANCIES_COUNT};

/*
    ---------------------------
    Extended argument utilities
    ---------------------------
*/

//Pushes an argument string to the shared extended argument vector
void _push_extended_argument(
    const char* argument, const uint32_t associated_opt, const char* read_point, 
    const uint32_t vec_pos
);

/*
    ------------------------------
    Readpoint/writepoint utilities
    ------------------------------
*/

/*
    Obtains a value for the _read_point from where to start to read/write information.

    It's priority order is:
    1. Boolean arguments
    2. Data need arguments

    If one argument type is not initialized, obtains the remaining type
*/
extern inline void _obtain_read_point();

/*
    Swaps the buffer _read_point to the next argument type read point 
    (boolean args -> data args... and viceversa)
*/
extern inline void _swap_read_point();

/*
    Gets the actual state of the read_point
*/
extern inline char* _get_actual_read_point();

/*
    Gets the actual state of the checkpoint
*/
extern inline uint32_t _get_actual_checkpoint();

/*
    Gets the actual state of the extended argument searcher 
    checkpoint
*/
extern inline uint32_t _get_actual_ext_checkpoint();

/*
    Resets the finder checkpoint and read_point
*/
extern inline void _reset_finders();

/*
    Resets the extended argument buffer finders

    Note:
    For the moment just one, there probably will be more in 
    later versions
*/
extern inline void _reset_ext_finders();


/*
    -----------------------
    Error control utilities
    -----------------------
*/

/*
    Sets the error configuration that will stop cargs from working

    The error_core must be a cargs_error enum type to work
*/
extern inline void _cargs_declare_error(const char* error_arg, const uint8_t is_extended, const uint32_t error_code);


/*
    -------------------------
    General purpose utilities
    -------------------------
*/

/*
    Removes redundant option letters

    The mode must be a _redunancy_remove_mode enum type
*/
void _remove_redundancies(const uint32_t mode);

/*
    Finds the given argument char looking into the argument buffers

    Returns zero if the argument has not been found, otherwise returns the 
    checkpoint (the next character position to the character found).

    This function works more efficient when the char array is written following 
    the argument order given.

    The found position would be -> checkpoint - 1
*/
uint32_t _find_argument_char(const char argument_char);

/*
    Finds the extended argument.
    The char pointer location must be the argument pointer plus the double 
    argument identificator offset, like:
    
    --some-extended-arg -> some-extended-arg (resulting argument without double id)

    Works with the library integrated checkpoint, so it is possible to check 
    it's value with _get_actual_checkpoint function.

    Returns 0 if not found and 1 if found
*/
uint8_t _find_extended_argument(const char* ext_arg);

/*
    Configures the argument data pointer to store the data position, 
    count, the number of data strings of the data argument and if there are 
    zero (or not enough) data strings throws an error leaving the pointers 
    to NULL and does nothing more.

    The extended_argument position pointer can be set to NULL if the data argument 
    found is not an extended arg, this way, the function will use the checkpoint 
    to determine the argument data location, respecting the argument buffers order.

    Also advances the argv index position to one before the next argument.
    It is one before the next because of iteration causes.
*/
uint8_t _add_argument_data(const char** argv, uint32_t* actual_position, const uint32_t* extended_argument_position);

/*
    Scans a non-extended argument string

    Supports multiple boolean argument values like -abcdf but if there is any data argument
    into the multi-boolean argument string, it will notify and throw a cargs_error
*/
uint8_t _read_non_extended_argument(const char** argv, uint32_t* index);


#endif