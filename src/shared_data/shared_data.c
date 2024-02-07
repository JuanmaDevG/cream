#include "shared_data.h"

char _arg_id = '-';

/*
    ----------------------
    General purpose buffer
    ----------------------
*/

_expandable_stack _cream_general_buffer = {{0}, NULL, 0, 0, NULL, NULL};


/*
    ----------------------------
    Argument options information
    ----------------------------
*/

char* _cream_declared_arg_option_chars = NULL;
size_t _cream_option_count = 0;
_cream_argument* _cream_declared_arg_options = NULL;
_cream_option_data* _cream_declared_options_data = NULL;
_cream_argument* _cream_valid_arg_options[ASCII_TABLE_SIZE] = {NULL};


/*
    ------------------
    Extended arguments
    ------------------
*/

size_t _cream_ext_arg_count = 0;


/*
    ----------------------------------
    Anonymous argument storage buffers
    ----------------------------------
*/

size_t _cream_anon_arg_count = 0;
_expandable_stack _cream_anonymous_args = {{0}, NULL, 0, 0, NULL, NULL};
char** _cream_anonymous_relocated_args = NULL;