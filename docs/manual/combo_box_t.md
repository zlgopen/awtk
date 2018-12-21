## combo\_box\_t
### 概述
![image](images/combo_box_t_0.png)

 combobox控件。
### 函数
<p id="combo_box_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#combo_box_t_combo_box_append_option">combo\_box\_append\_option</a> |  追加一个选项。 |
| <a href="#combo_box_t_combo_box_cast">combo\_box\_cast</a> |  转换combo_box对象(供脚本语言使用)。 |
| <a href="#combo_box_t_combo_box_count_options">combo\_box\_count\_options</a> |  获取选项个数。 |
| <a href="#combo_box_t_combo_box_create">combo\_box\_create</a> |  创建combo_box对象 |
| <a href="#combo_box_t_combo_box_get_option">combo\_box\_get\_option</a> |  获取第index个选项。 |
| <a href="#combo_box_t_combo_box_get_text">combo\_box\_get\_text</a> |  获取combo_box的文本。 |
| <a href="#combo_box_t_combo_box_get_value">combo\_box\_get\_value</a> |  获取combo_box的值。 |
| <a href="#combo_box_t_combo_box_reset_options">combo\_box\_reset\_options</a> |  重置所有选项。 |
| <a href="#combo_box_t_combo_box_set_open_window">combo\_box\_set\_open\_window</a> |  点击按钮时可以打开popup窗口，本函数可设置窗口的名称。 |
| <a href="#combo_box_t_combo_box_set_options">combo\_box\_set\_options</a> |  设置选项。 |
| <a href="#combo_box_t_combo_box_set_selected_index">combo\_box\_set\_selected\_index</a> |  设置第index个选项为当前选中的选项。 |
### 属性
<p id="combo_box_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#combo_box_t_open_window">open\_window</a> | char* |  为点击按钮时，要打开窗口的名称。 |
| <a href="#combo_box_t_options">options</a> | char* |  设置可选项(冒号分隔值和文本，分号分隔选项，如:1:red;2:green;3:blue)。 |
| <a href="#combo_box_t_selected_index">selected\_index</a> | int32_t |  当前选中的选项。 |
| <a href="#combo_box_t_value">value</a> | int32_t |  值。 |
### 事件
<p id="combo_box_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
| EVT\_VALUE\_WILL\_CHANGE | event\_t | 值即将改变事件。 |
| EVT\_VALUE\_CHANGED | event\_t | 值改变事件。 |
#### combo\_box\_append\_option 函数
* 函数原型：

```
ret_t combo_box_append_option (widget_t* widget, int32_t value, char* text);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | combo\_box对象。 |
| value | int32\_t | 值。 |
| text | char* | 文本。 |
* 函数功能：

> <p id="combo_box_t_combo_box_append_option"> 追加一个选项。



#### combo\_box\_cast 函数
* 函数原型：

```
widget_t* combo_box_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | combo\_box对象。 |
| widget | widget\_t* | combo\_box对象。 |
* 函数功能：

> <p id="combo_box_t_combo_box_cast"> 转换combo_box对象(供脚本语言使用)。



#### combo\_box\_count\_options 函数
* 函数原型：

```
int32_t combo_box_count_options (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回选项个数。 |
| widget | widget\_t* | combo\_box对象。 |
* 函数功能：

> <p id="combo_box_t_combo_box_count_options"> 获取选项个数。



#### combo\_box\_create 函数
* 函数原型：

```
widget_t* combo_box_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
* 函数功能：

> <p id="combo_box_t_combo_box_create"> 创建combo_box对象



#### combo\_box\_get\_option 函数
* 函数原型：

```
combo_box_option_t* combo_box_get_option (widget_t* widget, uint32_t index);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | combo\_box\_option\_t* | 返回index个选项。 |
| widget | widget\_t* | combo\_box对象。 |
| index | uint32\_t | 选项的索引。 |
* 函数功能：

> <p id="combo_box_t_combo_box_get_option"> 获取第index个选项。



#### combo\_box\_get\_text 函数
* 函数原型：

```
char* combo_box_get_text (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回文本。 |
| widget | widget\_t* | combo\_box对象。 |
* 函数功能：

> <p id="combo_box_t_combo_box_get_text"> 获取combo_box的文本。



#### combo\_box\_get\_value 函数
* 函数原型：

```
int32_t combo_box_get_value (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回值。 |
| widget | widget\_t* | combo\_box对象。 |
* 函数功能：

> <p id="combo_box_t_combo_box_get_value"> 获取combo_box的值。



#### combo\_box\_reset\_options 函数
* 函数原型：

```
ret_t combo_box_reset_options (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | combo\_box对象。 |
* 函数功能：

> <p id="combo_box_t_combo_box_reset_options"> 重置所有选项。



#### combo\_box\_set\_open\_window 函数
* 函数原型：

```
ret_t combo_box_set_open_window (widget_t* widget, char* open_window);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | combo\_box对象。 |
| open\_window | char* | 弹出窗口的名称。 |
* 函数功能：

> <p id="combo_box_t_combo_box_set_open_window"> 点击按钮时可以打开popup窗口，本函数可设置窗口的名称。



#### combo\_box\_set\_options 函数
* 函数原型：

```
ret_t combo_box_set_options (widget_t* widget, char* options);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | combo\_box对象。 |
| options | char* | 选项。 |
* 函数功能：

> <p id="combo_box_t_combo_box_set_options"> 设置选项。



#### combo\_box\_set\_selected\_index 函数
* 函数原型：

```
ret_t combo_box_set_selected_index (widget_t* widget, uint32_t index);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | combo\_box对象。 |
| index | uint32\_t | 选项的索引。 |
* 函数功能：

> <p id="combo_box_t_combo_box_set_selected_index"> 设置第index个选项为当前选中的选项。



#### open\_window 属性
-----------------------
> <p id="combo_box_t_open_window"> 为点击按钮时，要打开窗口的名称。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
#### options 属性
-----------------------
> <p id="combo_box_t_options"> 设置可选项(冒号分隔值和文本，分号分隔选项，如:1:red;2:green;3:blue)。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
#### selected\_index 属性
-----------------------
> <p id="combo_box_t_selected_index"> 当前选中的选项。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
#### value 属性
-----------------------
> <p id="combo_box_t_value"> 值。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
