
a=wbuffer_create()

assert(wbuffer_write_int8(a, -10, -20) == 2);
assert(wbuffer_write_uint8(a, 10, 20) == 2);
assert(wbuffer_get_cursor(a)==4)

assert(wbuffer_write_int16(a, -10, -20) == 4);
assert(wbuffer_write_uint16(a, 10, 20) == 4);
assert(wbuffer_get_cursor(a)==12)

assert(wbuffer_write_int32(a, -10, -20) == 8);
assert(wbuffer_write_uint32(a, 10, 20) == 8);
assert(wbuffer_get_cursor(a)==28)

assert(wbuffer_write_int64(a, -10, -20) == 16);
assert(wbuffer_write_uint64(a, 10, 20) == 16);
assert(wbuffer_get_cursor(a)==60)

assert(wbuffer_write_float(a, -10, -20) == 8);
assert(wbuffer_write_double(a, 10, 20) == 16);
assert(wbuffer_get_cursor(a)==84)

assert(wbuffer_write_string(a, "ab", "cd") == 6);
assert(wbuffer_write_binary(a, "1234", 4) == 4);
assert(wbuffer_get_cursor(a)==94)

b=rbuffer_create(wbuffer_get_data(a), wbuffer_get_cursor(a))

assert(rbuffer_get_cursor(b)==0)
assert(rbuffer_read_int8(b) == -10)
assert(rbuffer_read_int8(b) == -20)
assert(rbuffer_read_uint8(b) == 10)
assert(rbuffer_read_uint8(b) == 20)

assert(rbuffer_read_int16(b) == -10)
assert(rbuffer_read_int16(b) == -20)
assert(rbuffer_read_uint16(b) == 10)
assert(rbuffer_read_uint16(b) == 20)

assert(rbuffer_read_int32(b) == -10)
assert(rbuffer_read_int32(b) == -20)
assert(rbuffer_read_uint32(b) == 10)
assert(rbuffer_read_uint32(b) == 20)

assert(rbuffer_read_int64(b) == -10)
assert(rbuffer_read_int64(b) == -20)
assert(rbuffer_read_uint64(b) == 10)
assert(rbuffer_read_uint64(b) == 20)

assert(rbuffer_read_float(b) == -10)
assert(rbuffer_read_float(b) == -20)
assert(rbuffer_read_double(b) == 10)
assert(rbuffer_read_double(b) == 20)

assert(rbuffer_read_string(b) == "ab")
assert(rbuffer_read_string(b) == "cd")

unset(a)
unset(b)


