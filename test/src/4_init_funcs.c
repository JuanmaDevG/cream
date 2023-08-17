#include "init_funcs_test.h"

int main()
{
    cargs_set_args("abc", "def");
    cargs_set_maximum_data("befgh", 2, 5, 7, 21, 54);
    cargs_set_minimum_data("befgh", 1, 2, 3, 43, 123);

    finish(4, "init functions");
}