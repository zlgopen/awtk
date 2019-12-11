## fs\_dir\_t
### 概述
文件夹接口。
----------------------------------
### 函数
<p id="fs_dir_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fs_dir_t_fs_dir_close">fs\_dir\_close</a> | 关闭文件夹对象。 |
| <a href="#fs_dir_t_fs_dir_read">fs\_dir\_read</a> | 读取文件夹对象。 |
| <a href="#fs_dir_t_fs_dir_rewind">fs\_dir\_rewind</a> | 重置文件夹读取位置到开始。 |
#### fs\_dir\_close 函数
-----------------------

* 函数功能：

> <p id="fs_dir_t_fs_dir_close">关闭文件夹对象。

* 函数原型：

```
ret_t fs_dir_close (fs_dir_t* dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dir | fs\_dir\_t* | 文件夹对象。 |
#### fs\_dir\_read 函数
-----------------------

* 函数功能：

> <p id="fs_dir_t_fs_dir_read">读取文件夹对象。

* 函数原型：

```
ret_t fs_dir_read (fs_dir_t* dir, fs_item_t* item);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dir | fs\_dir\_t* | 文件夹对象。 |
| item | fs\_item\_t* | 返回一项。 |
#### fs\_dir\_rewind 函数
-----------------------

* 函数功能：

> <p id="fs_dir_t_fs_dir_rewind">重置文件夹读取位置到开始。

* 函数原型：

```
ret_t fs_dir_rewind (fs_dir_t* dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dir | fs\_dir\_t* | 文件夹对象。 |
