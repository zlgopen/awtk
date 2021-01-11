## action\_thread\_pool\_t
### 概述
action线程池。
----------------------------------
### 函数
<p id="action_thread_pool_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#action_thread_pool_t_action_thread_pool_create">action\_thread\_pool\_create</a> | 创建action_thread_pool对象。 |
| <a href="#action_thread_pool_t_action_thread_pool_create">action\_thread\_pool\_create</a> | 创建action_thread_pool对象。 |
| <a href="#action_thread_pool_t_action_thread_pool_destroy">action\_thread\_pool\_destroy</a> | 销毁。 |
| <a href="#action_thread_pool_t_action_thread_pool_exec">action\_thread\_pool\_exec</a> | 执行action。 |
### 属性
<p id="action_thread_pool_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#action_thread_pool_t_max_thread_nr">max\_thread\_nr</a> | uint32\_t | 最大线程数。 |
| <a href="#action_thread_pool_t_min_idle_nr">min\_idle\_nr</a> | uint32\_t | 最小空闲线程数 |
| <a href="#action_thread_pool_t_priority">priority</a> | tk\_thread\_priority\_t | 线程池中的线程优先级 |
| <a href="#action_thread_pool_t_stack_size">stack\_size</a> | uint32\_t | 线程池中的线程栈大小 |
#### action\_thread\_pool\_create 函数
-----------------------

* 函数功能：

> <p id="action_thread_pool_t_action_thread_pool_create">创建action_thread_pool对象。

* 函数原型：

```
action_thread_pool_t* action_thread_pool_create (uint16_t max_thread_nr, uint16_t min_idle_nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_thread\_pool\_t* | action\_thread\_pool对象。 |
| max\_thread\_nr | uint16\_t | 最大线程数。 |
| min\_idle\_nr | uint16\_t | 最小空闲线程数。 |
#### action\_thread\_pool\_create 函数
-----------------------

* 函数功能：

> <p id="action_thread_pool_t_action_thread_pool_create">创建action_thread_pool对象。

* 函数原型：

```
action_thread_pool_t* action_thread_pool_create (uint16_t max_thread_nr, uint16_t min_idle_nr, uint32_t stack_size, tk_thread_priority_t priority);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_thread\_pool\_t* | action\_thread\_pool对象。 |
| max\_thread\_nr | uint16\_t | 最大线程数。 |
| min\_idle\_nr | uint16\_t | 最小空闲线程数。 |
| stack\_size | uint32\_t | 栈的大小。 |
| priority | tk\_thread\_priority\_t | 优先级 |
#### action\_thread\_pool\_destroy 函数
-----------------------

* 函数功能：

> <p id="action_thread_pool_t_action_thread_pool_destroy">销毁。

* 函数原型：

```
ret_t action_thread_pool_destroy (action_thread_pool_t* thread_pool);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread\_pool | action\_thread\_pool\_t* | action\_thread\_pool对象。 |
#### action\_thread\_pool\_exec 函数
-----------------------

* 函数功能：

> <p id="action_thread_pool_t_action_thread_pool_exec">执行action。

* 函数原型：

```
ret_t action_thread_pool_exec (action_thread_pool_t* thread_pool, qaction_t* action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread\_pool | action\_thread\_pool\_t* | action\_thread\_pool对象。 |
| action | qaction\_t* | action对象。 |
#### max\_thread\_nr 属性
-----------------------
> <p id="action_thread_pool_t_max_thread_nr">最大线程数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### min\_idle\_nr 属性
-----------------------
> <p id="action_thread_pool_t_min_idle_nr">最小空闲线程数

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### priority 属性
-----------------------
> <p id="action_thread_pool_t_priority">线程池中的线程优先级

* 类型：tk\_thread\_priority\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### stack\_size 属性
-----------------------
> <p id="action_thread_pool_t_stack_size">线程池中的线程栈大小

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
