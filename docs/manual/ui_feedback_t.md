## ui\_feedback\_t
### 概述
提供按键音、触屏音和震动等反馈。
----------------------------------
### 函数
<p id="ui_feedback_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#ui_feedback_t_ui_feedback_init">ui\_feedback\_init</a> | 初始化。 |
| <a href="#ui_feedback_t_ui_feedback_request">ui\_feedback\_request</a> | 请求反馈。 |
#### ui\_feedback\_init 函数
-----------------------

* 函数功能：

> <p id="ui_feedback_t_ui_feedback_init">初始化。
设置实际的处理函数。

* 函数原型：

```
ret_t ui_feedback_init (ui_on_feedback_t on_feedback, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| on\_feedback | ui\_on\_feedback\_t | 实际的处理函数。 |
| ctx | void* | 回调函数的上下文。 |
#### ui\_feedback\_request 函数
-----------------------

* 函数功能：

> <p id="ui_feedback_t_ui_feedback_request">请求反馈。

* 函数原型：

```
ret_t ui_feedback_request (widget_t* widget, event_t* evt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| evt | event\_t* | event对象。 |
