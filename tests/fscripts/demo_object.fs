a = object_create()
assert(!value_is_null(a))
assert(object_set(a, "age", 123))
assert(object_get(a, "age") == 123)
print(object_get(a, "age"))
assert(object_remove(a, "age"))
assert(!value_is_valid(object_get(a, "age")))

unset(a)
