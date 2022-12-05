## theme\_default\_t
### 概述
窗体样式默认实现。
负责管理缺省的窗体样式数据，方便实现style\_const。
----------------------------------
### 函数
<p id="theme_default_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#theme_default_t_theme_default_create">theme\_default\_create</a> | 创建窗体样式对象。 |
| <a href="#theme_default_t_theme_default_create_ex">theme\_default\_create\_ex</a> | 创建窗体样式对象。 |
#### theme\_default\_create 函数
-----------------------

* 函数功能：

> <p id="theme_default_t_theme_default_create">创建窗体样式对象。

* 函数原型：

```
theme_t* theme_default_create (const uint8_t* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回窗体样式对象。 |
| data | const uint8\_t* | 窗体样式数据。 |
#### theme\_default\_create\_ex 函数
-----------------------

* 函数功能：

> <p id="theme_default_t_theme_default_create_ex">创建窗体样式对象。

* 函数原型：

```
theme_t* theme_default_create_ex (const uint8_t* data, bool_t need_free_data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回窗体样式对象。 |
| data | const uint8\_t* | 窗体样式数据。 |
| need\_free\_data | bool\_t | 销毁时是否释放数据。 |
