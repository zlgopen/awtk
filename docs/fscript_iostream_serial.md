## 串口扩展函数 

### 1.iostream\_serial\_create

> 创建串口输入输出流对象。
----------------------------

#### 原型

```js
iostream_serial_create(device) => object
iostream_serial_create(device, baudrate, bytesize, parity, stopbits, flowcontrol) => object
```

* device 为串口名称。
* baudrate 为波特率，缺省为115200。
* bytesize 为字节位数，缺省为8。
* parity 为奇偶校验，缺省为none。
* stopbits 为停止位，缺省为1。
* flowcontrol 为流控，缺省为none。

> 备注：其中 device 为必填参数，其他均为可选参数。

#### 示例

```js
var a = iostream_serial_create("COM4");
var b = iostream_serial_create("COM5", 115200, 8, "none", 1, "none");
```

### 完整示例

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
