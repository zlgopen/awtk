## remote\_ui\_service\_t
### 概述
remote ui服务端。
----------------------------------
### 函数
<p id="remote_ui_service_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#remote_ui_service_t_remote_ui_service_create">remote\_ui\_service\_create</a> | 创建remote ui服务端。 |
| <a href="#remote_ui_service_t_remote_ui_service_hook_log">remote\_ui\_service\_hook\_log</a> | 是否hook log。 |
| <a href="#remote_ui_service_t_remote_ui_service_start_with_uart">remote\_ui\_service\_start\_with\_uart</a> | 启动基于串口的Remote UI服务。 |
#### remote\_ui\_service\_create 函数
-----------------------

* 函数功能：

> <p id="remote_ui_service_t_remote_ui_service_create">创建remote ui服务端。

* 函数原型：

```
tk_service_t* remote_ui_service_create (tk_iostream_t* io, void* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_service\_t* | 返回service对象。 |
| io | tk\_iostream\_t* | IO流(由service释放)。 |
| args | void* | 参数。 |
#### remote\_ui\_service\_hook\_log 函数
-----------------------

* 函数功能：

> <p id="remote_ui_service_t_remote_ui_service_hook_log">是否hook log。

* 函数原型：

```
ret_t remote_ui_service_hook_log (remote_ui_service_t* ui, bool_t hook);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ui | remote\_ui\_service\_t* | remote ui服务端。 |
| hook | bool\_t | 是否hook log。 |
#### remote\_ui\_service\_start\_with\_uart 函数
-----------------------

* 函数功能：

> <p id="remote_ui_service_t_remote_ui_service_start_with_uart">启动基于串口的Remote UI服务。
> 在很多RTOS上，没有select，所以通过timer定时检查串口是否有数据。

* 函数原型：

```
tk_service_t* remote_ui_service_start_with_uart (tk_iostream_t* io, void* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_service\_t* | 返回service对象。 |
| io | tk\_iostream\_t* | IO流(由service释放)。 |
| args | void* | 参数。 |
