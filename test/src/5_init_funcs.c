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

//Everything ok, no limits, no constraints
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
    "another_file.txt",
    "--file",
    "repeated_option_file.csv",
    "overload_arg_pointer_list.txt"
};

//Option z does not exist and should throw errors
char* tmp_argv2[] = {
    "program_name",
    "-z"
};

//Option z does not exist in multi_boolean format
char* tmp_argv3[] = {
    "program_name",
    "-az"
};

//Option --some-random-option does not exist in extended arguments
char* tmp_argv4[] = {
    "program_name",
    "--some-random-option"
};

//Let's assume argument option a is mandatory and throws an error
char* tmp_argv5[] = {
    "program_name", 
    "-ct",
    "some_anon_arg", 
    "-f"                //Lonely data options should be possible if there's no minimum data required
};

//A wrong identificator should give an error when anonymous arguments are interpreted as errors
char* tmp_argv6[] = {
    "program_name",
    "-c",
    "/h"
};

//Error because -f now accepts a minimum of one ergument (like a filename for example)
char* tmp_argv8[] = {
    "program_name",
    "-f"
};

//Error because repeated argument options are treated as errors
char* tmp_argv9[] = {
    "program_name",
    "-f=file.txt",
    "-f"
};

//Error as while treating anonymous arguments as errors
char* tmp_argv10[] = {
    "program_name",
    "-acf",
    "-s=some_file.devnotes",
    "anonymous_arg_that_should_fail"
};

int main()
{
    cargs_set_args(tmp_bool_args, tmp_data_args);
    cargs_associate_extended("actfs", "abort-when-fail", "copy-symbols", "time-benchmark", "file", "save-in");

    /*
        DEV NOTE:
        remember to update argc each argument load and each argument modification
    */
    tmp_argc = 13;
    cargs_load_args(tmp_argc, (const char**)tmp_argv1);

    assert(cargs_error_code == CARGS_NO_ERROR);
    assert(_cargs_anon_arg_count == 3);
    assert(_cargs_anon_args->package.count == 2);
    assert(_cargs_get_list_package(_cargs_anon_args, 1));
    assert(!_cargs_get_list_package(_cargs_anon_args, 2));
    assert(_cargs_get_bit(_cargs_bool_bit_vec, 0));
    assert(_cargs_get_bit(_cargs_bool_bit_vec, 1));
    assert(_cargs_get_bit(_cargs_bool_bit_vec, 2));
    assert(_cargs_get_bit(_cargs_data_bit_vec, 0));
    assert(_cargs_get_bit(_cargs_data_bit_vec, 1));
    assert(_cargs_bank_stack_pointer);
    assert(_cargs_equals_operator_pointer_bank);
    assert(_cargs_data_args_count == 2);
    assert(_cargs_data_packs[0].count == 1);
    assert(strcmp(_cargs_data_packs[0].values[0], "file.txt") == 0);
    assert(_cargs_data_packs[1].count == 2);
    assert(_cargs_get_list_package(_cargs_redundant_opt_data->first_node, 0));
    assert(!_cargs_get_list_package(_cargs_redundant_opt_data->first_node, 1));
    assert(cargs_clean());

    //Test: error managing and declaring

    tmp_argc = 2;
    cargs_set_args(tmp_bool_args, tmp_data_args);
    cargs_load_args(tmp_argc, (const char**)tmp_argv2);
    assert(cargs_error_code == CARGS_NON_EXISTENT);

    cargs_load_args(tmp_argc, (const char**)tmp_argv3);
    assert(cargs_error_code == CARGS_NON_EXISTENT);

    cargs_associate_extended("actfs", "abort-when-fail", "copy-symbols", "time-benchmark", "file", "save-in");
    cargs_load_args(tmp_argc, (const char**)tmp_argv4);
    assert(cargs_error_code == CARGS_NON_EXISTENT);
    assert(cargs_clean());

    cargs_set_args(tmp_bool_args, tmp_data_args);
    cargs_make_mandatory("a");
    cargs_load_args(tmp_argc, (const char**)tmp_argv5);
    assert(cargs_error_code == CARGS_MANDATORY);
    assert(cargs_clean());

    cargs_set_args(tmp_bool_args, tmp_data_args);
    cargs_treat_anonymous_args_as_errors(true);
    tmp_argc = 3;
    cargs_load_args(tmp_argc, (const char**)tmp_argv6);
    assert(cargs_error_code == CARGS_NON_EXISTENT);

    //Setting a minimum of arguments to option -f and checking error
    cargs_set_args(tmp_bool_args, tmp_data_args);
    cargs_set_minimum_data("f", 1);
    tmp_argc = 2;
    cargs_load_args(tmp_argc, (const char**)tmp_argv8);
    assert(cargs_error_code == CARGS_NOT_ENOUGH_DATA);

    //Treating redudant argument options as errors
    cargs_set_args(tmp_bool_args, tmp_data_args);
    cargs_treat_repeated_args_as_errors(true);
    tmp_argc = 3;
    cargs_load_args(tmp_argc, (const char**)tmp_argv9);
    assert(cargs_error_code == CARGS_REDUNDANT_ARGUMENT);
    assert(cargs_clean());

    //Treating anonymous arguments as errors
    cargs_treat_repeated_args_as_errors(false);
    cargs_treat_anonymous_args_as_errors(true);
    cargs_set_args(tmp_bool_args, tmp_data_args);
    tmp_argc = 4;
    cargs_load_args(tmp_argc, (const char**)tmp_argv10);
    assert(cargs_error_code == CARGS_NON_EXISTENT);
    assert(cargs_clean());

    finish(5, "init functions");
}