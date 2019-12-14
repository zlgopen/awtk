## widget\_animator\_manager\_t
### 概述
控件动画管理器。
----------------------------------
### 函数
<p id="widget_animator_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#widget_animator_manager_t_widget_animator_manager">widget\_animator\_manager</a> | 获取缺省的控件动画管理器。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_add">widget\_animator\_manager\_add</a> | 添加指定的动画，动画在创建时调用本函数。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_count">widget\_animator\_manager\_count</a> | 获取动画个数。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_create">widget\_animator\_manager\_create</a> | 创建控件动画管理器。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_deinit">widget\_animator\_manager\_deinit</a> | 析构控件动画管理器。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_destroy">widget\_animator\_manager\_destroy</a> | 析构并释放控件动画管理器。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_find">widget\_animator\_manager\_find</a> | 查找满足条件的动画。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_init">widget\_animator\_manager\_init</a> | 初始化控件动画管理器。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_pause">widget\_animator\_manager\_pause</a> | 暂停满足条件的动画。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_remove">widget\_animator\_manager\_remove</a> | 移出指定的动画，动画在销毁时调用本函数。。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_remove_all">widget\_animator\_manager\_remove\_all</a> | 销毁满足条件的动画。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_set">widget\_animator\_manager\_set</a> | 设置缺省的控件动画管理器。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_set_time_scale">widget\_animator\_manager\_set\_time\_scale</a> | 设置时间倍率。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_start">widget\_animator\_manager\_start</a> | 播放满足条件的动画。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_stop">widget\_animator\_manager\_stop</a> | 停止满足条件的动画。 |
| <a href="#widget_animator_manager_t_widget_animator_manager_time_elapse">widget\_animator\_manager\_time\_elapse</a> | 时间流失，调用本函数更新动画(public for test)。 |
### 属性
<p id="widget_animator_manager_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#widget_animator_manager_t_time_scale">time\_scale</a> | float\_t | 时间倍率，可以让时间变快或变慢。 |
#### widget\_animator\_manager 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager">获取缺省的控件动画管理器。

* 函数原型：

```
widget_animator_manager_t* widget_animator_manager ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_animator\_manager\_t* | 返回控件动画管理器。 |
#### widget\_animator\_manager\_add 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_add">添加指定的动画，动画在创建时调用本函数。

* 函数原型：

```
ret_t widget_animator_manager_add (widget_animator_manager_t* am, widget_animator_t* animator);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| animator | widget\_animator\_t* | 动画对象。 |
#### widget\_animator\_manager\_count 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_count">获取动画个数。

* 函数原型：

```
ret_t widget_animator_manager_count (widget_animator_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回动画个数。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
#### widget\_animator\_manager\_create 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_create">创建控件动画管理器。

* 函数原型：

```
widget_animator_manager_t* widget_animator_manager_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_animator\_manager\_t* | 返回控件动画管理器。 |
#### widget\_animator\_manager\_deinit 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_deinit">析构控件动画管理器。

* 函数原型：

```
ret_t widget_animator_manager_deinit (widget_animator_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
#### widget\_animator\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_destroy">析构并释放控件动画管理器。

* 函数原型：

```
ret_t widget_animator_manager_destroy (widget_animator_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
#### widget\_animator\_manager\_find 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_find">查找满足条件的动画。

* 函数原型：

```
widget_animator_t* widget_animator_manager_find (widget_animator_manager_t* am, widget_t* widget, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_animator\_t* | 成功返回动画对象，失败返回NULL。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| widget | widget\_t* | 控件对象。 |
| name | const char* | 动画名称。 |
#### widget\_animator\_manager\_init 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_init">初始化控件动画管理器。

* 函数原型：

```
widget_animator_manager_t* widget_animator_manager_init (widget_animator_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_animator\_manager\_t* | 返回控件动画管理器。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
#### widget\_animator\_manager\_pause 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_pause">暂停满足条件的动画。
1.widget为NULL时，暂停所有名称为name的动画。
2.name为NULL时，暂停所有widget相关的动画。
3.widget和name均为NULL，暂停所有动画。

* 函数原型：

```
ret_t widget_animator_manager_pause (widget_animator_manager_t* am, widget_t* widget, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| widget | widget\_t* | 控件对象。 |
| name | const char* | 动画名称。 |
#### widget\_animator\_manager\_remove 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_remove">移出指定的动画，动画在销毁时调用本函数。。

* 函数原型：

```
ret_t widget_animator_manager_remove (widget_animator_manager_t* am, widget_animator_t* animator);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| animator | widget\_animator\_t* | 动画对象。 |
#### widget\_animator\_manager\_remove\_all 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_remove_all">销毁满足条件的动画。

* 函数原型：

```
ret_t widget_animator_manager_remove_all (widget_animator_manager_t* am, widget_t* widget, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| widget | widget\_t* | 控件对象。 |
| name | const char* | 动画名称。 |
#### widget\_animator\_manager\_set 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_set">设置缺省的控件动画管理器。

* 函数原型：

```
ret_t widget_animator_manager_set (widget_animator_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
#### widget\_animator\_manager\_set\_time\_scale 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_set_time_scale">设置时间倍率。
设置动画的时间倍率，<0: 时间倒退，<1: 时间变慢，>1 时间变快。

* 函数原型：

```
ret_t widget_animator_manager_set_time_scale (widget_animator_manager_t* am, widget_t* widget, const char* name, float_t time_scale);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| widget | widget\_t* | 控件对象。 |
| name | const char* | 动画名称。 |
| time\_scale | float\_t | 时间倍率。 |
#### widget\_animator\_manager\_start 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_start">播放满足条件的动画。
1.widget为NULL时，播放所有名称为name的动画。
2.name为NULL时，播放所有widget相关的动画。
3.widget和name均为NULL，播放所有动画。

* 函数原型：

```
ret_t widget_animator_manager_start (widget_animator_manager_t* am, widget_t* widget, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| widget | widget\_t* | 控件对象。 |
| name | const char* | 动画名称。 |
#### widget\_animator\_manager\_stop 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_stop">停止满足条件的动画。
1.widget为NULL时，停止所有名称为name的动画。
2.name为NULL时，停止所有widget相关的动画。
3.widget和name均为NULL，停止所有动画。

* 函数原型：

```
ret_t widget_animator_manager_stop (widget_animator_manager_t* am, widget_t* widget, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| widget | widget\_t* | 控件对象。 |
| name | const char* | 动画名称。 |
#### widget\_animator\_manager\_time\_elapse 函数
-----------------------

* 函数功能：

> <p id="widget_animator_manager_t_widget_animator_manager_time_elapse">时间流失，调用本函数更新动画(public for test)。

* 函数原型：

```
ret_t widget_animator_manager_time_elapse (widget_animator_manager_t* am, uint32_t delta_time);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | widget\_animator\_manager\_t* | 控件动画管理器。 |
| delta\_time | uint32\_t | 流失的时间(毫秒)。 |
#### time\_scale 属性
-----------------------
> <p id="widget_animator_manager_t_time_scale">时间倍率，可以让时间变快或变慢。

* 类型：float\_t

