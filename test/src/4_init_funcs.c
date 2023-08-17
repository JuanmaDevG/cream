#include "init_funcs_test.h"

int main()
{
    cargs_set_args(NULL, "abc");
    printf("Set arguments please\n"); //MOD
    cargs_set_minimum_data("hbcd", 1, 2, 3, 43, 123);
    cargs_set_maximum_data("hbcd", 2, 5, 7, 21, 54);

    assert(_cargs_minimum_data[1] == 1);
    assert(_cargs_maximum_data[1] == 2);
    assert(_cargs_minimum_data[2] == 2);
    assert(_cargs_maximum_data[2] == 5);

    finish(4, "init functions");
}