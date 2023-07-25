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
    Obtains a char pointer read point from where to start to read/write information.

    It's priority order is:
    1. Boolean arguments
    2. Data need arguments

    If one argument type is not initialized, obtains the remaining type
*/
inline char* _obtain_read_point();

/*
    Swaps a char pointer to the next argument type read point (boolean args -> data args... and viceversa)
*/
inline void _swap_read_point(char* read_point);


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
    Finds the given argument letter looking into the argument letter buffers

    If the argument letter is not found, the checkpoint will be set to zero
    otherwise the checkpoint will be set to the position next to where the 
    argument character was found.

    Returns zero if the argument has not been found, otherwise return 1
*/
uint8_t _find_argument_letter(const char argument_char, char const* read_point, uint32_t* checkpoint);