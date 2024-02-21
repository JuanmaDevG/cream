#include "shared_data.h"

/*
    ---------
    Dev note:
    ---------

    Constants can only be casted to allow theis modification on an initialization function, they're meant to be const to avoid memory corruption 
    because of accidental assignation operators.
*/

_expandable_stack _cream_general_buffer = {{0}, NULL, 0, 0, NULL, NULL};

char _cream_arg_id_back_limit = '\0';
char _cream_arg_id_front_limit = '\0';
uint8_t* _cream_arg_id_bit_vec = NULL;

const uint32_t _cream_arg_option_count = 0u;
const char _cream_back_arg_opt_limit = '\0';
const char _cream_front_arg_opt_limit = '\0';
const char* _cream_declared_option_chars = NULL;
_cream_argument_option* _cream_declared_arg_options = NULL;
_cream_option_data* _cream_arg_options_data = NULL;

size_t _cream_anon_arg_count = 0;
_expandable_stack _cream_anonymous_args = {{0}, NULL, 0, 0, NULL, NULL};
char** _cream_anonymous_relocated_args = NULL;

//TODO: extended args specified in header file (trie in development)