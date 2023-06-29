## drop\_file\_event\_t
### 概述
![image](images/drop_file_event_t_0.png)

文件拖入事件。
----------------------------------
### 函数
<p id="drop_file_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#drop_file_event_t_drop_file_event_cast">drop\_file\_event\_cast</a> | 把event对象转drop_file_event_t对象。 |
| <a href="#drop_file_event_t_drop_file_event_init">drop\_file\_event\_init</a> | 初始化事件。 |
### 属性
<p id="drop_file_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#drop_file_event_t_filename">filename</a> | const char* | 文件名。 |
#### drop\_file\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="drop_file_event_t_drop_file_event_cast">把event对象转drop_file_event_t对象。

* 函数原型：

```
drop_file_event_t* drop_file_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | drop\_file\_event\_t* | event 对象。 |
| event | event\_t* | event对象。 |
#### drop\_file\_event\_init 函数
-----------------------

* 函数功能：

> <p id="drop_file_event_t_drop_file_event_init">初始化事件。

* 函数原型：

```
event_t* drop_file_event_init (drop_file_event_t* event, void* target, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | event对象。 |
| event | drop\_file\_event\_t* | event对象。 |
| target | void* | 事件目标。 |
| filename | const char* | 文件名。 |
#### filename 属性
-----------------------
> <p id="drop_file_event_t_filename">文件名。

* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
