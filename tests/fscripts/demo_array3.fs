a = array_create();
print(a.size);
print(!a.size);
assert(array_last_index_of(a,1), -1);
assert(array_index_of(a,2) == -1);
print(array_push(a,1));
print(array_push(a,2));
print(array_last_index_of(a,2));
assert(array_get(a, 0) == 1)
assert(array_get(a, -1) == 2)
assert(array_get(a, -2) == 1)

assert(array_remove(a, -2))
assert(a.size, 1);

assert(array_remove(a, -1))
print(a.size)

assert(array_insert(a, 0, 1))
assert(array_insert(a, 1, 2))
assert(array_insert(a, 2, 3))
assert(array_insert(a, -1, 4))

assert(array_get(a, -1) == 3)
assert(array_get(a, -2) == 4)

