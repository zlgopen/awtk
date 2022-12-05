## tk\_istream\_stats\_t
### 概述
![image](images/tk_istream_stats_t_0.png)

带统计功能的输入流。
----------------------------------
### 函数
<p id="tk_istream_stats_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_istream_stats_t_tk_istream_stats_create">tk\_istream\_stats\_create</a> | 创建istream对象。 |
#### tk\_istream\_stats\_create 函数
-----------------------

* 函数功能：

> <p id="tk_istream_stats_t_tk_istream_stats_create">创建istream对象。

* 函数原型：

```
tk_istream_t* tk_istream_stats_create (tk_istream_t* impl);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_istream\_t* | 返回istream对象。 |
| impl | tk\_istream\_t* | 被统计的输入流对象 |
