#include "init_funcs_test.h"

int main()
{
    cargs_set_args("abc", "def");
    cargs_make_mandatory("aef");

    //MAKE THE MANDATORY ASSERTIONS    

    finish(3, "init functions");
}