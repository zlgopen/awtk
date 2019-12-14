## style\_factory\_t
### 概述
style工厂接口，主要用于创建style。

> 在特殊情况下，可以实现自己的style工厂，比如支持CSS和可实时修改的style。
----------------------------------
### 函数
<p id="style_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#style_factory_t_style_factory">style\_factory</a> | 获取缺省的style factory。 |
| <a href="#style_factory_t_style_factory_create_style">style\_factory\_create\_style</a> | 创建style。 |
| <a href="#style_factory_t_style_factory_set">style\_factory\_set</a> | 设置缺省的style factory(需要自定义的style factory才设置)。 |
#### style\_factory 函数
-----------------------

* 函数功能：

> <p id="style_factory_t_style_factory">获取缺省的style factory。

* 函数原型：

```
style_factory_t* style_factory ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | style\_factory\_t* | 返回style factory对象。 |
#### style\_factory\_create\_style 函数
-----------------------

* 函数功能：

> <p id="style_factory_t_style_factory_create_style">创建style。

* 函数原型：

```
style_t* style_factory_create_style (style_factory_t* factory, widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | style\_t* | 返回style对象。 |
| factory | style\_factory\_t* | factory对象。 |
| widget | widget\_t* | 控件对象。 |
#### style\_factory\_set 函数
-----------------------

* 函数功能：

> <p id="style_factory_t_style_factory_set">设置缺省的style factory(需要自定义的style factory才设置)。

* 函数原型：

```
ret_t style_factory_set (style_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | style\_factory\_t* | factory对象。 |
