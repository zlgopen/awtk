# 读缓冲区扩展函数

rbuffer 有以下属性：

* data 缓冲区的指针
* cursor 当前读的位置
* capacity 最大容量

## rbuffer\_create

> 创建 rbuffer 对象。
----------------------------

### 原型

```js
rbuffer_create(str) => object
rbuffer_create(binary) => object
rbuffer_create(data, size) => object
```

### 示例

```js
var a = rbuffer_create("hello");
```

## rbuffer\_skip

> 跳过指定长度的数据。
----------------------------

### 原型

```js
rbuffer_skip(rbuffer, size) => bool
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_skip(a, 2));
```

## rbuffer\_rewind

> 重置读取指针到 0。
----------------------------

### 原型

```js
rbuffer_rewind(rbuffer) => bool
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_skip(a, 2));
print(rbuffer_rewind(a));
```

## rbuffer\_read\_uint8

> 读取 uint8_t 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_uint8(rbuffer) => uint8_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_uint8(a));
```

## rbuffer\_read\_uint16

> 读取 uint16_t 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_uint16(rbuffer) => uint16_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_uint16(a));
```

## rbuffer\_read\_uint32

> 读取 uint32_t 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_uint32(rbuffer) => uint32_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_uint32(a));
```

## rbuffer\_read\_uint64

> 读取 uint64_t 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_uint64(rbuffer) => uint64_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_uint64(a));
```

## rbuffer\_read\_int8

> 读取 int8_t 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_int8(rbuffer) => int8_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_int8(a));
```

## rbuffer\_read\_int16

> 读取 int16_t 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_int16(rbuffer) => int16_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_int16(a));
```

## rbuffer\_read\_int32

> 读取 int32_t 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_int32(rbuffer) => int32_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_int32(a));
```

## rbuffer\_read\_int64

> 读取 int64_t 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_int64(rbuffer) => int64_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_int64(a));
```

## rbuffer\_read\_float

> 读取 float 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_float(rbuffer) => float
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_float(a));
```

## rbuffer\_read\_double

> 读取 double 类型的数据。
----------------------------

### 原型

```js
rbuffer_read_double(rbuffer) => double
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_double(a));
```

## rbuffer\_read\_string

> 读取字符串。
----------------------------

### 原型

```js
rbuffer_read_string(rbuffer) => str
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_string(a));
```

## rbuffer\_read\_binary

> 读取指定长度的二进制数据。
----------------------------

### 原型

```js
rbuffer_read_binary(rbuffer, size) => binary
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_read_binary(a, 5));
```

## rbuffer\_get\_data

> 获取 buffer 的指针。
----------------------------

### 原型

```js
rbuffer_get_data(rbuffer) => pointer
```

### 示例

```js
var a = rbuffer_create("hello");
var b = rbuffer_get_data(a);
```

## rbuffer\_get\_cursor

> 获取 buffer 当前的读取位置。
----------------------------

### 原型

```js
rbuffer_get_cursor(rbuffer) => uint32_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_get_cursor(a));
```

## rbuffer\_get\_capacity

> 获取 buffer 的容量。
----------------------------

### 原型

```js
rbuffer_get_capacity(rbuffer) => uint32_t
```

### 示例

```js
var a = rbuffer_create("hello");
print(rbuffer_get_capacity(a));
```

## 更多示例

```js
...

var b=rbuffer_create(wbuffer_get_data(a), wbuffer_get_cursor(a))

assert(rbuffer_get_cursor(b)==0)
assert(rbuffer_read_int8(b)== -10)

assert(rbuffer_get_cursor(b)==1)
assert(rbuffer_read_int16(b)==-1122)

assert(rbuffer_get_cursor(b)==3)
assert(rbuffer_read_int32(b)==-11223344)

assert(rbuffer_get_cursor(b)==7)
assert(rbuffer_read_int64(b)==-1122334455667788)

assert(rbuffer_get_cursor(b)==15)
assert(rbuffer_read_string(b)=="hello")
assert(rbuffer_get_cursor(b)==21)
var c = rbuffer_read_binary(b, 6)
assert(rbuffer_get_cursor(b)==27)

wbuffer_rewind(a)
assert(wbuffer_get_cursor(a)==0)
wbuffer_write_binary(a, c)
assert(wbuffer_get_cursor(a)==6)

unset(a)
unset(b)
unset(c)
```
