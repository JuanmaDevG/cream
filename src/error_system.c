#include "error_system.h"

/*
    Buffers
*/

uint32_t _cream_error_count = 0;
_expandable_stack _cream_error_buffer = {{0}, NULL, 0, 0, NULL, NULL};

char* _cream_out_error_msg = NULL;

//All the error messages start by "The argument arg_name " and then the error string
const char* _cream_error_strings[] = {
    NULL,
    "does not exist.",
    "is required.",
    "requires extra data and there is not enough.",
    "is repeated and it is not allowed to repeat argument options.", 
    "is redundant.\nMake sure not to duplicate it when calling cream_set_args function.", 
    "There is not enough memory to initialize cream library.\nYou can try setting less arguments or to recompile the library reducing MEM_BLOCK_SIZE from exp_stack.h", 
    "contains too many data elements and less are required.", 
    "contains arguments that are not bound to any argument option."
};

const uint32_t _cream_error_lengths[] =
{
    0, 
    15, 
    12, 
    44, 
    61, 
    81, 
    159
};

bool _cream_treat_anonymous_args_as_errors = false;
bool _cream_include_argument_zero = false;
bool _cream_promise_first_ext_arg_char_is_ext_arg = false;
bool _cream_enable_multiple_opt_per_arg = false;


/*
    Functions
*/

extern inline void _cream_declare_error(const uint32_t _error_type, const char* error_arg, const bool is_extended, const char* _optional_plot_twist)
{
    _cream_error_count++;
    cream_error err = {_error_type, (char*)error_arg, is_extended, (char*)_optional_plot_twist};
    _stack_push_block(&_cream_error_buffer, &err, sizeof(cream_error));
}

extern inline void _cream_reset_error_system()
{
    _cream_error_count = 0;
    _stack_free_expandable(&_cream_error_buffer);
    if(_cream_out_error_msg)
    {
        free(_cream_out_error_msg);
        _cream_out_error_msg = NULL;
    }
}