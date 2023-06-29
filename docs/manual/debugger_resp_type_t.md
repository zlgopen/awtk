## debugger\_resp\_type\_t
### 概述
响应/推送的消息类型。
### 常量
<p id="debugger_resp_type_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| DEBUGGER\_RESP\_ATTACH | attach响应码。 |
| DEBUGGER\_RESP\_STOP | 停止运行响应码。 |
| DEBUGGER\_RESP\_IS\_PAUSED | 查询是否处于暂停状态响应码。 |
| DEBUGGER\_RESP\_PAUSE | 暂停运行响应码。 |
| DEBUGGER\_RESP\_STEP\_IN | 运行进入函数响应码。 |
| DEBUGGER\_RESP\_STEP\_OUT | 运行退出函数响应码。 |
| DEBUGGER\_RESP\_STEP\_OVER | 运行下一步响应码。 |
| DEBUGGER\_RESP\_STEP\_LOOP\_OVER | 运行下一行响应码。（跳出循环） |
| DEBUGGER\_RESP\_CONTINUE | 运行到下一个断点响应码。 |
| DEBUGGER\_RESP\_SET\_BREAK\_POINT | 设置断点响应码。 |
| DEBUGGER\_RESP\_REMOVE\_BREAK\_POINT | 移除断点响应码。 |
| DEBUGGER\_RESP\_CLEAR\_BREAK\_POINTS | 清除断点响应码。 |
| DEBUGGER\_RESP\_GET\_SELF | 获取self对象响应码。 |
| DEBUGGER\_RESP\_GET\_LOCAL | 获取局部变量和函数参数对象响应码。 |
| DEBUGGER\_RESP\_GET\_GLOBAL | 获取global对象响应码。 |
| DEBUGGER\_RESP\_GET\_CODE | 获取源代码响应码。 |
| DEBUGGER\_RESP\_GET\_DEBUGGERS | 获取调试器列表响应码。 |
| DEBUGGER\_RESP\_GET\_BREAK\_POINTS | 获取断点列表响应码。 |
| DEBUGGER\_RESP\_GET\_CALLSTACK | 获取callstack响应码。 |
| DEBUGGER\_RESP\_UPDATE\_CODE | 更新源代码响应码。 |
| DEBUGGER\_RESP\_LAUNCH | 执行代码响应码。 |
| DEBUGGER\_RESP\_DEINIT | 断开调试器响应码。 |
| DEBUGGER\_RESP\_MSG\_LOG | 程序打印日志的响应码/事件码。 |
| DEBUGGER\_RESP\_MSG\_ERROR | 程序出现错误的响应码/事件码。 |
| DEBUGGER\_RESP\_MSG\_BREAKED | 程序中断的响应码/事件码。 |
| DEBUGGER\_RESP\_MSG\_FRAME\_CHANGED | 用户切换调用堆栈的frame. |
| DEBUGGER\_RESP\_MSG\_COMPLETED | 程序执行完成的响应码/事件码。 |
