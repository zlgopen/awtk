## XmlParser
### 概述
xml解析器
----------------------------------
### 函数
<p id="XmlParser_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#XmlParser_xml_parser_create">xml\_parser\_create</a> | 创建解析器。 |
| <a href="#XmlParser_xml_parser_destroy">xml\_parser\_destroy</a> | 销毁parser对象。 |
| <a href="#XmlParser_xml_parser_parse">xml\_parser\_parse</a> | 解析数据。 |
| <a href="#XmlParser_xml_parser_parse_file">xml\_parser\_parse\_file</a> | 解析文件。 |
| <a href="#XmlParser_xml_parser_set_builder">xml\_parser\_set\_builder</a> | 设置builder。 |
| <a href="#XmlParser_xml_parser_set_trim_text">xml\_parser\_set\_trim\_text</a> | 设置是否去掉文本两端的空白字符。 |
#### xml\_parser\_create 函数
-----------------------

* 函数功能：

> <p id="XmlParser_xml_parser_create">创建解析器。

* 函数原型：

```
XmlParser* xml_parser_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | XmlParser* | 返回parser对象。 |
#### xml\_parser\_destroy 函数
-----------------------

* 函数功能：

> <p id="XmlParser_xml_parser_destroy">销毁parser对象。

* 函数原型：

```
void xml_parser_destroy (XmlParser* thiz);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| thiz | XmlParser* | parser对象。 |
#### xml\_parser\_parse 函数
-----------------------

* 函数功能：

> <p id="XmlParser_xml_parser_parse">解析数据。

* 函数原型：

```
void xml_parser_parse (XmlParser* thiz, const char* xml, int length);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| thiz | XmlParser* | parser对象。 |
| xml | const char* | 数据。 |
| length | int | 数据长度。 |
#### xml\_parser\_parse\_file 函数
-----------------------

* 函数功能：

> <p id="XmlParser_xml_parser_parse_file">解析文件。

* 函数原型：

```
void xml_parser_parse_file (XmlParser* thiz, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| thiz | XmlParser* | parser对象。 |
| filename | const char* | 文件名。 |
#### xml\_parser\_set\_builder 函数
-----------------------

* 函数功能：

> <p id="XmlParser_xml_parser_set_builder">设置builder。

* 函数原型：

```
void xml_parser_set_builder (XmlParser* thiz, XmlBuilder* builder);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| thiz | XmlParser* | parser对象。 |
| builder | XmlBuilder* | builder对象。 |
#### xml\_parser\_set\_trim\_text 函数
-----------------------

* 函数功能：

> <p id="XmlParser_xml_parser_set_trim_text">设置是否去掉文本两端的空白字符。

> 默认值为去掉文本两端的空白字符，如果不x希望去掉，调用本函数设置为FALSE。

* 函数原型：

```
void xml_parser_set_trim_text (XmlParser* thiz, bool_t trim_text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回无。 |
| thiz | XmlParser* | parser对象。 |
| trim\_text | bool\_t | 是否去掉文本两端的空白字符。 |
