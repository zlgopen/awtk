## im\_commit\_event\_t
### 概述
![image](images/im_commit_event_t_0.png)

输入法提交输入的文本事件。
----------------------------------
### 函数
<p id="im_commit_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#im_commit_event_t_im_commit_event_init">im\_commit\_event\_init</a> | 初始化im_commit事件。 |
### 属性
<p id="im_commit_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#im_commit_event_t_replace;">replace;</a> | bool\_t | 是否替换原来的文本。 |
| <a href="#im_commit_event_t_text">text</a> | char* | 提交的文本。 |
#### im\_commit\_event\_init 函数
-----------------------

* 函数功能：

> <p id="im_commit_event_t_im_commit_event_init">初始化im_commit事件。

* 函数原型：

```
event_t* im_commit_event_init (im_commit_event_t* e, const char* text, bool_t replace);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | 返回事件对象。 |
| e | im\_commit\_event\_t* | 事件对象。 |
| text | const char* | 文本。 |
| replace | bool\_t | 是否替代当前的内容。 |
#### replace; 属性
-----------------------
> <p id="im_commit_event_t_replace;">是否替换原来的文本。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### text 属性
-----------------------
> <p id="im_commit_event_t_text">提交的文本。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
