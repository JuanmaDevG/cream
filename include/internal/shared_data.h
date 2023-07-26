#pragma once

#include <stdint.h>

/*
    Argument packages
*/

//Argument package associated to a single option
typedef struct ArgPackage{
    uint32_t count;
    char** values;          //Pointer to first value position in argv
} ArgPackage;

//Argument Package Vector that contains mutiple Argument Packages
typedef struct ArgPackageVec {
    size_t size;
    ArgPackage* packages;
} ArgPackageVec;


/*
    Extended argument structures
*/

//Extended argument name associated to the corresponding char argument argument
typedef struct {
    char* read_point;
    uint32_t associated_opt; //Option symbol position (to look into bit vectors and ArgPackage vectors)
    char* name;
} ExtArg;

//Extended Argument Vector that contains multiple extended arguments
typedef struct {
    size_t size;
    ExtArg* args;
} ExtArgVec;


/*
    Shared data declarations
*/

extern char _arg_id;

extern size_t _bool_args_count;
extern char* _bool_args;

extern char* _data_args;
extern ArgPackageVec _data_packs;

extern ExtArgVec _extended_args;

extern char* _mandatory_arguments;

/*
    Error managing declarations
*/

enum cargs_error { CARGS_NO_ERROR, CARGS_NON_EXISTENT, CARGS_MANDATORY, CARGS_WRONG_ID };

extern char* _cargs_error_buffer_str;

extern uint8_t _cargs_is_extended;
extern char* _cargs_error_argument;

extern const char* _cargs_error_strings[];

/*
    Utility pointers
*/

extern char* _read_point;
extern uint32_t _checkpoint;