#include "init_funcs_test.h"

int main()
{
    cargs_set_args("abc", "def");
    cargs_associate_extended("cef", "copy", "export", "file");

    assert(_extended_args.size == 3);
    assert(_extended_args.args);
    assert(cargs_clean());
    ExtArg* ptr = _extended_args.args;
    printf("Assertions 1\n"); //MOD
    
    assert(strcmp(ptr[0].name, "copy") == 0);
    assert(strcmp(ptr[1].name, "export") == 0);
    assert(strcmp(ptr[2].name, "file") == 0);

    cargs_set_args(NULL, "lsi");
    cargs_associate_extended("lsi", "load", "save_in", "ignore");
    assert(_extended_args.size == 3); //Reallocation went well

    finish(2, "init functions");
}