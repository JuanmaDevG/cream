#include "init_funcs_test.h"
#include "main_funcs.h"

/*
    This test serves as a real use example of this library
*/

enum bool_argument {ARG_A, ARG_B, ARG_C };
enum data_argument {ARG_D, ARG_E };

char* program_input[] = {
    "program_name",
    "-ab",
    "-d=some_value",
    "-e",
    "some_other_value",
    "-e",
    "some_repeated_option_value",
    "another_repeated_option_value"
};

int main()
{
    cargs_set_args("abc", "de");
    cargs_load_args(8, (const char**)program_input);

    assert(cargs_error_code == CARGS_NO_ERROR);
    assert(cargs_check_bool(ARG_A));
    assert(cargs_check_bool(ARG_B));
    assert(!cargs_check_bool(ARG_C));
    assert(cargs_check_data(ARG_D));
    assert(cargs_check_data(ARG_E));

    char** cached_data_buffer = cargs_get_data(ARG_D);
    assert(cargs_get_data_count(ARG_D) == 1);
    assert(cached_data_buffer);
    assert(strcmp(cached_data_buffer[0], "some_value") == 0);
    
    assert(cargs_get_data_count(ARG_E) == 3);
    cached_data_buffer = cargs_get_data(ARG_E);
    assert(strcmp(cached_data_buffer[0], "some_other_value") == 0);
    assert(strcmp(cached_data_buffer[1], "some_repeated_option_value") == 0);
    assert(strcmp(cached_data_buffer[2], "another_repeated_option_value") == 0);

    assert(cargs_clean());

    finish(1, "main functions");
    return 0;
}