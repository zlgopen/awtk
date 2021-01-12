## ostream 扩展函数

### 1.ostream\_seek
### 2.ostream\_tell
### 3.ostream\_flush
### 4.ostream\_write\_uint8
### 5.ostream\_write\_uint16
### 6.ostream\_write\_uint32
### 7.ostream\_write\_uint64
### 8.ostream\_write\_int8
### 9.ostream\_write\_int16
### 10.ostream\_write\_int32
### 11.ostream\_write\_int64
### 12.ostream\_write\_float
### 13.ostream\_write\_double
### 14.ostream\_write\_string
### 15.ostream\_write\_binary

### 更多示例

```js
a = ostream_file_create("test.bin", "wb+")

assert(ostream_write_uint8(a, 1) == 1)
assert(ostream_write_int8(a, -1) == 1)
assert(ostream_tell(a), 2)

assert(ostream_write_uint16(a, 2) == 2)
assert(ostream_write_int16(a, -2) == 2)
assert(ostream_tell(a), 6)

assert(ostream_write_uint32(a, 3) == 4)
assert(ostream_write_int32(a, -3) == 4)
assert(ostream_tell(a), 14)

assert(ostream_write_uint64(a, 4) == 8)
assert(ostream_write_int64(a, -4) == 8)
assert(ostream_tell(a), 30)

assert(ostream_write_float(a, 5) == 4)
assert(ostream_write_double(a, -5) == 8)
assert(ostream_tell(a), 42)

assert(ostream_write_string(a, "hello") == 5)
assert(ostream_write_binary(a, "world", 5) == 5)
assert(ostream_tell(a), 52)

assert(ostream_flush(a))
```
