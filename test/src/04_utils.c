#include "utils_test.h"

/*
    REMOVING REDUNDANCIES
*/

int main()
{
    _bool_args = new_bool_args;
    _data_args = new_data_args;
    _bool_args_count = strlen(new_bool_args);
    _data_packs.size = strlen(new_data_args);

    _data_args[0] = 'a';
    _data_args[2] = 'b';

    _remove_redundancies(REMOVE_DATA_REDUNDANCIES);

    assert(strcmp(_data_args,"hjkl") == 0);

    _bool_args[1] = 'h';
    _bool_args[3] = 'l';

    _remove_redundancies(REMOVE_BOOL_REDUNDANCIES);

    assert(strcmp(_bool_args, "acef") == 0);

    /*
    //--------------------------
    //Finding extended arguments
    //--------------------------

    _extended_args.size = 3;
    _extended_args.args = (ExtArg*)calloc(_extended_args.size, sizeof(ExtArg));
    _extended_args.args[1].associated_opt = 3; //f
    _extended_args.args[1].read_point = _bool_args;
    _extended_args.args[1].name = (char*)malloc(strlen("force") +1);
    memcpy(_extended_args.args[1].name, "force", 6);

    assert(!_find_extended_argument("must-not-find"));
    assert(_get_actual_ext_checkpoint() == 0);

    assert(_find_extended_argument("force"));
    assert(_get_actual_ext_checkpoint() == 2);

    //-----------------------------------------------------------
    //Adding argument data pointers to the argument data packages
    //-----------------------------------------------------------
    
    //force must be data args but don't mind for this example
    char* tmp_argv[] = {
        "-h",
        "application",
        "reader",
        "writer",
        "--force",
        "file1.txt",
        "file2.txt",
        "file3.txt"
    };
    _data_packs.size = 4;
    _data_packs.packages = (ArgPackage*)malloc(_data_packs.size * sizeof(ArgPackage));

    uint32_t argument_position = 4, extended_arg_vec_position = 1;
    //No needed find operation because it is extended
    assert(_add_argument_data(8, (const char**)tmp_argv, &argument_position, &extended_arg_vec_position));

    //Package number 3 because the extended arg associated option position is 3
    assert(_data_packs.packages[3].count == 3);
    assert(_data_packs.packages[3].values = tmp_argv +5);

    //Now the non-extended argument version
    argument_position = 0;
    _find_argument_char('h');
    assert(_add_argument_data(8, (const char**)tmp_argv, &argument_position, NULL));
    assert(_data_packs.packages[0].count == 3);
    assert(_data_packs.packages[0].values == tmp_argv +1);
    */

    printf(finish_msg, 4);
    return 0;
}