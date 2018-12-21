## theme\_t
### 概述
 主题。

### 函数
<p id="theme_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#theme_t_theme">theme</a> | 获取缺省的主题对象。 |
| <a href="#theme_t_theme_create">theme\_create</a> | 创建主题对象。 |
| <a href="#theme_t_theme_deinit">theme\_deinit</a> | 析构主题对象。 |
| <a href="#theme_t_theme_destroy">theme\_destroy</a> | 析构并释放主题对象。 |
| <a href="#theme_t_theme_find_style">theme\_find\_style</a> | 查找满足条件的style。 |
| <a href="#theme_t_theme_init">theme\_init</a> | 初始化主题对象。 |
| <a href="#theme_t_theme_set">theme\_set</a> | 设置缺省的主题对象。 |
### 属性
<p id="theme_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="theme_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### theme 函数
* 函数原型：

```
theme_t* theme ();
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回主题对象。 |
* 函数功能：

> <p id="theme_t_theme"> 获取缺省的主题对象。



#### theme\_create 函数
* 函数原型：

```
theme_t* theme_create (const uint8_t* data);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回主题对象。 |
| data | const uint8\_t* | 主题数据。 |
* 函数功能：

> <p id="theme_t_theme_create"> 创建主题对象。




#### theme\_deinit 函数
* 函数原型：

```
ret_t theme_deinit (theme_t* theme);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| theme | theme\_t* | 主题对象。 |
* 函数功能：

> <p id="theme_t_theme_deinit"> 析构主题对象。




#### theme\_destroy 函数
* 函数原型：

```
ret_t theme_destroy (theme_t* theme);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| theme | theme\_t* | 主题对象。 |
* 函数功能：

> <p id="theme_t_theme_destroy"> 析构并释放主题对象。




#### theme\_find\_style 函数
* 函数原型：

```
theme_t* theme_find_style (theme_t* data, const char* widget_type, const char* name, widget_state_t widget_state);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回主题对象。 |
| data | theme\_t* | 主题对象。 |
| widget\_type | const char* | 控件的类型名。 |
| name | const char* | style的名称。 |
| widget\_state | widget\_state\_t | 控件的状态。 |
* 函数功能：

> <p id="theme_t_theme_find_style"> 查找满足条件的style。




#### theme\_init 函数
* 函数原型：

```
theme_t* theme_init (theme_t* theme, const uint8_t* data);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回主题对象。 |
| theme | theme\_t* | 主题对象。 |
| data | const uint8\_t* | 主题数据。 |
* 函数功能：

> <p id="theme_t_theme_init"> 初始化主题对象。




#### theme\_set 函数
* 函数原型：

```
ret_t theme_set (theme_t* theme);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| theme | theme\_t* | 主题对象。 |
* 函数功能：

> <p id="theme_t_theme_set"> 设置缺省的主题对象。




