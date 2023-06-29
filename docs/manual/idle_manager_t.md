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
| <a href="#idle_manager_t_idle_manager_add_with_id">idle\_manager\_add\_with\_id</a> | 添加 idle（可以指定 idle_id ，如果发现 idle_id 冲突则添加失败）。 |
| <a href="#idle_manager_t_idle_manager_add_with_type">idle\_manager\_add\_with\_type</a> | 添加对应类型的idle。 |
| <a href="#idle_manager_t_idle_manager_add_with_type_and_id">idle\_manager\_add\_with\_type\_and\_id</a> | 添加对应类型和id的idle。 |
| <a href="#idle_manager_t_idle_manager_append">idle\_manager\_append</a> | 追加idle。 |
| <a href="#idle_manager_t_idle_manager_count">idle\_manager\_count</a> | 返回idle的个数。 |
| <a href="#idle_manager_t_idle_manager_create">idle\_manager\_create</a> | 创建idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_deinit">idle\_manager\_deinit</a> | 析构idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_destroy">idle\_manager\_destroy</a> | 析构并释放idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_dispatch">idle\_manager\_dispatch</a> | 检查全部idle的函数，如果时间到期，调用相应的idle函数。 |
| <a href="#idle_manager_t_idle_manager_exist">idle\_manager\_exist</a> | 对应回调函数和上下文的idle是否存在。 |
| <a href="#idle_manager_t_idle_manager_find">idle\_manager\_find</a> | 查找指定ID的idle。 |
| <a href="#idle_manager_t_idle_manager_get_next_idle_id">idle\_manager\_get\_next\_idle\_id</a> | 获取下一个可用的 idle_id。 |
| <a href="#idle_manager_t_idle_manager_init">idle\_manager\_init</a> | 初始化idle_manager_t管理器。 |
| <a href="#idle_manager_t_idle_manager_remove">idle\_manager\_remove</a> | 根据idle_id删除idle。 |
| <a href="#idle_manager_t_idle_manager_remove_all">idle\_manager\_remove\_all</a> | 删除全部idle。 |
| <a href="#idle_manager_t_idle_manager_remove_all_by_ctx">idle\_manager\_remove\_all\_by\_ctx</a> | 根据上下文删除所有符合条件的idle。 |
| <a href="#idle_manager_t_idle_manager_remove_all_by_ctx_and_type">idle\_manager\_remove\_all\_by\_ctx\_and\_type</a> | 移除对应类型和上下文的所有idle。 |
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
uint32_t idle_manager_add (idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回idle的ID，TK\_INVALID\_ID表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| on\_idle | idle\_func\_t | idle回调函数。 |
| ctx | void* | idle回调函数的上下文。 |
#### idle\_manager\_add\_with\_id 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_add_with_id">添加 idle（可以指定 idle_id ，如果发现 idle_id 冲突则添加失败）。

* 函数原型：

```
uint32_t idle_manager_add_with_id (idle_manager_t* idle_manager, uint32_t id, idle_func_t on_idle, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回idle的ID，TK\_INVALID\_ID表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| id | uint32\_t | idle\_id。 |
| on\_idle | idle\_func\_t | idle回调函数。 |
| ctx | void* | idle回调函数的上下文。 |
#### idle\_manager\_add\_with\_type 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_add_with_type">添加对应类型的idle。

* 函数原型：

```
uint32_t idle_manager_add_with_type (idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx, uint16_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回idle\_id。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| on\_idle | idle\_func\_t | idle回调函数。 |
| ctx | void* | 上下文。 |
| type | uint16\_t | 类型。 |
#### idle\_manager\_add\_with\_type\_and\_id 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_add_with_type_and_id">添加对应类型和id的idle。

* 函数原型：

```
uint32_t idle_manager_add_with_type_and_id (idle_manager_t* idle_manager, uint32_t id, idle_func_t on_idle, void* ctx, uint16_t type, bool_t check_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回idle\_id。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| id | uint32\_t | id。 |
| on\_idle | idle\_func\_t | idle回调函数。 |
| ctx | void* | 上下文。 |
| type | uint16\_t | 类型。 |
| check\_id | bool\_t | 是否校验id。 |
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
#### idle\_manager\_dispatch 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_dispatch">检查全部idle的函数，如果时间到期，调用相应的idle函数。

* 函数原型：

```
ret_t idle_manager_dispatch (idle_manager_t* idle_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
#### idle\_manager\_exist 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_exist">对应回调函数和上下文的idle是否存在。

* 函数原型：

```
bool_t idle_manager_exist (idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| on\_idle | idle\_func\_t | idle回调函数。 |
| ctx | void* | 上下文。 |
#### idle\_manager\_find 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_find">查找指定ID的idle。

* 函数原型：

```
const idle_info_t* idle_manager_find (idle_manager_t* idle_manager, uint32_t idle_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const idle\_info\_t* | 返回idle的信息。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| idle\_id | uint32\_t | idle\_id。 |
#### idle\_manager\_get\_next\_idle\_id 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_get_next_idle_id">获取下一个可用的 idle_id。

* 函数原型：

```
uint32_t idle_manager_get_next_idle_id (idle_manager_t* idle_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回idle的ID，TK\_INVALID\_ID表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
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
#### idle\_manager\_remove\_all\_by\_ctx 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_remove_all_by_ctx">根据上下文删除所有符合条件的idle。

* 函数原型：

```
ret_t idle_manager_remove_all_by_ctx (idle_manager_t* idle_manager, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| ctx | void* | idle回调函数的上下文。 |
#### idle\_manager\_remove\_all\_by\_ctx\_and\_type 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_remove_all_by_ctx_and_type">移除对应类型和上下文的所有idle。

* 函数原型：

```
ret_t idle_manager_remove_all_by_ctx_and_type (idle_manager_t* idle_manager, uint16_t type, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
| type | uint16\_t | 类型。 |
| ctx | void* | 上下文。 |
#### idle\_manager\_set 函数
-----------------------

* 函数功能：

> <p id="idle_manager_t_idle_manager_set">设置缺省的idle_manager_t管理器。

* 函数原型：

```
ret_t idle_manager_set (idle_manager_t* idle_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_manager | idle\_manager\_t* | idle\_manager\_t管理器对象。 |
