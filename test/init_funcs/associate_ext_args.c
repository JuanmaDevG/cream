#include "init_funcs_test.h"

/*
    Associating extended arguments to it's short version
*/

int main()
{
    cargs_set_args("abc", "def");
    cargs_associate_extended("cef", "copy", "export", "file");

    assert(_extended_args.size == 3);
    assert(_extended_args.args);
    
    ExtArg* ptr = _extended_args.args;
    assert(strcmp(ptr[0].name, "copy") == 0);
    assert(strcmp(ptr[1].name, "export") == 0);
    assert(strcmp(ptr[2].name, "file") == 0);

    assert(cargs_clean());
    cargs_set_args(NULL, "lsi");
    cargs_associate_extended("lsi", "load", "save_in", "ignore");
    assert(_extended_args.size == 3); //Reallocation went well
    assert(strcmp(_extended_args.args[0].name, "load") == 0);

    cargs_set_args("abc", NULL);
    cargs_associate_extended("ac", "abort", "copy");
    ptr = _extended_args.args;
    assert(strcmp(ptr[0].name, "abort") == 0);
    assert(strcmp(ptr[1].name, "copy") == 0);

    finish(2, "init functions");
}