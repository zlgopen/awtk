## timer\_manager\_t
### 概述
定时器管理器。
----------------------------------
### 函数
<p id="timer_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#timer_manager_t_timer_manager">timer\_manager</a> | 获取缺省的定时器管理器。 |
| <a href="#timer_manager_t_timer_manager_add">timer\_manager\_add</a> | 添加定时器。 |
| <a href="#timer_manager_t_timer_manager_append">timer\_manager\_append</a> | 追加定时器。 |
| <a href="#timer_manager_t_timer_manager_count">timer\_manager\_count</a> | 返回定时器的个数。 |
| <a href="#timer_manager_t_timer_manager_create">timer\_manager\_create</a> | 创建定时器管理器。 |
| <a href="#timer_manager_t_timer_manager_deinit">timer\_manager\_deinit</a> | 析构定时器管理器。 |
| <a href="#timer_manager_t_timer_manager_destroy">timer\_manager\_destroy</a> | 析构并释放定时器管理器。 |
| <a href="#timer_manager_t_timer_manager_find">timer\_manager\_find</a> | 查找指定ID的定时器。 |
| <a href="#timer_manager_t_timer_manager_init">timer\_manager\_init</a> | 初始化定时器管理器。 |
| <a href="#timer_manager_t_timer_manager_next_time">timer\_manager\_next\_time</a> | 返回最近的定时器到期时间。 |
| <a href="#timer_manager_t_timer_manager_remove">timer\_manager\_remove</a> | 根据id删除定时器。 |
| <a href="#timer_manager_t_timer_manager_reset">timer\_manager\_reset</a> | 重置定时器。 |
| <a href="#timer_manager_t_timer_manager_set">timer\_manager\_set</a> | 设置缺省的定时器管理器。 |
#### timer\_manager 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager">获取缺省的定时器管理器。

* 函数原型：

```
timer_manager_t* timer_manager ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | timer\_manager\_t* | 返回定时器管理器对象。 |
#### timer\_manager\_add 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_add">添加定时器。

* 函数原型：

```
uint32_t timer_manager_add (timer_manager_t* timer_manager, timer_func_t* on_timer, void* ctx, uint32_t duration);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回timer的ID，TK\_INVALID\_ID表示失败。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
| on\_timer | timer\_func\_t* | timer回调函数。 |
| ctx | void* | timer回调函数的上下文。 |
| duration | uint32\_t | 时间。 |
#### timer\_manager\_append 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_append">追加定时器。

* 函数原型：

```
ret_t timer_manager_append (timer_manager_t* timer_manager, timer_info_t* timer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。。 |
| timer | timer\_info\_t* | timer对象。 |
#### timer\_manager\_count 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_count">返回定时器的个数。

* 函数原型：

```
uint32_t timer_manager_count (timer_manager_t* timer_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回timer的个数。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
#### timer\_manager\_create 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_create">创建定时器管理器。

* 函数原型：

```
timer_manager_t* timer_manager_create (timer_get_time_t get_time);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | timer\_manager\_t* | 返回定时器管理器对象。 |
| get\_time | timer\_get\_time\_t | 获取当前时间的函数。 |
#### timer\_manager\_deinit 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_deinit">析构定时器管理器。

* 函数原型：

```
ret_t timer_manager_deinit (timer_manager_t* timer_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
#### timer\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_destroy">析构并释放定时器管理器。

* 函数原型：

```
ret_t timer_manager_destroy (timer_manager_t* timer_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
#### timer\_manager\_find 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_find">查找指定ID的定时器。

* 函数原型：

```
timer_info_t* timer_manager_find (timer_manager_t* timer_manager, uint32_t timer_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | timer\_info\_t* | 返回timer的信息。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
| timer\_id | uint32\_t | timer\_id |
#### timer\_manager\_init 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_init">初始化定时器管理器。

* 函数原型：

```
timer_manager_t* timer_manager_init (timer_manager_t* timer_manager, timer_get_time_t get_time);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | timer\_manager\_t* | 返回定时器管理器对象。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
| get\_time | timer\_get\_time\_t | 获取当前时间的函数。 |
#### timer\_manager\_next\_time 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_next_time">返回最近的定时器到期时间。

* 函数原型：

```
uint64_t timer_manager_next_time (timer_manager_t* timer_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint64\_t | 返回最近的timer到期时间。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
#### timer\_manager\_remove 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_remove">根据id删除定时器。

* 函数原型：

```
ret_t timer_manager_remove (timer_manager_t* timer_manager, uint32_t timer_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
| timer\_id | uint32\_t | timer\_id。 |
#### timer\_manager\_reset 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_reset">重置定时器。

* 函数原型：

```
ret_t timer_manager_reset (timer_manager_t* timer_manager, uint32_t* timer_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| timer\_manager | timer\_manager\_t* | 定时器管理器对象。 |
| timer\_id | uint32\_t* | timer\_id。 |
#### timer\_manager\_set 函数
-----------------------

* 函数功能：

> <p id="timer_manager_t_timer_manager_set">设置缺省的定时器管理器。

* 函数原型：

```
ret_t timer_manager_set (timer_manager_t* timer_manager_t);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| timer\_manager\_t | timer\_manager\_t* | 定时器管理器对象。 |
