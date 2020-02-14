## file\_browser\_t
### 概述
文件浏览器模型。
----------------------------------
### 函数
<p id="file_browser_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#file_browser_t_file_browser_create">file\_browser\_create</a> | 创建file_browser对象 |
| <a href="#file_browser_t_file_browser_create_dir">file\_browser\_create\_dir</a> | 在当前目录下创建指定子目录。 |
| <a href="#file_browser_t_file_browser_create_file">file\_browser\_create\_file</a> | 在当前目录下创建指定文件。 |
| <a href="#file_browser_t_file_browser_destroy">file\_browser\_destroy</a> | 销毁file_browser对象 |
| <a href="#file_browser_t_file_browser_enter">file\_browser\_enter</a> | 进入当前目录下的指定子目录。 |
| <a href="#file_browser_t_file_browser_get_item">file\_browser\_get\_item</a> | 获取指定项序数的项目。 |
| <a href="#file_browser_t_file_browser_get_items_nr">file\_browser\_get\_items\_nr</a> | 获取当前目录下文件项个数。 |
| <a href="#file_browser_t_file_browser_refresh">file\_browser\_refresh</a> | 刷新。 |
| <a href="#file_browser_t_file_browser_remove">file\_browser\_remove</a> | 删除当前目录下的指定子目录或文件。 |
| <a href="#file_browser_t_file_browser_set_compare">file\_browser\_set\_compare</a> | 设置排序用的比较函数。 |
| <a href="#file_browser_t_file_browser_set_cwd">file\_browser\_set\_cwd</a> | 设置当前目录。 |
| <a href="#file_browser_t_file_browser_set_filter">file\_browser\_set\_filter</a> | 设置过滤函数。 |
| <a href="#file_browser_t_file_browser_set_ignore_hidden_files">file\_browser\_set\_ignore\_hidden\_files</a> | 设置是否忽略隐藏文件。 |
| <a href="#file_browser_t_file_browser_sort_by_mtime">file\_browser\_sort\_by\_mtime</a> | 按修改时间排序。 |
| <a href="#file_browser_t_file_browser_sort_by_name">file\_browser\_sort\_by\_name</a> | 按名称排序。 |
| <a href="#file_browser_t_file_browser_sort_by_size">file\_browser\_sort\_by\_size</a> | 按大小排序。 |
| <a href="#file_browser_t_file_browser_sort_by_type">file\_browser\_sort\_by\_type</a> | 按类型排序。 |
| <a href="#file_browser_t_file_browser_up">file\_browser\_up</a> | 返回上一级目录。 |
### 属性
<p id="file_browser_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#file_browser_t_cwd">cwd</a> | char* | 当前路径。 |
#### file\_browser\_create 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_create">创建file_browser对象

* 函数原型：

```
file_browser_t* file_browser_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | file\_browser\_t* | 对象。 |
#### file\_browser\_create\_dir 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_create_dir">在当前目录下创建指定子目录。

* 函数原型：

```
ret_t file_browser_create_dir (file_browser_t* fb, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| name | const char* | 当前目录下的子目录名。 |
#### file\_browser\_create\_file 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_create_file">在当前目录下创建指定文件。

* 函数原型：

```
ret_t file_browser_create_file (file_browser_t* fb, const char* name, const void* buff, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| name | const char* | 当前目录下的文件名。 |
| buff | const void* | 文件内容。 |
| size | uint32\_t | 文件内容的长度。 |
#### file\_browser\_destroy 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_destroy">销毁file_browser对象

* 函数原型：

```
ret_t file_browser_destroy (file_browser_t* fb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
#### file\_browser\_enter 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_enter">进入当前目录下的指定子目录。

* 函数原型：

```
ret_t file_browser_enter (file_browser_t* fb, const char* dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| dir | const char* | 当前目录下的子目录名。 |
#### file\_browser\_get\_item 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_get_item">获取指定项序数的项目。

* 函数原型：

```
fb_item_t* file_browser_get_item (file_browser_t* fb, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fb\_item\_t* | 返回指定项序数的项目。 |
| fb | file\_browser\_t* | file browser对象。 |
| index | uint32\_t | 序数。 |
#### file\_browser\_get\_items\_nr 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_get_items_nr">获取当前目录下文件项个数。

* 函数原型：

```
uint32_t file_browser_get_items_nr (file_browser_t* fb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回当前目录下文件项个数。 |
| fb | file\_browser\_t* | file browser对象。 |
#### file\_browser\_refresh 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_refresh">刷新。

* 函数原型：

```
ret_t file_browser_refresh (file_browser_t* fb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
#### file\_browser\_remove 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_remove">删除当前目录下的指定子目录或文件。

* 函数原型：

```
ret_t file_browser_remove (file_browser_t* fb, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| name | const char* | 当前目录下的子目录名/文件名。 |
#### file\_browser\_set\_compare 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_set_compare">设置排序用的比较函数。

* 函数原型：

```
ret_t file_browser_set_compare (file_browser_t* fb, tk_compare_t compare);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| compare | tk\_compare\_t | 排序用的比较函数。 |
#### file\_browser\_set\_cwd 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_set_cwd">设置当前目录。

* 函数原型：

```
ret_t file_browser_set_cwd (file_browser_t* fb, const char* cwd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| cwd | const char* | 当前目录。 |
#### file\_browser\_set\_filter 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_set_filter">设置过滤函数。

* 函数原型：

```
ret_t file_browser_set_filter (file_browser_t* fb, tk_filter_t filter, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| filter | tk\_filter\_t | 过滤函数。 |
| ctx | void* | 过滤函数的上下文。 |
#### file\_browser\_set\_ignore\_hidden\_files 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_set_ignore_hidden_files">设置是否忽略隐藏文件。

* 函数原型：

```
ret_t file_browser_set_ignore_hidden_files (file_browser_t* fb, bool_t ignore_hidden_files);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| ignore\_hidden\_files | bool\_t | 是否忽略隐藏文件。 |
#### file\_browser\_sort\_by\_mtime 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_sort_by_mtime">按修改时间排序。

* 函数原型：

```
ret_t file_browser_sort_by_mtime (file_browser_t* fb, bool_t ascending);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| ascending | bool\_t | 按升序排。 |
#### file\_browser\_sort\_by\_name 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_sort_by_name">按名称排序。

* 函数原型：

```
ret_t file_browser_sort_by_name (file_browser_t* fb, bool_t ascending);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| ascending | bool\_t | 按升序排。 |
#### file\_browser\_sort\_by\_size 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_sort_by_size">按大小排序。

* 函数原型：

```
ret_t file_browser_sort_by_size (file_browser_t* fb, bool_t ascending);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| ascending | bool\_t | 按升序排。 |
#### file\_browser\_sort\_by\_type 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_sort_by_type">按类型排序。

* 函数原型：

```
ret_t file_browser_sort_by_type (file_browser_t* fb, bool_t ascending);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
| ascending | bool\_t | 按升序排。 |
#### file\_browser\_up 函数
-----------------------

* 函数功能：

> <p id="file_browser_t_file_browser_up">返回上一级目录。

* 函数原型：

```
ret_t file_browser_up (file_browser_t* fb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fb | file\_browser\_t* | file browser对象。 |
#### cwd 属性
-----------------------
> <p id="file_browser_t_cwd">当前路径。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
