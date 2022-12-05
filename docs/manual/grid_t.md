## grid\_t
### 概述
![image](images/grid_t_0.png)

网格。
在xml中使用"grid"标签创建控件。如：

```xml
<!-- ui -->
<grid x="c" y="50" w="100" h="100"/>
```

可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：

```xml
<!-- style -->
<grid>
<style name="default" grid_color="gray" border_color="black" odd_bg_color="#f5f5f5" even_bg_color="#eeeeee">
<normal />
</style>
</grid>
```
----------------------------------
### 函数
<p id="grid_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#grid_t_grid_cast">grid\_cast</a> | 转换为grid对象(供脚本语言使用)。 |
| <a href="#grid_t_grid_create">grid\_create</a> | 创建grid对象 |
| <a href="#grid_t_grid_get_widget_vtable">grid\_get\_widget\_vtable</a> | 获取 grid 虚表。 |
| <a href="#grid_t_grid_set_columns_definition">grid\_set\_columns\_definition</a> | 设置 各列的参数。 |
| <a href="#grid_t_grid_set_rows">grid\_set\_rows</a> | 设置 行数。 |
| <a href="#grid_t_grid_set_show_grid">grid\_set\_show\_grid</a> | 设置 是否显示网格。 |
### 属性
<p id="grid_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#grid_t_columns_definition">columns\_definition</a> | char* | 各列的参数。 |
| <a href="#grid_t_rows">rows</a> | uint32\_t | 行数。 |
| <a href="#grid_t_show_grid">show\_grid</a> | bool\_t | 是否显示网格。 |
#### grid\_cast 函数
-----------------------

* 函数功能：

> <p id="grid_t_grid_cast">转换为grid对象(供脚本语言使用)。

* 函数原型：

```
widget_t* grid_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | grid对象。 |
| widget | widget\_t* | grid对象。 |
#### grid\_create 函数
-----------------------

* 函数功能：

> <p id="grid_t_grid_create">创建grid对象

* 函数原型：

```
widget_t* grid_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | grid对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
#### grid\_get\_widget\_vtable 函数
-----------------------

* 函数功能：

> <p id="grid_t_grid_get_widget_vtable">获取 grid 虚表。

* 函数原型：

```
const widget_vtable_t* grid_get_widget_vtable ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const widget\_vtable\_t* | 成功返回 grid 虚表。 |
#### grid\_set\_columns\_definition 函数
-----------------------

* 函数功能：

> <p id="grid_t_grid_set_columns_definition">设置 各列的参数。

* 函数原型：

```
ret_t grid_set_columns_definition (widget_t* widget, const char* columns_definition);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| columns\_definition | const char* | 各列的参数。 |
#### grid\_set\_rows 函数
-----------------------

* 函数功能：

> <p id="grid_t_grid_set_rows">设置 行数。

* 函数原型：

```
ret_t grid_set_rows (widget_t* widget, uint32_t rows);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| rows | uint32\_t | 行数。 |
#### grid\_set\_show\_grid 函数
-----------------------

* 函数功能：

> <p id="grid_t_grid_set_show_grid">设置 是否显示网格。

* 函数原型：

```
ret_t grid_set_show_grid (widget_t* widget, bool_t show_grid);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| show\_grid | bool\_t | 是否显示网格。 |
#### columns\_definition 属性
-----------------------
> <p id="grid_t_columns_definition">各列的参数。
各列的参数之间用英文的分号(;)分隔，每列参数的格式为：

col(w=?,left_margin=?,right_margin=?,top_maorgin=?,bottom_margin=?)

* w 为列的宽度(必须存在)。取值在(0-1]区间时，视为grid控件宽度的比例，否则为像素宽度。
(如果为负数，将计算结果加上控件的宽度)
* left_margin(可选，可缩写为l) 该列左边的边距。
* right_margin(可选，可缩写为r) 该列右边的边距。
* top_margin(可选，可缩写为t) 该列顶部的边距。
* bottom_margin(可选，可缩写为b) 该列底部的边距。
* margin(可选，可缩写为m) 同时指定上面4个边距。
* fill_available(可选，可缩写为f) 填充剩余宽度(只有一列可以指定)。

* 类型：char*

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
#### rows 属性
-----------------------
> <p id="grid_t_rows">行数。

* 类型：uint32\_t

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
#### show\_grid 属性
-----------------------
> <p id="grid_t_show_grid">是否显示网格。

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
