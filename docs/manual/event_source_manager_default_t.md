## event\_source\_manager\_default\_t
### 概述
![image](images/event_source_manager_default_t_0.png)

缺省的事件源管理器。
----------------------------------
### 函数
<p id="event_source_manager_default_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#event_source_manager_default_t_event_source_manager_default_create">event\_source\_manager\_default\_create</a> | 创建事件源管理器。 |
#### event\_source\_manager\_default\_create 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_default_t_event_source_manager_default_create">创建事件源管理器。

* 函数原型：

```
ret_t event_source_manager_default_create (event_source_manager_t* manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
