## tk\_stream\_factory\_t
### 概述
流工厂。
----------------------------------
### 函数
<p id="tk_stream_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_stream_factory_t_tk_stream_factory_create_iostream">tk\_stream\_factory\_create\_iostream</a> | 创建iostream对象。目前支持： |
#### tk\_stream\_factory\_create\_iostream 函数
-----------------------

* 函数功能：

> <p id="tk_stream_factory_t_tk_stream_factory_create_iostream">创建iostream对象。目前支持：
* tcp client: tcp://host:port
* udp client: udp://host:port
* serial    : serial://device?baudrate=115200&stopbits=1
* serial    : serial://device?baudrate=115200&stopbits=1&parity=odd&flowcontrol=soft&bytesize=8

串口URL参数说明：
* baudrate {int} 波特率。常用值： 9600, 38400, 57600, 115200。(默认值：115200)
* stopbits {string} 停止位。可选值：1, 1.5, 2。(默认值：1)
* parity {string} 校验位。可选值：none, odd, even, mark, space。(默认值：none)
* flowcontrol {string} 流控。可选值：none, hardware, software。(默认值：none)
* bytesize {string} 数据位。可选值：5, 6, 7, 8 。(默认值：8)

* 函数原型：

```
tk_iostream_t* tk_stream_factory_create_iostream (const char* url);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_iostream\_t* | 返回iostream对象。 |
| url | const char* | URL。 |
