## fs\_t
### 概述
文件系统接口。
----------------------------------
### 函数
<p id="fs_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fs_t_file_exist">file\_exist</a> | 判断文件是否存在。 |
| <a href="#fs_t_file_get_size">file\_get\_size</a> | 获取文件大小。 |
| <a href="#fs_t_file_read">file\_read</a> | 读取文件的全部内容。 |
| <a href="#fs_t_file_read_part">file\_read\_part</a> | 从某个位置读取文件。 |
| <a href="#fs_t_file_remove">file\_remove</a> | 刪除文件。 |
| <a href="#fs_t_file_write">file\_write</a> | 写入文件。 |
| <a href="#fs_t_fs_create_dir">fs\_create\_dir</a> | 创建目录。 |
| <a href="#fs_t_fs_dir_exist">fs\_dir\_exist</a> | 判断目录是否存在。 |
| <a href="#fs_t_fs_dir_rename">fs\_dir\_rename</a> | 目录重命名。 |
| <a href="#fs_t_fs_file_exist">fs\_file\_exist</a> | 判断文件是否存在。 |
| <a href="#fs_t_fs_file_rename">fs\_file\_rename</a> | 文件重命名。 |
| <a href="#fs_t_fs_get_cwd">fs\_get\_cwd</a> | 获取当前所在目录。 |
| <a href="#fs_t_fs_get_disk_info">fs\_get\_disk\_info</a> | 获取文件系统信息。 |
| <a href="#fs_t_fs_get_exe">fs\_get\_exe</a> | 获取可执行文件所在目录。 |
| <a href="#fs_t_fs_get_file_size">fs\_get\_file\_size</a> | 获取文件大小。 |
| <a href="#fs_t_fs_get_user_storage_path">fs\_get\_user\_storage\_path</a> | 获取home目录或者应用程序可以写入数据的目录。 |
| <a href="#fs_t_fs_open_dir">fs\_open\_dir</a> | 打开目录。 |
| <a href="#fs_t_fs_open_file">fs\_open\_file</a> | 打开文件。 |
| <a href="#fs_t_fs_remove_dir">fs\_remove\_dir</a> | 刪除目录。 |
| <a href="#fs_t_fs_remove_file">fs\_remove\_file</a> | 刪除文件。 |
| <a href="#fs_t_fs_stat">fs\_stat</a> | 获取文件信息。 |
| <a href="#fs_t_os_fs">os\_fs</a> | 获取缺省的文件系统对象。 |
#### file\_exist 函数
-----------------------

* 函数功能：

> <p id="fs_t_file_exist">判断文件是否存在。

* 函数原型：

