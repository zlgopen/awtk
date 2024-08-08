## zip\_file\_t
### 概述
zip文件。
----------------------------------
### 函数
<p id="zip_file_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#zip_file_t_zip_file_extract">zip\_file\_extract</a> | 解压zip文件。 |
#### zip\_file\_extract 函数
-----------------------

* 函数功能：

> <p id="zip_file_t_zip_file_extract">解压zip文件。

* 函数原型：

```
ret_t zip_file_extract (const char* zipfile, const char* dst_dir, const char* password);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| zipfile | const char* | zip文件名。 |
| dst\_dir | const char* | 目录。 |
| password | const char* | 密码(可选) |
