## ui\_binary\_writer\_t
### 概述
![image](images/ui_binary_writer_t_0.png)

生成二进制格式的UI描述数据。
----------------------------------
### 函数
<p id="ui_binary_writer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#ui_binary_writer_t_ui_binary_writer_init">ui\_binary\_writer\_init</a> | 初始化ui\_binary\_writer对象。 |
#### ui\_binary\_writer\_init 函数
-----------------------

* 函数功能：

> <p id="ui_binary_writer_t_ui_binary_writer_init">初始化ui\_binary\_writer对象。

* 函数原型：

```
ui_builder_t* ui_binary_writer_init (ui_binary_writer_t* writer, wbuffer_t* wbuffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ui\_builder\_t* | 返回ui\\_builder对象。 |
| writer | ui\_binary\_writer\_t* | writer对象。 |
| wbuffer | wbuffer\_t* | 保存结果的buffer。 |
