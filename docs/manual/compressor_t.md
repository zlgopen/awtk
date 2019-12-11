## compressor\_t
### 概述
压缩解压接口。
----------------------------------
### 函数
<p id="compressor_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#compressor_t_compressor_compress">compressor\_compress</a> | 压缩数据。 |
| <a href="#compressor_t_compressor_destroy">compressor\_destroy</a> | 销毁compressor对象。 |
| <a href="#compressor_t_compressor_uncompress">compressor\_uncompress</a> | 解压数据。 |
#### compressor\_compress 函数
-----------------------

* 函数功能：

> <p id="compressor_t_compressor_compress">压缩数据。

* 函数原型：

```
ret_t compressor_compress (compressor_t* compressor, const void* data, uint32_t size, wbuffer_t* out);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| compressor | compressor\_t* | compressor对象。 |
| data | const void* | 待压缩的数据。 |
| size | uint32\_t | 数据长度。 |
| out | wbuffer\_t* | 压缩之后的数据。 |
#### compressor\_destroy 函数
-----------------------

* 函数功能：

> <p id="compressor_t_compressor_destroy">销毁compressor对象。

* 函数原型：

```
ret_t compressor_destroy (compressor_t* compressor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| compressor | compressor\_t* | compressor对象。 |
#### compressor\_uncompress 函数
-----------------------

* 函数功能：

> <p id="compressor_t_compressor_uncompress">解压数据。

* 函数原型：

```
ret_t compressor_uncompress (compressor_t* compressor, const void* data, uint32_t size, wbuffer_t* out);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| compressor | compressor\_t* | compressor对象。 |
| data | const void* | 待解压的数据。 |
| size | uint32\_t | 数据长度。 |
| out | wbuffer\_t* | 解压之后的数据。 |
