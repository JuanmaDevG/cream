#include "utils_test.h"

/*
    ARGUMENT CHARACTER FINDING
*/

int main()
{
    init_test_args();

    assert(_find_argument_char('a'));
    assert(_get_actual_read_point() == _bool_args);
    assert(_get_actual_checkpoint() == 1);

    assert(_find_argument_char('h'));
    assert(_get_actual_read_point() == _data_args);
    assert(_get_actual_checkpoint() == 2);

    assert(_find_argument_char('f'));
    assert(_get_actual_read_point() == _bool_args);
    assert(_get_actual_checkpoint() == 6);

    assert(!_find_argument_char('p'));
    assert(_get_actual_read_point() == NULL);
    assert(_get_actual_checkpoint() == 0);

    assert(!_find_argument_char('\0'));
    assert(_get_actual_read_point() == NULL);
    assert(_get_actual_checkpoint() == 0);


    finish(5);
    return 0;
}