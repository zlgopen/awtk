## theme\_t
### 概述
窗体样式。
----------------------------------
### 函数
<p id="theme_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#theme_t_theme">theme</a> | 获取缺省的窗体样式对象。 |
| <a href="#theme_t_theme_destroy">theme\_destroy</a> | 析构并释放窗体样式对象。 |
| <a href="#theme_t_theme_find_style">theme\_find\_style</a> | 查找满足条件的style。 |
| <a href="#theme_t_theme_foreach">theme\_foreach</a> | 遍历全部数据。 |
| <a href="#theme_t_theme_get_style_type">theme\_get\_style\_type</a> | 获取窗体样式的风格类型。 |
| <a href="#theme_t_theme_load_from_data">theme\_load\_from\_data</a> | 加载窗体样式对象。 |
| <a href="#theme_t_theme_set">theme\_set</a> | 设置缺省的窗体样式对象。 |
| <a href="#theme_t_theme_set_theme_data">theme\_set\_theme\_data</a> | 设置窗体样式的数据。 |
#### theme 函数
-----------------------

* 函数功能：

> <p id="theme_t_theme">获取缺省的窗体样式对象。

* 函数原型：

```
theme_t* theme ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回窗体样式对象。 |
#### theme\_destroy 函数
-----------------------

* 函数功能：

> <p id="theme_t_theme_destroy">析构并释放窗体样式对象。

* 函数原型：

```
ret_t theme_destroy (theme_t* theme);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| theme | theme\_t* | 窗体样式对象。 |
#### theme\_find\_style 函数
-----------------------

* 函数功能：

> <p id="theme_t_theme_find_style">查找满足条件的style。

* 函数原型：

```
const uint8_t* theme_find_style (theme_t* theme, const char* widget_type, const char* name, const char* widget_state);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const uint8\_t* | 返回风格对象数据。 |
| theme | theme\_t* | 窗体样式对象。 |
| widget\_type | const char* | 控件的类型名。 |
| name | const char* | style的名称。 |
| widget\_state | const char* | 控件的状态。 |
#### theme\_foreach 函数
-----------------------

* 函数功能：

> <p id="theme_t_theme_foreach">遍历全部数据。

* 函数原型：

```
ret_t theme_foreach (theme_t* theme, theme_on_data_t on_data, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| theme | theme\_t* | 窗体样式对象。 |
| on\_data | theme\_on\_data\_t | 回调函数。 |
| ctx | void* | 回调函数的上下文。 |
#### theme\_get\_style\_type 函数
-----------------------

* 函数功能：

> <p id="theme_t_theme_get_style_type">获取窗体样式的风格类型。

* 函数原型：

```
const char* theme_get_style_type (theme_t* theme);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回风格类型。 |
| theme | theme\_t* | 窗体样式对象。 |
#### theme\_load\_from\_data 函数
-----------------------

* 函数功能：

> <p id="theme_t_theme_load_from_data">加载窗体样式对象。

* 函数原型：

```
theme_t* theme_load_from_data (const char* name, const uint8_t* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回窗体样式对象。 |
| name | const char* | 资源名。 |
| data | const uint8\_t* | 窗体样式数据。 |
| size | uint32\_t | 资源长度。 |
#### theme\_set 函数
-----------------------

* 函数功能：

> <p id="theme_t_theme_set">设置缺省的窗体样式对象。

* 函数原型：

```
ret_t theme_set (theme_t* theme);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| theme | theme\_t* | 窗体样式对象。 |
#### theme\_set\_theme\_data 函数
-----------------------

* 函数功能：

> <p id="theme_t_theme_set_theme_data">设置窗体样式的数据。

* 函数原型：

```
ret_t theme_set_theme_data (theme_t* theme, const uint8_t* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| theme | theme\_t* | 窗体样式对象。 |
| data | const uint8\_t* | 窗体样式数据。 |
