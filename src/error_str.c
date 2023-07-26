#include "shared_data.h"

char* _cargs_error_str_buffer = NULL; //Will probably use alloca()

uint8_t _cargs_is_extended = 0;
char* _cargs_error_argument = NULL;

const char* _cargs_error_strings[] = {
    "No errors, argument load finished successfully",
    "The argument \0 does not exist",
    "The argument \0 is required",
    "The argument \0 has a wrong identificator"
};