## font\_loader\_stb\_t
### 概述
![image](images/font_loader_stb_t_0.png)

stb字体加载器。

stb主要用于加载矢量字体(如truetype)，它功能强大，体积小巧。
----------------------------------
### 函数
<p id="font_loader_stb_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#font_loader_stb_t_font_loader_stb">font\_loader\_stb</a> | 获取stb字体加载器对象。 |
| <a href="#font_loader_stb_t_font_stb_create">font\_stb\_create</a> | 创建stb字体对象。 |
#### font\_loader\_stb 函数
-----------------------

* 函数功能：

> <p id="font_loader_stb_t_font_loader_stb">获取stb字体加载器对象。

* 函数原型：

```
font_loader_t* font_loader_stb ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_loader\_t* | 返回字体加载器对象。 |
#### font\_stb\_create 函数
-----------------------

* 函数功能：

> <p id="font_loader_stb_t_font_stb_create">创建stb字体对象。

* 函数原型：

```
font_t* font_stb_create (const char* name, const uint8_t* buff , uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_t* | 返回字体对象。 |
| name | const char* | 字体名称。 |
|  | const uint8\_t* buff | 。 |
| size | uint32\_t | 字体数据长度。 |
