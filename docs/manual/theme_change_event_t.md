## theme\_change\_event\_t
### 概述
![image](images/theme_change_event_t_0.png)

主题变化事件。
----------------------------------
### 函数
<p id="theme_change_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#theme_change_event_t_theme_change_event_cast">theme\_change\_event\_cast</a> | 把event对象转theme_change_event_t对象。 |
| <a href="#theme_change_event_t_theme_change_event_init">theme\_change\_event\_init</a> | 初始theme change event。 |
### 属性
<p id="theme_change_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#theme_change_event_t_name">name</a> | const char* | 主题名称。 |
#### theme\_change\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="theme_change_event_t_theme_change_event_cast">把event对象转theme_change_event_t对象。

* 函数原型：

```
theme_change_event_t* theme_change_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_change\_event\_t* | 返回event对象。 |
| event | event\_t* | event对象。 |
#### theme\_change\_event\_init 函数
-----------------------

* 函数功能：

> <p id="theme_change_event_t_theme_change_event_init">初始theme change event。

* 函数原型：

```
event_t* theme_change_event_init (theme_change_event_t* event, uint32_t type, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | 返回event对象。 |
| event | theme\_change\_event\_t* | event对象。 |
| type | uint32\_t | 事件类型。 |
| name | const char* | 主题名。 |
#### name 属性
-----------------------
> <p id="theme_change_event_t_name">主题名称。

* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
