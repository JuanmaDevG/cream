#include "utils_test.h"

/*
    ERROR CONTROL UTILITIES
*/

const char* my_err_arg = "some-argument";

int main()
{
    _cargs_declare_error(
        my_err_arg, 1, CARGS_NON_EXISTENT
    );

    //Just copies the pointer
    assert(_cargs_error_argument == my_err_arg);
    assert(cargs_error_code == CARGS_NON_EXISTENT);
    assert(_cargs_is_extended == (uint8_t)1);

    printf(finish_msg, 3);
    return 0;
}