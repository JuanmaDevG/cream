#pragma once

#include "exp_stack.h"
#include <string.h>


//Argument package associated to a single option
typedef struct ArgPackage {
    uint32_t count;
    char** values;          //Pointer to first value position in argv
} ArgPackage;

//Extended argument name associated to the corresponding char argument argument
typedef struct {
    char* read_point;
    uint32_t associated_opt; //Option symbol position (to look into bit vectors and ArgPackage vectors)
    char* name;
} ExtArg;


typedef struct {
    uint8_t* read_point;        //Pointer to the corresponding bit vector
    uint32_t position;
} _cargs_buffer_position;

typedef struct _cargs_anonymous_node {
    ArgPackage package;
    struct _cargs_anonymous_node* next;
    struct _cargs_anonymous_node* previous;
} _cargs_anonymous_node;

typedef struct _cargs_data_storage_list {
    _cargs_anonymous_node* first_node;
    _cargs_anonymous_node* last_node;
} _cargs_data_storage_list;


/*
    ------------------------
    Shared data declarations
    ------------------------
*/

extern char _arg_id;

extern size_t _cargs_bool_args_count;
extern char* _cargs_bool_args;
extern uint8_t* _cargs_bool_bit_vec;

extern size_t _cargs_data_args_count;
extern char* _cargs_data_args;
extern uint8_t* _cargs_data_bit_vec;

extern ArgPackage* _cargs_data_packs;
extern uint32_t _cargs_bank_stack_pointer;
extern char** _cargs_equals_operator_pointer_bank;

extern size_t _cargs_ext_arg_count;                     //Stores the extended argument count
extern ExtArg* _cargs_ext_args;                         //Stores the pointer references to the actual character arguments (NOT BUFFER ALLOCATED, READ ONLY)

extern _expandable_stack _cargs_general_buffer;


/*
    ------------------------------------------
    Mandatory application arguments management
    ------------------------------------------
*/

extern _cargs_buffer_position* _cargs_mandatory_args;
extern uint32_t _cargs_mandatory_arg_count;


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