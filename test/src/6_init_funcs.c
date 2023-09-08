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
    printf("-----------------------------------------------------------\n");
    printf("This test does not have assertions, just prints the results\n");
    printf("-----------------------------------------------------------\n");
    printf("%s\n", cargs_get_error());

    cargs_error_code = CARGS_NON_EXISTENT;
    printf("%s\n", cargs_get_error());

    cargs_error_code = CARGS_NOT_ENOUGH_DATA;
    printf("%s\n", cargs_get_error());

    cargs_error_code = CARGS_REDUNDANT_ARGUMENT;
    printf("%s\n", cargs_get_error());

    cargs_error_code = CARGS_WRONG_ID;
    printf("%s\n", cargs_get_error());

    finish(6, "init functions");
}