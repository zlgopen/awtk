## ui\_load\_event\_t
### 概述
![image](images/ui_load_event_t_0.png)

UI加载完成事件。
----------------------------------
### 函数
<p id="ui_load_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#ui_load_event_t_ui_load_event_cast">ui\_load\_event\_cast</a> | 把event对象转ui_load_event_t对象。 |
| <a href="#ui_load_event_t_ui_load_event_init">ui\_load\_event\_init</a> | 初始化事件。 |
### 属性
<p id="ui_load_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#ui_load_event_t_name">name</a> | const char* | UI的名称。 |
| <a href="#ui_load_event_t_root">root</a> | widget\_t* | UI的根控件对象。 |
#### ui\_load\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="ui_load_event_t_ui_load_event_cast">把event对象转ui_load_event_t对象。

* 函数原型：

```
ui_load_event_t* ui_load_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ui\_load\_event\_t* | event 对象。 |
| event | event\_t* | event对象。 |
#### ui\_load\_event\_init 函数
-----------------------

* 函数功能：

> <p id="ui_load_event_t_ui_load_event_init">初始化事件。

* 函数原型：

```
event_t* ui_load_event_init (ui_load_event_t* event, void* target, widget_t* root, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | event对象。 |
| event | ui\_load\_event\_t* | event对象。 |
| target | void* | 事件目标。 |
| root | widget\_t* | UI的根控件对象。 |
| name | const char* | UI的名称。 |
#### name 属性
-----------------------
> <p id="ui_load_event_t_name">UI的名称。

* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### root 属性
-----------------------
> <p id="ui_load_event_t_root">UI的根控件对象。

* 类型：widget\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
