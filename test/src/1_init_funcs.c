#include "init_funcs_test.h"

/*
    TESTING IDENTIFICATOR CHANGE AND ARGUMENTS SET
*/

int main()
{
    cargs_set_identificator('/'); //Windows like id
    assert(_arg_id == '/');
    cargs_set_identificator('-'); //I personally prefer Unix like id

    //Boolean arg + no data args
    cargs_set_args("abcdef", NULL);
    assert(_bool_args != NULL);
    assert(strcmp(_bool_args, "abcdef") == 0);
    assert(_data_args == NULL);

    cargs_set_args(NULL, "abc");
    assert(_bool_args == NULL && _data_args != NULL);
    assert(strcmp(_data_args, "abc") == 0);
    assert(_cargs_minimum_data[_data_packs.size -1] == 0); //The last allocation position is accessible (no segfault)

    cargs_set_args("abcd", "aefgh"); //Adding a redundnacy 'a'
    assert(_bool_args != NULL && _data_args != NULL);
    assert(strcmp(_data_args, "efgh") == 0);

    finish(1, "init functions");
    return 0;
}