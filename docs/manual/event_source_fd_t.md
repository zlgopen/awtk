## event\_source\_fd\_t
### 概述
![image](images/event_source_fd_t_0.png)

fd事件源。用于文件、管道和套接字。
----------------------------------
### 函数
<p id="event_source_fd_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#event_source_fd_t_event_source_fd_create">event\_source\_fd\_create</a> | 创建fd事件源。 |
#### event\_source\_fd\_create 函数
-----------------------

* 函数功能：

> <p id="event_source_fd_t_event_source_fd_create">创建fd事件源。

* 函数原型：

```
event_source_t* event_source_fd_create (int fd, event_source_on_event_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_source\_t* | 返回事件源对象。 |
| fd | int | 可以用select等待的文件描述符。 |
| on\_event | event\_source\_on\_event\_t | 用户的事件处理函数。 |
| ctx | void* | on\_event函数的上下文。 |
