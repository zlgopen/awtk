## data\_reader\_file\_t
### 概述
![image](images/data_reader_file_t_0.png)

基于文件实现的 data_reader。通过 data_reader_factory 创建 reader 时，URL的格式如下(请用函数data_reader_file_build_url生成)：

```
file://filename
```
----------------------------------
### 函数
<p id="data_reader_file_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#data_reader_file_t_data_reader_file_create">data\_reader\_file\_create</a> | 创建基于文件的data reader。 |
#### data\_reader\_file\_create 函数
-----------------------

* 函数功能：

> <p id="data_reader_file_t_data_reader_file_create">创建基于文件的data reader。
> 不要直接调用，而是注册到 data\_reader\_factory后，通过data\_reader\_factory调用。

* 函数原型：

```
data_reader_t* data_reader_file_create (const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_reader\_t* | 返回data reader对象。 |
| filename | const char* | 文件名。 |
