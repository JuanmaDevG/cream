#pragma once

#include "exp_stack.h"
#include <string.h>


//Argument package associated to a single option
typedef struct _cream_data_package {
    uint32_t count;
    char** values;                  //Pointer to first data value position in argv
    uint8_t inline_data_offset;     //Represents the offset if the argument's data is inside the argument option (-LsomeDirectory)
} _cream_data_package;

typedef struct _cream_option_data {
    uint32_t max_data_peices;           //Absolute maximum data pieces for this argument option (cannot surpass)
    uint32_t min_data_pieces;           //Absolute minimum data pieces for this argument option (must be major or equal)
    uint8_t max_data_per_call;          //Maximum data pieces per argument option call
    uint8_t min_data_per_call;          //Minimum data pieces per argument option call
    uint32_t actual_data_count;         //Actual string data pieces that the argument has associated
    _expandable_stack data;             //Contains the _cream_data_packages and can be extracted with the expandable stack functions
    char** data_relocation_buffer;      //If not NULL, means that the expandable stack has been erased to move all the data string pointers to this buffer
} _cream_option_data;

typedef struct _cream_argument {
    bool is_mandatory;                      //If true, the error system will offer the possibility to interpret as error
    bool cannot_be_repeated;                //If false and has been used, the error system will interpret the argument repetition as an error
    bool has_been_used_already;             //If true, the error system will offer the possibility to interpret it's repetition as an error
    _cream_option_data* data_container;     //Won't be null if the argument is a data argument
    char* extended_version;                 //Pointer to extended version: {-f -> --force} (does not contain argument option id like '-')
} _cream_argument;


/*
    ------------------------
    Shared data declarations
    ------------------------
*/

extern const char* _cream_arg_id; //TODO: redefine (and support for multiple arg ids)

extern _expandable_stack _cream_general_buffer;                 //A buffer for general usage data

extern const char cream_limit_back_char;                        //Character from the arg option input with lowest value
extern const char cream_limit_back_char;                        //Character from the arg option input with the highest value
extern _cream_argument** _cream_valid_arg_options;              //A list of independent pointers to the memory block that contains the information from available arguments

extern char* _cream_declared_option_chars;                      //Points to the declared options (first bool options, then data options)
extern uint32_t _cream_option_count;                            //Counts the number of argument options
extern _cream_argument* _cream_declared_arg_options;            //The memory block that contains the available argument options information
extern _cream_option_data* _cream_declared_options_data;        //The memory block that contains the input data from the available data argument options

extern size_t _cream_ext_arg_count;                             //Stores the extended argument count

extern size_t _cream_anon_arg_count;                            //Contains the total number of anonymous arguments
extern _expandable_stack _cream_anonymous_args;                 //Contains _cream_data_package objects that point to anonymous arguments groups
extern char** _cream_anonymous_relocated_args;                  //Extracts the buffer information to generate
