
a=wbuffer_create()
wbuffer_write_uint8(a, 0x10) 
assert(wbuffer_get_cursor(a)==1)
wbuffer_write_uint16(a, 0x1122) 
assert(wbuffer_get_cursor(a)==3)
wbuffer_write_uint32(a, 0x11223344) 
assert(wbuffer_get_cursor(a)==7)
wbuffer_write_uint64(a, 0x1122334455667788)
assert(wbuffer_get_cursor(a)==15)

wbuffer_write_string(a, "hello");
assert(wbuffer_get_cursor(a)==21)

wbuffer_write_binary(a, "wolrd", 6);
assert(wbuffer_get_cursor(a)==27)

b=rbuffer_create(wbuffer_get_data(a), wbuffer_get_cursor(a));

assert(rbuffer_get_cursor(b)==0)
assert(rbuffer_read_uint8(b)==0x10)

assert(rbuffer_get_cursor(b)==1)
assert(rbuffer_read_uint16(b)==0x1122)

assert(rbuffer_get_cursor(b)==3)
assert(rbuffer_read_uint32(b)==0x11223344)

assert(rbuffer_get_cursor(b)==7)
assert(rbuffer_read_uint64(b)==0x1122334455667788)

assert(rbuffer_get_cursor(b)==15)
assert(rbuffer_read_string(b)=="hello")
assert(rbuffer_get_cursor(b)==21)
c = rbuffer_read_binary(b, 6)
assert(rbuffer_get_cursor(b)==27)

wbuffer_rewind(a);
assert(wbuffer_get_cursor(a)==0)
wbuffer_write_binary(a, c);
assert(wbuffer_get_cursor(a)==6)

unset(a)
unset(b)
