## qaction\_t
### 概述
代表一个action，放在action queue中。
----------------------------------
### 函数
<p id="qaction_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#qaction_t_qaction_create">qaction\_create</a> | 创建action对象。 |
| <a href="#qaction_t_qaction_destroy">qaction\_destroy</a> | 销毁。 |
| <a href="#qaction_t_qaction_exec">qaction\_exec</a> | 执行。 |
| <a href="#qaction_t_qaction_notify">qaction\_notify</a> | 事件通知。 |
| <a href="#qaction_t_qaction_set_on_event">qaction\_set\_on\_event</a> | 设置事件处理函数(回调函数在后台线程执行)。 |
### 属性
<p id="qaction_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#qaction_t_args">args</a> | uint32\_t* | exec的参数(视具体的action而不同)。 |
| <a href="#qaction_t_exec">exec</a> | qaction\_exec\_t | 执行函数。 |
| <a href="#qaction_t_on_event">on\_event</a> | qaction\_on\_event\_t | 事件处理函数。如进度、错误和完成等。 |
#### qaction\_create 函数
-----------------------

* 函数功能：

> <p id="qaction_t_qaction_create">创建action对象。

* 函数原型：

```
qaction_t* qaction_create (qaction_exec_t exec, void* args, uint32_t args_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | qaction\_t* | 返回action对象。 |
| exec | qaction\_exec\_t | 执行函数。 |
| args | void* | 参数。 |
| args\_size | uint32\_t | 参数长度。 |
#### qaction\_destroy 函数
-----------------------

* 函数功能：

> <p id="qaction_t_qaction_destroy">销毁。

* 函数原型：

```
ret_t qaction_destroy (qaction_t* q);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | qaction\_t* | qaction对象。 |
#### qaction\_exec 函数
-----------------------

* 函数功能：

> <p id="qaction_t_qaction_exec">执行。

* 函数原型：

```
ret_t qaction_exec (qaction_t* action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| action | qaction\_t* | action对象。 |
#### qaction\_notify 函数
-----------------------

* 函数功能：

> <p id="qaction_t_qaction_notify">事件通知。

* 函数原型：

```
ret_t qaction_notify (qaction_t* action, event_t** event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| action | qaction\_t* | action对象。 |
| event | event\_t** | event对象。 |
#### qaction\_set\_on\_event 函数
-----------------------

* 函数功能：

> <p id="qaction_t_qaction_set_on_event">设置事件处理函数(回调函数在后台线程执行)。

> exec执行完成后，会触发EVT\_DONE事件，一般在EVT\_DONE事件中调用qaction\_destroy函数销毁action。

* 函数原型：

```
ret_t qaction_set_on_event (qaction_t* action, qaction_on_event_t on_event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| action | qaction\_t* | action对象。 |
| on\_event | qaction\_on\_event\_t | 事件处理函数。 |
#### args 属性
-----------------------
> <p id="qaction_t_args">exec的参数(视具体的action而不同)。

* 类型：uint32\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### exec 属性
-----------------------
> <p id="qaction_t_exec">执行函数。

* 类型：qaction\_exec\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### on\_event 属性
-----------------------
> <p id="qaction_t_on_event">事件处理函数。如进度、错误和完成等。

* 类型：qaction\_on\_event\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
