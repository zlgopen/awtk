## 写缓冲扩展函数

wbuffer 有以下属性：

* data 缓冲区的指针
* cursor 当前写的位置
* capacity 最大容量

### 1.wbuffer\_create

> 创建 wbuffer 对象。
----------------------------

#### 原型

```js
wbuffer_create() => object
```

### 2.wbuffer\_attach

> 创建 wbuffer 对象，并附加到指定的数据上。
----------------------------

#### 原型

```js
wbuffer_attach(str) => object
wbuffer_attach(binary) => object
wbuffer_attach(data, size) => object
```

### 3.wbuffer\_skip

> 跳过指定长度的数据。
----------------------------

#### 原型

```js
wbuffer_skip(wbuffer, size) => bool
```

### 4.wbuffer\_write\_uint8

> 写入 uint8_t 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_uint8(wbuffer, v, ...) => int32_t
```

> 返回写入数据的长度（字节数）。

### 5.wbuffer\_write\_uint16

> 写入 uint16_t 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_uint16(wbuffer, v, ...) => int32_t
```

> 返回写入数据的长度（字节数）。

### 6.wbuffer\_write\_uint32

> 写入 uint32_t 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_uint32(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 7.wbuffer\_write\_uint64

> 写入 uint64_t 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_uint64(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 8.wbuffer\_write\_int8

> 写入 int8_t 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_int8(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 9.wbuffer\_write\_int16

> 写入 int16_t 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_int16(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 10.wbuffer\_write\_int32

> 写入 int32_t 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_int32(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 11.wbuffer\_write\_int64

> 写入 int64_t 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_int64(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 12.wbuffer\_write\_float

> 写入 float 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_float(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 13.wbuffer\_write\_double

> 写入 double 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_double(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 14.wbuffer\_write\_string

> 写入 字符串 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_string(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 15.wbuffer\_write\_binary

> 写入 二进制 类型的数据。
----------------------------

#### 原型

```js
wbuffer_write_binary(wbuffer, v, ...) => int32_t
```
> 返回写入数据的长度（字节数）。

### 4.wbuffer\_rewind
> 重置读取指针到 0。
----------------------------

#### 原型

```js
rbuffer_rewind(rbuffer) => bool
```

### 17.wbuffer\_get\_data

> 获取 buffer 的指针。
----------------------------

#### 原型

```js
rbuffer_get_data(rbuffer) => pointer
```

### 18.wbuffer\_get\_cursor
> 获取 buffer 当前的读取位置。
----------------------------

#### 原型

```js
rbuffer_get_cursor(rbuffer) => uint32_t
```

### 19.wbuffer\_get\_capacity
> 获取 buffer 的容量。
----------------------------

#### 原型

```js
rbuffer_get_capacity(rbuffer) => uint32_t
```

### 更多示例

```js
a=wbuffer_create()
wbuffer_write_int8(a, -10) 
assert(wbuffer_get_cursor(a)==1)
wbuffer_write_int16(a, -1122) 
assert(wbuffer_get_cursor(a)==3)
wbuffer_write_int32(a, -11223344) 
assert(wbuffer_get_cursor(a)==7)
wbuffer_write_int64(a, -1122334455667788)
assert(wbuffer_get_cursor(a)==15)

wbuffer_write_string(a, "hello")
assert(wbuffer_get_cursor(a)==21)

wbuffer_write_binary(a, "wolrd", 6)
assert(wbuffer_get_cursor(a)==27)
```
