## label\_t
### 概述
 文本控件。
![image](images/label_t_0.png)

### 函数
<p id="label_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#label_t_label_create">label\_create</a> |  创建label对象 |
| <a href="#label_t_label_set_length">label\_set\_length</a> |  设置最大可显示字符数。 |
### 属性
<p id="label_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#label_t_length">length</a> | int32_t |  显示字符的长度。主要用于动态改变显示字符的个数，来实现类似[拨号中...]的动画效果。 |
### 事件
<p id="label_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### label\_create 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
<p id="label_t_label_create"> 创建label对象



#### label\_set\_length 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| repeat | int32\_t | 释放EVT\_CLICK事件的时间间隔(毫秒)。 |
<p id="label_t_label_set_length"> 设置最大可显示字符数。



#### length 属性
-----------------------
<p id="label_t_length"> 显示字符的长度。主要用于动态改变显示字符的个数，来实现类似[拨号中...]的动画效果。


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
