## hal\_shell\_t
### 概述
a shell to access network interface
----------------------------------
### 函数
<p id="hal_shell_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#hal_shell_t_network_shell_run">network\_shell\_run</a> | 运行shell。 |
#### network\_shell\_run 函数
-----------------------

* 函数功能：

> <p id="hal_shell_t_network_shell_run">运行shell。

* 函数原型：

```
ret_t network_shell_run (network_interface_t* network_interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| network\_interface | network\_interface\_t* | 网口对象。 |
