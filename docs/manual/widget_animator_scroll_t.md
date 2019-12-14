## widget\_animator\_scroll\_t
### 概述
滚动控件的动画。
本动画也可以用widget_animator_prop2实现，但滚动控件需要访问内部数据结构，出于可读性考虑保留独立实现。
----------------------------------
### 函数
<p id="widget_animator_scroll_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#widget_animator_scroll_t_widget_animator_scroll_create">widget\_animator\_scroll\_create</a> | 创建动画对象。 |
| <a href="#widget_animator_scroll_t_widget_animator_scroll_set_params">widget\_animator\_scroll\_set\_params</a> | 设置动画对象的参数。 |
#### widget\_animator\_scroll\_create 函数
-----------------------

* 函数功能：

> <p id="widget_animator_scroll_t_widget_animator_scroll_create">创建动画对象。

* 函数原型：

```
widget_animator_t* widget_animator_scroll_create (widget_t* widget, uint32_t duration, uint32_t delay, easing_type_t easing);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_animator\_t* | 成功返回动画对象，失败返回NULL。 |
| widget | widget\_t* | 控件对象。 |
| duration | uint32\_t | 动画持续时间。 |
| delay | uint32\_t | 动画执行时间。 |
| easing | easing\_type\_t | 插值函数类型。 |
#### widget\_animator\_scroll\_set\_params 函数
-----------------------

* 函数功能：

> <p id="widget_animator_scroll_t_widget_animator_scroll_set_params">设置动画对象的参数。

* 函数原型：

```
ret_t widget_animator_scroll_set_params (widget_animator_t* animator, xy_t x_from, xy_t y_from, xy_t x_to, xy_t y_to);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象本身。 |
| x\_from | xy\_t | x起点值。 |
| y\_from | xy\_t | y起点值。 |
| x\_to | xy\_t | x终点值。 |
| y\_to | xy\_t | y终点值。 |
