## tk\_ostream\_shdlc\_t
### 概述
![image](images/tk_ostream_shdlc_t_0.png)

reliable ostream base on simple HDLC
----------------------------------
### 函数
<p id="tk_ostream_shdlc_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_ostream_shdlc_t_action_queue_create">action\_queue\_create</a> | 创建action_queue对象。 |
| <a href="#tk_ostream_shdlc_t_action_queue_destroy">action\_queue\_destroy</a> | 销毁。 |
| <a href="#tk_ostream_shdlc_t_action_queue_recv">action\_queue\_recv</a> | 接收一个请求。 |
| <a href="#tk_ostream_shdlc_t_action_queue_send">action\_queue\_send</a> | 发送一个请求。 |
| <a href="#tk_ostream_shdlc_t_tk_ostream_shdlc_create">tk\_ostream\_shdlc\_create</a> | 创建ostream对象。 |
### 属性
<p id="tk_ostream_shdlc_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#tk_ostream_shdlc_t_compress_threshold">compress\_threshold</a> | uint32\_t | 激活压缩的阈值。 |
| <a href="#tk_ostream_shdlc_t_retry_times">retry\_times</a> | uint32\_t | 失败重传次数。 |
| <a href="#tk_ostream_shdlc_t_timeout">timeout</a> | uint32\_t | 读写超时时间(ms) |
#### action\_queue\_create 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_shdlc_t_action_queue_create">创建action_queue对象。

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

> <p id="tk_ostream_shdlc_t_action_queue_destroy">销毁。

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

> <p id="tk_ostream_shdlc_t_action_queue_recv">接收一个请求。

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

> <p id="tk_ostream_shdlc_t_action_queue_send">发送一个请求。

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
#### tk\_ostream\_shdlc\_create 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_shdlc_t_tk_ostream_shdlc_create">创建ostream对象。

> 只能由iostream_shdlc调用。

* 函数原型：

```
tk_ostream_t* tk_ostream_shdlc_create (tk_iostream_shdlc_t* iostream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_ostream\_t* | 返回ostream对象。 |
| iostream | tk\_iostream\_shdlc\_t* | iostream对象。 |
#### compress\_threshold 属性
-----------------------
> <p id="tk_ostream_shdlc_t_compress_threshold">激活压缩的阈值。

* 类型：uint32\_t

#### retry\_times 属性
-----------------------
> <p id="tk_ostream_shdlc_t_retry_times">失败重传次数。

* 类型：uint32\_t

#### timeout 属性
-----------------------
> <p id="tk_ostream_shdlc_t_timeout">读写超时时间(ms)

* 类型：uint32\_t

