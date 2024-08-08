## svg\_path\_t
### 概述

----------------------------------
### 函数
<p id="svg_path_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#svg_path_t_svg_path_parse">svg\_path\_parse</a> | 解析路径。 |
#### svg\_path\_parse 函数
-----------------------

* 函数功能：

> <p id="svg_path_t_svg_path_parse">解析路径。

* 函数原型：

```
ret_t svg_path_parse (const char* path, void* ctx, tk_visit_t on_path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* | 路径数据。 |
| ctx | void* | 回调函数上下文。 |
| on\_path | tk\_visit\_t | 路径处理回调函数。 |
