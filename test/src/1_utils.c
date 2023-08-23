#include "utils_test.h"

/*
    READ POINT UTILITY TEST
*/

char* custom_bool_args = "abcdef";
char* custom_data_args = "ghijkl";

int main()
{
    assert(_read_point == NULL);

    _obtain_read_point();
    assert(_read_point == NULL);

    _swap_read_point();
    assert(_read_point == NULL);
    
    _cargs_bool_args = custom_bool_args;
    _obtain_read_point();
    assert(_read_point != NULL);

    _cargs_bool_args = NULL;
    _cargs_data_args = custom_data_args;
    _obtain_read_point();
    assert(_read_point == _cargs_data_args);

    _read_point = NULL;
    _cargs_bool_args = custom_bool_args;
    _cargs_data_args = custom_data_args;
    _swap_read_point();
    assert(_read_point = _cargs_bool_args);
    _swap_read_point();
    assert(_read_point == _cargs_data_args);
    _swap_read_point();
    assert(_read_point == _cargs_bool_args);

    assert(_get_actual_read_point() == _cargs_bool_args);
    assert(_get_actual_checkpoint() == 0);
    assert(_get_actual_ext_checkpoint() == 0);
    
    _reset_finders();
    assert(_checkpoint == 0);
    assert(_read_point == NULL);

    _reset_ext_finders();
    assert(_extended_checkpoint == 0);

    finish(1, "utils");
    return 0;
}