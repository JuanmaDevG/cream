#include "utils_test.h"

/*
    PLAYING AROUND WITH INLINE EQUALS OPERATORS IN ARGUMENTS
*/

int main()
{
    init_test_args();

    const int what_option = 4;
    uint32_t data_position = _cargs_search_equals_operator(tmp_argv[15]);
    assert(data_position == 3);
    assert(_cargs_store_equals_operator_data(tmp_argv[15] + data_position, what_option)); //Is always _cargs_data_args cause is data
    assert(_cargs_bank_stack_pointer == 1);
    assert(_cargs_data_packs.packages[what_option].count == 1);
    assert(_cargs_data_packs.packages[what_option].values[0] == _cargs_equals_operator_pointer_bank[0]);
    assert(strcmp(_cargs_data_packs.packages[what_option].values[0], "some_argument") == 0);

    finish(11, "utils");
}