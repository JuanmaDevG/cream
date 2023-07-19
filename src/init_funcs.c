#include "init_funcs.h"

void cargs_set_identificator(const char new_id) { arg_id = new_id; }

//Makes the boolean args bit vector adding the values
void cargs_set_boolean_args(const char* arg_letters) {
    if(arg_letters == NULL) return;
    _bool_args_count = strlen(arg_letters);

    _bool_args = (char*)malloc(_bool_args_count);
    memcpy(_bool_args, arg_letters, _bool_args_count * sizeof(char));
    remove_redundancies(_data_args, &(_data_packs.size), _bool_args, &_bool_args_count);

    //Calculating the bits for bit boolean vector
    _bool_args_bit_vec = (uint8_t*)calloc(
        (_bool_args_count >> 3) + (size_t)(_bool_args_count & (size_t)0b111 == 0 ? 0 : 1),
        sizeof(uint8_t)
    );
}

void cargs_associate_extended(const char* arg_letters, ...) {
    //Buscar las letras
    for(size_t idx = 0; arg_letters[idx] != '\0'; idx++)
    {
        //Looks for the letters
    }

    //Mirar la va_list para los argumentos a asociar
    //TODO
}

void cargs_set_data_args(const char* arg_letters) {
    if(!arg_letters) return;
    _data_packs.size = strlen(arg_letters);

    _data_args = (char*)malloc(_data_packs.size);
    memcpy(_data_args, arg_letters, _data_packs.size);
    remove_redundancies(_bool_args, _bool_args_count,_data_args, &(_data_packs.size));
    _data_packs.packages = (ArgPackage*)calloc(_data_packs.size, sizeof(ArgPackage));

    _data_args_bit_vec = (uint8_t*)calloc(
        (_data_packs.size >> 3) + (size_t)(_data_packs.size & (size_t)0b111 == 0 ? 0 : 1),
        sizeof(uint8_t)
    );
}
