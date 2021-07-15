a = array_create_with_str("1,2,3", ",");
assert(a.size == 3);

d = array_join(a, ":");
assert(d == "1:2:3")

b = array_dup(a);
assert(b.size == 3);
d = array_join(b, ":");
assert(d == "1:2:3")

c = array_dup(a, 1, 3);
assert(c.size == 2);
d = array_join(c, ":");
assert(d == "2:3")

