#include "shared_data.h"

char _arg_id = '-';

/*
    ----------------------
    General purpose buffer
    ----------------------
*/

_expandable_stack _cargs_general_buffer = {{0}, NULL, 0, 0, NULL, NULL};

/*
    ---------------------------
    Boolean argument structures
    ---------------------------
*/

size_t _cargs_bool_args_count = 0;
char* _cargs_bool_args = NULL;
uint8_t* _cargs_bool_bit_vec = NULL;


/*
    ------------------------
    Data argument structures
    ------------------------
*/

size_t _cargs_data_args_count = 0;
char* _cargs_data_args = NULL;
uint8_t* _cargs_data_bit_vec = NULL;
ArgPackage* _cargs_data_packs = NULL;
uint32_t _cargs_bank_stack_pointer = 0;
char** _cargs_equals_operator_pointer_bank = NULL;


/*
    ----------------------------------------
    Extended arguments management structures
    ----------------------------------------
*/

ExtArgVec _extended_args = {0, NULL};

/*
    -------------------
    Mandatory arguments
    -------------------
*/

_cargs_buffer_position* _cargs_mandatory_args = NULL;
uint32_t _cargs_mandatory_arg_count = 0;

/*
    ---------------------------------------------------------------
    Anonymous args and data limits on argument control declarations
    ---------------------------------------------------------------
*/

uint32_t _cargs_anon_arg_count = 0;
_cargs_anonymous_node* _cargs_anon_args = NULL;
_cargs_anonymous_node* _cargs_anon_last = NULL;
uint32_t _cargs_anonymous_relocated_buf_size = 0;
char** _cargs_anonymous_relocated_args = NULL;

uint8_t* _cargs_maximum_data = NULL;
uint8_t* _cargs_minimum_data = NULL;

/*
    ----------------
    Utility pointers
    ----------------
*/

char* _read_point = NULL;
uint32_t _checkpoint = 0;
uint32_t _extended_checkpoint = 0;

/*
    ------------------------------------------
    Redundant data argument storage structures
    ------------------------------------------
*/

_cargs_data_storage_list* _cargs_redundant_opt_data = NULL;

uint8_t* _cargs_is_data_relocated_bit_vec = NULL;