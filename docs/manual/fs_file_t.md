## fs\_file\_t
### 概述

 文件接口。


----------------------------------
### 函数
<p id="fs_file_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fs_file_t_fs_file_close">fs\_file\_close</a> |  |
| <a href="#fs_file_t_fs_file_eof">fs\_file\_eof</a> |  |
| <a href="#fs_file_t_fs_file_read">fs\_file\_read</a> |  |
| <a href="#fs_file_t_fs_file_seek">fs\_file\_seek</a> |  |
| <a href="#fs_file_t_fs_file_truncate">fs\_file\_truncate</a> |  |
| <a href="#fs_file_t_fs_file_write">fs\_file\_write</a> |  |
#### fs\_file\_close 函数
-----------------------

* 函数功能：

> <p id="fs_file_t_fs_file_close">
 关闭文件。





* 函数原型：

```
ret_t fs_file_close (fs_file_t* file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| file | fs\_file\_t* | 文件对象。 |
#### fs\_file\_eof 函数
-----------------------

* 函数功能：

> <p id="fs_file_t_fs_file_eof">
 判断文件是否结束。





* 函数原型：

```
bool_t fs_file_eof (fs_file_t* file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示结束，否则表示没结束。 |
| file | fs\_file\_t* | 文件对象。 |
#### fs\_file\_read 函数
-----------------------

* 函数功能：

> <p id="fs_file_t_fs_file_read">
 读取文件。





* 函数原型：

```
int32_t fs_file_read (fs_file_t* file, void* buffer, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际读取的字节数。 |
| file | fs\_file\_t* | 文件对象。 |
| buffer | void* | 用于返回数据的缓冲区。 |
| size | uint32\_t | 缓冲区大小。 |
#### fs\_file\_seek 函数
-----------------------

* 函数功能：

> <p id="fs_file_t_fs_file_seek">
 定位读写指针到指定的位置。





* 函数原型：

```
ret_t fs_file_seek (fs_file_t* file, uint32_t offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| file | fs\_file\_t* | 文件对象。 |
| offset | uint32\_t | 数据长度。 |
#### fs\_file\_truncate 函数
-----------------------

* 函数功能：

> <p id="fs_file_t_fs_file_truncate">
 清除文件内容。





* 函数原型：

```
ret_t fs_file_truncate (fs_file_t* file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| file | fs\_file\_t* | 文件对象。 |
#### fs\_file\_write 函数
-----------------------

* 函数功能：

> <p id="fs_file_t_fs_file_write">
 写入文件。





* 函数原型：

```
int32_t fs_file_write (fs_file_t* file, const void* buffer, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际写入的字节数。 |
| file | fs\_file\_t* | 文件对象。 |
| buffer | const void* | 数据缓冲区。 |
| size | uint32\_t | 数据长度。 |
