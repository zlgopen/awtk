## csv\_file\_t
### 概述
操作CSV文件。
----------------------------------
### 函数
<p id="csv_file_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#csv_file_t_csv_file_append_row">csv\_file\_append\_row</a> | 追加一行。 |
| <a href="#csv_file_t_csv_file_clear">csv\_file\_clear</a> | 保存。 |
| <a href="#csv_file_t_csv_file_create">csv\_file\_create</a> | 根据文件创建csv对象。 |
| <a href="#csv_file_t_csv_file_create_empty">csv\_file\_create\_empty</a> | 创建空的csv对象。 |
| <a href="#csv_file_t_csv_file_create_with_buff">csv\_file\_create\_with\_buff</a> | 根据buff创建csv对象。 |
| <a href="#csv_file_t_csv_file_destroy">csv\_file\_destroy</a> | 销毁csv对象。 |
| <a href="#csv_file_t_csv_file_get">csv\_file\_get</a> | 获取指定行列的数据。 |
| <a href="#csv_file_t_csv_file_get_checked_rows">csv\_file\_get\_checked\_rows</a> | 获取checked行数(包括标题)。 |
| <a href="#csv_file_t_csv_file_get_cols">csv\_file\_get\_cols</a> | 获取列数。 |
| <a href="#csv_file_t_csv_file_get_rows">csv\_file\_get\_rows</a> | 获取行数(包括标题)。 |
| <a href="#csv_file_t_csv_file_get_title">csv\_file\_get\_title</a> | 获取标题(不存在则返回NULL)。 |
| <a href="#csv_file_t_csv_file_insert_row">csv\_file\_insert\_row</a> | 插入一行。 |
| <a href="#csv_file_t_csv_file_is_row_checked">csv\_file\_is\_row\_checked</a> | 判断指定行是否勾选。 |
| <a href="#csv_file_t_csv_file_load_buff">csv\_file\_load\_buff</a> | 从内存加载csv。 |
| <a href="#csv_file_t_csv_file_load_file">csv\_file\_load\_file</a> | 从文件加载csv。 |
| <a href="#csv_file_t_csv_file_load_file">csv\_file\_load\_file</a> | 保存。 |
| <a href="#csv_file_t_csv_file_reload">csv\_file\_reload</a> | 丢弃内存中的修改，重新加载当前文件。 |
| <a href="#csv_file_t_csv_file_remove_checked_rows">csv\_file\_remove\_checked\_rows</a> | 删除全部勾选的行。 |
| <a href="#csv_file_t_csv_file_remove_row">csv\_file\_remove\_row</a> | 删除指定行。 |
| <a href="#csv_file_t_csv_file_save">csv\_file\_save</a> | 保存。 |
| <a href="#csv_file_t_csv_file_set">csv\_file\_set</a> | 修改指定行列的数据。 |
| <a href="#csv_file_t_csv_file_set_filter">csv\_file\_set\_filter</a> | 设置过滤函数。 |
| <a href="#csv_file_t_csv_file_set_row_checked">csv\_file\_set\_row\_checked</a> | 勾选指定行。 |
| <a href="#csv_file_t_fargs_get_data_and_size">fargs\_get\_data\_and\_size</a> | 从参数中获取数据和长度。 |
| <a href="#csv_file_t_fscript_app_conf_register">fscript\_app\_conf\_register</a> | 注册app_conf函数。 |
| <a href="#csv_file_t_fscript_array_register">fscript\_array\_register</a> | 注册typed array函数。 |
| <a href="#csv_file_t_fscript_bits_register">fscript\_bits\_register</a> | 注册数学函数。 |
| <a href="#csv_file_t_fscript_crc_register">fscript\_crc\_register</a> | 注册crc16/crc32/cksum等函数。 |
| <a href="#csv_file_t_fscript_date_time_register">fscript\_date\_time\_register</a> | 注册date time函数。 |
| <a href="#csv_file_t_fscript_endian_register">fscript\_endian\_register</a> | 注册大端小断转换函数等函数。 |
| <a href="#csv_file_t_fscript_ext_init">fscript\_ext\_init</a> | 注册扩展函数。 |
| <a href="#csv_file_t_fscript_fs_register">fscript\_fs\_register</a> | 注册文件系统函数。 |
| <a href="#csv_file_t_fscript_iostream_file_register">fscript\_iostream\_file\_register</a> | 注册iostream_file相关函数。 |
| <a href="#csv_file_t_fscript_iostream_inet_register">fscript\_iostream\_inet\_register</a> | 注册iostream_inet相关函数。 |
| <a href="#csv_file_t_fscript_iostream_register">fscript\_iostream\_register</a> | 注册iostream相关函数。 |
| <a href="#csv_file_t_fscript_iostream_serial_register">fscript\_iostream\_serial\_register</a> | 注册iostream_serial相关函数。 |
| <a href="#csv_file_t_fscript_istream_register">fscript\_istream\_register</a> | 注册istream相关等函数。 |
| <a href="#csv_file_t_fscript_json_register">fscript\_json\_register</a> | 注册json函数。 |
| <a href="#csv_file_t_fscript_math_register">fscript\_math\_register</a> | 注册数学函数。 |
| <a href="#csv_file_t_fscript_object_register">fscript\_object\_register</a> | 注册object函数。 |
| <a href="#csv_file_t_fscript_ostream_register">fscript\_ostream\_register</a> | 注册ostream相关函数。 |
| <a href="#csv_file_t_fscript_rbuffer_register">fscript\_rbuffer\_register</a> | 注册rbuffer相关函数。 |
| <a href="#csv_file_t_fscript_typed_array_register">fscript\_typed\_array\_register</a> | 注册typed array函数。 |
| <a href="#csv_file_t_fscript_wbuffer_register">fscript\_wbuffer\_register</a> | 注册wbuffer相关函数。 |
| <a href="#csv_file_t_fscript_widget_register">fscript\_widget\_register</a> | 注册widget函数。 |
### 属性
<p id="csv_file_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#csv_file_t_has_title">has\_title</a> | bool\_t | 是否有标题。 |
#### csv\_file\_append\_row 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_append_row">追加一行。

