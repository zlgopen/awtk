## easing\_t
### 概述
动画趋势类。
----------------------------------
### 函数
<p id="easing_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#easing_t_easing_get">easing\_get</a> | 获取对应类型的操作函数。 |
| <a href="#easing_t_easing_register">easing\_register</a> | 注册指定名称的动画趋势。 |
#### easing\_get 函数
-----------------------

* 函数功能：

> <p id="easing_t_easing_get">获取对应类型的操作函数。

* 函数原型：

```
easing_func_t easing_get (easing_type_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | easing\_func\_t | 返回对应的操作函数地址。 |
| type | easing\_type\_t | 类型。 |
#### easing\_register 函数
-----------------------

* 函数功能：

> <p id="easing_t_easing_register">注册指定名称的动画趋势。

* 函数原型：

```
uint32_t easing_register (const char* type_name, easing_func_t easing_func);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回对应类型。 |
| type\_name | const char* | 类型名称。 |
| easing\_func | easing\_func\_t | 动画趋势函数。 |
