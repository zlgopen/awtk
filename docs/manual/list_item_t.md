## list\_item\_t
### 概述
![image](images/list_item_t_0.png)

 list item控件。

----------------------------------
### 函数
<p id="list_item_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#list_item_t_list_item_cast">list\_item\_cast</a> | 转换为list_item对象(供脚本语言使用)。 |
| <a href="#list_item_t_list_item_create">list\_item\_create</a> | 创建list_item对象 |
#### list\_item\_cast 函数
-----------------------

* 函数功能：

> <p id="list_item_t_list_item_cast"> 转换为list_item对象(供脚本语言使用)。



* 函数原型：

```
widget_t* list_item_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | list\_item对象。 |
| widget | widget\_t* | list\_item对象。 |
#### list\_item\_create 函数
-----------------------

* 函数功能：

> <p id="list_item_t_list_item_create"> 创建list_item对象



* 函数原型：

```
widget_t* list_item_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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
