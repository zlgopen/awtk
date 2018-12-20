## app\_bar\_t
### 概述
 app_bar。一个简单的容器控件，一般在窗口的顶部。
 它本身不提供布局功能(可用layout参数指定)，和view功能一致，仅提供具有语义的标签，让xml更具有可读性。
![image](images/app_bar_t_0.png)

### 函数
<p id="app_bar_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#app_bar_t_app_bar_cast">app\_bar\_cast</a> |  转换为app_bar对象(供脚本语言使用)。 |
| <a href="#app_bar_t_app_bar_create">app\_bar\_create</a> |  创建app_bar对象 |
### 属性
<p id="app_bar_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="app_bar_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### app\_bar\_cast 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | app\_bar对象。 |
| widget | widget\_t* | app\_bar对象。 |
<p id="app_bar_t_app_bar_cast"> 转换为app_bar对象(供脚本语言使用)。



#### app\_bar\_create 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
<p id="app_bar_t_app_bar_create"> 创建app_bar对象



