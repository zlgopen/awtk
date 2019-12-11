## socketpair\_t
### 概述
socketpair
----------------------------------
### 函数
<p id="socketpair_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#socketpair_t_tk_socketpair">tk\_socketpair</a> | 生成两个socket句柄。两者可以模拟客户端和服务器通信。 |
#### tk\_socketpair 函数
-----------------------

* 函数功能：

> <p id="socketpair_t_tk_socketpair">生成两个socket句柄。两者可以模拟客户端和服务器通信。

* 函数原型：

```
int tk_socketpair (int* socks);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 小于0表示失败。 |
| socks | int* | 由于返回生成的socket句柄。 |
