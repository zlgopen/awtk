SWIG_check_num_args(func_name, a, b) #define check_args_nr(a, b, r)
    return_value_if_fail(lua_gettop(L) >= a && lua_gettop(L) <= b, ret);
