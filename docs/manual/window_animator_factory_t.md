## window\_animator\_factory\_t
### 概述
窗口动画工厂对象。

提供通过类型创建窗口动画的能力。
让用户可以自定义窗口动画，并获得内置窗口动画同等待遇。
----------------------------------
### 函数
<p id="window_animator_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#window_animator_factory_t_window_animator_create_for_close">window\_animator\_create\_for\_close</a> | 为关闭窗口创建动画。 |
| <a href="#window_animator_factory_t_window_animator_create_for_open">window\_animator\_create\_for\_open</a> | 为打开窗口创建动画。 |
| <a href="#window_animator_factory_t_window_animator_factory">window\_animator\_factory</a> | 获取缺省的窗口动画工厂对象。 |
| <a href="#window_animator_factory_t_window_animator_factory_create">window\_animator\_factory\_create</a> | 创建窗口动画工厂对象。 |
| <a href="#window_animator_factory_t_window_animator_factory_create_animator">window\_animator\_factory\_create\_animator</a> | 创建指定类型的窗口动画。 |
| <a href="#window_animator_factory_t_window_animator_factory_destroy">window\_animator\_factory\_destroy</a> | 析构并释放窗口动画工厂对象。 |
| <a href="#window_animator_factory_t_window_animator_factory_register">window\_animator\_factory\_register</a> | 注册窗口动画创建函数。 |
| <a href="#window_animator_factory_t_window_animator_factory_set">window\_animator\_factory\_set</a> | 设置缺省的窗口动画工厂对象。 |
#### window\_animator\_create\_for\_close 函数
-----------------------

* 函数功能：

> <p id="window_animator_factory_t_window_animator_create_for_close">为关闭窗口创建动画。

* 函数原型：

```
window_animator_t* window_animator_create_for_close (char* type, canvas_t* c, widget_t* prev_win, widget_t* curr_win);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_animator\_t* | 窗口动画对象。 |
| type | char* | 动画类型。 |
| c | canvas\_t* | canvas。 |
| prev\_win | widget\_t* | 前一窗口。 |
| curr\_win | widget\_t* | 当前窗口。 |
#### window\_animator\_create\_for\_open 函数
-----------------------

* 函数功能：

> <p id="window_animator_factory_t_window_animator_create_for_open">为打开窗口创建动画。

* 函数原型：

```
window_animator_t* window_animator_create_for_open (char* type, canvas_t* c, widget_t* prev_win, widget_t* curr_win);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_animator\_t* | 窗口动画对象。 |
| type | char* | 动画类型。 |
| c | canvas\_t* | canvas。 |
| prev\_win | widget\_t* | 前一窗口。 |
| curr\_win | widget\_t* | 当前窗口。 |
#### window\_animator\_factory 函数
-----------------------

* 函数功能：

> <p id="window_animator_factory_t_window_animator_factory">获取缺省的窗口动画工厂对象。

* 函数原型：

```
window_animator_factory_t* window_animator_factory ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_animator\_factory\_t* | 返回窗口动画工厂对象。 |
#### window\_animator\_factory\_create 函数
-----------------------

* 函数功能：

> <p id="window_animator_factory_t_window_animator_factory_create">创建窗口动画工厂对象。

* 函数原型：

```
window_animator_factory_t* window_animator_factory_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_animator\_factory\_t* | 返回窗口动画工厂对象。 |
#### window\_animator\_factory\_create\_animator 函数
-----------------------

* 函数功能：

> <p id="window_animator_factory_t_window_animator_factory_create_animator">创建指定类型的窗口动画。

* 函数原型：

```
window_animator_t* window_animator_factory_create_animator (window_animator_factory_t* factory, bool_t open, object_t* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_animator\_t* | 返回窗口动画对象。 |
| factory | window\_animator\_factory\_t* | 窗口动画工厂对象。 |
| open | bool\_t | TRUE表示打开窗口时的动画，FALSE表示关闭窗口时的动画。 |
| args | object\_t* | 参数。 |
#### window\_animator\_factory\_destroy 函数
-----------------------

* 函数功能：

> <p id="window_animator_factory_t_window_animator_factory_destroy">析构并释放窗口动画工厂对象。

* 函数原型：

```
ret_t window_animator_factory_destroy (window_animator_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | window\_animator\_factory\_t* | 窗口动画工厂对象。 |
#### window\_animator\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="window_animator_factory_t_window_animator_factory_register">注册窗口动画创建函数。

* 函数原型：

```
ret_t window_animator_factory_register (window_animator_factory_t* factory, const char* type, window_animator_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | window\_animator\_factory\_t* | 窗口动画工厂对象。 |
| type | const char* | 窗口动画类型。 |
| create | window\_animator\_create\_t | 创建函数。 |
#### window\_animator\_factory\_set 函数
-----------------------

* 函数功能：

> <p id="window_animator_factory_t_window_animator_factory_set">设置缺省的窗口动画工厂对象。

* 函数原型：

```
ret_t window_animator_factory_set (window_animator_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | window\_animator\_factory\_t* | 窗口动画工厂对象。 |
