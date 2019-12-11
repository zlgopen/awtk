## font\_loader\_t
### 概述
字体加载接口。
----------------------------------
### 函数
<p id="font_loader_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#font_loader_t_font_loader_load">font\_loader\_load</a> | 加载字体。 |
#### font\_loader\_load 函数
-----------------------

* 函数功能：

> <p id="font_loader_t_font_loader_load">加载字体。

* 函数原型：

```
font_t* font_loader_load (font_loader_t* loader, const char* name, uint8_t* buff, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_t* | 成功返回font，失败返回NULL。 |
| loader | font\_loader\_t* | loader对象。 |
| name | const char* | 字体名字。 |
| buff | uint8\_t* | 字体数据。 |
| size | uint32\_t | 字体数据长度。 |
