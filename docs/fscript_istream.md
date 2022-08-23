## istream 扩展函数

### 1.istream\_seek

> 定位到指定的偏移量。
----------------------------

#### 原型

```js
istream_seek(istream, offset) => bool
```

> 仅当输入流支持 seek 才能调用本函数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
istream_seek(a, 10);
```

### 2.istream\_tell

> 获取当前的偏移量。
----------------------------

#### 原型

```js
istream_tell(istream) => uint32_t
```

> 仅当输入流支持 tell 才能调用本函数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
istream_seek(a, 10);
print(istream_tell(a));
```

### 3.istream\_read\_uint8

> 读取 uint8_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_uint8(istream, timeout) => uint8_t
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_uint8(a));
```

### 4.istream\_read\_uint16

> 读取 uint16_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_uint16(istream, timeout) => uint16_t
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_uint16(a));
```

### 5.istream\_read\_uint32

> 读取 uint32_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_uint32(istream, timeout) => uint32_t
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_uint32(a));
```

### 6.istream\_read\_uint64

> 读取 uint64_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_uint64(istream, timeout) => uint64_t
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_uint64(a));
```

### 7.istream\_read\_int8

> 读取 int8_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_int8(istream, timeout) => int8_t
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_int8(a));
```

### 8.istream\_read\_int16

> 读取 int16_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_int16(istream, timeout) => int16_t
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_int16(a));
```

### 9.istream\_read\_int32

> 读取 int32_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_int32(istream, timeout) => int32_t
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_int32(a));
```

### 10.istream\_read\_int64

> 读取 int64_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_int64(istream, timeout) => int64_t
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_int64(a));
```

### 11.istream\_read\_float

> 读取 float 类型的数据。
----------------------------

#### 原型

```js
istream_read_float(istream, timeout) => float
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_float(a));
```

### 12.istream\_read\_double

> 读取 double 类型的数据。
----------------------------

#### 原型

```js
istream_read_double(istream, timeout) => double
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_double(a));
```

### 13.istream\_read\_string

> 读取指定长度的字符串。
----------------------------

#### 原型

```js
istream_read_string(istream, size, timeout) => str
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_string(a, 10));
```

### 14.istream\_read\_binary

> 读取指定长度的二进制数据。
----------------------------

#### 原型

```js
istream_read_binary(istream, size, timeout) => binary
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_binary(a, 10));
```

### 15.istream\_read\_line

> 读取一行的字符串。
----------------------------

#### 原型

```js
istream_read_line(istream, size, timeout) => str
```

> timeout 为选填参数。

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_read_line(a, 10));
```

### 16.istream\_is\_eos

> 判读数据是否读完。
----------------------------

#### 原型

```js
istream_is_eos(istream) => bool
```

#### 示例

```js
var a = istream_file_create("test.bin", "rb");
print(istream_is_eos(a));
```

### 更多示例

```js
var a = istream_file_create("test.bin", "rb")
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
