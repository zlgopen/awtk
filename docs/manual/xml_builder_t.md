## xml\_builder\_t
### 概述
xml builder interface
----------------------------------
### 函数
<p id="xml_builder_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#xml_builder_t_xml_builder_destroy">xml\_builder\_destroy</a> | 销毁builder对象。 |
| <a href="#xml_builder_t_xml_builder_get_attr">xml\_builder\_get\_attr</a> | 获取属性。 |
| <a href="#xml_builder_t_xml_builder_on_comment">xml\_builder\_on\_comment</a> | 解析到注释时调用本函数。 |
| <a href="#xml_builder_t_xml_builder_on_end">xml\_builder\_on\_end</a> | 解析到tag结束时调用本函数。 |
| <a href="#xml_builder_t_xml_builder_on_error">xml\_builder\_on\_error</a> | 解析遇到错误时调用本函数。 |
| <a href="#xml_builder_t_xml_builder_on_pi">xml\_builder\_on\_pi</a> | 解析到处理指令时调用本函数。 |
| <a href="#xml_builder_t_xml_builder_on_start">xml\_builder\_on\_start</a> | 解析到tag开始时调用本函数。 |
| <a href="#xml_builder_t_xml_builder_on_text">xml\_builder\_on\_text</a> | 解析到文本时调用本函数。 |
#### xml\_builder\_destroy 函数
-----------------------

* 函数功能：

> <p id="xml_builder_t_xml_builder_destroy">销毁builder对象。

* 函数原型：

```
void xml_builder_destroy (XmlBuilder* builder);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| builder | XmlBuilder* | builder对象。 |
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
#### xml\_builder\_on\_comment 函数
-----------------------

* 函数功能：

> <p id="xml_builder_t_xml_builder_on_comment">解析到注释时调用本函数。

* 函数原型：

```
void xml_builder_on_comment (XmlBuilder* builder, const char* text, size_t length);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| builder | XmlBuilder* | builder对象。 |
| text | const char* | 注释。 |
| length | size\_t | 注释长度。 |
#### xml\_builder\_on\_end 函数
-----------------------

* 函数功能：

> <p id="xml_builder_t_xml_builder_on_end">解析到tag结束时调用本函数。

* 函数原型：

```
void xml_builder_on_end (XmlBuilder* builder, const char* tag);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| builder | XmlBuilder* | builder对象。 |
| tag | const char* | tag名称。 |
#### xml\_builder\_on\_error 函数
-----------------------

* 函数功能：

> <p id="xml_builder_t_xml_builder_on_error">解析遇到错误时调用本函数。

* 函数原型：

```
void xml_builder_on_error (XmlBuilder* builder, int line, int col, const char* message);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| builder | XmlBuilder* | builder对象。 |
| line | int | 出现错误的行。 |
| col | int | 出现错误的列。 |
| message | const char* | 错误信息。 |
#### xml\_builder\_on\_pi 函数
-----------------------

* 函数功能：

> <p id="xml_builder_t_xml_builder_on_pi">解析到处理指令时调用本函数。

* 函数原型：

```
void xml_builder_on_pi (XmlBuilder* builder, const char* tag, const char** attrs);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| builder | XmlBuilder* | builder对象。 |
| tag | const char* | tag名称。 |
| attrs | const char** | 属性列表。 |
#### xml\_builder\_on\_start 函数
-----------------------

* 函数功能：

> <p id="xml_builder_t_xml_builder_on_start">解析到tag开始时调用本函数。

* 函数原型：

```
void xml_builder_on_start (XmlBuilder* builder, const char* tag, const char** attrs);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| builder | XmlBuilder* | builder对象。 |
| tag | const char* | tag名称。 |
| attrs | const char** | 属性列表。 |
#### xml\_builder\_on\_text 函数
-----------------------

* 函数功能：

> <p id="xml_builder_t_xml_builder_on_text">解析到文本时调用本函数。

* 函数原型：

```
void xml_builder_on_text (XmlBuilder* builder, const char* text, size_t length);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| builder | XmlBuilder* | builder对象。 |
| text | const char* | 文本。 |
| length | size\_t | 文本长度。 |
