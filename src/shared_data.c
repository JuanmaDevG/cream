#include "shared_data.h"

char _arg_id = '-';

/*
    ----------------------
    General purpose buffer
    ----------------------
*/

_expandable_stack _cargs_general_buffer = {{0}, NULL, 0, 0, NULL, NULL};


/*
    ----------------------------
    Argument options information
    ----------------------------
*/

char* _cargs_declared_arg_option_chars = NULL;
size_t _cargs_option_count = 0;
_cargs_argument* _cargs_declared_arg_options = NULL;
_cargs_option_data* _cargs_declared_options_data = NULL;
_cargs_argument* _cargs_valid_arg_options[ASCII_TABLE_SIZE] = {NULL};


/*
    ------------------
    Extended arguments
    ------------------
*/

size_t _cargs_ext_arg_count = 0;


/*
    ----------------------------------
    Anonymous argument storage buffers
    ----------------------------------
*/

size_t _cargs_anon_arg_count = 0;
_expandable_stack _cargs_anonymous_args = {{0}, NULL, 0, 0, NULL, NULL};
char** _cargs_anonymous_relocated_args = NULL;