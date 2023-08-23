#include "utils_test.h"

/*
    Testing that redundant arg error function works properly
*/

int main()
{
    init_test_args();

    //Must write error
    _cargs_treat_repeated_args_as_errors = true;
    _cargs_bool_args[0] = '\\';
    const char* error_arg = "a";
    assert(_cargs_check_redundant_arg_error(_cargs_bool_args, 0, error_arg, false));
    assert(cargs_error_code == CARGS_REDUNDANT_ARGUMENT);
    assert(_cargs_is_extended == 0);
    assert(_cargs_error_argument == error_arg); //Must be the same pointer

    //Must keep CARGS_NO_ERROR
    cargs_error_code = CARGS_NO_ERROR;
    const char* do_not_care = "akfbha";
    assert(!_cargs_check_redundant_arg_error(_cargs_bool_args, 1, do_not_care, false));
    assert(cargs_error_code == CARGS_NO_ERROR);

    finish(12, "utils");
}