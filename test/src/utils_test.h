#pragma once

#include <stdio.h>
#include <assert.h>

#include "utils.h"

#include <stdio.h>

char new_bool_args[] = "abcdef";
char new_data_args[] = "ghijkl";

const char* finish_msg = "Finished test %d\n";

const int tmp_argc = 11;
char* tmp_argv[] = {
    "program_name.exe",
    "--goto",
    "some_file.txt",
    "some_oter_file.txt",
    "-i",               //import
    "something.lib",
    "some-other.dll",
    "any_module.lib",
    "-j",               //should give error because has non associated data
    "-abc",
    "-dkf"
};

void init_test_args()
{
    _bool_args = new_bool_args;
    _data_args = new_data_args;
    _cargs_maximum_data = (uint8_t*)calloc(strlen(new_data_args), sizeof(uint8_t));
    _cargs_minimum_data = (uint8_t*)calloc(strlen(new_data_args), sizeof(uint8_t));
}

void init_ext_arg_vec()
{
    _extended_args.size = strlen(new_data_args);
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
    memcpy(_extended_args.args[1].name, "goto", strlen("goto") +1);

    //Third element empty...
}

void init_data_packages()
{
    _data_packs.size = 3;
    _data_packs.packages = (ArgPackage*)calloc(_data_packs.size, sizeof(ArgPackage));
}

void finish(uint32_t test_id) { printf(finish_msg, test_id); }