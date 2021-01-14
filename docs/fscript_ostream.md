## ostream 扩展函数

### 1.ostream\_seek

> 定位到指定的偏移量。
----------------------------

#### 原型

```js
ostream_seek(ostream, offset) => bool
```

### 2.ostream\_tell

> 获取当前的偏移量。
----------------------------

#### 原型

```js
ostream_tell(ostream) => bool
```

> 仅当输入流支持 tell 才能调用本函数。

### 3.ostream\_flush

> 刷新缓冲区。
----------------------------

#### 原型

```js
ostream_flush(ostream) => bool
```

### 4.ostream\_write\_uint8

> 写入 uint8_t 类型的数据。
----------------------------

#### 原型

```js
ostream_write_uint8(ostream, v, ...) => int32_t
```

> 返回写入数据的长度（字节数）。

### 5.ostream\_write\_uint16

> 写入 uint16_t 类型的数据。
----------------------------

#### 原型

```js
ostream_write_uint16(ostream, v, ...) => int32_t
```

> 返回写入数据的长度（字节数）。

### 6.ostream\_write\_uint32

> 写入 uint32_t 类型的数据。
----------------------------

#### 原型

```js
ostream_write_uint32(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 7.ostream\_write\_uint64

> 写入 uint64_t 类型的数据。
----------------------------

#### 原型

```js
ostream_write_uint64(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 8.ostream\_write\_int8

> 写入 int8_t 类型的数据。
----------------------------

#### 原型

```js
ostream_write_int8(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 9.ostream\_write\_int16

> 写入 int16_t 类型的数据。
----------------------------

#### 原型

```js
ostream_write_int16(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 10.ostream\_write\_int32

> 写入 int32_t 类型的数据。
----------------------------

#### 原型

```js
ostream_write_int32(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 11.ostream\_write\_int64

> 写入 int64_t 类型的数据。
----------------------------

#### 原型

```js
ostream_write_int64(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 12.ostream\_write\_float

> 写入 float 类型的数据。
----------------------------

#### 原型

```js
ostream_write_float(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 13.ostream\_write\_double

> 写入 double 类型的数据。
----------------------------

#### 原型

```js
ostream_write_double(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 14.ostream\_write\_string

> 写入 字符串 类型的数据。
----------------------------

#### 原型

```js
ostream_write_string(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 15.ostream\_write\_binary

> 写入 二进制 类型的数据。
----------------------------

#### 原型

```js
ostream_write_binary(ostream, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

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
