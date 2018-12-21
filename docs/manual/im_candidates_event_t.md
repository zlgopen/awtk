## im\_candidates\_event\_t
### 概述
![image](images/im_candidates_event_t_0.png)

 输入法请求显示候选字的事件。

### 函数
<p id="im_candidates_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#im_candidates_event_t_input_method">input\_method</a> | 获取全局输入法对象。 |
| <a href="#im_candidates_event_t_input_method_commit_text">input\_method\_commit\_text</a> | 提交输入文本。 |
| <a href="#im_candidates_event_t_input_method_create">input\_method\_create</a> | 创建输入法对象。在具体实现中实现。 |
| <a href="#im_candidates_event_t_input_method_destroy">input\_method\_destroy</a> | 销毁输入法对象。在具体实现中实现。 |
| <a href="#im_candidates_event_t_input_method_dispatch">input\_method\_dispatch</a> | 分发一个事件。 |
| <a href="#im_candidates_event_t_input_method_dispatch_action">input\_method\_dispatch\_action</a> | 软键盘上的action按钮被点击时，调用本函数分发EVT_IM_ACTION事件。 |
| <a href="#im_candidates_event_t_input_method_dispatch_candidates">input\_method\_dispatch\_candidates</a> | 请求显示候选字。 |
| <a href="#im_candidates_event_t_input_method_dispatch_key">input\_method\_dispatch\_key</a> | 提交按键。 |
| <a href="#im_candidates_event_t_input_method_dispatch_to_widget">input\_method\_dispatch\_to\_widget</a> | 分发一个事件当前焦点控件。 |
| <a href="#im_candidates_event_t_input_method_off">input\_method\_off</a> | 注销指定事件的处理函数。 |
| <a href="#im_candidates_event_t_input_method_on">input\_method\_on</a> | 注册指定事件的处理函数。 |
| <a href="#im_candidates_event_t_input_method_request">input\_method\_request</a> | 打开或关闭输入法。 |
| <a href="#im_candidates_event_t_input_method_set">input\_method\_set</a> | 设置全局输入法对象。 |
| <a href="#im_candidates_event_t_input_method_update_action_button_info">input\_method\_update\_action\_button\_info</a> | 设置软键盘上的action按钮的信息。 |
### 属性
<p id="im_candidates_event_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#im_candidates_event_t_candidates">candidates</a> | char* | 可选的文本，多个文本以\0分隔。如：里\0李\0力\0离\0 |
| <a href="#im_candidates_event_t_candidates_nr">candidates\_nr</a> | uint32_t | 可选的文本的个数。 |
### 事件
<p id="im_candidates_event_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### input\_method 函数
* 函数原型：

```
input_method_t* input_method ();
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | input\_method\_t* | 成功返回输入法对象，失败返回NULL。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method"> 获取全局输入法对象。




#### input\_method\_commit\_text 函数
* 函数原型：

```
ret_t input_method_commit_text (input_method_t* im, char* text);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
| text | char* | 文本。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_commit_text"> 提交输入文本。




#### input\_method\_create 函数
* 函数原型：

```
input_method_t* input_method_create ();
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | input\_method\_t* | 成功返回输入法对象，失败返回NULL。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_create"> 创建输入法对象。在具体实现中实现。




#### input\_method\_destroy 函数
* 函数原型：

```
ret_t input_method_destroy (input_method_t* im);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_destroy"> 销毁输入法对象。在具体实现中实现。




#### input\_method\_dispatch 函数
* 函数原型：

```
ret_t input_method_dispatch (input_method_t* im, event_t* e);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 控件对象。 |
| e | event\_t* | 事件。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_dispatch"> 分发一个事件。




#### input\_method\_dispatch\_action 函数
* 函数原型：

```
ret_t input_method_dispatch_action (input_method_t* im);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_dispatch_action"> 软键盘上的action按钮被点击时，调用本函数分发EVT_IM_ACTION事件。




#### input\_method\_dispatch\_candidates 函数
* 函数原型：

```
ret_t input_method_dispatch_candidates (input_method_t* im, char* strs, uint32_t nr);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
| strs | char* | 候选字列表。 |
| nr | uint32\_t | 候选字个数。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_dispatch_candidates"> 请求显示候选字。




#### input\_method\_dispatch\_key 函数
* 函数原型：

```
ret_t input_method_dispatch_key (input_method_t* im, uint32_t key);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
| key | uint32\_t | 键值。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_dispatch_key"> 提交按键。




#### input\_method\_dispatch\_to\_widget 函数
* 函数原型：

```
ret_t input_method_dispatch_to_widget (input_method_t* im, event_t* e);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 控件对象。 |
| e | event\_t* | 事件。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_dispatch_to_widget"> 分发一个事件当前焦点控件。




#### input\_method\_off 函数
* 函数原型：

```
ret_t input_method_off (input_method_t* im, uint32_t id);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
| id | uint32\_t | input\_method\_on返回的ID。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_off"> 注销指定事件的处理函数。




#### input\_method\_on 函数
* 函数原型：

```
uint32_t input_method_on (input_method_t* im, event_type_t type, event_func_t on_event, void* ctx);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回id，用于input\_method\_off。 |
| im | input\_method\_t* | 输入法对象。 |
| type | event\_type\_t | 事件类型。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_on"> 注册指定事件的处理函数。




#### input\_method\_request 函数
* 函数原型：

```
ret_t input_method_request (input_method_t* im, widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
| widget | widget\_t* | 焦点控件，为NULL时关闭输入法，非NULL时打开输入法。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_request"> 打开或关闭输入法。




#### input\_method\_set 函数
* 函数原型：

```
ret_t input_method_set (input_method_t* im);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_set"> 设置全局输入法对象。




#### input\_method\_update\_action\_button\_info 函数
* 函数原型：

```
ret_t input_method_update_action_button_info (input_method_t* im, char* text, bool_t enable);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| im | input\_method\_t* | 输入法对象。 |
| text | char* | 按钮的文本。 |
| enable | bool\_t | 按钮的是否可用。 |
* 函数功能：

> <p id="im_candidates_event_t_input_method_update_action_button_info"> 设置软键盘上的action按钮的信息。




#### candidates 属性
-----------------------
> <p id="im_candidates_event_t_candidates"> 可选的文本，多个文本以\0分隔。如：里\0李\0力\0离\0



* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
#### candidates\_nr 属性
-----------------------
> <p id="im_candidates_event_t_candidates_nr"> 可选的文本的个数。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
