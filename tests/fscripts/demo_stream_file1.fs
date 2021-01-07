b = 0
a = ostream_file_create("test.bin", "wb+")

assert(ostream_write_uint8(a, 1, 2, 3, 4) == 4)
assert(ostream_write_int8(a, -1, -2, -3, -4) == 4)
assert(ostream_tell(a), 8)

assert(ostream_write_uint16(a, 1, 2) == 4)
assert(ostream_write_int16(a, -1, -2) == 4)
assert(ostream_tell(a), 16)

assert(ostream_write_uint32(a, 1, 2) == 8)
assert(ostream_write_int32(a, -1, -2) == 8)
assert(ostream_tell(a), 32)

assert(ostream_write_uint64(a, 1, 2) == 16)
assert(ostream_write_int64(a, -1, -2) == 16)
assert(ostream_tell(a), 64)

assert(ostream_write_float(a, 1, 2) == 8)
assert(ostream_tell(a), 72)

assert(ostream_write_double(a, 1, 2) == 16)
assert(ostream_tell(a), 88)

a = istream_file_create("test.bin", "rb")
assert(istream_read_uint8(a)==1)
assert(istream_read_uint8(a)==2)
assert(istream_read_uint8(a)==3)
assert(istream_read_uint8(a)==4)
assert(istream_read_int8(a)==-1)
assert(istream_read_int8(a)==-2)
assert(istream_read_int8(a)==-3)
assert(istream_read_int8(a)==-4)

assert(istream_read_uint16(a)==1)
assert(istream_read_uint16(a)==2)
assert(istream_read_int16(a)==-1)
assert(istream_read_int16(a)==-2)

assert(istream_read_uint32(a)==1)
assert(istream_read_uint32(a)==2)
assert(istream_read_int32(a)==-1)
assert(istream_read_int32(a)==-2)

assert(istream_read_uint64(a)==1)
assert(istream_read_uint64(a)==2)
assert(istream_read_int64(a)==-1)
assert(istream_read_int64(a)==-2)

assert(istream_read_float(a) == 1)
assert(istream_read_float(a) == 2)
assert(istream_read_double(a) == 1)
assert(istream_read_double(a) == 2)

unset(a)

