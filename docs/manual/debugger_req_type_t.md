## debugger\_req\_type\_t
### 概述
请求的消息类型。
### 常量
<p id="debugger_req_type_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| DEBUGGER\_REQ\_ATTACH | attach请求码。 |
| DEBUGGER\_REQ\_STOP | 停止运行请求码。 |
| DEBUGGER\_REQ\_PAUSE | 暂停运行请求码。 |
| DEBUGGER\_REQ\_IS\_PAUSED | 查询是否处于暂停状态请求码。 |
| DEBUGGER\_REQ\_STEP\_IN | 运行进入函数请求码。 |
| DEBUGGER\_REQ\_STEP\_OUT | 运行退出函数请求码。 |
| DEBUGGER\_REQ\_STEP\_OVER | 运行下一步请求码。 |
| DEBUGGER\_REQ\_STEP\_LOOP\_OVER | 运行下一行请求码。（跳转循环） |
| DEBUGGER\_REQ\_CONTINUE | 运行到下一个断点请求码。 |
| DEBUGGER\_REQ\_SET\_BREAK\_POINT | 设置断点请求码。 |
| DEBUGGER\_REQ\_REMOVE\_BREAK\_POINT | 移除断点请求码。 |
| DEBUGGER\_REQ\_CLEAR\_BREAK\_POINTS | 清除断点请求码。 |
| DEBUGGER\_REQ\_GET\_SELF | 获取self对象请求码。 |
| DEBUGGER\_REQ\_GET\_LOCAL | 获取局部变量和函数参数对象请求码。 |
| DEBUGGER\_REQ\_GET\_GLOBAL | 获取global对象请求码。 |
| DEBUGGER\_REQ\_GET\_CODE | 获取源代码请求码。 |
| DEBUGGER\_REQ\_GET\_DEBUGGERS | 获取调试器列表。 |
| DEBUGGER\_REQ\_GET\_BREAK\_POINTS | 获取断点列表。 |
| DEBUGGER\_REQ\_GET\_CALLSTACK | 获取callstack请求码。 |
| DEBUGGER\_REQ\_UPDATE\_CODE | 更新源代码请求码。 |
| DEBUGGER\_REQ\_LAUNCH | 执行代码请求码。 |
| DEBUGGER\_REQ\_DEINIT | 断开调试器请求码。 |
| DEBUGGER\_REQ\_RESTART | 重新运行运行请求码。 |
