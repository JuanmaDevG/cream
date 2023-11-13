#pragma once

#include "exp_stack.h"
#include <string.h>


//Argument package associated to a single option
typedef struct _cargs_data_package {
    uint32_t count;
    char** values;          //Pointer to first data value position in argv
} _cargs_data_package;

typedef struct _cargs_argument_data {
    uint8_t maximum_data;               //Maximum data pieces supported
    uint8_t minimum_data;               //Minimum data pieces supported
    uint32_t actual_data;               //Actual data pieces that the argument contains (could be many more for argument options with no declared limits)
    _expandable_stack data;             //Contains the _cargs_data_packages and can be extracted with the expandable stack functions
} _cargs_argument_data;

typedef struct _cargs_argument {
    bool is_it_mandatory;                   //If true, the error system will offer the possibility to interpret as error
    bool is_it_redundant;                   //If true, the error system will offer the possibility to interpret as error
    _cargs_argument_data* data_container;   //Won't be null if the argument is a data argument
} _cargs_argument;

//Extended argument name associated to the corresponding argument properties
typedef struct {
    _cargs_argument* associated_argument;
    char* name;
} _cargs_extended_argument;


/*
    ------------------------
    Shared data declarations
    ------------------------
*/

extern char _arg_id;

#define INVALID_CHARS 33
#define ASCII_TABLE_SIZE (256 - INVALID_CHARS)
#define BIT_ASCII_TABLE_SIZE ((ASCII_TABLE_SIZE / 8) +1)

extern _cargs_argument* _cargs_available_args;                          //The memory block that contains the available argument options information
extern _cargs_argument_data* _cargs_available_args_data;                //The memory block that contains the input data from the available data argument options
extern _cargs_argument* _cargs_argument_options[ASCII_TABLE_SIZE];      //A list of pointers to the memory block that contains the information from available arguments

extern uint8_t _cargs_arg_confirmation_bits[BIT_ASCII_TABLE_SIZE];      //Confirms the existence of an argument option into the user input
extern uint8_t _cargs_is_arg_relocated_bits[BIT_ASCII_TABLE_SIZE];      //Confirms if the argument (that must exist in the input) has been relocated to extract it's data

extern uint32_t _cargs_bank_stack_pointer;
extern char** _cargs_equals_operator_pointer_bank;

extern size_t _cargs_ext_arg_count;                     //Stores the extended argument count
extern _cargs_extended_argument* _cargs_ext_args;       //Stores the pointer references to the actual character arguments (NOT BUFFER ALLOCATED, READ ONLY)

extern _expandable_stack _cargs_general_buffer;         //A buffer for general usage data


/*
    --------------------------------------------------
    Anonymous args and data limits on argument control
    --------------------------------------------------
*/

extern uint32_t _cargs_anon_arg_count;                  //Counts all the available anonymous arguments
extern _expandable_stack _cargs_anonymous_args;         //Contains _cargs_package objects that point to anonymous arguments groups
extern char** _cargs_anonymous_relocated_args;          //Extracts the buffer information to generate
