#pragma once

#include "shared_data.h"

enum cream_error_type { 
    CREAM_NO_ERROR, 
    CREAM_NON_EXISTENT, 
    CREAM_MANDATORY,
    CREAM_NOT_ENOUGH_DATA, 
    CREAM_REDUNDANT_ARGUMENT,
    CREAM_REDUNDANT_OPTION, 
    CREAM_NOT_ENOUGH_MEMORY, 
    CREAM_TOO_MUCH_DATA, 
    CREAM_ARGUMENTS_NOT_BOUND_TO_ANY_OPTION
};

typedef struct {
    uint32_t err_type;                                  //The enum error type
    char* arg_option;                                   //Argument option that causes the error (iformation can be obtained with: _cream_valid_arg_options)
    bool is_extended;                                   //Is the option an extended option? (like: --force)
    char* plot_twist;                                   //Formatted string about why the error happened and how to solve it
} cream_error;

extern uint32_t _cream_error_count;                     //Contains the number of total errors declared by cream
extern _expandable_stack _cream_error_buffer;           //Contains the cream_error objects that the error system caches during an argument load

//TODO: make a solution to obtain a specific error string based in the argument character that failed
extern char* _cream_out_error_msg;                      //A string buffer where the output error messages are written (and separated by newline characters)

extern const char* _cream_error_strings[];              //Contains the default error strings
extern const uint32_t _cream_error_lengths[];           //Contains the lengths

extern bool _cream_treat_anonymous_args_as_errors;      //True if anonymous arguments must be treated as errors
extern bool _cream_include_argument_zero;               //True if the argument zero (in case of program input, the program name) should be included in the following argument load
extern bool _cream_promise_first_ext_arg_char_is_ext_arg;//Promises to the cream error system that the first extended argument char is the argument and no need to compare strings
extern bool _cream_enable_multiple_opts_per_arg;        //Enables multiple data argument options per arg string (like: -abcd) and disables in-option data values (like: -L/mylib/dir/)

/*
    Pushes a new cream_error to the error buffer
*/
void _cream_declare_error(const uint32_t error_type, const char* error_arg, const bool is_extended, const char* optional_plot_twist);

/*
    Removes all the cached errors BUT DOES NOT CHANGE the error caching flags state.

    For example:
    If _cream_treat_aonymous_args_as_errors is set to true, it will keep true
*/
void _cream_reset_error_system();

/*
    Returns an array of errors
*/
cream_error* _cream_get_error_str();