#pragma once

#include <stdint.h>
#include <stdbool.h>

enum cargs_error { 
    CARGS_NO_ERROR, 
    CARGS_NON_EXISTENT, 
    CARGS_MANDATORY,
    CARGS_NOT_ENOUGH_DATA, 
    CARGS_REDUNDANT_ARGUMENT
};

extern enum cargs_error cargs_error_code;
extern char* _cargs_error_buffer_str;

extern uint8_t _cargs_is_error_extended;
extern char* _cargs_error_argument;

extern const char* _cargs_error_strings[];
extern bool _cargs_treat_repeated_args_as_errors;
extern bool _cargs_treat_anonymous_args_as_errors;
extern bool _cargs_include_argument_zero;

/*
    Sets the error configuration that will stop cargs from working

    The error_core must be a cargs_error enum type to work
*/
void _cargs_declare_error(const char* error_arg, const uint8_t is_extended, const uint32_t error_code);