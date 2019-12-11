## ubjson\_parser\_t
### 概述
ubjson parser
----------------------------------
### 函数
<p id="ubjson_parser_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#ubjson_parser_t_ubjson_dump">ubjson\_dump</a> | 将ubjson数据打印到控制台。 |
| <a href="#ubjson_parser_t_ubjson_parse">ubjson\_parse</a> | 解析ubjson数据，遇到key/value时调用提供的回调函数。 |
| <a href="#ubjson_parser_t_ubjson_to_object">ubjson\_to\_object</a> | 解析ubjson数据，生成object对象。 |
#### ubjson\_dump 函数
-----------------------

* 函数功能：

> <p id="ubjson_parser_t_ubjson_dump">将ubjson数据打印到控制台。

* 函数原型：

```
ret_t ubjson_dump (void* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| data | void* | 数据。 |
| size | uint32\_t | 数据的长度。 |
#### ubjson\_parse 函数
-----------------------

* 函数功能：

> <p id="ubjson_parser_t_ubjson_parse">解析ubjson数据，遇到key/value时调用提供的回调函数。

* 函数原型：

```
ret_t ubjson_parse (void* data, uint32_t size, ubjson_on_key_value_t on_key_value, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| data | void* | 数据。 |
| size | uint32\_t | 数据的长度。 |
| on\_key\_value | ubjson\_on\_key\_value\_t | 回调函数。 |
| ctx | void* | 回调函数的上下文。 |
#### ubjson\_to\_object 函数
-----------------------

* 函数功能：

> <p id="ubjson_parser_t_ubjson_to_object">解析ubjson数据，生成object对象。

* 函数原型：

```
void* ubjson_to_object (void* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回object表示的数据。 |
| data | void* | 数据。 |
| size | uint32\_t | 数据的长度。 |
