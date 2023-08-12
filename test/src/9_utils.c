#include "utils_test.h"

/*
    ADDING AND DELETING ANONYMOUS ARGUMENTS
*/

int main()
{
    init_test_args();

    //Store anonymous arguments
    uint32_t anon_index = 11;
    _cargs_store_anonymous_arguments(tmp_argc, (const char**)tmp_argv, &anon_index);
    assert(_cargs_anon_arg_count == 4);
    assert(anon_index == 14);
    assert(_cargs_anon_args == _cargs_anon_last);
    assert(_cargs_anon_args->next == NULL);
    assert(_cargs_anon_args->previous == NULL);
    assert(_cargs_anon_args->package.count == 4);
    assert(strcmp(_cargs_anon_args->package.values[0], "anonymous_arg1") == 0);
    assert(strcmp(_cargs_anon_args->package.values[3], "anonymous_arg4") == 0);

    //Deleting anonymous arguments
    assert(_cargs_delete_list_head(&_cargs_anon_args, &_cargs_anon_last));
    assert(_cargs_anon_args == NULL);
    assert(_cargs_anon_last == NULL);
    //List functions do not touch the _cargs_anon_arg_count
    assert(_cargs_anon_arg_count == 4);

    finish(9, "utils");
}