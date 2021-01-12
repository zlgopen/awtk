//test string
assert(crc16("123") == 40116)
assert(crc32("123") == u32(2286445522))
assert(cksum("123") == u16(65385))

//test binary
a=typed_array_create("i8", 10)
assert(typed_array_push(a, 1,2,3) == 3)
print(crc16(a.data, a.size))
print(crc32(a.data, a.size))
print(cksum(a.data, a.size))
unset(a);

//test binary
assert(file_write("test.bin", "hello", 5))
assert(file_exist("test.bin"))
a = file_read_binary("test.bin")
assert(crc16(a) == 13501)
assert(crc32(a) == 907060870)
assert(cksum(a)==65003)
assert(file_remove("test.bin"))
unset(a)
