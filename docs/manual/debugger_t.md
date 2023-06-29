## debugger\_t
### 概述
调试器接口。
----------------------------------
### 函数
<p id="debugger_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_t_debugger_attach">debugger\_attach</a> | 附近到指定的代码片段。 |
| <a href="#debugger_t_debugger_clear_break_points">debugger\_clear\_break\_points</a> | 清除全部断点。 |
| <a href="#debugger_t_debugger_continue">debugger\_continue</a> | 执行到下一个断点。 |
| <a href="#debugger_t_debugger_deinit">debugger\_deinit</a> | 释放资源。 |
| <a href="#debugger_t_debugger_dispatch_messages">debugger\_dispatch\_messages</a> | dispatch_messages(仅适用于客户端) |
| <a href="#debugger_t_debugger_get_break_points">debugger\_get\_break\_points</a> | 获取断点列表。 |
| <a href="#debugger_t_debugger_get_callstack">debugger\_get\_callstack</a> | 获取callstack。 |
| <a href="#debugger_t_debugger_get_code">debugger\_get\_code</a> | 获取代码。 |
| <a href="#debugger_t_debugger_get_debuggers">debugger\_get\_debuggers</a> | 获取调试器列表。 |
| <a href="#debugger_t_debugger_get_global">debugger\_get\_global</a> | 获取全局对象。 |
| <a href="#debugger_t_debugger_get_local">debugger\_get\_local</a> | 获取局部变量对象。 |
| <a href="#debugger_t_debugger_get_self">debugger\_get\_self</a> | 获取self对象。 |
| <a href="#debugger_t_debugger_get_state">debugger\_get\_state</a> | 获取调试状态。 |
| <a href="#debugger_t_debugger_get_threads">debugger\_get\_threads</a> | 获取线程(仅用于调试原生程序，脚本不支持)。 |
| <a href="#debugger_t_debugger_get_var">debugger\_get\_var</a> | 获取变量的详细信息。 |
| <a href="#debugger_t_debugger_is_paused">debugger\_is\_paused</a> | 查看当前是否处于暂停状态。 |
| <a href="#debugger_t_debugger_is_paused_or_running">debugger\_is\_paused\_or\_running</a> | 查看当前是否处于暂停运行状态。 |
| <a href="#debugger_t_debugger_is_running">debugger\_is\_running</a> | 查看当前是否处于运行状态。 |
| <a href="#debugger_t_debugger_launch">debugger\_launch</a> | 执行代码。 |
| <a href="#debugger_t_debugger_lock">debugger\_lock</a> | 锁定debugger对象。 |
| <a href="#debugger_t_debugger_match">debugger\_match</a> | 检查code_id是否与当前debugger匹配。 |
| <a href="#debugger_t_debugger_pause">debugger\_pause</a> | 暂停运行。 |
| <a href="#debugger_t_debugger_remove_break_point">debugger\_remove\_break\_point</a> | 清除断点。 |
| <a href="#debugger_t_debugger_remove_break_point_ex">debugger\_remove\_break\_point\_ex</a> | 清除断点。 |
| <a href="#debugger_t_debugger_restart">debugger\_restart</a> | 重新运行。 |
| <a href="#debugger_t_debugger_set_break_point">debugger\_set\_break\_point</a> | 设置断点。 |
| <a href="#debugger_t_debugger_set_break_point_ex">debugger\_set\_break\_point\_ex</a> | 设置断点。 |
| <a href="#debugger_t_debugger_set_current_frame">debugger\_set\_current\_frame</a> | 从callstack中选择当前的frame。 |
| <a href="#debugger_t_debugger_set_state">debugger\_set\_state</a> | 设置状态。 |
| <a href="#debugger_t_debugger_step_in">debugger\_step\_in</a> | 进入函数。 |
| <a href="#debugger_t_debugger_step_loop_over">debugger\_step\_loop\_over</a> | 执行下一条语句(跳过循环) |
| <a href="#debugger_t_debugger_step_out">debugger\_step\_out</a> | 执行到函数结束。 |
| <a href="#debugger_t_debugger_step_over">debugger\_step\_over</a> | 执行到下一行代码。（不进入函数） |
| <a href="#debugger_t_debugger_stop">debugger\_stop</a> | 终止程序运行。 |
| <a href="#debugger_t_debugger_unlock">debugger\_unlock</a> | 解锁debugger对象。 |
| <a href="#debugger_t_debugger_update_code">debugger\_update\_code</a> | 更新代码。 |
#### debugger\_attach 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_attach">附近到指定的代码片段。

