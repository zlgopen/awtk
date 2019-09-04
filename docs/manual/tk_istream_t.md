## tk\_istream\_t
### 概述
![image](images/tk_istream_t_0.png)


 input stream interface



----------------------------------
### 函数
<p id="tk_istream_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_istream_t_tk_istream_read">tk\_istream\_read</a> |  |
| <a href="#tk_istream_t_tk_istream_read_len">tk\_istream\_read\_len</a> |  |
| <a href="#tk_istream_t_tk_istream_seek">tk\_istream\_seek</a> |  |
#### tk\_istream\_read 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read">
 读取数据。






* 函数原型：

```
int32_t tk_istream_read (tk_istream_t* stream, uint8_t* buff, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示读取失败，否则返回实际读取数据的长度。 |
| stream | tk\_istream\_t* | istream对象。 |
| buff | uint8\_t* | 返回数据的缓冲区。 |
| max\_size | uint32\_t | 缓冲区的大小。 |
#### tk\_istream\_read\_len 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_len">
 读取指定长度的数据。






* 函数原型：

```
int32_t tk_istream_read_len (tk_istream_t* stream, uint8_t* buff, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示读取失败，否则返回实际读取数据的长度。 |
| stream | tk\_istream\_t* | istream对象。 |
| buff | uint8\_t* | 返回数据的缓冲区。 |
| max\_size | uint32\_t | 缓冲区的大小。 |
#### tk\_istream\_seek 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_seek">
 设置偏移量。






* 函数原型：

```
ret_t tk_istream_seek (tk_istream_t* stream, uint32_t offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| offset | uint32\_t | 偏移量。 |
