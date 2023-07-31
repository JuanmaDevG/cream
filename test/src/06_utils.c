#include "utils_test.h"

/*
    FINDING EXTENDED ARGUMENTS IN THE EXTENDED ARGUMENT VECTOR BUFFER
*/

int main()
{
    init_test_args();
    init_ext_arg_vec();

    assert(_find_extended_argument("abort"));
    assert(_extended_checkpoint == 1);

    assert(_find_extended_argument("goto"));
    assert(_extended_checkpoint == 2);

    assert(!_find_extended_argument("not-found"));
    assert(_extended_checkpoint == 0);

    assert(!_find_extended_argument("another-not-found"));
    assert(_extended_checkpoint == 0);

    printf(finish_msg, 6);
    return 0;
}