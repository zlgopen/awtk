## file\_dialog\_t
### 概述
文件对话框。
----------------------------------
### 函数
<p id="file_dialog_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#file_dialog_t_tk_choose_file">tk\_choose\_file</a> | 选择单个文件，用完后请调用TKMEM_FREE销毁返回的字符串。 |
| <a href="#file_dialog_t_tk_choose_file_for_save">tk\_choose\_file\_for\_save</a> | 选择单个文件用于保存，用完后请调用TKMEM_FREE销毁返回的字符串。 |
| <a href="#file_dialog_t_tk_choose_files">tk\_choose\_files</a> | 多选文件，用完后请调用darray_destroy销毁返回的数组。 |
| <a href="#file_dialog_t_tk_choose_folder">tk\_choose\_folder</a> | 选择单个目录，用完后请调用TKMEM_FREE销毁返回的字符串。 |
#### tk\_choose\_file 函数
-----------------------

* 函数功能：

> <p id="file_dialog_t_tk_choose_file">选择单个文件，用完后请调用TKMEM_FREE销毁返回的字符串。

* 函数原型：

```
char* tk_choose_file (const char* filters, const char* init_dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回NULL表示失败，返回非NULL为文件名。 |
| filters | const char* | 过滤规则(文件扩展名列表,如:".c.cpp.cxx")。 |
| init\_dir | const char* | 初始目录。 |
#### tk\_choose\_file\_for\_save 函数
-----------------------

* 函数功能：

> <p id="file_dialog_t_tk_choose_file_for_save">选择单个文件用于保存，用完后请调用TKMEM_FREE销毁返回的字符串。

* 函数原型：

```
char* tk_choose_file_for_save (const char* filters, const char* init_dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回NULL表示失败，返回非NULL为文件名。 |
| filters | const char* | 过滤规则(文件扩展名列表,如:".c.cpp.cxx")。 |
| init\_dir | const char* | 初始目录。 |
#### tk\_choose\_files 函数
-----------------------

* 函数功能：

> <p id="file_dialog_t_tk_choose_files">多选文件，用完后请调用darray_destroy销毁返回的数组。

* 函数原型：

```
darray_t* tk_choose_files (const char* filters, const char* init_dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | darray\_t* | 返回NULL表示失败，返回非NULL为文件名列表。 |
| filters | const char* | 过滤规则(文件扩展名列表,如:".c.cpp.cxx")。 |
| init\_dir | const char* | 初始目录。 |
#### tk\_choose\_folder 函数
-----------------------

* 函数功能：

> <p id="file_dialog_t_tk_choose_folder">选择单个目录，用完后请调用TKMEM_FREE销毁返回的字符串。

* 函数原型：

```
char* tk_choose_folder (const char* init_dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回NULL表示失败，返回非NULL为文件名。 |
| init\_dir | const char* | 初始目录。 |
