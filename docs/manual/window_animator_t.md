## window\_animator\_t
### 概述
窗口动画。
----------------------------------
### 函数
<p id="window_animator_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#window_animator_t_window_animator_begin_frame">window\_animator\_begin\_frame</a> | begin frame |
| <a href="#window_animator_t_window_animator_create">window\_animator\_create</a> | 创建窗口动画对象。 |
| <a href="#window_animator_t_window_animator_destroy">window\_animator\_destroy</a> | 销毁窗口动画对象。 |
| <a href="#window_animator_t_window_animator_end_frame">window\_animator\_end\_frame</a> | end frame |
| <a href="#window_animator_t_window_animator_update">window\_animator\_update</a> | 更新动画。 |
#### window\_animator\_begin\_frame 函数
-----------------------

* 函数功能：

> <p id="window_animator_t_window_animator_begin_frame">begin frame

* 函数原型：

```
ret_t window_animator_begin_frame (window_animator_t* wa);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wa | window\_animator\_t* | 窗口动画对象。 |
#### window\_animator\_create 函数
-----------------------

* 函数功能：

> <p id="window_animator_t_window_animator_create">创建窗口动画对象。

>供子类构造函数用。

* 函数原型：

```
window_animator_t* window_animator_create (bool_t open, const window_animator_vtable_t* vt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_animator\_t* | 返回窗口动画对象。 |
| open | bool\_t | TRUE表示打开窗口的动画，FALSE表示关闭窗口的动画。 |
| vt | const window\_animator\_vtable\_t* | 虚表对象。 |
#### window\_animator\_destroy 函数
-----------------------

* 函数功能：

> <p id="window_animator_t_window_animator_destroy">销毁窗口动画对象。

* 函数原型：

```
ret_t window_animator_destroy (window_animator_t* wa);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wa | window\_animator\_t* | 窗口动画对象。 |
#### window\_animator\_end\_frame 函数
-----------------------

* 函数功能：

> <p id="window_animator_t_window_animator_end_frame">end frame

* 函数原型：

```
ret_t window_animator_end_frame (window_animator_t* wa);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wa | window\_animator\_t* | 窗口动画对象。 |
#### window\_animator\_update 函数
-----------------------

* 函数功能：

> <p id="window_animator_t_window_animator_update">更新动画。

* 函数原型：

```
ret_t window_animator_update (window_animator_t* wa, canvas_t* canvas, uint32_t time_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wa | window\_animator\_t* | 窗口动画对象。 |
| canvas | canvas\_t* | 画布对象。 |
| time\_ms | uint32\_t | 当前时间(毫秒)。 |
