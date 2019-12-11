## progress\_event\_t
### 概述
![image](images/progress_event_t_0.png)

进度变化事件。
----------------------------------
### 函数
<p id="progress_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#progress_event_t_progress_event_cast">progress\_event\_cast</a> | 把event对象转progress_event_t对象，主要给脚本语言使用。 |
| <a href="#progress_event_t_progress_event_init">progress\_event\_init</a> | 初始progress event。 |
### 属性
<p id="progress_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#progress_event_t_percent">percent</a> | uint32\_t | 进度百分比。 |
#### progress\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="progress_event_t_progress_event_cast">把event对象转progress_event_t对象，主要给脚本语言使用。

* 函数原型：

```
progress_event_t* progress_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | progress\_event\_t* | 返回event对象。 |
| event | event\_t* | event对象。 |
#### progress\_event\_init 函数
-----------------------

* 函数功能：

> <p id="progress_event_t_progress_event_init">初始progress event。

* 函数原型：

```
event_t* progress_event_init (progress_event_t* event, uint32_t percent);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | 返回event对象。 |
| event | progress\_event\_t* | event对象。 |
| percent | uint32\_t | 进度。 |
#### percent 属性
-----------------------
> <p id="progress_event_t_percent">进度百分比。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
