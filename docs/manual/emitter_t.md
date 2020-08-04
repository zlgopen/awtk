## emitter\_t
### 概述
事件分发器, 用于实现观察者模式。
----------------------------------
### 函数
<p id="emitter_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#emitter_t_emitter_cast">emitter\_cast</a> | 转换为emitter对象(供脚本语言使用)。 |
| <a href="#emitter_t_emitter_create">emitter\_create</a> | 创建emitter对象。 |
| <a href="#emitter_t_emitter_deinit">emitter\_deinit</a> | 析构。 |
| <a href="#emitter_t_emitter_destroy">emitter\_destroy</a> | 销毁。 |
| <a href="#emitter_t_emitter_disable">emitter\_disable</a> | 禁用。 |
| <a href="#emitter_t_emitter_dispatch">emitter\_dispatch</a> | 分发事件。如果当前分发的回调函数返回RET_REMOVE，该回调函数将被移出。 |
| <a href="#emitter_t_emitter_dispatch_simple_event">emitter\_dispatch\_simple\_event</a> | 分发事件。 |
| <a href="#emitter_t_emitter_enable">emitter\_enable</a> | 启用。 |
| <a href="#emitter_t_emitter_exist">emitter\_exist</a> | 判断指定的事件和回调函数是否已经注册。 |
| <a href="#emitter_t_emitter_find">emitter\_find</a> | 通过ID查找emitter_item_t，主要用于辅助测试。 |
| <a href="#emitter_t_emitter_forward">emitter\_forward</a> | 分发事件 |
| <a href="#emitter_t_emitter_init">emitter\_init</a> | 初始化emitter对象。 |
| <a href="#emitter_t_emitter_off">emitter\_off</a> | 注销指定事件的处理函数。 |
| <a href="#emitter_t_emitter_off_by_ctx">emitter\_off\_by\_ctx</a> | 注销指定事件的处理函数。 |
| <a href="#emitter_t_emitter_off_by_func">emitter\_off\_by\_func</a> | 注销指定事件的处理函数。 |
| <a href="#emitter_t_emitter_off_by_tag">emitter\_off\_by\_tag</a> | 注销指定事件的处理函数。 |
| <a href="#emitter_t_emitter_on">emitter\_on</a> | 注册指定事件的处理函数。 |
| <a href="#emitter_t_emitter_on_with_tag">emitter\_on\_with\_tag</a> | 注册指定事件的处理函数。 |
| <a href="#emitter_t_emitter_set_on_destroy">emitter\_set\_on\_destroy</a> | 设置一个回调函数，在emitter被销毁时调用(方便脚本语言去释放回调函数)。 |
| <a href="#emitter_t_emitter_size">emitter\_size</a> | 获取注册的回调函数个数，主要用于辅助测试。 |
### 属性
<p id="emitter_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#emitter_t_enable">enable</a> | bool\_t | 禁用标志。禁用时dispatch无效。 |
#### emitter\_cast 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_cast">转换为emitter对象(供脚本语言使用)。

主要给脚本语言使用。

* 函数原型：

```
emitter_t* emitter_cast (emitter_t* emitter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | emitter\_t* | 对象。 |
| emitter | emitter\_t* | emitter对象。 |
#### emitter\_create 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_create">创建emitter对象。

* 函数原型：

```
emitter_t* emitter_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | emitter\_t* | 对象。 |
#### emitter\_deinit 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_deinit">析构。

* 函数原型：

