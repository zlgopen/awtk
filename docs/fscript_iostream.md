## iostream 扩展函数

### 1.iostream\_get\_istream

> 获取输入流对象。
----------------------------

#### 原型

```js
iostream_get_istream(iostream) => object
```

#### 示例

```js
var a = iostream_serial_create("COM4");
var b = iostream_get_istream(a);
```

### 2.iostream\_get\_ostream

> 获取输出流对象。
----------------------------

#### 原型

```js
iostream_get_ostream(iostream) => object
```

#### 示例

```js
var a = iostream_serial_create("COM4");
var b = iostream_get_ostream(a);
```

### 更多示例
```js
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
```
