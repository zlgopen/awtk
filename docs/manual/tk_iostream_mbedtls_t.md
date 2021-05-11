## tk\_iostream\_mbedtls\_t
### 概述
![image](images/tk_iostream_mbedtls_t_0.png)

基于MBEDTLS实现的输入输出流。
----------------------------------
### 函数
<p id="tk_iostream_mbedtls_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_iostream_mbedtls_t_tk_iostream_mbedtls_create">tk\_iostream\_mbedtls\_create</a> | 创建iostream对象。 |
#### tk\_iostream\_mbedtls\_create 函数
-----------------------

* 函数功能：

> <p id="tk_iostream_mbedtls_t_tk_iostream_mbedtls_create">创建iostream对象。

* 函数原型：

```
tk_iostream_t* tk_iostream_mbedtls_create (mbedtls_conn_t* conn);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_iostream\_t* | 返回iostream对象。 |
| conn | mbedtls\_conn\_t* | ssl连接。 |
