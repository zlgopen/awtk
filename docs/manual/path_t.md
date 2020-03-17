## path\_t
### 概述
路径相关的工具函数。
----------------------------------
### 函数
<p id="path_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#path_t_path_abs">path\_abs</a> | 返回绝对路径。 |
| <a href="#path_t_path_app_root">path\_app\_root</a> | 获取app所在目录。 |
| <a href="#path_t_path_basename">path\_basename</a> | 返回文件名。 |
| <a href="#path_t_path_build">path\_build</a> | 构造路径。 |
| <a href="#path_t_path_cwd">path\_cwd</a> | 获取当前所在目录。 |
| <a href="#path_t_path_dirname">path\_dirname</a> | 返回目录。 |
| <a href="#path_t_path_exe">path\_exe</a> | 获取可执行文件所在目录。 |
| <a href="#path_t_path_exist">path\_exist</a> | 判断目录是否存在。 |
| <a href="#path_t_path_extname">path\_extname</a> | 返回文件扩展名。 |
| <a href="#path_t_path_is_abs">path\_is\_abs</a> | 判断路径是否为绝对路径。 |
| <a href="#path_t_path_normalize">path\_normalize</a> | 规范路径字符形式。 |
| <a href="#path_t_path_replace_basename">path\_replace\_basename</a> | 替换文件名。 |
| <a href="#path_t_path_replace_extname">path\_replace\_extname</a> | 替换文件扩展名。 |
#### path\_abs 函数
-----------------------

* 函数功能：

> <p id="path_t_path_abs">返回绝对路径。

* 函数原型：

```
ret_t path_abs (const char* path* path, char* result, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* path* | 路径。 |
| result | char* | 用于返回绝对路径。 |
| size | uint32\_t | 缓冲区大小。 |
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
#### path\_basename 函数
-----------------------

* 函数功能：

> <p id="path_t_path_basename">返回文件名。

* 函数原型：

```
ret_t path_basename (const char* path* path, char* result, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* path* | 路径。 |
| result | char* | 用于返回文件名。 |
| size | uint32\_t | 缓冲区大小。 |
#### path\_build 函数
-----------------------

* 函数功能：

> <p id="path_t_path_build">构造路径。

> 可变参数为字符串，以NULL参数结束。

* 函数原型：

```
ret_t path_build (char* result, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| result | char* | 用于返回路径。 |
| size | uint32\_t | 缓冲区大小。 |
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
ret_t path_dirname (const char* path* path, char* result, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* path* | 路径。 |
| result | char* | 用于返回目录。 |
| size | uint32\_t | 缓冲区大小。 |
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
ret_t path_exist (const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回TRUE表示成功，否则表示失败。 |
| path | const char* | 目录。 |
#### path\_extname 函数
-----------------------

* 函数功能：

> <p id="path_t_path_extname">返回文件扩展名。

* 函数原型：

```
ret_t path_extname (const char* path* path, char* result, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* path* | 路径。 |
| result | char* | 用于返回文件扩展名。 |
| size | uint32\_t | 缓冲区大小。 |
#### path\_is\_abs 函数
-----------------------

* 函数功能：

> <p id="path_t_path_is_abs">判断路径是否为绝对路径。

* 函数原型：

```
ret_t path_is_abs (const char* path* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回FALSE表示不是绝对路径，否则表示是绝对路径。 |
| path | const char* path* | 路径。 |
#### path\_normalize 函数
-----------------------

* 函数功能：

> <p id="path_t_path_normalize">规范路径字符形式。

* 函数原型：

```
ret_t path_normalize (const char* path* path, char* result, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| path | const char* path* | 路径。 |
| result | char* | 用于返回规范后的路径。 |
| size | uint32\_t | 缓冲区大小。 |
#### path\_replace\_basename 函数
-----------------------

* 函数功能：

> <p id="path_t_path_replace_basename">替换文件名。

* 函数原型：

```
ret_t path_replace_basename (char* result, uint32_t size, char* filename, char* basename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| result | char* | 用于返回结果。 |
| size | uint32\_t | 缓冲区大小。 |
| filename | char* | 原始文件路径。 |
| basename | char* | 替换后的文件名。 |
#### path\_replace\_extname 函数
-----------------------

* 函数功能：

> <p id="path_t_path_replace_extname">替换文件扩展名。

* 函数原型：

```
ret_t path_replace_extname (char* result, uint32_t size, char* filename, char* extname);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| result | char* | 用于返回结果。 |
| size | uint32\_t | 缓冲区大小。 |
| filename | char* | 原始文件路径。 |
| extname | char* | 替换后的文件扩展名。 |
