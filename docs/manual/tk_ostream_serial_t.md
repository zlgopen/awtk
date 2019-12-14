## tk\_ostream\_serial\_t
### 概述
基于串口实现的输出流。

input stream base on fd
----------------------------------
### 函数
<p id="tk_ostream_serial_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_ostream_serial_t_tk_ostream_serial_create">tk\_ostream\_serial\_create</a> | 创建ostream对象。 |
#### tk\_ostream\_serial\_create 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_serial_t_tk_ostream_serial_create">创建ostream对象。

* 函数原型：

```
tk_ostream_t* tk_ostream_serial_create (int fd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_ostream\_t* | 返回ostream对象。 |
| fd | int | fd. |
