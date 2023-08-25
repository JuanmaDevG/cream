#pragma once

#include <stdio.h>
#include <assert.h>

#include "utils.h"
#include "bit_vec_ops.h"


char new_bool_args[] = "abcdef";
char new_data_args[] = "ghijkl";

const char* finish_msg = "Finished test %d from %s\n";

const int tmp_argc = 16;
char* tmp_argv[] = {
    "program_name.exe",
    "--goto",
    "some_file.txt",
    "some_oter_file.txt",
    "-i",               //import
    "something.lib",
    "some-other.dll",
    "any_module.lib",
    "-j",               //Should not give error if has not minimum argument number set
    "-abc",
    "-dkf",
    "anonymous_arg1",   //Some anonymous arguments 
    "anonymous_arg2", 
    "anonymous_arg3", 
    "anonymous_arg4",
    "-k=some_argument"
};

void init_test_args()
{
    _cargs_bool_args = new_bool_args;
    _cargs_bool_args_count = strlen(new_bool_args);
    _cargs_bool_bit_vec = (uint8_t*)calloc(_cargs_get_byte_size(strlen(new_bool_args)), sizeof(uint8_t));
    _cargs_data_args = new_data_args;
    _cargs_data_packs.size = strlen(new_data_args);
    _cargs_data_bit_vec = (uint8_t*)calloc(_cargs_get_byte_size(strlen(new_data_args)), sizeof(uint8_t));
    _cargs_data_packs.packages = (ArgPackage*)calloc(_cargs_data_packs.size, sizeof(ArgPackage));
    _cargs_equals_operator_pointer_bank = (char**)malloc(strlen(new_data_args));
    _cargs_maximum_data = (uint8_t*)calloc(strlen(new_data_args), sizeof(uint8_t));
    _cargs_minimum_data = (uint8_t*)calloc(strlen(new_data_args), sizeof(uint8_t));
    _cargs_redundant_opt_data = (_cargs_data_storage_list*)calloc(_cargs_data_packs.size, sizeof(_cargs_data_storage_list));
}

void init_ext_arg_vec()
{
    _extended_args.size = strlen(new_data_args);
    _extended_args.args = (ExtArg*)calloc(_extended_args.size, sizeof(ExtArg));

    //First element boolean argument
    _extended_args.args[0].associated_opt = 0;
    _extended_args.args[0].read_point = _cargs_bool_args;
    _extended_args.args[0].name = (char*)malloc(strlen("abort") +1);
    memcpy(_extended_args.args[0].name, "abort", strlen("abort") +1);

    //Second element data argument
    _extended_args.args[1].associated_opt = 0;
    _extended_args.args[1].read_point = _cargs_data_args;
    _extended_args.args[1].name = (char*)malloc(strlen("goto") +1);
    memcpy(_extended_args.args[1].name, "goto", strlen("goto") +1);

    //Third element empty...
}

void finish(uint32_t test_id, const char* test_name) { printf(finish_msg, test_id, test_name); }