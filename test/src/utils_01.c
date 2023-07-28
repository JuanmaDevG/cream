#include <assert.h>

#include "utils.h"

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
    
    _bool_args = custom_bool_args;
    _obtain_read_point();
    assert(_read_point != NULL);

    _bool_args = NULL;
    _data_args = custom_data_args;
    _obtain_read_point();
    assert(_read_point == _data_args);

    _read_point = NULL;
    _bool_args = custom_bool_args;
    _data_args = custom_data_args;
    _swap_read_point();
    assert(_read_point = _bool_args);
    _swap_read_point();
    assert(_read_point == _data_args);
    _swap_read_point();
    assert(_read_point == _bool_args);

    return 0;
}