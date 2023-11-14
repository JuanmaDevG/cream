#pragma once

#include "exp_stack.h"
#include <string.h>


//Argument package associated to a single option
typedef struct _cargs_data_package {
    uint32_t count;
    char** values;          //Pointer to first data value position in argv
} _cargs_data_package;

typedef struct _cargs_argument_data {
    uint8_t maximum_data_count;         //Maximum data pieces supported
    uint8_t minimum_data_count;         //Minimum data pieces supported
    uint32_t actual_data_count;         //Actual string data pieces that the argument contains (could be many more for argument options with no declared limits)
    _expandable_stack data;             //Contains the _cargs_data_packages and can be extracted with the expandable stack functions
    char** data_relocation_buffer;      //If not NULL, means that the expandable stack has been erased to move all the data string pointers to this buffer
} _cargs_argument_data;

typedef struct _cargs_argument {
    bool is_it_mandatory;                   //If true, the error system will offer the possibility to interpret as error
    bool is_it_redundant;                   //If true, the error system will offer the possibility to interpret as error
    _cargs_argument_data* data_container;   //Won't be null if the argument is a data argument
    char* extended_version;                 //Pointer to extended version: {-f -> --force} (without '-', that's only the id, sintactically double for extended)
} _cargs_argument;


/*
    ------------------------
    Shared data declarations
    ------------------------
*/

extern char _arg_id;

#define INVALID_CHARS 33
#define ASCII_TABLE_SIZE (256 - INVALID_CHARS)
#define BIT_ASCII_TABLE_SIZE ((ASCII_TABLE_SIZE / 8) +1)

extern _expandable_stack _cargs_general_buffer;                         //A buffer for general usage data

extern size_t _cargs_option_count;                                      //Counts the number of argument options 
extern _cargs_argument* _cargs_declared_arg_options;                    //The memory block that contains the available argument options information
extern _cargs_argument_data* _cargs_declared_options_data;              //The memory block that contains the input data from the available data argument options
extern _cargs_argument* _cargs_valid_arg_options[ASCII_TABLE_SIZE];     //A list of pointers to the memory block that contains the information from available arguments

extern uint8_t _cargs_arg_confirmation_bits[BIT_ASCII_TABLE_SIZE];      //Confirms the existence of an argument option into the user input

extern uint32_t _cargs_bank_stack_pointer;                              //Top pointer for the equals operator bank that identifies an empty place
extern char** _cargs_equals_operator_pointer_bank;                      //Contains char pointers where to store the equals operator pointer option (one per argument option)

extern size_t _cargs_ext_arg_count;                                     //Stores the extended argument count
extern char** _cargs_ext_args;                                          //Stores all the extended argument strings

extern uint32_t _cargs_anon_arg_count;                                  //Counts all the available anonymous arguments
extern _expandable_stack _cargs_anonymous_args;                         //Contains _cargs_package objects that point to anonymous arguments groups
extern char** _cargs_anonymous_relocated_args;                          //Extracts the buffer information to generate
