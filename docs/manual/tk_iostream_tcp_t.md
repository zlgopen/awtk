## tk\_iostream\_tcp\_t
### 概述
![image](images/tk_iostream_tcp_t_0.png)

基于TCP实现的输入输出流。
----------------------------------
### 函数
<p id="tk_iostream_tcp_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_iostream_tcp_t_tk_iostream_tcp_create">tk\_iostream\_tcp\_create</a> | 创建iostream对象。 |
#### tk\_iostream\_tcp\_create 函数
-----------------------

* 函数功能：

> <p id="tk_iostream_tcp_t_tk_iostream_tcp_create">创建iostream对象。

* 函数原型：

```
tk_iostream_t* tk_iostream_tcp_create (int sock);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_iostream\_t* | 返回iostream对象。 |
| sock | int | socket。 |
