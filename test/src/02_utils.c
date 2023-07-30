#include <assert.h>
#include <stdio.h>

#include "utils.h"

/*
    PUSHING SOME EXTENDED ARGUMENTS
*/

const uint32_t vec_size = 2;
_bool_args = "s";
_data_args = "s";

int main()
{
    _extended_args.size = vec_size;
    _extended_args.args = (ExtArg*)calloc(vec_size, sizeof(ExtArg));

    _push_extended_argument("some-argument", 0, _bool_args, 0);
    _push_extended_argument("some-other-arg", 0, _data_args, 1);

    //Boolean first extended argument
    assert(strcmp(_extended_args.args[0].name, "some-argument") == 0);
    assert(_extended_args.args[0].associated_opt == 0);
    assert(_extended_args.args[0].read_point == _bool_args);
    
    //Data second extended argument
    assert(strcmp(_extended_args.args[1].name, "some-other-arg") == 0);
    assert(_extended_args.args[1].associated_opt == 0);
    assert(_extended_args.args[1].read_point == _data_args);

    free(_extended_args.args);
    printf("Finished test 2\n");
    return 0;
}