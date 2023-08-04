#include "utils_test.h"

int main()
{
    init_test_args();

    uint32_t arg_idx = 9;
    _read_non_extended_argument(tmp_argc, tmp_argv, arg_idx);
    //Make the argument assertions and error catching

    finish(8);
}