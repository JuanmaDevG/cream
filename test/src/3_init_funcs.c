#include "init_funcs_test.h"

int main()
{
    cargs_set_args("abc", "def");
    cargs_make_mandatory("aef");

    assert(_cargs_mandatory_arg_count == 3);
    assert(_cargs_mandatory_args[0].position == 0); //the position is the associated option, not the mandatory vector position
    assert(_cargs_mandatory_args[0].read_point == _bool_args);
    assert(_cargs_mandatory_args[1].position == 1);
    assert(_cargs_mandatory_args[1].read_point == _data_args);
    assert(_cargs_mandatory_args[2].position == 2);
    assert(_cargs_mandatory_args[2].read_point == _data_args);

    //Invalid arguments should be invalidated

    finish(3, "init functions");
}