
a = array_create_repeated(100, 10)
assert(a.size == 10)
assert(array_pop(a), 100)
assert(array_clear(a))
assert(a.size == 0)
