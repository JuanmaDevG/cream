#include "utils_test.h"

int main()
{
    init_test_args();
    init_ext_arg_vec();
    init_data_packages();

    _cargs_push_anon_node((const char**)tmp_argv +2, 2);
    const struct ArgPackage* pack = _cargs_get_anon_package(1);
    assert(pack == NULL);
    pack = _cargs_get_anon_package(0);
    assert(pack != NULL);
    assert(pack->count == 2);
    char** compare_pointer = (char**)tmp_argv +2;
    assert(pack->values == compare_pointer);

    finish(2, "anonymous argument list");
    return 0;
}