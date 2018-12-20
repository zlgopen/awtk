## column\_t
### 概述
 column。一个简单的容器控件，垂直排列一组控件。
 它本身不提供布局功能(可用layout参数指定)，和view功能一致，仅提供具有语义的标签，让xml更具有可读性。
![image](images/column_t_0.png)

### 函数
<p id="column_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#column_t_column_cast">column\_cast</a> |  转换为column对象(供脚本语言使用)。 |
| <a href="#column_t_column_create">column\_create</a> |  创建column对象 |
### 属性
<p id="column_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="column_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### column\_cast 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | column对象。 |
| widget | widget\_t* | column对象。 |
<p id="column_t_column_cast"> 转换为column对象(供脚本语言使用)。



#### column\_create 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
<p id="column_t_column_create"> 创建column对象



