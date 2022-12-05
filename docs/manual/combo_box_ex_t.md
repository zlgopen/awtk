## combo\_box\_ex\_t
### 概述
![image](images/combo_box_ex_t_0.png)

扩展combo_box控件。支持以下功能：
* 支持滚动。项目比较多时显示滚动条。
* 自动调整弹出窗口的宽度。根据最长文本自动调整弹出窗口的宽度。
* 支持分组显示。如果item的文本以"seperator."开头，视为一个分组开始，其后的文本为分组的标题。比如: "seperator.basic"，会创建一个basic为标题的分组。
----------------------------------
### 函数
<p id="combo_box_ex_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#combo_box_ex_t_combo_box_ex_create">combo\_box\_ex\_create</a> | 创建combo_box_ex对象 |
#### combo\_box\_ex\_create 函数
-----------------------

* 函数功能：

> <p id="combo_box_ex_t_combo_box_ex_create">创建combo_box_ex对象

* 函数原型：

```
widget_t* combo_box_ex_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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
