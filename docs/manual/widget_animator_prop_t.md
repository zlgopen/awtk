## widget\_animator\_prop\_t
### 概述
通过修改对象的指定属性形成动画效果。
----------------------------------
### 函数
<p id="widget_animator_prop_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#widget_animator_prop_t_widget_animator_prop_create">widget\_animator\_prop\_create</a> | 创建单属性动画对象。 |
| <a href="#widget_animator_prop_t_widget_animator_prop_set_params">widget\_animator\_prop\_set\_params</a> | 设置动画对象的参数。 |
#### widget\_animator\_prop\_create 函数
-----------------------

* 函数功能：

> <p id="widget_animator_prop_t_widget_animator_prop_create">创建单属性动画对象。

* 函数原型：

```
widget_animator_t* widget_animator_prop_create (widget_t* widget, uint32_t duration, uint32_t delay, easing_type_t easing, const char* prop_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_animator\_t* | 成功返回动画对象，失败返回NULL。 |
| widget | widget\_t* | 控件对象。 |
| duration | uint32\_t | 动画持续时间。 |
| delay | uint32\_t | 动画执行时间。 |
| easing | easing\_type\_t | 插值函数类型。 |
| prop\_name | const char* | 属性的名称。 |
#### widget\_animator\_prop\_set\_params 函数
-----------------------

* 函数功能：

> <p id="widget_animator_prop_t_widget_animator_prop_set_params">设置动画对象的参数。

* 函数原型：

```
ret_t widget_animator_prop_set_params (widget_animator_t* animator, double from, double to);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象本身。 |
| from | double | prop起始值。 |
| to | double | prop结束值。 |
