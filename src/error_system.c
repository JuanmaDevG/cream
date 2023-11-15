#include "error_system.h"

/*
    Buffers
*/

uint32_t _cargs_error_count = 0;
_expandable_stack _cargs_error_buffer = {{0}, NULL, 0, 0, NULL, NULL};

char** _cargs_out_error_msg = NULL;

//All the error messages start by "The argument arg_name " and then the error string
const char* _cargs_error_strings[] = {
    NULL,
    "does not exist.",
    "is required.",
    "requires extra data and there is not enough.",
    "is repeated and it is not allowed to repeat argument options."
};

bool _cargs_treat_anonymous_args_as_errors = false;
bool _cargs_include_argument_zero = false;
bool _cargs_promise_first_ext_arg_char_is_ext_arg = false;
bool _cargs_enable_multiple_opt_per_arg = false;


/*
    Functions
*/

extern inline void _cargs_declare_error(const uint32_t _error_type, const char* error_arg, const bool is_extended, const char* _optional_plot_twist)
{
    cargs_error err = {_error_type, (char*)error_arg, is_extended, (char*)_optional_plot_twist};
    _stack_push_block(&_cargs_error_buffer, &err, sizeof(cargs_error));
}