a=typed_array_create("i32", 10)

typed_array_push(a, 123);
assert(typed_array_get_size(a), 1);
assert(typed_array_get_bytes(a), 4);

typed_array_push(a, 234); 
assert(typed_array_get_size(a), 2);
assert(typed_array_get_bytes(a), 8);

assert(typed_array_get(a, 0)==123)
assert(typed_array_get(a, 1)==234)

assert(typed_array_set(a, 0, 100))
assert(typed_array_set(a, 1, 200))

assert(typed_array_get(a, 0)==100)
assert(typed_array_get(a, 1)==200)

assert(typed_array_get(a, 0)==100)
assert(typed_array_get(a, 1)==200)

assert(typed_array_insert(a, 0, 10))
assert(typed_array_get(a, 0)==10)
assert(typed_array_get(a, 1)==100)
assert(typed_array_get(a, 2)==200)
assert(typed_array_get_size(a), 3);

assert(typed_array_pop(a), 200);
assert(typed_array_get_size(a), 2);

assert(typed_array_clear(a));
assert(typed_array_get_size(a) == 0);

unset(a);

