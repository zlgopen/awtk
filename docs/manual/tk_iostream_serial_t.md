## tk\_iostream\_serial\_t
### 概述
![image](images/tk_iostream_serial_t_0.png)

基于串口实现的输入输出流。
----------------------------------
### 函数
<p id="tk_iostream_serial_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_iostream_serial_t_tk_iostream_serial_config">tk\_iostream\_serial\_config</a> | 配置串口。 |
| <a href="#tk_iostream_serial_t_tk_iostream_serial_create">tk\_iostream\_serial\_create</a> | 创建iostream对象。 |
#### tk\_iostream\_serial\_config 函数
-----------------------

* 函数功能：

> <p id="tk_iostream_serial_t_tk_iostream_serial_config">配置串口。

* 函数原型：

```
ret_t tk_iostream_serial_config (tk_iostream_t* iostream, int32_t baudrate, bytesize_t bytesize, parity_t parity, stopbits_t stopbits, flowcontrol_t flowcontrol);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| iostream | tk\_iostream\_t* | 串口对象。 |
| baudrate | int32\_t | 波特率。 |
| bytesize | bytesize\_t | 字节位数。 |
| parity | parity\_t | 奇偶校验。 |
| stopbits | stopbits\_t | 停止位。 |
| flowcontrol | flowcontrol\_t | 流控。 |
#### tk\_iostream\_serial\_create 函数
-----------------------

* 函数功能：

> <p id="tk_iostream_serial_t_tk_iostream_serial_create">创建iostream对象。
> 缺省配置: baudrate=115200 bytesize=8 stopbits=1 flowcontrol=0 parity=0

* 函数原型：

```
tk_iostream_t* tk_iostream_serial_create (const char* port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_iostream\_t* | 返回iostream对象。 |
| port | const char* | port name。 |