```
bool_t file_exist (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示成功，否则表示失败。 |
| name | const char* | 文件名。 |
#### file\_get\_size 函数
-----------------------

* 函数功能：

> <p id="fs_t_file_get_size">获取文件大小。

* 函数原型：

```
int32_t file_get_size (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回非负表示文件大小，否则表示失败。 |
| name | const char* | 文件名。 |
#### file\_read 函数
-----------------------

* 函数功能：

> <p id="fs_t_file_read">读取文件的全部内容。

* 函数原型：

```
void* file_read (const char* name, uint32_t* size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回读取的数据，需要调用TKMEM\_FREE释放。 |
| name | const char* | 文件名。 |
| size | uint32\_t* | 返回实际读取的长度。 |
#### file\_read\_part 函数
-----------------------

* 函数功能：

> <p id="fs_t_file_read_part">从某个位置读取文件。

* 函数原型：

```
int32_t file_read_part (const char* name, const void* buffer, uint32_t size, uint32_t offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际读取的字节数。 |
| name | const char* | 文件名。 |
| buffer | const void* | 数据缓冲区。 |
| size | uint32\_t | 数据长度。 |
| offset | uint32\_t | 偏移量。 |
#### file\_remove 函数
-----------------------

* 函数功能：

> <p id="fs_t_file_remove">刪除文件。

* 函数原型：

```
ret_t file_remove (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| name | const char* | 文件名。 |
#### file\_write 函数
-----------------------

* 函数功能：

> <p id="fs_t_file_write">写入文件。

* 函数原型：

```
ret_t file_write (const char* name, const void* buffer, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| name | const char* | 文件名。 |
| buffer | const void* | 数据缓冲区。 |
| size | uint32\_t | 数据长度。 |
#### fs\_create\_dir 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_create_dir">创建目录。

* 函数原型：

```
ret_t fs_create_dir (fs_t* fs, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 目录名称。 |
#### fs\_dir\_exist 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_dir_exist">判断目录是否存在。

* 函数原型：

```
bool_t fs_dir_exist (fs_t* fs, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 目录名称。 |
#### fs\_dir\_rename 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_dir_rename">目录重命名。

* 函数原型：

```
ret_t fs_dir_rename (fs_t* fs, const char* name, const char* new_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 旧目录名称。 |
| new\_name | const char* | 新目录名称。 |
#### fs\_file\_exist 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_file_exist">判断文件是否存在。

* 函数原型：

```
bool_t fs_file_exist (fs_t* fs, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 文件名。 |
#### fs\_file\_rename 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_file_rename">文件重命名。

* 函数原型：

```
ret_t fs_file_rename (fs_t* fs, const char* name, const char* new_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 旧文件名。 |
| new\_name | const char* | 新文件名。 |
#### fs\_get\_cwd 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_get_cwd">获取当前所在目录。

* 函数原型：

```
ret_t fs_get_cwd (fs_t* fs, char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| path | char* | 保存当前所在目录的路径。 |
#### fs\_get\_disk\_info 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_get_disk_info">获取文件系统信息。

* 函数原型：

```
ret_t fs_get_disk_info (fs_t* fs, const char* value, int32_t* free_kb, int32_t* total_kb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回不是-1表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| value | const char* | 卷名。 |
| free\_kb | int32\_t* | 用于返回空闲空间大小(KB) |
| total\_kb | int32\_t* | 用于返回总共空间大小(KB) |
#### fs\_get\_exe 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_get_exe">获取可执行文件所在目录。

* 函数原型：

```
ret_t fs_get_exe (fs_t* fs, char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| path | char* | 保存可执行文件的路径。 |
#### fs\_get\_file\_size 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_get_file_size">获取文件大小。

* 函数原型：

```
ret_t fs_get_file_size (fs_t* fs, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回不是-1表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 文件名。 |
#### fs\_get\_user\_storage\_path 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_get_user_storage_path">获取home目录或者应用程序可以写入数据的目录。

* 函数原型：

```
ret_t fs_get_user_storage_path (fs_t* fs, char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| path | char* | 保存路径。 |
#### fs\_open\_dir 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_open_dir">打开目录。

* 函数原型：

```
fs_dir_t fs_open_dir (fs_t* fs, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fs\_dir\_t | 返回非NULL表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 目录名称。 |
#### fs\_open\_file 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_open_file">打开文件。

* 函数原型：

```
ret_t fs_open_file (fs_t* fs, const char* name, const char* mode);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回非NULL表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 文件名。 |
| mode | const char* | 打开方式，取值请参考POSIX的[fopen函数](https://www.runoob.com/cprogramming/c-function-fopen.html)相应的参数。 |
#### fs\_remove\_dir 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_remove_dir">刪除目录。

* 函数原型：

```
ret_t fs_remove_dir (fs_t* fs, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 目录名称。 |
#### fs\_remove\_file 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_remove_file">刪除文件。

* 函数原型：

```
ret_t fs_remove_file (fs_t* fs, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 文件名。 |
#### fs\_stat 函数
-----------------------

* 函数功能：

> <p id="fs_t_fs_stat">获取文件信息。

* 函数原型：

```
ret_t fs_stat (fs_t* fs, const char* name, fs_stat_info_t* fst);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fs | fs\_t* | 文件系统对象，一般赋值为os\_fs()。 |
| name | const char* | 文件名。 |
| fst | fs\_stat\_info\_t* | 文件状态信息。 |
#### os\_fs 函数
-----------------------

* 函数功能：

> <p id="fs_t_os_fs">获取缺省的文件系统对象。

* 函数原型：

```
fs_t* os_fs ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fs\_t* | 返回文件系统对象。 |
