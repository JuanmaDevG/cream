#include "shared_data.h"

enum cargs_error cargs_error_code = CARGS_NO_ERROR;
char* _cargs_error_str_buffer = NULL;

uint8_t _cargs_is_extended = 0;
char* _cargs_error_argument = NULL;

const char* _cargs_error_strings[] = {
    NULL,
    "The argument \0 does not exist",
    "The argument \0 is required",
    "The argument \0 has a wrong identificator",
    "The argument \0 is into a multi-bool argument string \n(Note:) It should be separated because it's an argument that requires data",
    "The argument \0 requires extra elements and not enough were given.\n(Note:) Read the usage message"
};