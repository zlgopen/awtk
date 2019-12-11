## tk\_istream\_file\_t
### 概述
![image](images/tk_istream_file_t_0.png)

基于文件实现的输入流。
----------------------------------
### 函数
<p id="tk_istream_file_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_istream_file_t_tk_istream_file_create">tk\_istream\_file\_create</a> | 创建istream对象。 |
#### tk\_istream\_file\_create 函数
-----------------------

* 函数功能：

> <p id="tk_istream_file_t_tk_istream_file_create">创建istream对象。

* 函数原型：

```
tk_istream_t* tk_istream_file_create (const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_istream\_t* | 返回istream对象。 |
| filename | const char* | 文件名。 |
