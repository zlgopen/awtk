## fs\_item\_t
### 概述
代表目录或文件。
----------------------------------
### 函数
<p id="fs_item_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fs_item_t_fs_item_create">fs\_item\_create</a> | 创建文件或目录对象。 |
| <a href="#fs_item_t_fs_item_destroy">fs\_item\_destroy</a> | 销毁文件或目录对象。 |
### 属性
<p id="fs_item_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#fs_item_t_is_dir">is\_dir</a> | bool\_t | 是否是目录。 |
| <a href="#fs_item_t_is_link">is\_link</a> | bool\_t | 是否是符号链接。 |
| <a href="#fs_item_t_is_reg_file">is\_reg\_file</a> | bool\_t | 是否是普通文件。 |
| <a href="#fs_item_t_name">name</a> | char* | 名称。 |
#### fs\_item\_create 函数
-----------------------

* 函数功能：

> <p id="fs_item_t_fs_item_create">创建文件或目录对象。

* 函数原型：

```
fs_item_t* fs_item_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fs\_item\_t* | 返回文件或目录对象。 |
#### fs\_item\_destroy 函数
-----------------------

* 函数功能：

> <p id="fs_item_t_fs_item_destroy">销毁文件或目录对象。

* 函数原型：

```
ret_t fs_item_destroy (fs_item_t* item);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| item | fs\_item\_t* | 文件或目录对象。 |
#### is\_dir 属性
-----------------------
> <p id="fs_item_t_is_dir">是否是目录。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### is\_link 属性
-----------------------
> <p id="fs_item_t_is_link">是否是符号链接。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### is\_reg\_file 属性
-----------------------
> <p id="fs_item_t_is_reg_file">是否是普通文件。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### name 属性
-----------------------
> <p id="fs_item_t_name">名称。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
