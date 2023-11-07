#include "utils_test.h"

/*
    DEPRECATED TEST
*/

int main()
{

    init_test_args();

    uint32_t arg_idx = 9;

    //Should not give error because everything is boolean
    assert(_read_non_extended_argument(tmp_argc, (const char**)tmp_argv, &arg_idx));
    assert(cargs_error_code == CARGS_NO_ERROR);
    assert(_cargs_get_bit(_cargs_bool_bit_vec, 0));
    assert(_cargs_get_bit(_cargs_bool_bit_vec, 1));
    assert(_cargs_get_bit(_cargs_bool_bit_vec, 2));
    assert(_cargs_error_argument == NULL);

    arg_idx++;
    /*
        assert(!_read_non_extended_argument(tmp_argc, (const char**)tmp_argv, &arg_idx));
        assert(cargs_error_code == CARGS_MULTI_BOOL_ARG_ISSUE);
        assert(_cargs_error_argument == tmp_argv[arg_idx] +2);
        assert(_cargs_is_error_extended == 0);
        assert(!_cargs_get_bit(_cargs_bool_bit_vec, 5));
    */

    finish(8, "utils");
    return 0;
}