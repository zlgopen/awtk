## svg\_helper\_t
### 概述

----------------------------------
### 函数
<p id="svg_helper_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#svg_helper_t_action_queue_create">action\_queue\_create</a> | 创建action_queue对象。 |
| <a href="#svg_helper_t_action_queue_destroy">action\_queue\_destroy</a> | 销毁。 |
| <a href="#svg_helper_t_action_queue_recv">action\_queue\_recv</a> | 接收一个请求。 |
| <a href="#svg_helper_t_action_queue_send">action\_queue\_send</a> | 发送一个请求。 |
| <a href="#svg_helper_t_bsvg_to_svg">bsvg\_to\_svg</a> | bsvg to svg |
| <a href="#svg_helper_t_svg_to_bsvg">svg\_to\_bsvg</a> | svg to bsvg |
#### action\_queue\_create 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_action_queue_create">创建action_queue对象。

* 函数原型：

```
action_queue_t* action_queue_create (uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_queue\_t* | action\_queue对象。 |
| capacity | uint32\_t | action的容量。 |
#### action\_queue\_destroy 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_action_queue_destroy">销毁。

* 函数原型：

```
ret_t action_queue_destroy (action_queue_t* q);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | action\_queue\_t* | action\_queue对象。 |
#### action\_queue\_recv 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_action_queue_recv">接收一个请求。

* 函数原型：

```
ret_t action_queue_recv (action_queue_t* q, qaction_t** action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | action\_queue\_t* | action\_queue对象。 |
| action | qaction\_t** | 用于返回action对象。 |
#### action\_queue\_send 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_action_queue_send">发送一个请求。

* 函数原型：

```
ret_t action_queue_send (action_queue_t* q, qaction_t* action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | action\_queue\_t* | action\_queue对象。 |
| action | qaction\_t* | action对象。 |
#### bsvg\_to\_svg 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_bsvg_to_svg">bsvg to svg

* 函数原型：

```
ret_t bsvg_to_svg (uint32_t* data, uint32_t size, str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| data | uint32\_t* | bsvg数据。 |
| size | uint32\_t | bsvg数据长度。 |
| str | str\_t* | 用于返回svg数据。 |
#### svg\_to\_bsvg 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_svg_to_bsvg">svg to bsvg

* 函数原型：

```
ret_t svg_to_bsvg (const char* xml, uint32_t size, uint32_t* out, uint32_t* out_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| xml | const char* | svg数据。 |
| size | uint32\_t | svg数据长度。 |
| out | uint32\_t* | 用于返回bsvg数据。 |
| out\_size | uint32\_t* | bsvg数据长度。 |
