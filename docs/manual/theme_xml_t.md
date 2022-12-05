## theme\_xml\_t
### 概述
XML格式的窗体样式。
----------------------------------
### 函数
<p id="theme_xml_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#theme_xml_t_theme_xml_create">theme\_xml\_create</a> | 创建窗体样式对象。 |
| <a href="#theme_xml_t_theme_xml_gen">theme\_xml\_gen</a> | 生成二进制的数据。 |
#### theme\_xml\_create 函数
-----------------------

* 函数功能：

> <p id="theme_xml_t_theme_xml_create">创建窗体样式对象。

* 函数原型：

```
theme_t* theme_xml_create (const char* xml);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | theme\_t* | 返回窗体样式对象。 |
| xml | const char* | XML格式窗体样式数据。 |
#### theme\_xml\_gen 函数
-----------------------

* 函数功能：

> <p id="theme_xml_t_theme_xml_gen">生成二进制的数据。

* 函数原型：

```
uint8_t* theme_xml_gen (const char* xml, uint32_t* size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t* | 返回二进制的数据。 |
| xml | const char* | XML格式窗体样式数据。 |
| size | uint32\_t* | 用于返回数据长度。 |
