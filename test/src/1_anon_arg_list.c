#include "utils_test.h"

int main()
{
    init_test_args();
    init_ext_arg_vec();
    init_data_packages();

    _cargs_push_anon_node((const char**)tmp_argv +2, 2);
    assert(_cargs_anon_args);
    assert(_cargs_anon_last);
    assert(_cargs_anon_args == _cargs_anon_last);
    assert(_cargs_anon_arg_count == 2);
    assert(_cargs_anon_args->next == NULL);
    assert(_cargs_anon_last->previous == NULL);
    assert(_cargs_anon_args->package.count == 2);
    assert(_cargs_anon_args->package.values != NULL);
    assert(_cargs_anon_last->previous == NULL);

    _cargs_push_anon_node((const char**)tmp_argv +5, 3);
    assert(_cargs_anon_args != _cargs_anon_last);
    assert(_cargs_anon_arg_count == 5);
    assert(_cargs_anon_last->previous != NULL);

    finish(1, "anonymous argument list");
    return 0;
}