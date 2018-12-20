## keyboard\_t
### 概述
 键盘。
![image](images/keyboard_t_0.png)

### 函数
<p id="keyboard_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#keyboard_t_keyboard_close">keyboard\_close</a> |  关闭keyboard窗口。 |
| <a href="#keyboard_t_keyboard_create">keyboard\_create</a> |  创建keyboard对象 |
### 属性
<p id="keyboard_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="keyboard_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### keyboard\_close 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| parent | widget\_t* | keyboard对象。 |
<p id="keyboard_t_keyboard_close"> 关闭keyboard窗口。



#### keyboard\_create 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
<p id="keyboard_t_keyboard_create"> 创建keyboard对象



