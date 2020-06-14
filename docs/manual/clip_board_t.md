## clip\_board\_t
### 概述
剪切板接口。
----------------------------------
### 函数
<p id="clip_board_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#clip_board_t_clip_board">clip\_board</a> | 获取缺省的剪切板对象。 |
| <a href="#clip_board_t_clip_board_clear">clip\_board\_clear</a> | 清空剪切板中的数据。 |
| <a href="#clip_board_t_clip_board_destroy">clip\_board\_destroy</a> | 销毁剪切板对象。 |
| <a href="#clip_board_t_clip_board_get_data">clip\_board\_get\_data</a> | 从剪切板中获取数据。 |
| <a href="#clip_board_t_clip_board_get_text">clip\_board\_get\_text</a> | 从剪切板中获取文本(UTF8)数据。 |
| <a href="#clip_board_t_clip_board_set">clip\_board\_set</a> | 设置缺省的剪切板对象。 |
| <a href="#clip_board_t_clip_board_set_data">clip\_board\_set\_data</a> | 设置数据到剪切板中。 |
| <a href="#clip_board_t_clip_board_set_text">clip\_board\_set\_text</a> | 设置文本(UTF8)数据到剪切板。 |
| <a href="#clip_board_t_data_reader_asset_create">data\_reader\_asset\_create</a> | 创建基于asset的data reader。 |
#### clip\_board 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_clip_board">获取缺省的剪切板对象。

* 函数原型：

```
ret_t clip_board ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回缺省剪切板对象。 |
#### clip\_board\_clear 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_clip_board_clear">清空剪切板中的数据。

* 函数原型：

```
ret_t clip_board_clear (clip_board_t* cl);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cl | clip\_board\_t* | 剪切板对象。 |
#### clip\_board\_destroy 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_clip_board_destroy">销毁剪切板对象。

* 函数原型：

```
ret_t clip_board_destroy (clip_board_t* cl);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cl | clip\_board\_t* | 剪切板对象。 |
#### clip\_board\_get\_data 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_clip_board_get_data">从剪切板中获取数据。

* 函数原型：

```
ret_t clip_board_get_data (clip_board_t* cl, clip_board_data_type_t* type, void** data data, uint32_t* size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cl | clip\_board\_t* | 剪切板对象。 |
| type | clip\_board\_data\_type\_t* | 返回数据类型(可为NULL)。 |
| data | void** data | 返回数据(可为NULL)。 |
| size | uint32\_t* | 返回数据长度(可为NULL)。 |
#### clip\_board\_get\_text 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_clip_board_get_text">从剪切板中获取文本(UTF8)数据。

* 函数原型：

```
const char* clip_board_get_text ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回文本数据。 |
#### clip\_board\_set 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_clip_board_set">设置缺省的剪切板对象。

* 函数原型：

```
ret_t clip_board_set (clip_board_t* cl);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cl | clip\_board\_t* | 剪切板对象。 |
#### clip\_board\_set\_data 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_clip_board_set_data">设置数据到剪切板中。

* 函数原型：

```
ret_t clip_board_set_data (clip_board_t* cl, clip_board_data_type_t type, const void* data data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cl | clip\_board\_t* | 剪切板对象。 |
| type | clip\_board\_data\_type\_t | 数据类型。 |
| data | const void* data | 数据。 |
| size | uint32\_t | 数据长度 |
#### clip\_board\_set\_text 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_clip_board_set_text">设置文本(UTF8)数据到剪切板。

* 函数原型：

```
ret_t clip_board_set_text (const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text | const char* | 文本。 |
#### data\_reader\_asset\_create 函数
-----------------------

* 函数功能：

> <p id="clip_board_t_data_reader_asset_create">创建基于asset的data reader。

* 函数原型：

```
data_reader_t* data_reader_asset_create (const char* assetname);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_reader\_t* | 返回data reader对象。 |
| assetname | const char* | 资源的名称。 |
