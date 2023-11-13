#include <stdio.h>
#include <assert.h>
#include "init_funcs.h"

const char* finish_msg = "Finished test %d from %s\n";

void finish(uint32_t test_id, const char* test_name) { printf(finish_msg, test_id, test_name); }