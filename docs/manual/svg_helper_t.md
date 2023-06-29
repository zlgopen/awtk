## svg\_helper\_t
### 概述

----------------------------------
### 函数
<p id="svg_helper_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#svg_helper_t_bsvg_to_svg">bsvg\_to\_svg</a> | bsvg to svg |
| <a href="#svg_helper_t_svg_file_to_bsvg">svg\_file\_to\_bsvg</a> | svg to bsvg |
| <a href="#svg_helper_t_svg_to_bsvg">svg\_to\_bsvg</a> | svg to bsvg |
#### bsvg\_to\_svg 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_bsvg_to_svg">bsvg to svg

* 函数原型：

```
ret_t bsvg_to_svg (const uint32_t* data, uint32_t size, str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| data | const uint32\_t* | bsvg数据。 |
| size | uint32\_t | bsvg数据长度。 |
| str | str\_t* | 用于返回svg数据。 |
#### svg\_file\_to\_bsvg 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_svg_file_to_bsvg">svg to bsvg

* 函数原型：

```
ret_t svg_file_to_bsvg (const char* ifilename, const char* ofilename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ifilename | const char* | svg文件名。 |
| ofilename | const char* | bsvg文件名。 |
#### svg\_to\_bsvg 函数
-----------------------

* 函数功能：

> <p id="svg_helper_t_svg_to_bsvg">svg to bsvg

* 函数原型：

```
ret_t svg_to_bsvg (const char* xml, uint32_t size, uint32_t** out, uint32_t* out_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| xml | const char* | svg数据。 |
| size | uint32\_t | svg数据长度。 |
| out | uint32\_t** | 用于返回bsvg数据。 |
| out\_size | uint32\_t* | bsvg数据长度。 |
