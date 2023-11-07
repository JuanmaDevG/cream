#include "utils_test.h"

/*
    REMOVING REDUNDANCIES
*/

int main()
{
    _cargs_bool_args = new_bool_args;
    _cargs_data_args = new_data_args;
    _cargs_bool_args_count = strlen(new_bool_args);
    _cargs_data_args_count = strlen(new_data_args);

    _cargs_data_args[0] = 'a';
    _cargs_data_args[2] = 'b';

    _remove_redundancies(REMOVE_DATA_REDUNDANCIES);

    assert(strcmp(_cargs_data_args,"hjkl") == 0);

    _cargs_bool_args[1] = 'h';
    _cargs_bool_args[3] = 'l';

    _remove_redundancies(REMOVE_BOOL_REDUNDANCIES);

    assert(strcmp(_cargs_bool_args, "acef") == 0);

    finish(4, "utils");
    return 0;
}