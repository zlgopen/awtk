## input\_method\_sdl\_t
### 概述
![image](images/input_method_sdl_t_0.png)


 SDL输入法实现。

 SDL输入法使用平台自身的输入法引擎，一般在桌面Linux、MacOS、Windows、Android和iOS上使用。




----------------------------------
### 函数
<p id="input_method_sdl_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#input_method_sdl_t_children_layouter_register_builtins">children\_layouter\_register\_builtins</a> | 注册内置的子控件布局算法。 |
| <a href="#input_method_sdl_t_input_method_sdl_create">input\_method\_sdl\_create</a> |  |
#### children\_layouter\_register\_builtins 函数
-----------------------

* 函数功能：

> <p id="input_method_sdl_t_children_layouter_register_builtins"> 注册内置的子控件布局算法。




* 函数原型：

```
ret_t children_layouter_register_builtins ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### input\_method\_sdl\_create 函数
-----------------------

* 函数功能：

> <p id="input_method_sdl_t_input_method_sdl_create">
 创建SDL输入法对象。





* 函数原型：

```
input_method_t* input_method_sdl_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | input\_method\_t* | 返回输入法对象。 |