* 函数原型：

```
ret_t csv_file_append_row (csv_file_t* csv, const char* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| data | const char* | 数据。 |
#### csv\_file\_clear 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_clear">保存。

* 函数原型：

```
ret_t csv_file_clear (csv_file_t* csv, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| filename | const char* | 文件名。 |
#### csv\_file\_create 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_create">根据文件创建csv对象。

* 函数原型：

```
csv_file_t* csv_file_create (const char* filename, char sep);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | csv\_file\_t* | 返回csv对象。 |
| filename | const char* | 文件名。 |
| sep | char | 分隔符。 |
#### csv\_file\_create\_empty 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_create_empty">创建空的csv对象。

* 函数原型：

```
csv_file_t* csv_file_create_empty (char sep, csv_file_filter_t filter, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | csv\_file\_t* | 返回csv对象。 |
| sep | char | 分隔符。 |
| filter | csv\_file\_filter\_t | 过滤函数。 |
| ctx | void* | 过滤函数的上下文。 |
#### csv\_file\_create\_with\_buff 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_create_with_buff">根据buff创建csv对象。

* 函数原型：

```
csv_file_t csv_file_create_with_buff (const char* buff, uint32_t size, char sep);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | csv\_file\_t | 返回csv对象。 |
| buff | const char* | 数据。 |
| size | uint32\_t | 数据长度。 |
| sep | char | 分隔符。 |
#### csv\_file\_destroy 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_destroy">销毁csv对象。

* 函数原型：

```
ret_t csv_file_destroy (csv_file_t* csv);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
#### csv\_file\_get 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_get">获取指定行列的数据。

* 函数原型：

```
const char* csv_file_get (csv_file_t* csv, uint32_t row, uint32_t col);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回数据。 |
| csv | csv\_file\_t* | csv对象。 |
| row | uint32\_t | 行号。 |
| col | uint32\_t | 列号。 |
#### csv\_file\_get\_checked\_rows 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_get_checked_rows">获取checked行数(包括标题)。

* 函数原型：

```
uint32_t csv_file_get_checked_rows (csv_file_t* csv);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回checked行数。 |
| csv | csv\_file\_t* | csv对象。 |
#### csv\_file\_get\_cols 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_get_cols">获取列数。

* 函数原型：

```
uint32_t csv_file_get_cols (csv_file_t* csv);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回列数。 |
| csv | csv\_file\_t* | csv对象。 |
#### csv\_file\_get\_rows 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_get_rows">获取行数(包括标题)。

* 函数原型：

```
uint32_t csv_file_get_rows (csv_file_t* csv);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回行数。 |
| csv | csv\_file\_t* | csv对象。 |
#### csv\_file\_get\_title 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_get_title">获取标题(不存在则返回NULL)。

* 函数原型：

```
const char* csv_file_get_title (csv_file_t* csv);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回标题。 |
| csv | csv\_file\_t* | csv对象。 |
#### csv\_file\_insert\_row 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_insert_row">插入一行。

* 函数原型：

```
ret_t csv_file_insert_row (csv_file_t* csv, uint32_t row, const char* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| row | uint32\_t | 行号。 |
| data | const char* | 数据。 |
#### csv\_file\_is\_row\_checked 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_is_row_checked">判断指定行是否勾选。

* 函数原型：

```
bool_t csv_file_is_row_checked (csv_file_t* csv, uint32_t row);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示勾选，否则表示没勾选。 |
| csv | csv\_file\_t* | csv对象。 |
| row | uint32\_t | 行号。 |
#### csv\_file\_load\_buff 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_load_buff">从内存加载csv。

* 函数原型：

```
ret_t csv_file_load_buff (csv_file_t* csv, const char* buff, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| buff | const char* | 数据。 |
| size | uint32\_t | 数据长度。 |
#### csv\_file\_load\_file 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_load_file">从文件加载csv。

* 函数原型：

```
ret_t csv_file_load_file (csv_file_t* csv, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| filename | const char* | 文件名。 |
#### csv\_file\_load\_file 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_load_file">保存。

* 函数原型：

```
ret_t csv_file_load_file (csv_file_t* csv, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| filename | const char* | 文件名。 |
#### csv\_file\_reload 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_reload">丢弃内存中的修改，重新加载当前文件。

* 函数原型：

```
ret_t csv_file_reload (csv_file_t* csv);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
#### csv\_file\_remove\_checked\_rows 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_remove_checked_rows">删除全部勾选的行。

* 函数原型：

```
ret_t csv_file_remove_checked_rows (csv_file_t* csv);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
#### csv\_file\_remove\_row 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_remove_row">删除指定行。

* 函数原型：

```
ret_t csv_file_remove_row (csv_file_t* csv, uint32_t row);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| row | uint32\_t | 行号。 |
#### csv\_file\_save 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_save">保存。

* 函数原型：

```
ret_t csv_file_save (csv_file_t* csv, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| filename | const char* | 文件名。 |
#### csv\_file\_set 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_set">修改指定行列的数据。

* 函数原型：

```
ret_t csv_file_set (csv_file_t* csv, uint32_t row, uint32_t col, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| row | uint32\_t | 行号。 |
| col | uint32\_t | 列号。 |
| value | const char* | 值。 |
#### csv\_file\_set\_filter 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_set_filter">设置过滤函数。

* 函数原型：

```
ret_t csv_file_set_filter (csv_file_t* csv, csv_file_filter_t filter, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| filter | csv\_file\_filter\_t | 过滤函数。 |
| ctx | void* | 过滤函数的上下文。 |
#### csv\_file\_set\_row\_checked 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_csv_file_set_row_checked">勾选指定行。

* 函数原型：

```
ret_t csv_file_set_row_checked (csv_file_t* csv, uint32_t row, bool_t checked);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| csv | csv\_file\_t* | csv对象。 |
| row | uint32\_t | 行号。 |
| checked | bool\_t | 是否勾选。 |
#### fargs\_get\_data\_and\_size 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fargs_get_data_and_size">从参数中获取数据和长度。

* 函数原型：

```
ret_t fargs_get_data_and_size (fscript_args_t* args, const uint8_t** , uint32_t* ret_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| args | fscript\_args\_t* | 参数。 |
|  | const uint8\_t** | 。 |
| ret\_size | uint32\_t* | 返回数据的长度。 |
#### fscript\_app\_conf\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_app_conf_register">注册app_conf函数。

* 函数原型：

```
ret_t fscript_app_conf_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_array\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_array_register">注册typed array函数。

* 函数原型：

```
ret_t fscript_array_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_bits\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_bits_register">注册数学函数。

* 函数原型：

```
ret_t fscript_bits_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_crc\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_crc_register">注册crc16/crc32/cksum等函数。

* 函数原型：

```
ret_t fscript_crc_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_date\_time\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_date_time_register">注册date time函数。

* 函数原型：

```
ret_t fscript_date_time_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_endian\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_endian_register">注册大端小断转换函数等函数。

* 函数原型：

```
ret_t fscript_endian_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_ext\_init 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_ext_init">注册扩展函数。

* 函数原型：

```
ret_t fscript_ext_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_fs\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_fs_register">注册文件系统函数。

* 函数原型：

```
ret_t fscript_fs_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_iostream\_file\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_iostream_file_register">注册iostream_file相关函数。

* 函数原型：

```
ret_t fscript_iostream_file_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_iostream\_inet\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_iostream_inet_register">注册iostream_inet相关函数。

* 函数原型：

```
ret_t fscript_iostream_inet_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_iostream\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_iostream_register">注册iostream相关函数。

* 函数原型：

```
ret_t fscript_iostream_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_iostream\_serial\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_iostream_serial_register">注册iostream_serial相关函数。

* 函数原型：

```
ret_t fscript_iostream_serial_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_istream\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_istream_register">注册istream相关等函数。

* 函数原型：

```
ret_t fscript_istream_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_json\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_json_register">注册json函数。

* 函数原型：

```
ret_t fscript_json_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_math\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_math_register">注册数学函数。

* 函数原型：

```
ret_t fscript_math_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_object\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_object_register">注册object函数。

* 函数原型：

```
ret_t fscript_object_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_ostream\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_ostream_register">注册ostream相关函数。

* 函数原型：

```
ret_t fscript_ostream_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_rbuffer\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_rbuffer_register">注册rbuffer相关函数。

* 函数原型：

```
ret_t fscript_rbuffer_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_typed\_array\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_typed_array_register">注册typed array函数。

* 函数原型：

```
ret_t fscript_typed_array_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_wbuffer\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_wbuffer_register">注册wbuffer相关函数。

* 函数原型：

```
ret_t fscript_wbuffer_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_widget\_register 函数
-----------------------

* 函数功能：

> <p id="csv_file_t_fscript_widget_register">注册widget函数。

* 函数原型：

```
ret_t fscript_widget_register ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### has\_title 属性
-----------------------
> <p id="csv_file_t_has_title">是否有标题。

* 类型：bool\_t

