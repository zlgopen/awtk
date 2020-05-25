## bsvg\_t
### 概述
SVG的二进制格式。
----------------------------------
### 函数
<p id="bsvg_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#bsvg_t_bsvg_init">bsvg\_init</a> | 初始化bsvg对象。 |
#### bsvg\_init 函数
-----------------------

* 函数功能：

> <p id="bsvg_t_bsvg_init">初始化bsvg对象。

* 函数原型：

```
bsvg_t* bsvg_init (bsvg_t* , const uint32_t* data, size size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bsvg\_t* | 返回解析后的svg。 |
|  | bsvg\_t* | 。 |
| data | const uint32\_t* | svg数据。 |
| size | size | data长度。 |
