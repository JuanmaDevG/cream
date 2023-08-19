#include "init_funcs_test.h"

/*
    Loading the arguments and trying error codes
    Testing the functions the hard as possible
*/

// (abort-when-fail) (copy-symbols) (time-benchmark)
const char* tmp_bool_args = "act";
// (file) (save-in)
const char* tmp_data_args = "fs";

//argc to change every argument load
int tmp_argc = 0;

/*
    Argument sets to test the loads
*/

//Everything ok no limits
char* tmp_argv1[] = {
    "program_name",
    "anonymous_arg1",
    "anonymous_arg2",
    "-a",
    "-ct",
    "-f=file.txt",
    "anonymous_arg3",
    "--save-in",
    "file_result.txt",
    "another_file.txt"
};

int main()
{
    cargs_set_args(tmp_bool_args, tmp_data_args);
    cargs_associate_extended("actfs", "abort-when-fail", "copy-symbols", "time-benchmark", "file", "save-in");

    tmp_argc = 9;
    cargs_load_args(tmp_argc, (const char**)tmp_argv1);

    printf("%s %s\n", _cargs_error_argument, _cargs_error_strings[cargs_error_code]); //MOD
    printf("Minimum_data: %u\n", _cargs_minimum_data[1]); //MOD
    assert(cargs_error_code == CARGS_NO_ERROR);
    assert(_cargs_anon_args != NULL);
    assert(_cargs_anon_arg_count == 3);
    assert(_cargs_anon_args->package.count == 2);
    assert(_cargs_anon_args->next != NULL);
    assert(_bool_args[0] == '\\');
    assert(_bool_args[1] == '\\');
    assert(_bool_args[2] == '\\');
    assert(_cargs_equals_operator_pointer_bank);
    assert(_data_packs.size == 2);
    assert(_data_packs.packages[0].count == 1);
    assert(_data_packs.packages[1].count == 2);


    finish(5, "init functions");
}