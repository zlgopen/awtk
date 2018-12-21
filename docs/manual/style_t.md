## style\_t
### 概述
 控件风格。

### 函数
<p id="style_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#style_t_style_destroy">style\_destroy</a> | 销毁style对象 |
| <a href="#style_t_style_get_color">style\_get\_color</a> | 获取指定id的颜色值。 |
| <a href="#style_t_style_get_int">style\_get\_int</a> | 获取指定id的整数格式的值。 |
| <a href="#style_t_style_get_str">style\_get\_str</a> | 获取指定id的字符串格式的值。 |
| <a href="#style_t_style_is_valid">style\_is\_valid</a> | 检查style对象是否有效 |
| <a href="#style_t_style_notify_widget_state_changed">style\_notify\_widget\_state\_changed</a> | widget状态改变时，通知style更新数据。 |
### 属性
<p id="style_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="style_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### style\_destroy 函数
* 函数原型：

```
ret_t style_destroy (style_t* s);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| s | style\_t* | style对象。 |
* 函数功能：

> <p id="style_t_style_destroy"> 销毁style对象




#### style\_get\_color 函数
* 函数原型：

```
color_t style_get_color (style_t* s, style_id_t id, color_t defval);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t | 返回颜色值。 |
| s | style\_t* | style对象。 |
| id | style\_id\_t | 属性ID。 |
| defval | color\_t | 缺省值。 |
* 函数功能：

> <p id="style_t_style_get_color"> 获取指定id的颜色值。




#### style\_get\_int 函数
* 函数原型：

```
int32_t style_get_int (style_t* s, style_id_t id, int32_t defval);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回整数格式的值。 |
| s | style\_t* | style对象。 |
| id | style\_id\_t | 属性ID。 |
| defval | int32\_t | 缺省值。 |
* 函数功能：

> <p id="style_t_style_get_int"> 获取指定id的整数格式的值。




#### style\_get\_str 函数
* 函数原型：

```
const char* style_get_str (style_t* s, style_id_t id, const char* defval);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回字符串格式的值。 |
| s | style\_t* | style对象。 |
| id | style\_id\_t | 属性ID。 |
| defval | const char* | 缺省值。 |
* 函数功能：

> <p id="style_t_style_get_str"> 获取指定id的字符串格式的值。




#### style\_is\_valid 函数
* 函数原型：

```
bool_t style_is_valid (style_t* s);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回是否有效。 |
| s | style\_t* | style对象。 |
* 函数功能：

> <p id="style_t_style_is_valid"> 检查style对象是否有效




#### style\_notify\_widget\_state\_changed 函数
* 函数原型：

```
ret_t style_notify_widget_state_changed (style_t* s, widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| s | style\_t* | style对象。 |
| widget | widget\_t* | 控件对象。 |
* 函数功能：

> <p id="style_t_style_notify_widget_state_changed"> widget状态改变时，通知style更新数据。




