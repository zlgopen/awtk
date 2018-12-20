## rich\_text\_t
### 概述
 rich_text控件，实现简单的图文混排。

![image](images/rich_text_t_0.png)

### 函数
<p id="rich_text_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#rich_text_t_rich_text_create">rich\_text\_create</a> |  创建rich_text对象 |
| <a href="#rich_text_t_rich_text_set_text">rich\_text\_set\_text</a> |  设置文本。 |
### 属性
<p id="rich_text_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#rich_text_t_line_gap">line\_gap</a> | int32_t |  行间距。 |
### 事件
<p id="rich_text_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### rich\_text\_create 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
<p id="rich_text_t_rich_text_create"> 创建rich_text对象




#### rich\_text\_set\_text 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| text | char* | 文本。 |
<p id="rich_text_t_rich_text_set_text"> 设置文本。




#### line\_gap 属性
-----------------------
<p id="rich_text_t_line_gap"> 行间距。



* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
