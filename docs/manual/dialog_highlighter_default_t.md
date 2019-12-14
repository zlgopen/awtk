## dialog\_highlighter\_default\_t
### 概述
缺省对话框高亮策略。

>本策略在背景上画一层半透明的蒙版来高亮(突出)对话框本身。
>对于性能不高的平台，建议将start\_alpha和end\_alpha设为相同。
----------------------------------
### 函数
<p id="dialog_highlighter_default_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#dialog_highlighter_default_t_dialog_highlighter_default_create">dialog\_highlighter\_default\_create</a> | 创建缺省的对话框高亮策略。 |
### 属性
<p id="dialog_highlighter_default_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#dialog_highlighter_default_t_end_alpha">end\_alpha</a> | uint8\_t | 结束alpha，打开对话框的动画结束(直到对话框被关闭)时的alpha值。 |
| <a href="#dialog_highlighter_default_t_start_alpha">start\_alpha</a> | uint8\_t | 起始alpha，打开对话框的动画开始时的alpha值。 |
#### dialog\_highlighter\_default\_create 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_default_t_dialog_highlighter_default_create">创建缺省的对话框高亮策略。

* 函数原型：

```
dialog_highlighter_t* dialog_highlighter_default_create (object_t* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | dialog\_highlighter\_t* | 返回对话框高亮策略对象。 |
| args | object\_t* | 参数。 |
#### end\_alpha 属性
-----------------------
> <p id="dialog_highlighter_default_t_end_alpha">结束alpha，打开对话框的动画结束(直到对话框被关闭)时的alpha值。

* 类型：uint8\_t

#### start\_alpha 属性
-----------------------
> <p id="dialog_highlighter_default_t_start_alpha">起始alpha，打开对话框的动画开始时的alpha值。

* 类型：uint8\_t

