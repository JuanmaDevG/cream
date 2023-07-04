// Argument parser library in C

#include "cargs.h"

static char identificator = '-';
static char* boolean_args = NULL;
static char* data_args = NULL;

void cargs_set_identificator(const char new_identificator) {
    identificator = new_identificator;
}

void cargs_set_boolean_args(const uint32_t count, const char* arg_letters) {
    // "abcd" ;  'a', 'b', 'c', 'd'
    boolean_args = (char*)malloc(count * sizeof(char));
    for (int i = 0; i < count; i++) {
        boolean_args[i] = arg_letters[i];
    }

}

void cargs_associate_extended(const char** ext_names, const char* arg_letters) {
    // TODO
}

void cargs_set_data_args(const uint32_t count, const char* arg_letters) {
    // TODO
}
