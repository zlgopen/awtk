## tk\_iostream\_t
### 概述
![image](images/tk_iostream_t_0.png)


 input/output stream interface


----------------------------------
### 函数
<p id="tk_iostream_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_iostream_t_tk_iostream_get_istream">tk\_iostream\_get\_istream</a> |  |
| <a href="#tk_iostream_t_tk_iostream_get_ostream">tk\_iostream\_get\_ostream</a> |  |
#### tk\_iostream\_get\_istream 函数
-----------------------

* 函数功能：

> <p id="tk_iostream_t_tk_iostream_get_istream">
 获取输入流对象。





* 函数原型：

```
tk_istream_t* tk_iostream_get_istream (tk_iostream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_istream\_t* | 返回输入流对象。 |
| stream | tk\_iostream\_t* | iostream对象。 |
#### tk\_iostream\_get\_ostream 函数
-----------------------

* 函数功能：

> <p id="tk_iostream_t_tk_iostream_get_ostream">
 获取输出流对象。





* 函数原型：

```
tk_ostream_t* tk_iostream_get_ostream (tk_iostream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_ostream\_t* | 返回输出流对象。 |
| stream | tk\_iostream\_t* | iostream对象。 |
