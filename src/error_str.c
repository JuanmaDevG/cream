#include "shared_data.h"

enum cargs_error cargs_error_code = CARGS_NO_ERROR;
char* _cargs_error_buffer_str = NULL;

uint8_t _cargs_is_extended = 0;
char* _cargs_error_argument = NULL;

//All the error messages start by "The argument arg_name " and then the error string
const char* _cargs_error_strings[] = {
    NULL,
    "does not exist",
    "is required",
    "has a wrong identificator",
    "is into a multi-bool argument string \n(Note:) Data arguments require input and must be alone",
    "requires extra elements and not enough were given.\n(Note:) Read the usage message"
};