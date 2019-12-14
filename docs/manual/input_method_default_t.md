## input\_method\_default\_t
### 概述
![image](images/input_method_default_t_0.png)

缺省输入法实现。输入法会启用软键盘，但是开发者可以自定义输入法引擎。

目前可以选择不启用输入法引擎或启用拼音输入法引擎。

开发者也可以开发手写输入法或语音输入法，如果实现时遇到问题可以和我们交流。
----------------------------------
### 函数
<p id="input_method_default_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#input_method_default_t_input_method_default_create">input\_method\_default\_create</a> | 创建缺省输入法对象。 |
#### input\_method\_default\_create 函数
-----------------------

* 函数功能：

> <p id="input_method_default_t_input_method_default_create">创建缺省输入法对象。

* 函数原型：

```
input_method_t* input_method_default_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | input\_method\_t* | 返回输入法对象。 |
