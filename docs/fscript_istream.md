## istream 扩展函数

### 1.istream\_seek

> 定位到指定的偏移量。
----------------------------

#### 原型

```js
istream_seek(istream, offset) => bool
```

> 仅当输入流支持 seek 才能调用本函数。

### 2.istream\_tell

> 获取当前的偏移量。
----------------------------

#### 原型

```js
istream_tell(istream) => uint32_t
```

> 仅当输入流支持 tell 才能调用本函数。

### 3.istream\_read\_uint8

> 读取 uint8_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_uint8(istream) => uint8_t
```

### 4.istream\_read\_uint16

> 读取 uint16_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_uint16(istream) => uint16_t
```

### 5.istream\_read\_uint32

> 读取 uint32_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_uint32(istream) => uint32_t
```

### 6.istream\_read\_uint64

> 读取 uint64_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_uint64(istream) => uint64_t
```

### 7.istream\_read\_int8

> 读取 int8_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_int8(istream) => int8_t
```

### 8.istream\_read\_int16

> 读取 int16_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_int16(istream) => int16_t
```

### 9.istream\_read\_int32

> 读取 int32_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_int32(istream) => int32_t
```

### 10.istream\_read\_int64

> 读取 int64_t 类型的数据。
----------------------------

#### 原型

```js
istream_read_int64(istream) => int64_t
```

### 11.istream\_read\_float

> 读取 float 类型的数据。
----------------------------

#### 原型

```js
istream_read_float(istream) => float
```

### 12.istream\_read\_double

> 读取 double 类型的数据。
----------------------------

#### 原型

```js
istream_read_double(istream) => double
```

### 13.istream\_read\_string

> 读取指定长度的字符串。
----------------------------

#### 原型

```js
istream_read_string(istream, size) => str
```

### 14.istream\_read\_binary

> 读取指定长度的二进制数据。
----------------------------

#### 原型

```js
istream_read_binary(istream, size) => binary
```

### 15.istream\_read\_line

> 读取一行的字符串。
----------------------------

#### 原型

```js
istream_read_line(istream, size) => str
```

### 16.istream\_is\_eos

> 判读数据是否读完。
----------------------------

#### 原型

```js
istream_is_eos(istream) => bool
```

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
