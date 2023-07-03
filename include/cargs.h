// Argument parser library in C

#ifndef CARGS_H
#define CARGS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct ArgPackage {
    u_int32_t count;
    char** values;
} ArgPackage;

typedef struct ArgPackageVec {
    size_t size;
    ArgPackage* arg_pckgs;
} ArgPackageVec;

// Sets the identificator character for arguments. If this function is 
// not called, by default is '-' like in unix-like systems.
void cargs_set_identificator(const char new_identificator);

// Sets how many boolean (existent or non existent) args and each_one's letter.
void cargs_set_boolean_args(const u_int32_t count, const char* arg_letters);

// Associates an extended version of an argument letter to make the argument 
// parsing more verbose.
void cargs_associate_extended(const char** ext_names, const char* arg_letters);

// Arguments that require data, so their extra arguments data will be 
// grouped.
void cargs_set_data_args(const u_int32_t count, const char* arg_letters);


#endif // CARGS_H