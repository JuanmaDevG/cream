#include "shared_data.h"

char arg_id = '-';

/*
    Boolean argument structures
*/
//
size_t _bool_args_count = 0;
char* _bool_args = NULL;


/*
    Data argument structures
*/
//
char* _data_args = NULL;
ArgPackageVec _data_packs = {0, NULL};


/*
    Extended arguments management structures
*/
//
ExtArgVec _extended_args = {0, NULL};