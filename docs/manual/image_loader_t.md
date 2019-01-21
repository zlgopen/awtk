## image\_loader\_t
### 概述
 图片加载接口。


----------------------------------
### 函数
<p id="image_loader_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#image_loader_t_image_loader_load">image\_loader\_load</a> | 加载图片。 |
#### image\_loader\_load 函数
-----------------------

* 函数功能：

> <p id="image_loader_t_image_loader_load"> 加载图片。



* 函数原型：

```
ret_t image_loader_load (image_loader_t* loader, const asset_info_t* asset, bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| loader | image\_loader\_t* | loader对象。 |
| asset | const asset\_info\_t* | 资源。 |
| bitmap | bitmap\_t* | 用于返回位图对象。 |
