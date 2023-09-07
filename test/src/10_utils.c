#include "utils_test.h"

/*
    CHECK MANDATORY ARGUMENTS AND THROWING AN ERROR
*/

int main()
{
    init_test_args();

    _cargs_mandatory_args = (_cargs_buffer_position*)calloc(3, sizeof(_cargs_buffer_position));
    _cargs_mandatory_arg_count = 2;
    _cargs_set_bit(_cargs_bool_bit_vec, 0, true);
    _cargs_mandatory_args[0].position = 0;
    _cargs_mandatory_args[0].read_point = _cargs_bool_bit_vec;
    _cargs_mandatory_args[1].position = 5;                      //argument char from data: l
    _cargs_mandatory_args[1].read_point = _cargs_data_bit_vec;

    _cargs_set_bit(_cargs_bool_bit_vec, 0, true);
    assert(!_cargs_check_mandatory_arguments());
    assert(cargs_error_code == CARGS_MANDATORY);
    assert(_cargs_error_argument[0] == 'l');
    assert(_cargs_is_error_extended == 0);

    finish(10, "utils");
}