#include <assert.h>
#include <stdio.h>

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

    assert(_get_actual_read_point() == _bool_args);
    assert(_get_actual_checkpoint() == 0);
    assert(_get_actual_ext_checkpoint() == 0);
    
    _reset_finders();
    assert(_checkpoint == 0);
    assert(_read_point == NULL);

    _reset_ext_finders();
    assert(_extended_checkpoint == 0);

    printf("Finished test 1\n");
    return 0;
}