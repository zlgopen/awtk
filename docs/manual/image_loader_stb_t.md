## image\_loader\_stb\_t
### 概述
![image](images/image_loader_stb_t_0.png)

stb图片加载器。

stb主要用于加载jpg/png/gif等格式的图片，它功能强大，体积小巧。
----------------------------------
### 函数
<p id="image_loader_stb_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#image_loader_stb_t_image_loader_stb">image\_loader\_stb</a> | 获取stb图片加载器对象。 |
| <a href="#image_loader_stb_t_stb_load_image">stb\_load\_image</a> | 加载图片。 |
#### image\_loader\_stb 函数
-----------------------

* 函数功能：

> <p id="image_loader_stb_t_image_loader_stb">获取stb图片加载器对象。

* 函数原型：

```
image_loader_t* image_loader_stb ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | image\_loader\_t* | 返回图片加载器对象。 |
#### stb\_load\_image 函数
-----------------------

* 函数功能：

> <p id="image_loader_stb_t_stb_load_image">加载图片。

* 函数原型：

```
ret_t stb_load_image (int32_t subtype, const uint8_t* buff, uint32_t buff_size, bitmap_t* image, bitmap_format_t transparent_bitmap_format, bitmap_format_t opaque_bitmap_format, lcd_orientation_t o);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| subtype | int32\_t | 资源类型。 |
| buff | const uint8\_t* | 资源数据。 |
| buff\_size | uint32\_t | 资源数据长度。 |
| image | bitmap\_t* | image 对象。 |
| transparent\_bitmap\_format | bitmap\_format\_t | 带透明通道的位图格式（只能 BITMAP\_FMT\_RGBA8888 和 BITMAP\_FMT\_RGBA8888 二选一，其他类型默认都为 BITMAP\_FMT\_RGBA8888） |
| opaque\_bitmap\_format | bitmap\_format\_t | 不透明位图格式（暂时支持 BITMAP\_FMT\_RGBA8888，BITMAP\_FMT\_RGBA8888，16 位色和 24 位色以及 mono 格式） |
| o | lcd\_orientation\_t | 旋转方向 |
