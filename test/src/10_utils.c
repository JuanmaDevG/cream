#include "utils_test.h"

/*
    CHECK MANDATORY ARGUMENTS AND THROWING AN ERROR
*/

int main()
{
    init_test_args();

    _cargs_mandatory_args = (_cargs_buffer_position*)calloc(2, sizeof(_cargs_buffer_position));
    _cargs_mandatory_arg_count = 2;
    _bool_args[0] = '\\';
    _cargs_mandatory_args[0].position = 0;
    _cargs_mandatory_args[0].read_point = _bool_args;
    _cargs_mandatory_args[1].position = 2;
    _cargs_mandatory_args[1].read_point = _data_args;

    assert(!_cargs_check_mandatory_arguments());
    assert(cargs_error_code == CARGS_MANDATORY);
    assert(_cargs_error_argument[0] == 'i');
    assert(_cargs_is_extended == 0);

    finish(10, "utils");
}