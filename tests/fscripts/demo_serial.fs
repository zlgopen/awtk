//
// start serial echo server first
// ./bin/serial_recv 4000
//

var a = iostream_serial_create("/dev/ttys001", 0)
var b = iostream_get_ostream(a)
var c = iostream_get_istream(a)

assert(ostream_write_uint32(b, 6) == 4)
assert(ostream_write_string(b, "hello\n") == 6)

assert(istream_read_uint32(c, 3000) == 6)
assert(istream_read_string(c, 6, 3000) == "hello\n")

unset(c)
unset(b)
unset(a)
