a=typed_array_create("i8", 8)
typed_array_push(a, 1, 2, 3, 4, 5, 6, 7, 8);

b=wbuffer_attach(a.data, a.bytes)
assert(wbuffer_write_int8(b, 1, 2, 3, 4) == 4)
assert(b.cursor==4)
assert(b.capacity==8)

print(b.data, b.cursor, b.capacity)

unset(a)
unset(b)

