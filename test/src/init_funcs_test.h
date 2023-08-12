#include <stdio.h>
#include <assert.h>
#include "init_funcs.h"

const char* finish_msg = "Finished %d from %s\n";

//Some more needed data...


void finish(uint32_t test_id, const char* test_name) { printf(finish_msg, test_id, test_name); }