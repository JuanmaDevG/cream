#pragma once

#include <stdio.h>
#include <assert.h>

#include "utils.h"

char new_bool_args[] = "abcdef";
char new_data_args[] = "ghijkl";

const char* finish_msg = "Finished test %d\n";

void init_test_args()
{
    _bool_args = new_bool_args;
    _data_args = new_data_args;
}

void init_ext_arg_vec()
{
    _extended_args.size = 3;
    _extended_args.args = (ExtArg*)calloc(_extended_args.size, sizeof(ExtArg));

    //First element boolean argument
    _extended_args.args[0].associated_opt = 0;
    _extended_args.args[0].read_point = _bool_args;
    _extended_args.args[0].name = (char*)malloc(strlen("abort") +1);
    memcpy(_extended_args.args[0].name, "abort", strlen("abort") +1);

    //Second element data argument
    _extended_args.args[1].associated_opt = 0;
    _extended_args.args[1].read_point = _data_args;
    _extended_args.args[1].name = (char*)malloc(strlen("goto") +1);
    memcpy(_extended_args.args[0].name, "goto", strlen("goto") +1);

    //Third element empty...
}