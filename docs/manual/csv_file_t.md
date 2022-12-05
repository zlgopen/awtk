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
#### has\_title 属性
-----------------------
> <p id="csv_file_t_has_title">是否有标题。

* 类型：bool\_t

