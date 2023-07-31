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