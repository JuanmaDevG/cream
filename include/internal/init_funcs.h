#pragma once

#include "utils.h"
#include "shared_data.h"    //To have more types (like u_int16_t) include sys/types.h


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
*/
void cargs_associate_extended(const char* arg_letters, const char** ext_names);

/*
    Arguments that require data, so their extra arguments data will be 
    grouped.
*/
void cargs_set_data_args(const char* arg_letters);