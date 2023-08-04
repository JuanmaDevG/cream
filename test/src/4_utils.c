#include "utils_test.h"

/*
    REMOVING REDUNDANCIES
*/

int main()
{
    _bool_args = new_bool_args;
    _data_args = new_data_args;
    _bool_args_count = strlen(new_bool_args);
    _data_packs.size = strlen(new_data_args);

    _data_args[0] = 'a';
    _data_args[2] = 'b';

    _remove_redundancies(REMOVE_DATA_REDUNDANCIES);

    assert(strcmp(_data_args,"hjkl") == 0);

    _bool_args[1] = 'h';
    _bool_args[3] = 'l';

    _remove_redundancies(REMOVE_BOOL_REDUNDANCIES);

    assert(strcmp(_bool_args, "acef") == 0);

    finish(4);
    return 0;
}