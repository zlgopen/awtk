a = object_create()
assert(!value_is_null(a))
assert(object_set(a, "age", 123))
assert(a.age ==  123)

assert(object_set(a, "name", "awtk"))
assert(a.name == "awtk")

assert(object_get(a, "age") == 123)
print(object_get(a, "age"))
assert(object_remove(a, "age"))
assert(!value_is_valid(object_get(a, "age")))
unset(a)

person = object_create()
assert(!value_is_null(person))

assert(object_set(person, "age", 123))
assert(person.age ==  123)

assert(object_set(person, "name", "awtk"))
assert(person.name == "awtk")

unset(person)


