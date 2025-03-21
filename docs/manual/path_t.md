## path\_t
### 概述
路径相关的工具函数。
----------------------------------
### 函数
<p id="path_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#path_t_path_abs">path\_abs</a> | 返回绝对路径。 |
| <a href="#path_t_path_abs_normalize">path\_abs\_normalize</a> | 将相对路径转换为绝对路径并规范路径字符形式。 |
| <a href="#path_t_path_abs_normalize_with_root">path\_abs\_normalize\_with\_root</a> | 将相对路径转换为绝对路径并规范路径字符形式。 |
| <a href="#path_t_path_app_root">path\_app\_root</a> | 获取app所在目录。 |
| <a href="#path_t_path_app_root_ex">path\_app\_root\_ex</a> | 获取app所在目录，并追加subpath到后面。 |
| <a href="#path_t_path_basename">path\_basename</a> | 返回文件名。 |
| <a href="#path_t_path_basename_ex">path\_basename\_ex</a> | 返回文件名。 |
| <a href="#path_t_path_build">path\_build</a> | 构造路径。 |
| <a href="#path_t_path_cwd">path\_cwd</a> | 获取当前所在目录。 |
| <a href="#path_t_path_dirname">path\_dirname</a> | 返回目录。 |
| <a href="#path_t_path_exe">path\_exe</a> | 获取可执行文件所在目录。 |
| <a href="#path_t_path_exist">path\_exist</a> | 判断目录是否存在。 |
| <a href="#path_t_path_expand_vars">path\_expand\_vars</a> | 将路径中的变量展开。 |
| <a href="#path_t_path_extname">path\_extname</a> | 返回文件扩展名。 |
| <a href="#path_t_path_extname_is">path\_extname\_is</a> | ```c |
| <a href="#path_t_path_is_abs">path\_is\_abs</a> | 判断路径是否为绝对路径。 |
| <a href="#path_t_path_normalize">path\_normalize</a> | 规范路径字符形式。 |
| <a href="#path_t_path_prepend_app_root">path\_prepend\_app\_root</a> | 将前面路径加上app root。 |
| <a href="#path_t_path_prepend_temp_path">path\_prepend\_temp\_path</a> | 将前面路径加上临时文件目录。 |
| <a href="#path_t_path_prepend_user_storage_path">path\_prepend\_user\_storage\_path</a> | 将前面路径加上用户目录。 |
| <a href="#path_t_path_remove_last_slash">path\_remove\_last\_slash</a> | 去掉后面的/和\\字符。 |
| <a href="#path_t_path_replace_basename">path\_replace\_basename</a> | 替换文件名。 |
| <a href="#path_t_path_replace_extname">path\_replace\_extname</a> | 替换文件扩展名。 |
#### path\_abs 函数
-----------------------

* 函数功能：

> <p id="path_t_path_abs">返回绝对路径。

* 函数原型：

```
ret_t path_abs (const char* path, char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* | 路径。 |
| result | char* | 用于返回绝对路径。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_abs\_normalize 函数
-----------------------

* 函数功能：

> <p id="path_t_path_abs_normalize">将相对路径转换为绝对路径并规范路径字符形式。

* 函数原型：

```
ret_t path_abs_normalize (const char* filename, char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| filename | const char* | 相对路径。 |
| result | char* | 用于返回绝对路径。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_abs\_normalize\_with\_root 函数
-----------------------

* 函数功能：

> <p id="path_t_path_abs_normalize_with_root">将相对路径转换为绝对路径并规范路径字符形式。

* 函数原型：

```
const char* path_abs_normalize_with_root (const char* root, const char* rel_filename, char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回绝对路径。 |
| root | const char* | 根目录。 |
| rel\_filename | const char* | 相对路径。 |
| filename | char* | 用于返回绝对路径。 |
#### path\_app\_root 函数
-----------------------

* 函数功能：

> <p id="path_t_path_app_root">获取app所在目录。

* 函数原型：

```
ret_t path_app_root (char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | char* | 保存app所在目录。 |
#### path\_app\_root\_ex 函数
-----------------------

* 函数功能：

> <p id="path_t_path_app_root_ex">获取app所在目录，并追加subpath到后面。

* 函数原型：

```
ret_t path_app_root_ex (char* path, const char* subpath);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | char* | 保存app所在目录。 |
| subpath | const char* | 子目录。 |
#### path\_basename 函数
-----------------------

* 函数功能：

> <p id="path_t_path_basename">返回文件名。

* 函数原型：

```
ret_t path_basename (const char* path, char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* | 路径。 |
| result | char* | 用于返回文件名。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_basename\_ex 函数
-----------------------

* 函数功能：

> <p id="path_t_path_basename_ex">返回文件名。

* 函数原型：

```
ret_t path_basename_ex (const char* path, bool_t remove_ext_name, char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* | 路径。 |
| remove\_ext\_name | bool\_t | 是否去掉扩展名。 |
| result | char* | 用于返回文件名。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_build 函数
-----------------------

* 函数功能：

> <p id="path_t_path_build">构造路径。

> 可变参数为字符串，以NULL参数结束。

* 函数原型：

```
ret_t path_build (char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| result | char* | 用于返回路径。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_cwd 函数
-----------------------

* 函数功能：

> <p id="path_t_path_cwd">获取当前所在目录。

* 函数原型：

```
ret_t path_cwd (char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | char* | 保存当前所在目录的路径。 |
#### path\_dirname 函数
-----------------------

* 函数功能：

> <p id="path_t_path_dirname">返回目录。

* 函数原型：

```
ret_t path_dirname (const char* path, char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* | 路径。 |
| result | char* | 用于返回目录。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_exe 函数
-----------------------

* 函数功能：

> <p id="path_t_path_exe">获取可执行文件所在目录。

* 函数原型：

```
ret_t path_exe (char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | char* | 保存可执行文件所在目录。 |
#### path\_exist 函数
-----------------------

* 函数功能：

> <p id="path_t_path_exist">判断目录是否存在。

* 函数原型：

```
bool_t path_exist (const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示成功，否则表示失败。 |
| path | const char* | 目录。 |
#### path\_expand\_vars 函数
-----------------------

* 函数功能：

> <p id="path_t_path_expand_vars">将路径中的变量展开。

```
${app_dir} -> app 文件目录。
${temp_dir} -> 临时文件目录。
${user_dir} -> 用户目录。
```

* 函数原型：

```
ret_t path_expand_vars (const char* filename, char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| filename | const char* | 相对路径。 |
| result | char* | 用于返回结果。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_extname 函数
-----------------------

* 函数功能：

> <p id="path_t_path_extname">返回文件扩展名。

* 函数原型：

```
ret_t path_extname (const char* path, char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* | 路径。 |
| result | char* | 用于返回文件扩展名。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_extname\_is 函数
-----------------------

* 函数功能：

> <p id="path_t_path_extname_is">```c
assert(path_extname_is("test.jpg", ".jpg"));
assert(path_extname_is("test.JPG", ".jpg"));
```
检查是否是指定的扩展名。

* 函数原型：

```
bool_t path_extname_is (const char* path, const char* extname);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示是，否则表示不是。 |
| path | const char* | 路径。 |
| extname | const char* | 扩展名。 |
#### path\_is\_abs 函数
-----------------------

* 函数功能：

> <p id="path_t_path_is_abs">判断路径是否为绝对路径。

* 函数原型：

```
bool_t path_is_abs (const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回FALSE表示不是绝对路径，否则表示是绝对路径。 |
| path | const char* | 路径。 |
#### path\_normalize 函数
-----------------------

* 函数功能：

> <p id="path_t_path_normalize">规范路径字符形式。

* 函数原型：

```
ret_t path_normalize (const char* path, char* result, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* | 路径。 |
| result | char* | 用于返回规范后的路径。 |
| size | int32\_t | 缓冲区大小。 |
#### path\_prepend\_app\_root 函数
-----------------------

* 函数功能：

> <p id="path_t_path_prepend_app_root">将前面路径加上app root。

* 函数原型：

```
const char* path_prepend_app_root (char* full_path, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回完整路径。 |
| full\_path | char* | 用于返回完整路径。 |
| path | const char* | 路径。 |
#### path\_prepend\_temp\_path 函数
-----------------------

* 函数功能：

> <p id="path_t_path_prepend_temp_path">将前面路径加上临时文件目录。

* 函数原型：

```
const char* path_prepend_temp_path (char* full_path, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回完整路径。 |
| full\_path | char* | 用于返回完整路径。 |
| path | const char* | 路径。 |
#### path\_prepend\_user\_storage\_path 函数
-----------------------

* 函数功能：

> <p id="path_t_path_prepend_user_storage_path">将前面路径加上用户目录。

* 函数原型：

```
const char* path_prepend_user_storage_path (char* full_path, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回完整路径。 |
| full\_path | char* | 用于返回完整路径。 |
| path | const char* | 路径。 |
#### path\_remove\_last\_slash 函数
-----------------------

* 函数功能：

> <p id="path_t_path_remove_last_slash">去掉后面的/和\\字符。

* 函数原型：

```
ret_t path_remove_last_slash (char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | char* | 目录。 |
#### path\_replace\_basename 函数
-----------------------

* 函数功能：

> <p id="path_t_path_replace_basename">替换文件名。

* 函数原型：

```
ret_t path_replace_basename (char* result, int32_t size, const char* filename, const char* basename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| result | char* | 用于返回结果。 |
| size | int32\_t | 缓冲区大小。 |
| filename | const char* | 原始文件路径。 |
| basename | const char* | 替换后的文件名。 |
#### path\_replace\_extname 函数
-----------------------

* 函数功能：

> <p id="path_t_path_replace_extname">替换文件扩展名。

* 函数原型：

```
