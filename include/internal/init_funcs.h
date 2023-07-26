#pragma once

#include <stdarg.h>

#include "utils.h"
#include "shared_data.h"


/*
    Sets the identificator character for arguments. If this function is 
    not called, by default is '-' like in unix-like systems.
*/
void cargs_set_identificator(const char new_identificator);

/*
    Sets how many boolean (existent or non existent) args and each_one's letter.
*/
void cargs_set_boolean_args(const char* arg_characters);

/*
    Associates an extended version of an argument letter to make the argument 
    parsing more verbose.

    Is more efficient for the library to receive the arg_characters string ordered as given, 
    otherwise the loop will be much longer
*/
void cargs_associate_extended(const char* arg_characters, ...);

/*
    Arguments that require data, so their extra arguments data will be 
    grouped.
*/
void cargs_set_data_args(const char* arg_characters);

/*
    Makes the characters given as parameter mandatory to use

    This makes cargs to throw an error code while trying to execute 
    cargs_load_args function and the specified arguments are not found
*/
void cargs_make_mandatory(const char* arg_characters);

/*
    Loads the program arguments, checks them and sets the data pointers ready to use.
    After correctly using this function, you can use get functions.

    If any argument error is given, it will return the argument

    WARNING: 
    This function must be called when the argument setters are called, otherwise
    will have no effect (but of course you can leave boolean or data arguments empty) 
*/
uint32_t cargs_load_args(const int argc, const char** argv);

/*
    Gets the pointer to a loaded error string
*/
const char* cargs_get_error(uint32_t err_code);