```
ret_t emitter_deinit (emitter_t* emitter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
#### emitter\_destroy 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_destroy">销毁。

* 函数原型：

```
ret_t emitter_destroy (emitter_t* emitter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
#### emitter\_disable 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_disable">禁用。

禁用后emitter_dispatch无效，但可以注册和注销。

* 函数原型：

```
ret_t emitter_disable (emitter_t* emitter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
#### emitter\_dispatch 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_dispatch">分发事件。如果当前分发的回调函数返回RET_REMOVE，该回调函数将被移出。
禁用状态下，本函数不做任何事情。

* 函数原型：

```
ret_t emitter_dispatch (emitter_t* emitter, event_t* e);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 如果当前分发的回调函数返回RET\_STOP，dispatch中断分发，并返回RET\_STOP，否则返回RET\_OK。 |
| emitter | emitter\_t* | emitter对象。 |
| e | event\_t* | 事件对象。 |
#### emitter\_dispatch\_simple\_event 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_dispatch_simple_event">分发事件。
> 对emitter_dispatch的包装，分发一个简单的事件。
如果当前分发的回调函数返回RET_STOP，dispatch中断分发，并返回RET_STOP，否则返回RET_OK。

* 函数原型：

```
ret_t emitter_dispatch_simple_event (emitter_t* emitter, event_type_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t |  |
| emitter | emitter\_t* | emitter对象。 |
| type | event\_type\_t | 事件类型。 |
#### emitter\_enable 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_enable">启用。

* 函数原型：

```
ret_t emitter_enable (emitter_t* emitter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
#### emitter\_exist 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_exist">判断指定的事件和回调函数是否已经注册。

* 函数原型：

```
bool_t emitter_exist (emitter_t* emitter, event_type_t type, event_func_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示已经注册，否则表示没有注册。 |
| emitter | emitter\_t* | emitter对象。 |
| type | event\_type\_t | 事件类型。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
#### emitter\_find 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_find">通过ID查找emitter_item_t，主要用于辅助测试。

* 函数原型：

```
ret_t emitter_find (emitter_t* emitter, uint32_t id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
| id | uint32\_t | emitter\_on返回的ID。 |
#### emitter\_forward 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_forward">分发事件

* 函数原型：

```
ret_t emitter_forward (void* ctx, event_t* e);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | void* | emitter对象。 |
| e | event\_t* | 分发的事件。 |
#### emitter\_init 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_init">初始化emitter对象。

* 函数原型：

```
emitter_t* emitter_init (emitter_t* emitter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | emitter\_t* | 对象。 |
| emitter | emitter\_t* | emitter对象。 |
#### emitter\_off 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_off">注销指定事件的处理函数。

* 函数原型：

```
ret_t emitter_off (emitter_t* emitter, uint32_t id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
| id | uint32\_t | emitter\_on返回的ID。 |
#### emitter\_off\_by\_ctx 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_off_by_ctx">注销指定事件的处理函数。

* 函数原型：

```
ret_t emitter_off_by_ctx (emitter_t* emitter, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
| ctx | void* | 事件处理函数上下文。 |
#### emitter\_off\_by\_func 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_off_by_func">注销指定事件的处理函数。

* 函数原型：

```
ret_t emitter_off_by_func (emitter_t* emitter, event_type_t type, event_func_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
| type | event\_type\_t | 事件类型。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
#### emitter\_off\_by\_tag 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_off_by_tag">注销指定事件的处理函数。

* 函数原型：

```
ret_t emitter_off_by_tag (emitter_t* emitter, uint32_t tag);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
| tag | uint32\_t | tag。 |
#### emitter\_on 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_on">注册指定事件的处理函数。

* 函数原型：

```
uint32_t emitter_on (emitter_t* emitter, event_type_t type, event_func_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回id，用于emitter\_off。 |
| emitter | emitter\_t* | emitter对象。 |
| type | event\_type\_t | 事件类型。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
#### emitter\_on\_with\_tag 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_on_with_tag">注册指定事件的处理函数。

* 函数原型：

```
uint32_t emitter_on_with_tag (emitter_t* emitter, event_type_t type, event_func_t on_event, void* ctx, uint32_t tag);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回id，用于emitter\_off。 |
| emitter | emitter\_t* | emitter对象。 |
| type | event\_type\_t | 事件类型。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
| tag | uint32\_t | tag。 |
#### emitter\_set\_on\_destroy 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_set_on_destroy">设置一个回调函数，在emitter被销毁时调用(方便脚本语言去释放回调函数)。

* 函数原型：

```
ret_t emitter_set_on_destroy (emitter_t* emitter, uint32_t id, tk_destroy_t on_destroy, void* on_destroy_ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| emitter | emitter\_t* | emitter对象。 |
| id | uint32\_t | emitter\_on返回的ID。 |
| on\_destroy | tk\_destroy\_t | 回调函数。 |
| on\_destroy\_ctx | void* | 回调函数上下文。 |
#### emitter\_size 函数
-----------------------

* 函数功能：

> <p id="emitter_t_emitter_size">获取注册的回调函数个数，主要用于辅助测试。

* 函数原型：

```
uint32_t emitter_size (emitter_t* emitter);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 回调函数个数。 |
| emitter | emitter\_t* | emitter对象。 |
#### enable 属性
-----------------------
> <p id="emitter_t_enable">禁用标志。禁用时dispatch无效。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
