## fs\_stat\_info\_t
### 概述
文件状态信息。
----------------------------------
### 函数
<p id="fs_stat_info_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fs_stat_info_t_fs_stat_info_create">fs\_stat\_info\_create</a> | 创建文件状态信息对象。 |
| <a href="#fs_stat_info_t_fs_stat_info_destroy">fs\_stat\_info\_destroy</a> | 销毁文件状态信息对象。 |
### 属性
<p id="fs_stat_info_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#fs_stat_info_t_atime">atime</a> | uint64\_t | 最后访问时间。 |
| <a href="#fs_stat_info_t_ctime">ctime</a> | uint64\_t | 创建时间。 |
| <a href="#fs_stat_info_t_is_dir">is\_dir</a> | bool\_t | 是否为目录。 |
| <a href="#fs_stat_info_t_is_link">is\_link</a> | bool\_t | 是否为链接。 |
| <a href="#fs_stat_info_t_is_reg_file">is\_reg\_file</a> | bool\_t | 是否普通文件。 |
| <a href="#fs_stat_info_t_mtime">mtime</a> | uint64\_t | 最后修改时间。 |
| <a href="#fs_stat_info_t_size">size</a> | uint64\_t | 文件大小。 |
#### fs\_stat\_info\_create 函数
-----------------------

* 函数功能：

> <p id="fs_stat_info_t_fs_stat_info_create">创建文件状态信息对象。

* 函数原型：

```
fs_stat_info_t* fs_stat_info_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fs\_stat\_info\_t* | 返回文件状态信息对象。 |
#### fs\_stat\_info\_destroy 函数
-----------------------

* 函数功能：

> <p id="fs_stat_info_t_fs_stat_info_destroy">销毁文件状态信息对象。

* 函数原型：

```
ret_t fs_stat_info_destroy (fs_stat_info_t* fst);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fst | fs\_stat\_info\_t* | 文件状态信息对象。 |
#### atime 属性
-----------------------
> <p id="fs_stat_info_t_atime">最后访问时间。

* 类型：uint64\_t

#### ctime 属性
-----------------------
> <p id="fs_stat_info_t_ctime">创建时间。

* 类型：uint64\_t

#### is\_dir 属性
-----------------------
> <p id="fs_stat_info_t_is_dir">是否为目录。

* 类型：bool\_t

#### is\_link 属性
-----------------------
> <p id="fs_stat_info_t_is_link">是否为链接。

* 类型：bool\_t

#### is\_reg\_file 属性
-----------------------
> <p id="fs_stat_info_t_is_reg_file">是否普通文件。

* 类型：bool\_t

#### mtime 属性
-----------------------
> <p id="fs_stat_info_t_mtime">最后修改时间。

* 类型：uint64\_t

#### size 属性
-----------------------
> <p id="fs_stat_info_t_size">文件大小。

* 类型：uint64\_t

