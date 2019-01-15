## window\_animator\_t
### 概述
 窗口动画。


----------------------------------
### 函数
<p id="window_animator_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#window_animator_t_window_animator_create_for_close">window\_animator\_create\_for\_close</a> | 为关闭窗口创建动画。 |
| <a href="#window_animator_t_window_animator_create_for_open">window\_animator\_create\_for\_open</a> | 为打开窗口创建动画。 |
| <a href="#window_animator_t_window_animator_destroy">window\_animator\_destroy</a> | 销毁窗口动画对象。 |
| <a href="#window_animator_t_window_animator_update">window\_animator\_update</a> | 更新动画。 |
#### window\_animator\_create\_for\_close 函数
-----------------------

* 函数功能：

> <p id="window_animator_t_window_animator_create_for_close"> 为关闭窗口创建动画。




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

> <p id="window_animator_t_window_animator_create_for_open"> 为打开窗口创建动画。




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
#### window\_animator\_destroy 函数
-----------------------

* 函数功能：

> <p id="window_animator_t_window_animator_destroy"> 销毁窗口动画对象。




* 函数原型：

```
ret_t window_animator_destroy (window_animator_t* wa);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wa | window\_animator\_t* | 窗口动画对象。 |
#### window\_animator\_update 函数
-----------------------

* 函数功能：

> <p id="window_animator_t_window_animator_update"> 更新动画。




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
