#pragma once

#include "exp_stack.h"
#include <string.h>


//Argument package associated to a single option
typedef struct _cargs_data_package {
    uint32_t count;
    char** values;          //Pointer to first data value position in argv
} _cargs_data_package;

typedef struct _cargs_argument_data {
    uint32_t package_count;             //Counts the number of packages inside the buffer
    _cargs_data_package data;           //Obtains the data copying from the buffer
    _expandable_stack buffer;           //Contains all the argument data packages in it's blocks
} _cargs_argument_data;

typedef struct _cargs_argument {
    bool is_it_mandatory;                   //If true, the error system will offer the possibility to catch as error
    bool is_it_redundant;                   //If true, the error system will offer the possibility to catch as error
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

/*
    A table of as pointers as the ASCII table whose mem address is NULL if not existent arguments or 
    the memory address of the argument properties
*/
#define ASCII_TABLE_SIZE (256 - 33)
extern _cargs_argument* _cargs_argument_options[ASCII_TABLE_SIZE]; //Without non keyboard characters
extern _cargs_argument* _cargs_available_arguments;
extern _cargs_argument_data* _cargs_available_args_data;

/*
    Bit vectors that confirm the existence of the arguments into the user input
*/

extern uint8_t* _cargs_bool_bit_vec;
extern uint8_t* _cargs_data_bit_vec;

extern uint32_t _cargs_bank_stack_pointer;
extern char** _cargs_equals_operator_pointer_bank;

extern size_t _cargs_ext_arg_count;                     //Stores the extended argument count
extern _cargs_extended_argument* _cargs_ext_args;       //Stores the pointer references to the actual character arguments (NOT BUFFER ALLOCATED, READ ONLY)

extern _expandable_stack _cargs_general_buffer;


/*
    --------------------------------------------------
    Anonymous args and data limits on argument control
    --------------------------------------------------
*/

extern uint32_t _cargs_anon_arg_count;
extern _cargs_anonymous_node* _cargs_anon_args;
extern _cargs_anonymous_node* _cargs_anon_last;
extern uint32_t _cargs_anonymous_relocated_buf_size;
extern char** _cargs_anonymous_relocated_args;

extern uint8_t* _cargs_minimum_data;
extern uint8_t* _cargs_maximum_data;


/*
    ----------------
    Utility pointers
    ----------------
*/

extern char* _read_point;               //Pointer where to start to read an option char buffer
extern uint32_t _checkpoint;            //Position of the character next to the one that was found
extern uint32_t _extended_checkpoint;   //Checkpoint just made for extended arguments


/*
    ------------------------------------------
    Redundant data argument storage structures
    ------------------------------------------
*/


/*
    Contains as many places as as the _cargs_data_packs vector size.
    
    If any data argument option is repeated, and the redundant argument 
    options are not recognized as errors (_cargs_treat_redundant_arguments_as_errors = false)
    the data package of the redunant option is full, so will be allocated here.
*/
extern _cargs_data_storage_list* _cargs_redundant_opt_data;

extern uint8_t* _cargs_is_data_relocated_bit_vec;