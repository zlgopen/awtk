
a=wbuffer_create()
wbuffer_write_int8(a, -10) 
assert(wbuffer_get_cursor(a)==1)
wbuffer_write_int16(a, -1122) 
assert(wbuffer_get_cursor(a)==3)
wbuffer_write_int32(a, -11223344) 
assert(wbuffer_get_cursor(a)==7)
wbuffer_write_int64(a, -1122334455667788)
assert(wbuffer_get_cursor(a)==15)

wbuffer_write_string(a, "hello")
assert(wbuffer_get_cursor(a)==21)

wbuffer_write_binary(a, "wolrd", 6)
assert(wbuffer_get_cursor(a)==27)

b=rbuffer_create(wbuffer_get_data(a), wbuffer_get_cursor(a))
assert(rbuffer_get_capacity(b) == wbuffer_get_cursor(a))

assert(rbuffer_get_cursor(b)==0)
assert(rbuffer_read_int8(b)== -10)

assert(rbuffer_get_cursor(b)==1)
assert(rbuffer_read_int16(b)==-1122)

assert(rbuffer_get_cursor(b)==3)
assert(rbuffer_read_int32(b)==-11223344)

assert(rbuffer_get_cursor(b)==7)
assert(rbuffer_read_int64(b)==-1122334455667788)

assert(rbuffer_get_cursor(b)==15)
assert(rbuffer_read_string(b)=="hello")
assert(rbuffer_get_cursor(b)==21)
c = rbuffer_read_binary(b, 6)
assert(rbuffer_get_cursor(b)==27)

assert(rbuffer_rewind(b))
assert(rbuffer_skip(b, 1))
assert(rbuffer_get_cursor(b)==1)
assert(rbuffer_read_int16(b)==-1122)

wbuffer_rewind(a)
assert(wbuffer_get_cursor(a)==0)
wbuffer_write_binary(a, c)
assert(wbuffer_get_cursor(a)==6)

unset(a)
unset(b)


