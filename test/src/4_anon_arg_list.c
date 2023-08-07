#include "utils_test.h"


int main()
{
    _cargs_push_anon_node((const char**)tmp_argv +2, 2);
    _cargs_push_anon_node((const char**)tmp_argv +5, 3);
    _cargs_free_anon_list();

    assert(_cargs_anon_arg_count == 0);
    assert(_cargs_anon_args == NULL);
    assert(_cargs_anon_last == NULL);

    finish(4, "anonymous argument list");
    return 0;
}