* 函数原型：

```
ret_t debugger_attach (debugger_t* debugger, const char* lang, const char* code_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| lang | const char* | 代码的语言。 |
| code\_id | const char* | 代码的ID。 |
#### debugger\_clear\_break\_points 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_clear_break_points">清除全部断点。

* 函数原型：

```
ret_t debugger_clear_break_points (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_continue 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_continue">执行到下一个断点。
> 处于暂停状态才能执行本命令。

* 函数原型：

```
ret_t debugger_continue (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_deinit 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_deinit">释放资源。

* 函数原型：

```
ret_t debugger_deinit (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_dispatch\_messages 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_dispatch_messages">dispatch_messages(仅适用于客户端)

* 函数原型：

```
ret_t debugger_dispatch_messages (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_get\_break\_points 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_break_points">获取断点列表。

* 函数原型：

```
ret_t debugger_get_break_points (debugger_t* debugger, binary_data_t* break_points);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| break\_points | binary\_data\_t* | 断点列表(每行一个)。 |
#### debugger\_get\_callstack 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_callstack">获取callstack。

* 函数原型：

```
ret_t debugger_get_callstack (debugger_t* debugger, binary_data_t* callstack);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| callstack | binary\_data\_t* | callstack。 |
#### debugger\_get\_code 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_code">获取代码。

* 函数原型：

```
ret_t debugger_get_code (debugger_t* debugger, binary_data_t* code);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| code | binary\_data\_t* | 代码。 |
#### debugger\_get\_debuggers 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_debuggers">获取调试器列表。

* 函数原型：

```
ret_t debugger_get_debuggers (debugger_t* debugger, binary_data_t* debuggers);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| debuggers | binary\_data\_t* | 调试器列表(每行一个)。 |
#### debugger\_get\_global 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_global">获取全局对象。
> 处于暂停状态才能执行本命令。
返回数据结构请参考：https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Variables

* 函数原型：

```
tk_object_t* debugger_get_global (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回全局对象。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_get\_local 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_local">获取局部变量对象。
返回数据结构请参考：https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Variables
> 处于暂停状态才能执行本命令。

* 函数原型：

```
tk_object_t* debugger_get_local (debugger_t* debugger, uint32_t frame_index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回局部变量对象。 |
| debugger | debugger\_t* | debugger对象。 |
| frame\_index | uint32\_t | frame序数(0开始)。 |
#### debugger\_get\_self 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_self">获取self对象。
返回数据结构请参考：https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Variables
> 处于暂停状态才能执行本命令。

* 函数原型：

```
tk_object_t* debugger_get_self (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回self对象。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_get\_state 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_state">获取调试状态。

* 函数原型：

```
debugger_program_state_t debugger_get_state (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_program\_state\_t | 返回调试状态。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_get\_threads 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_threads">获取线程(仅用于调试原生程序，脚本不支持)。
返回数据结构请参考: https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Threads
> 处于暂停状态才能执行本命令。

* 函数原型：

```
tk_object_t* debugger_get_threads (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回全局对象。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_get\_var 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_get_var">获取变量的详细信息。
返回数据结构请参考：https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Variables

> 处于暂停状态才能执行本命令。
> 可以为路径，如:
> * a.name 为结构a下的成员name。
> * a.names[1].first为结构a下的成员names数组中第二个元素下的first成员。

* 函数原型：

```
tk_object_t* debugger_get_var (debugger_t* debugger, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回全局对象。 |
| debugger | debugger\_t* | debugger对象。 |
| path | const char* | 变量名或路径。为空或NULL时，返回全部变量。 |
#### debugger\_is\_paused 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_is_paused">查看当前是否处于暂停状态。

* 函数原型：

```
bool_t debugger_is_paused (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示处于暂停运行状态。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_is\_paused\_or\_running 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_is_paused_or_running">查看当前是否处于暂停运行状态。

* 函数原型：

```
bool_t debugger_is_paused_or_running (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示处于暂停运行状态。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_is\_running 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_is_running">查看当前是否处于运行状态。

* 函数原型：

```
bool_t debugger_is_running (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示处于暂停运行状态。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_launch 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_launch">执行代码。

* 函数原型：

```
ret_t debugger_launch (debugger_t* debugger, const char* lang, const binary_data_t* code);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| lang | const char* | 代码的语言。 |
| code | const binary\_data\_t* | 代码。 |
#### debugger\_lock 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_lock">锁定debugger对象。

* 函数原型：

```
ret_t debugger_lock (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_match 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_match">检查code_id是否与当前debugger匹配。

* 函数原型：

```
bool_t debugger_match (debugger_t* debugger, const char* code_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示匹配到。 |
| debugger | debugger\_t* | debugger对象。 |
| code\_id | const char* | 代码ID。 |
#### debugger\_pause 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_pause">暂停运行。
> 暂停后才能执行next/step_xxx等函数。

* 函数原型：

```
ret_t debugger_pause (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_remove\_break\_point 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_remove_break_point">清除断点。

* 函数原型：

```
ret_t debugger_remove_break_point (debugger_t* debugger, uint32_t line);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| line | uint32\_t | 代码行号。 |
#### debugger\_remove\_break\_point\_ex 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_remove_break_point_ex">清除断点。

* 函数原型：

```
ret_t debugger_remove_break_point_ex (debugger_t* debugger, const char* position);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| position | const char* | 位置(函数名或文件名:行号)。 |
#### debugger\_restart 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_restart">重新运行。

* 函数原型：

```
ret_t debugger_restart (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_set\_break\_point 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_set_break_point">设置断点。

* 函数原型：

```
ret_t debugger_set_break_point (debugger_t* debugger, uint32_t line);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| line | uint32\_t | 代码行号。 |
#### debugger\_set\_break\_point\_ex 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_set_break_point_ex">设置断点。

* 函数原型：

```
ret_t debugger_set_break_point_ex (debugger_t* debugger, const char* position);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| position | const char* | 位置(函数名或文件名:行号)。 |
#### debugger\_set\_current\_frame 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_set_current_frame">从callstack中选择当前的frame。
> 处于暂停状态才能执行本命令。

* 函数原型：

```
ret_t debugger_set_current_frame (debugger_t* debugger, uint32_t frame_index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| frame\_index | uint32\_t | frame序数(0表示当前) |
#### debugger\_set\_state 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_set_state">设置状态。

* 函数原型：

```
ret_t debugger_set_state (debugger_t* debugger, debugger_program_state_t state);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| state | debugger\_program\_state\_t | 状态。 |
#### debugger\_step\_in 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_step_in">进入函数。
> 处于暂停状态才能执行本命令。

* 函数原型：

```
ret_t debugger_step_in (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_step\_loop\_over 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_step_loop_over">执行下一条语句(跳过循环)
> 处于暂停状态才能执行本命令。

* 函数原型：

```
ret_t debugger_step_loop_over (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_step\_out 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_step_out">执行到函数结束。
> 处于暂停状态才能执行本命令。

* 函数原型：

```
ret_t debugger_step_out (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_step\_over 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_step_over">执行到下一行代码。（不进入函数）
> 处于暂停状态才能执行本命令。

* 函数原型：

```
ret_t debugger_step_over (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_stop 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_stop">终止程序运行。

* 函数原型：

```
ret_t debugger_stop (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_unlock 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_unlock">解锁debugger对象。

* 函数原型：

```
ret_t debugger_unlock (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_update\_code 函数
-----------------------

* 函数功能：

> <p id="debugger_t_debugger_update_code">更新代码。

* 函数原型：

```
ret_t debugger_update_code (debugger_t* debugger, const binary_data_t* code);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| code | const binary\_data\_t* | 代码。 |
