#pragma once

#include "shared_data.h"

enum cargs_error_type { 
    CARGS_NO_ERROR, 
    CARGS_NON_EXISTENT, 
    CARGS_MANDATORY,
    CARGS_NOT_ENOUGH_DATA, 
    CARGS_REDUNDANT_ARGUMENT
};

typedef struct {
    cargs_error_type err_type;                          //Activates the defatult error type
    char* arg_option;                                   //Argument option that causes the error (iformation can be obtained with: _cargs_valid_arg_options)
    bool is_extended;                                   //Is the option an extended option? (like: --force)
    char* plot_twist;                                   //Formatted string about why the error happened and how to solve it
} cargs_error;

extern _expandable_stack _cargs_error_buffer;           //Contains the cargs_error objects that the error system caches during an argument load

//TODO: make a solution to obtain a specific error string based in the argument character that failed
extern char* _cargs_out_error_msg;                      //A string buffer where the output error messages are written (and separated by newline characters)

extern const char* _cargs_error_strings[];              //Contains the default error strings

extern char* _cargs_repeated_args_that_are_errors;      //Contains the argument characters that should be interpreted as errors
extern bool _cargs_treat_anonymous_args_as_errors;      //True if anonymous arguments must be treated as errors
extern bool _cargs_include_argument_zero;               //True if the argument zero (in case of program input, the program name) should be included in the following argument load

/*
    Pushes a new cargs_error to the error buffer
*/
void _cargs_declare_error(const uint32_t error_type, const char* error_arg, const bool is_extended, const char* optional_plot_twist);