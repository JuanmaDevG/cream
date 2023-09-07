#include "init_funcs_test.h"

/*
    Trying the error code string generator
*/

int main()
{
    _cargs_error_argument = (char*)malloc(16);
    memcpy(_cargs_error_argument, "some-argument", 15);
    _cargs_is_error_extended = true;

    cargs_error_code = CARGS_MANDATORY;
    printf("This test does not have assertions, just prints the results\n");
    printf("%s\n", cargs_get_error());
}