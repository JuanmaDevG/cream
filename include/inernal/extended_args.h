#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t associated_opt; //Option symbol position (to look into bit vectors and ArgPackage vectors)
    char* name;
} ExtArg;

typedef struct {
    size_t size;
    ExtArg* args;
} ExtArgVec;