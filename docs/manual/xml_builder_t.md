## xml\_builder\_t
### 概述
xml builder interface
----------------------------------
### 函数
<p id="xml_builder_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#xml_builder_t_xml_builder_get_attr">xml\_builder\_get\_attr</a> | 获取属性。 |
#### xml\_builder\_get\_attr 函数
-----------------------

* 函数功能：

> <p id="xml_builder_t_xml_builder_get_attr">获取属性。

* 函数原型：

```
const char* xml_builder_get_attr (const char** attrs, const char* attr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回属性的值。 |
| attrs | const char** | 属性列表。 |
| attr | const char* | 属性名。 |
