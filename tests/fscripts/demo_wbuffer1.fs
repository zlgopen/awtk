a=wbuffer_create ()
wbuffer_write_binary(a, "wolrd", 6)
assert(wbuffer_get_cursor(a), 6)

b=rbuffer_create(wbuffer_get_data(a), wbuffer_get_cursor(a))
c = rbuffer_read_binary(b, 6)

assert(rbuffer_get_cursor(b), 6)

wbuffer_rewind(a)
assert(wbuffer_get_cursor(a) == 0)
wbuffer_write_binary(a, c)
assert(wbuffer_get_cursor(a)== 6)

unset(a)
unset(b)
unset(c)
