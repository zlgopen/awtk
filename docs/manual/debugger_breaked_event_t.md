## debugger\_breaked\_event\_t
### 概述
![image](images/debugger_breaked_event_t_0.png)

调试器中断运行的事件。
----------------------------------
### 函数
<p id="debugger_breaked_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_breaked_event_t_debugger_breaked_event_cast">debugger\_breaked\_event\_cast</a> | 把event对象转debugger_breaked_event_t对象。 |
| <a href="#debugger_breaked_event_t_debugger_breaked_event_init">debugger\_breaked\_event\_init</a> | 初始调试器中断运行的事件。 |
### 属性
<p id="debugger_breaked_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#debugger_breaked_event_t_line">line</a> | uint32\_t | 中断运行的行号。 |
#### debugger\_breaked\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="debugger_breaked_event_t_debugger_breaked_event_cast">把event对象转debugger_breaked_event_t对象。

* 函数原型：

```
debugger_breaked_event_t* debugger_breaked_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_breaked\_event\_t* | 返回event对象。 |
| event | event\_t* | event对象。 |
#### debugger\_breaked\_event\_init 函数
-----------------------

* 函数功能：

> <p id="debugger_breaked_event_t_debugger_breaked_event_init">初始调试器中断运行的事件。

* 函数原型：

```
event_t* debugger_breaked_event_init (debugger_breaked_event_t* event, uint32_t line);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | 返回event对象。 |
| event | debugger\_breaked\_event\_t* | event对象。 |
| line | uint32\_t | 中断运行的行号。 |
#### line 属性
-----------------------
> <p id="debugger_breaked_event_t_line">中断运行的行号。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
