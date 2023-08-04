#pragma once

#include "shared_data.h"

inline void _cargs_init_anon_list();

inline void _cargs_set_actual_node(char** data_location, uint32_t argument_count);

inline void _cargs_push_anon_node();