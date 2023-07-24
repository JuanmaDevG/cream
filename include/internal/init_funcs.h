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
void cargs_set_boolean_args(const char* arg_letters);

/*
    Associates an extended version of an argument letter to make the argument 
    parsing more verbose.

    Is more efficient for the library to receive the arg_letters string ordered as given, 
    otherwise the loop will be much longer
*/
void cargs_associate_extended(const char* arg_letters, ...);

/*
    Arguments that require data, so their extra arguments data will be 
    grouped.
*/
void cargs_set_data_args(const char* arg_letters);

/*
    Makes the letters given as parameter mandatory to use
*/
void cargs_make_mandatory(const char* arg_letters);

/*
    Loads the program arguments, checks them and sets the data pointers ready to use.
    After correctly using this function, you can use get functions

    WARNING: 
    This function must be called when the argument setters are called, otherwise
    will have no effect (but of course you can leave boolean or data arguments empty) 
*/
void cargs_load_args(const int argc, const char** argv);