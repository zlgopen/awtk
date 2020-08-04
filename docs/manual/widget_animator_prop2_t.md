## widget\_animator\_prop2\_t
### 概述
通过修改对象的两个指定属性形成动画效果。
----------------------------------
### 函数
<p id="widget_animator_prop2_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#widget_animator_prop2_t_widget_animator_prop2_create">widget\_animator\_prop2\_create</a> | 创建双属性动画对象。 |
| <a href="#widget_animator_prop2_t_widget_animator_prop2_set_params">widget\_animator\_prop2\_set\_params</a> | 设置动画对象的参数。 |
#### widget\_animator\_prop2\_create 函数
-----------------------

* 函数功能：

> <p id="widget_animator_prop2_t_widget_animator_prop2_create">创建双属性动画对象。

* 函数原型：

```
widget_animator_t* widget_animator_prop2_create (widget_t* widget, uint32_t duration, uint32_t delay, easing_type_t easing, const char* prop1_name, const char* prop2_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_animator\_t* | 成功返回动画对象，失败返回NULL。 |
| widget | widget\_t* | 控件对象。 |
| duration | uint32\_t | 动画持续时间。 |
| delay | uint32\_t | 动画执行时间。 |
| easing | easing\_type\_t | 插值函数类型。 |
| prop1\_name | const char* | 属性1的名称。 |
| prop2\_name | const char* | 属性2的名称。 |
#### widget\_animator\_prop2\_set\_params 函数
-----------------------

* 函数功能：

> <p id="widget_animator_prop2_t_widget_animator_prop2_set_params">设置动画对象的参数。

* 函数原型：

```
ret_t widget_animator_prop2_set_params (widget_animator_t* animator, double from1, double from2, double to1, double to2);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象本身。 |
| from1 | double | x的初值。 |
| from2 | double | y的初值。 |
| to1 | double | x的终值。 |
| to2 | double | y的终值。 |
