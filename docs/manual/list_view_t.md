## list\_view\_t
### 概述
![image](images/list_view_t_0.png)

 ListView控件。

----------------------------------
### 函数
<p id="list_view_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#list_view_t_list_view_cast">list\_view\_cast</a> | 转换为list_view对象(供脚本语言使用)。 |
| <a href="#list_view_t_list_view_create">list\_view\_create</a> | 创建list_view对象 |
| <a href="#list_view_t_list_view_set_auto_hide_scroll_bar">list\_view\_set\_auto\_hide\_scroll\_bar</a> | 设置是否自动隐藏滚动条。 |
| <a href="#list_view_t_list_view_set_default_item_height">list\_view\_set\_default\_item\_height</a> | 设置列表项的缺省高度。 |
| <a href="#list_view_t_list_view_set_item_height">list\_view\_set\_item\_height</a> | 设置列表项的高度。 |
### 属性
<p id="list_view_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#list_view_t_auto_hide_scroll_bar">auto\_hide\_scroll\_bar</a> | bool\_t | 如果不需要滚动条时，自动隐藏滚动条。 |
| <a href="#list_view_t_default_item_height">default\_item\_height</a> | int32\_t | 列表项的缺省高度。如果item_height <= 0 而且列表项自身的高度 <= 0，则使用缺省高度。 |
| <a href="#list_view_t_item_height">item\_height</a> | int32\_t | 列表项的高度。如果 item_height > 0，所有列表项使用固定高度，否则使用列表项自身的高度。 |
#### list\_view\_cast 函数
-----------------------

* 函数功能：

> <p id="list_view_t_list_view_cast"> 转换为list_view对象(供脚本语言使用)。



* 函数原型：

```
widget_t* list_view_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | list\_view对象。 |
| widget | widget\_t* | list\_view对象。 |
#### list\_view\_create 函数
-----------------------

* 函数功能：

> <p id="list_view_t_list_view_create"> 创建list_view对象



* 函数原型：

```
widget_t* list_view_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
#### list\_view\_set\_auto\_hide\_scroll\_bar 函数
-----------------------

* 函数功能：

> <p id="list_view_t_list_view_set_auto_hide_scroll_bar"> 设置是否自动隐藏滚动条。



* 函数原型：

```
ret_t list_view_set_auto_hide_scroll_bar (widget_t* widget, bool_t auto_hide_scroll_bar);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| auto\_hide\_scroll\_bar | bool\_t | 列表项的高度。 |
#### list\_view\_set\_default\_item\_height 函数
-----------------------

* 函数功能：

> <p id="list_view_t_list_view_set_default_item_height"> 设置列表项的缺省高度。



* 函数原型：

```
ret_t list_view_set_default_item_height (widget_t* widget, int32_t default_item_height);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| default\_item\_height | int32\_t | 列表项的高度。 |
#### list\_view\_set\_item\_height 函数
-----------------------

* 函数功能：

> <p id="list_view_t_list_view_set_item_height"> 设置列表项的高度。



* 函数原型：

```
ret_t list_view_set_item_height (widget_t* widget, int32_t item_height);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| item\_height | int32\_t | 列表项的高度。 |
#### auto\_hide\_scroll\_bar 属性
-----------------------
> <p id="list_view_t_auto_hide_scroll_bar"> 如果不需要滚动条时，自动隐藏滚动条。


* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### default\_item\_height 属性
-----------------------
> <p id="list_view_t_default_item_height"> 列表项的缺省高度。如果item_height <= 0 而且列表项自身的高度 <= 0，则使用缺省高度。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### item\_height 属性
-----------------------
> <p id="list_view_t_item_height"> 列表项的高度。如果 item_height > 0，所有列表项使用固定高度，否则使用列表项自身的高度。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
