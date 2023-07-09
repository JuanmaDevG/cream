#pragma once

#include <stdlib.h>
#include <stdint.h>     //To have system types (like u_int16_t) include sys/types.h
#include <stdbool.h>
#include <string.h>


typedef struct ArgPackage{
    uint32_t count = 0;
    char** values = NULL; //Pointer to first value position in argv
} ArgPackage;

typedef struct ArgPackageVec {
    size_t size = 0;
    ArgPackage* packages = NULL;
} ArgPackageVec;


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
void cargs_associate_extended(const char** ext_names, const char* arg_letters);

/*
    Arguments that require data, so their extra arguments data will be 
    grouped.
*/
void cargs_set_data_args(const char* arg_letters);
