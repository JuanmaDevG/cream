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
    uint32_t err_type;                                  //The enum error type
    char* arg_option;                                   //Argument option that causes the error (iformation can be obtained with: _cargs_valid_arg_options)
    bool is_extended;                                   //Is the option an extended option? (like: --force)
    char* plot_twist;                                   //Formatted string about why the error happened and how to solve it
} cargs_error;

extern uint32_t _cargs_error_count;                     //Contains the number of total errors declared by cargs
extern _expandable_stack _cargs_error_buffer;           //Contains the cargs_error objects that the error system caches during an argument load

//TODO: make a solution to obtain a specific error string based in the argument character that failed
extern char* _cargs_out_error_msg;                      //A string buffer where the output error messages are written (and separated by newline characters)

extern const char* _cargs_error_strings[];              //Contains the default error strings

extern bool _cargs_treat_anonymous_args_as_errors;      //True if anonymous arguments must be treated as errors
extern bool _cargs_include_argument_zero;               //True if the argument zero (in case of program input, the program name) should be included in the following argument load
extern bool _cargs_promise_first_ext_arg_char_is_ext_arg;//Promises to the cargs error system that the first extended argument char is the argument and no need to compare strings
extern bool _cargs_enable_multiple_opts_per_arg;        //Enables multiple data argument options per arg string (like: -abcd) and disables in-option data values (like: -L/mylib/dir/)

/*
    Pushes a new cargs_error to the error buffer
*/
void _cargs_declare_error(const uint32_t error_type, const char* error_arg, const bool is_extended, const char* optional_plot_twist);

/*
    Removes all the cached errors BUT DOES NOT CHANGE the error caching flags state.

    For example:
    If _cargs_treat_aonymous_args_as_errors is set to true, it will keep true
*/
void _cargs_reset_error_system();

/*
    Returns an array of errors
*/
cargs_error* _cargs_get_error_str();