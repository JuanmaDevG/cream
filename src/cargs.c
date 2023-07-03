// Argument parser library in C

#include "cargs.h"

void cargs_set_identificator(const char identificator) {
    cargs_identificator = identificator;
}

void cargs_set_boolean_args(const u_int32_t count, const char* arg_letters) {
    // "abcd" ;  'a', 'b', 'c', 'd'
    // TODO
}

void cargs_associate_extended(const char** ext_names, const char* arg_letters) {
    // TODO
}

void cargs_set_data_args(const u_int32_t count, const char* arg_letters) {
    // TODO
}
