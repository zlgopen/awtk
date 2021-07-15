a = array_create_with_str("2,1,3", ",", "int");
assert(array_sort(a));
d = array_join(a, ":");
assert(d == "1:2:3")

assert(array_sort(a, false));
d = array_join(a, ":");
assert(d == "3:2:1")


a = array_create_with_str("a2,B1,c3", ",", "str");
assert(array_sort(a));
d = array_join(a, ":");
assert(d == "B1:a2:c3")

a = array_create_with_str("a2,B1,c3", ",", "str");
assert(array_sort(a, true, true));
d = array_join(a, ":");
assert(d == "a2:B1:c3")

a = array_create_with_str("a2,B1,c3", ",", "str");
assert(array_sort(a, false, true));
d = array_join(a, ":");
assert(d == "c3:B1:a2")
