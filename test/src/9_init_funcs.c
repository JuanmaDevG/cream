#include <stdio.h>

#include "utils_test.h"
#include "init_funcs.h"

int main()
{
    //Init funcs code but first make functions about require minimum
    cargs_set_args("abcd", "efgh");
    cargs_make_mandatory("abfh");
    cargs_associate_extended("acf", "abort", "copy", "file");

    //Will try every function by itself but this is an example
    finish(9);
    return 0;
}