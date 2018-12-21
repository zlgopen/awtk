## dialog\_t
### 概述
![image](images/dialog_t_0.png)

 对话框控件。
### 函数
<p id="dialog_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#dialog_t_dialog_cast">dialog\_cast</a> |  转换dialog对象(供脚本语言使用)。 |
| <a href="#dialog_t_dialog_create">dialog\_create</a> |  创建dialog对象。 |
| <a href="#dialog_t_dialog_create_simple">dialog\_create\_simple</a> |  创建dialog对象，同时创建title/client。 |
| <a href="#dialog_t_dialog_get_client">dialog\_get\_client</a> |  获取client控件。 |
| <a href="#dialog_t_dialog_get_title">dialog\_get\_title</a> |  获取title控件。 |
| <a href="#dialog_t_dialog_modal">dialog\_modal</a> |  模态显示对话框。 |
| <a href="#dialog_t_dialog_open">dialog\_open</a> |  从资源文件中加载并创建Dialog对象。本函数在ui_loader/ui_builder_default里实现。 |
| <a href="#dialog_t_dialog_quit">dialog\_quit</a> |  退出模态显示。 |
| <a href="#dialog_t_dialog_set_title">dialog\_set\_title</a> |  设置对话框的标题文本。 |
### 属性
<p id="dialog_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="dialog_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### dialog\_cast 函数
* 函数原型：

```
widget_t* dialog_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | dialog对象。 |
| widget | widget\_t* | dialog对象。 |
* 函数功能：

> <p id="dialog_t_dialog_cast"> 转换dialog对象(供脚本语言使用)。



#### dialog\_create 函数
* 函数原型：

```
widget_t* dialog_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | dialog对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
* 函数功能：

> <p id="dialog_t_dialog_create"> 创建dialog对象。



#### dialog\_create\_simple 函数
* 函数原型：

```
widget_t* dialog_create_simple (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | dialog对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
* 函数功能：

> <p id="dialog_t_dialog_create_simple"> 创建dialog对象，同时创建title/client。



#### dialog\_get\_client 函数
* 函数原型：

```
widget_t* dialog_get_client (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | client对象。 |
| widget | widget\_t* | dialog对象。 |
* 函数功能：

> <p id="dialog_t_dialog_get_client"> 获取client控件。



#### dialog\_get\_title 函数
* 函数原型：

```
widget_t* dialog_get_title (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | title对象。 |
| widget | widget\_t* | dialog对象。 |
* 函数功能：

> <p id="dialog_t_dialog_get_title"> 获取title控件。



#### dialog\_modal 函数
* 函数原型：

```
ret_t dialog_modal (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | dialog对象。 |
* 函数功能：

> <p id="dialog_t_dialog_modal"> 模态显示对话框。
 dialog_modal返回后，dialog对象将在下一个idle函数中回收，也就是在dialog_modal调用完成后仍然可以访问dialog中控件，直到本次事件结束。



#### dialog\_open 函数
* 函数原型：

```
widget_t* dialog_open (char* name);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| name | char* | dialog的名称。 |
* 函数功能：

> <p id="dialog_t_dialog_open"> 从资源文件中加载并创建Dialog对象。本函数在ui_loader/ui_builder_default里实现。



#### dialog\_quit 函数
* 函数原型：

```
ret_t dialog_quit (widget_t* widget, uint32_t code);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | dialog对象。 |
| code | uint32\_t | 退出码，作为dialog\_modal的返回值。 |
* 函数功能：

> <p id="dialog_t_dialog_quit"> 退出模态显示。



#### dialog\_set\_title 函数
* 函数原型：

```
ret_t dialog_set_title (widget_t* widget, char* title);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | dialog对象。 |
| title | char* | 标题。 |
* 函数功能：

> <p id="dialog_t_dialog_set_title"> 设置对话框的标题文本。



