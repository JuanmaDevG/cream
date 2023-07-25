#include "shared_data.h"

char* _cargs_error_str_buffer = NULL; //Will probably use alloca()

uint8_t _cargs_is_extended = 0;
char* _cargs_error_argument = NULL;

const char* _cargs_error_strings[] = {
    "There was no error",
    "The boolean argument <a> does not exist",
    "The data argument <a> does not exist",
    "The argument <a> is required"
};