#pragma once

#include "bit_vec.h"
#include "exp_stack.h"
#include "trie.h"


//Argument package that contains a pointer to data values (this is set inside an exapandable stack)
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

typedef struct _cream_argument_option {
    bool is_mandatory;                      //If true, the error system will offer the possibility to interpret as error
    bool cannot_be_repeated;                //If false and has been used, the error system will interpret the argument repetition as an error
    bool has_been_used_already;             //If true, the error system will offer the possibility to interpret it's repetition as an error
    _cream_option_data* data_container;     //Won't be null if the argument is a data argument
} _cream_argument_option;


/*
    ------------------------
    Shared data declarations
    ------------------------
*/

extern _expandable_stack _cream_general_buffer;                 // A buffer for general usage data

extern const char _cream_arg_id_back_limit;                     // Argument identificator with lowest value
extern const char _cream_arg_id_front_limit;                    // Argument identificator with highest value
extern const uint8_t* _cream_arg_id_bit_vec;                    // Bit vector indicating the existence of an argument option

extern const uint32_t _cream_arg_option_count;                  // Total argument option count
extern const char _cream_back_arg_opt_limit;                    // Argument option character with lowest value
extern const char _cream_front_arg_opt_limit;                   // Argument option character with highest value
extern const char* _cream_declared_option_chars;                // Stores the argument option chars declared (not searched sequentially, just for some internal checking)
extern _cream_argument_option* _cream_declared_arg_options;     // Contains the available argument options information
extern _cream_option_data* _cream_arg_options_data;             // Contains the input data received by the corresponding data argument options

extern size_t _cream_anon_arg_count;                            // Contains the total number of anonymous arguments
extern _expandable_stack _cream_anonymous_args;                 // Contains _cream_data_package objects that point to anonymous arguments groups
extern char** _cream_anonymous_relocated_args;                  // Extracts the buffer information to generate

//-------------------------------------------------------------------------------------------------
//TODO: extended arguments (dependent, independent and commands) will be managed by trie structures
//-------------------------------------------------------------------------------------------------

//Dev note: the trie indexed pointer vector of a trie node must be void (that can be interpreted as a pointer to argument option or to anything)