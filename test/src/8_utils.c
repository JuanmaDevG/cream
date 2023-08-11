#include "utils_test.h"

/*
    MULTI BOOLEAN ARGUMENT STRING READ
*/

int main()
{
    init_test_args();

    //Non error argument
    uint32_t arg_idx = 9;

    assert(_read_non_extended_argument(tmp_argc, (const char**)tmp_argv, &arg_idx));
    assert(cargs_error_code == CARGS_NO_ERROR);
    assert(_bool_args[0] == '\\');
    assert(_bool_args[1] == '\\');
    assert(_bool_args[2] == '\\');
    assert(_cargs_error_argument == NULL);

    //Argument that should give error
    arg_idx++;

    assert(!_read_non_extended_argument(tmp_argc, (const char**)tmp_argv, &arg_idx));
    assert(cargs_error_code == CARGS_MULTI_BOOL_ARG_ISSUE);
    assert(_cargs_error_argument == tmp_argv[arg_idx] +2);
    assert(_cargs_is_extended == 0);
    assert(_bool_args[5] != '\\');

    finish(8, "utils");
    return 0;
}