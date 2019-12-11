## idle\_manager\_t
### 概述
idle_manager_t管理器。
----------------------------------
### 函数
<p id="idle_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#idle_manager_t_idle_manager">idle\_manager</a> | 获取缺省的idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_add">idle\_manager\_add</a> | 添加idle。 |
| <a href="#idle_manager_t_idle_manager_append">idle\_manager\_append</a> | 追加idle。 |
| <a href="#idle_manager_t_idle_manager_count">idle\_manager\_count</a> | 返回idle的个数。 |
| <a href="#idle_manager_t_idle_manager_create">idle\_manager\_create</a> | 创建idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_deinit">idle\_manager\_deinit</a> | 析构idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_destroy">idle\_manager\_destroy</a> | 析构并释放idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_find">idle\_manager\_find</a> | 查找指定ID的idle。 |
| <a href="#idle_manager_t_idle_manager_init">idle\_manager\_init</a> | 初始化idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_remove">idle\_manager\_remove</a> | 根据idle_id删除idle。 |
| <a href="#idle_manager_t_idle_manager_remove_all">idle\_manager\_remove\_all</a> | 删除全部idle。 |
| <a href="#idle_manager_t_idle_manager_set">idle\_manager\_set</a> | 设置缺省的idle_manager_t管理器。 |
#### idle\_manager 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager">获取缺省的idle_manager_t管理器。

* 函数原型：

```
idle_manager_t* idle_manager ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | idle\_manager\_t* | 返回idle\_manager\_t管理器对象。 |
#### idle\_manager\_add 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_add">添加idle。

* 函数原型：

```
uint32_t idle_manager_add (idle_manager_t* idle_manager, idle_func_t* on_idle, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回idle的ID，TK\_INVALID\_ID表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| on\_idle | idle\_func\_t* | idle回调函数。 |
| ctx | void* | idle回调函数的上下文。 |
#### idle\_manager\_append 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_append">追加idle。

* 函数原型：

```
ret_t idle_manager_append (idle_manager_t* idle_manager, idle_info_t* idle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| idle | idle\_info\_t* | idle对象。 |
#### idle\_manager\_count 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_count">返回idle的个数。

* 函数原型：

```
uint32_t idle_manager_count (idle_manager_t* idle_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回timer的个数。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
#### idle\_manager\_create 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_create">创建idle_manager_t管理器。

* 函数原型：

```
idle_manager_t* idle_manager_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | idle\_manager\_t* | 返回idle\_manager\_t管理器对象。 |
#### idle\_manager\_deinit 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_deinit">析构idle_manager_t管理器。

* 函数原型：

```
ret_t idle_manager_deinit (idle_manager_t* idle_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
#### idle\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_destroy">析构并释放idle_manager_t管理器。

* 函数原型：

```
ret_t idle_manager_destroy (idle_manager_t* idle_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
#### idle\_manager\_find 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_find">查找指定ID的idle。

* 函数原型：

```
idle_info_t* idle_manager_find (idle_manager_t* idle_manager, uint32_t idle_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | idle\_info\_t* | 返回idle的信息。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| idle\_id | uint32\_t | idle\_id。 |
#### idle\_manager\_init 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_init">初始化idle_manager_t管理器。

* 函数原型：

```
idle_manager_t* idle_manager_init (idle_manager_t* idle_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | idle\_manager\_t* | 返回idle\_manager\_t管理器对象。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
#### idle\_manager\_remove 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_remove">根据idle_id删除idle。

* 函数原型：

```
ret_t idle_manager_remove (idle_manager_t* idle_manager, uint32_t idle_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| idle\_id | uint32\_t | idle\_id。 |
#### idle\_manager\_remove\_all 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_remove_all">删除全部idle。

* 函数原型：

```
ret_t idle_manager_remove_all (idle_manager_t* idle_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
#### idle\_manager\_set 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_set">设置缺省的idle_manager_t管理器。

* 函数原型：

```
ret_t idle_manager_set (idle_manager_t* idle_manager_t);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager\_t | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
