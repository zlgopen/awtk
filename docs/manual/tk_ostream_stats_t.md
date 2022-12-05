## tk\_ostream\_stats\_t
### 概述
![image](images/tk_ostream_stats_t_0.png)

带统计功能的输出流。
----------------------------------
### 函数
<p id="tk_ostream_stats_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_ostream_stats_t_tk_ostream_stats_create">tk\_ostream\_stats\_create</a> | 创建ostream对象。 |
#### tk\_ostream\_stats\_create 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_stats_t_tk_ostream_stats_create">创建ostream对象。

* 函数原型：

```
tk_ostream_t* tk_ostream_stats_create (tk_ostream_t* impl);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_ostream\_t* | 返回ostream对象。 |
| impl | tk\_ostream\_t* | 被统计的输出流对象 |
