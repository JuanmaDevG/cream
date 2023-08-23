#include "init_funcs_test.h"

/*
    TESTING IDENTIFICATOR CHANGE, ARGUMENTS SET AND CLEANUP
*/

int main()
{
    cargs_set_identificator('/'); //Windows like option id
    assert(_arg_id == '/');
    cargs_set_identificator('-'); //I personally prefer Unix like id

    //Boolean arg + no data args
    cargs_set_args("abcdef", NULL);
    assert(_cargs_bool_args != NULL);
    assert(strcmp(_cargs_bool_args, "abcdef") == 0);
    assert(_cargs_data_args == NULL);

    cargs_set_args(NULL, "abc");
    assert(_cargs_bool_args == NULL && _cargs_data_args != NULL);
    assert(strcmp(_cargs_data_args, "abc") == 0);
    assert(_cargs_minimum_data[_cargs_data_packs.size -1] == 0); //The last allocation position is accessible (no segfault)

    cargs_set_args("abcd", "aefgh"); //Adding a redundnacy 'a'
    assert(_cargs_bool_args != NULL && _cargs_data_args != NULL);
    assert(strcmp(_cargs_data_args, "efgh") == 0);

    assert(cargs_clean());
    cargs_set_args(NULL, NULL);
    assert(!(_cargs_bool_args || _cargs_data_args || _extended_args.args || _cargs_mandatory_args)); //Everything is empty

    assert(!cargs_clean());
    cargs_set_args("abc", "def");
    assert(cargs_clean());

    finish(1, "init functions");
    return 0;
}