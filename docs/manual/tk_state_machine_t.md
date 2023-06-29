## tk\_state\_machine\_t
### 概述
状态机结构体
----------------------------------
### 函数
<p id="tk_state_machine_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_state_machine_t_tk_state_machine_current_get">tk\_state\_machine\_current\_get</a> | 获取当前状态 |
| <a href="#tk_state_machine_t_tk_state_machine_deinit">tk\_state\_machine\_deinit</a> | 反初始化 |
| <a href="#tk_state_machine_t_tk_state_machine_handle">tk\_state\_machine\_handle</a> | 事件处理 |
| <a href="#tk_state_machine_t_tk_state_machine_init">tk\_state\_machine\_init</a> | 初始化 |
| <a href="#tk_state_machine_t_tk_state_machine_previous_get">tk\_state\_machine\_previous\_get</a> | 获取上一个状态 |
#### tk\_state\_machine\_current\_get 函数
-----------------------

* 函数功能：

> <p id="tk_state_machine_t_tk_state_machine_current_get">获取当前状态

* 函数原型：

```
const tk_sm_state_t* tk_state_machine_current_get (tk_state_machine_t* fsm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const tk\_sm\_state\_t* | 返回 tk\_sm\_state\_t 对象 |
| fsm | tk\_state\_machine\_t* | 状态机地址 |
#### tk\_state\_machine\_deinit 函数
-----------------------

* 函数功能：

> <p id="tk_state_machine_t_tk_state_machine_deinit">反初始化

* 函数原型：

```
ret_t tk_state_machine_deinit (tk_state_machine_t* fsm, tk_sm_event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t 值 |
| fsm | tk\_state\_machine\_t* | 状态机地址 |
| event | tk\_sm\_event\_t* | 事件 |
#### tk\_state\_machine\_handle 函数
-----------------------

* 函数功能：

> <p id="tk_state_machine_t_tk_state_machine_handle">事件处理

* 函数原型：

```
ret_t tk_state_machine_handle (tk_state_machine_t* fsm, tk_sm_event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t 值 |
| fsm | tk\_state\_machine\_t* | 状态机地址 |
| event | tk\_sm\_event\_t* | 事件 |
#### tk\_state\_machine\_init 函数
-----------------------

* 函数功能：

> <p id="tk_state_machine_t_tk_state_machine_init">初始化

* 函数原型：

```
ret_t tk_state_machine_init (tk_state_machine_t* fsm, const tk_sm_state_t* initial_state);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t 值 |
| fsm | tk\_state\_machine\_t* | 状态机地址 |
| initial\_state | const tk\_sm\_state\_t* | 初始状态 |
#### tk\_state\_machine\_previous\_get 函数
-----------------------

* 函数功能：

> <p id="tk_state_machine_t_tk_state_machine_previous_get">获取上一个状态

* 函数原型：

```
const tk_sm_state_t* tk_state_machine_previous_get (tk_state_machine_t* fsm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const tk\_sm\_state\_t* | 返回 tk\_sm\_state\_t 对象 |
| fsm | tk\_state\_machine\_t* | 状态机地址 |
