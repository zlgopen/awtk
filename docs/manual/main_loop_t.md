## main\_loop\_t
### 概述
主循环接口。
----------------------------------
### 函数
<p id="main_loop_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#main_loop_t_main_loop">main\_loop</a> | 获取当前main_loop对象 |
| <a href="#main_loop_t_main_loop_add_event_source">main\_loop\_add\_event\_source</a> | 添加event_source。 |
| <a href="#main_loop_t_main_loop_remove_event_source">main\_loop\_remove\_event\_source</a> | 删除event_source。 |
#### main\_loop 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop">获取当前main_loop对象

* 函数原型：

```
main_loop_t* main_loop ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | main\_loop\_t* | 返回main\_loop对象。 |
#### main\_loop\_add\_event\_source 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_add_event_source">添加event_source。

* 函数原型：

```
ret_t main_loop_add_event_source (main_loop_t* l, event_source_t* source);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
| source | event\_source\_t* | event\_source对象。 |
#### main\_loop\_remove\_event\_source 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_remove_event_source">删除event_source。

* 函数原型：

```
ret_t main_loop_remove_event_source (main_loop_t* l, event_source_t* source);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
| source | event\_source\_t* | event\_source对象。 |
