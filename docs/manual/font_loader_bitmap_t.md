## font\_loader\_bitmap\_t
### 概述
![image](images/font_loader_bitmap_t_0.png)

bitmap字体加载器。

bitmap字体是用工具预先生成位图字体数据，优势是速度快，不占用内存，缺点是字体大小固定。
适合低端嵌入式平台。

tools/font_gen用于把矢量字体(如truetype)转换成位图字体。
----------------------------------
### 函数
<p id="font_loader_bitmap_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#font_loader_bitmap_t_font_loader_bitmap">font\_loader\_bitmap</a> | 获取bitmap字体加载器对象。 |
#### font\_loader\_bitmap 函数
-----------------------

* 函数功能：

> <p id="font_loader_bitmap_t_font_loader_bitmap">获取bitmap字体加载器对象。

* 函数原型：

```
font_loader_t* font_loader_bitmap ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_loader\_t* | 返回字体加载器对象。 |
