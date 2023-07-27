#pragma once

#include <stdlib.h>
#include <stdint.h>
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
inline void _obtain_read_point();

/*
    Swaps the buffer _read_point to the next argument type read point 
    (boolean args -> data args... and viceversa)
*/
inline void _swap_read_point();

/*
    Gets the actual state of the read_point
*/
inline char* _get_actual_read_point();

/*
    Gets the actual state of the checkpoint
*/
inline uint32_t _get_actual_checkpoint();

/*
    Gets the actual state of the extended argument searcher 
    checkpoint
*/
inline uint32_t _get_actual_ext_checkpoint();

/*
    Resets the finder checkpoint and read_point
*/
inline void _reset_finders();

/*
    Resets the extended argument buffer finders

    Note:
    For the moment just one, there probably will be more in 
    later versions
*/
inline void _reset_ext_finders();


/*
    -----------------------
    Error control utilities
    -----------------------
*/

/*
    Returns CARGS_WRONG_ID if the argument has wrong syntax, otherwise returns 
    CARGS_OK if everything goes well
*/
inline enum cargs_error _is_argument_wrong(const char* actual_argument);

/*
    If the extended argument is not found, returns CARGS_NON_EXISTENT
*/
inline enum cargs_error _does_extended_arg_exist(const char* actual_argument);


/*
    -------------------------
    General purpose utilities
    -------------------------
*/

/*
    Removes redundant option letters
*/
void _remove_redundancies(const enum _redundancy_remove_mode mode);

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
    counts the number of data strings of the data argument and if there are 
    zero data strings, sets the pointer to NULL.

    Also advances the argv index position to one before the next argument.
    It is one before the next because of iteration causes.
*/
void _add_argument_data(const char** argv, uint32_t* actual_position, const uint32_t extended_argument_position);