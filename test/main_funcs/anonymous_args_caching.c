#include "init_funcs_test.h"
#include "main_funcs.h"

/*
    This test file is to test the anonymous argument caching behaviour of the library
*/

int tmp_argc = 11;
char* tmp_argv[] = {
    "anonymous1",
    "anonymous2",
    "anonymous3",
    "-f=file.txt",
    "-e",               //-e supports max 3
    "some location",
    "some other location",
    "some another more location",
    "anonymous4",
    "anonymous5",
    "anonymous6"
};

char* compare_vec[] = {
    "anonymous1",
    "anonymous2",
    "anonymous3",
    "anonymous4",
    "anonymous5",
    "anonymous6"
};

int main()
{
    cargs_set_args(NULL, "fe");
    cargs_include_argument_zero(true);
    cargs_set_maximum_data("e", 3);
    cargs_set_minimum_data("e", 3);
    cargs_load_args(tmp_argc, (const char**) tmp_argv);

    assert(cargs_error_code == CARGS_NO_ERROR);
    assert(_cargs_anon_arg_count == 6);
    assert(cargs_check_data(0));
    assert(cargs_check_data(1));

    char** anon_data = cargs_get_anonymous_args();

    assert(strcmp(anon_data[0], compare_vec[0]) == 0);
    assert(strcmp(anon_data[1], compare_vec[1]) == 0);
    assert(strcmp(anon_data[2], compare_vec[2]) == 0);
    assert(strcmp(anon_data[3], compare_vec[3]) == 0);
    assert(strcmp(anon_data[4], compare_vec[4]) == 0);
    assert(strcmp(anon_data[5], compare_vec[5]) == 0);

    assert(cargs_clean());

    finish(2, "main functions");
}