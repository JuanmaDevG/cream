#include "utils_test.h"

/*
    ADDING DATA POINTERS TO THE VECTOR THAT CONTAINS THE ARGUMENT DATA
*/

int main()
{
    init_test_args();
    init_ext_arg_vec();

    //Extended argument test
    uint32_t arg_index = 1;
    _find_extended_argument(tmp_argv[arg_index] +2);
    uint32_t arg_position = _get_actual_ext_checkpoint() -1;
    _add_argument_data(tmp_argc, (const char**)tmp_argv, &arg_index, &arg_position);

    assert(_cargs_data_packs[0].count == 2);
    assert(_cargs_data_packs[0].values == tmp_argv +2);
    assert(arg_index == 3);

    //Non extended argument test
    arg_index++; //Loop iteration simulation
    _find_argument_char((tmp_argv[arg_index]+1)[0]);
    _add_argument_data(tmp_argc, (const char**)tmp_argv, &arg_index, NULL);

    assert(_cargs_data_packs[2].count == 3);
    assert(_cargs_data_packs[2].values == tmp_argv + 5);
    assert(arg_index == 7);

    assert(_cargs_data_packs[1].count == 0);
    assert(_cargs_data_packs[1].values == NULL);

    //Test with empty data argument (should not give error because of empty data argument count)
    arg_index++;
    _find_argument_char((tmp_argv[8] + 1)[0]);
    _add_argument_data(tmp_argc, (const char**)tmp_argv, &arg_index, NULL);
    assert(_cargs_error_argument == NULL);
    assert(cargs_error_code == CARGS_NO_ERROR);

    finish(7, "utils");
    return 0;
}