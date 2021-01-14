## 文件流扩展函数

### 1.istream\_file\_create

> 创建文件输入流对象。
----------------------------

#### 原型

```js
istream_file_create(filename, mode) => object
```

### 2.ostream\_file\_create

> 创建文件输出流对象。
----------------------------

#### 原型

```js
ostream_file_create(filename, mode) => object
```

### 完整示例

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
