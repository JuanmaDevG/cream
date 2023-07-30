#include <assert.h>

#include "utils.h"

/*
    PUSHING SOME EXTENDED ARGUMENTS
*/

const uint32_t vec_size = 5;

int main()
{
    _extended_args.size = vec_size;
    _extended_args.args = (ExtArg*)calloc(vec_size, sizeof(ExtArg));

    _push_extended_argument("some-argument", 0, _bool_args, 0);
    _push_extended_argument("some-other-arg", 0, _data_args, 0);

    //Some assertions with the attributes... Dev later
}