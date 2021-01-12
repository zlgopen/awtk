## istream 扩展函数

### 1.istream\_seek
### 2.istream\_tell
### 3.istream\_read\_uint8
### 4.istream\_read\_uint16
### 5.istream\_read\_uint32
### 6.istream\_read\_uint64
### 7.istream\_read\_int8
### 8.istream\_read\_int16
### 9.istream\_read\_int32
### 10.istream\_read\_int64
### 11.istream\_read\_float
### 12.istream\_read\_double
### 13.istream\_read\_string
### 14.istream\_read\_binary
### 15.istream\_read\_line
### 16.istream\_is\_eos

### 更多示例

```js
a = istream_file_create("test.bin", "rb")
assert(istream_read_uint8(a)==1)
assert(istream_read_int8(a)==-1)
assert(istream_tell(a), 2)

assert(istream_read_uint16(a)==2)
assert(istream_read_int16(a)==-2)
assert(istream_tell(a), 6)

assert(istream_read_uint32(a)==3)
assert(istream_read_int32(a)==-3)
assert(istream_tell(a), 14)

assert(istream_read_uint64(a)==4)
assert(istream_read_int64(a)==-4)
assert(istream_tell(a), 30)

assert(istream_read_float(a)==5)
assert(istream_read_double(a)==-5)
assert(istream_tell(a), 42)

assert(istream_read_string(a, 5)=="hello")
assert(istream_tell(a), 47)

istream_read_binary(a, 5)
assert(istream_tell(a), 52)
assert(istream_seek(a, 0))
assert(istream_read_uint8(a)==1)
assert(istream_read_int8(a)==-1)
assert(istream_tell(a) == 2)

unset(a)
```
