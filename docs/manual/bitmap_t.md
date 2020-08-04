## bitmap\_t
### 概述
位图。
----------------------------------
### 函数
<p id="bitmap_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#bitmap_t_bitmap_create">bitmap\_create</a> | 创建图片对象(一般供脚本语言中使用)。 |
| <a href="#bitmap_t_bitmap_create_ex">bitmap\_create\_ex</a> | 创建图片对象。 |
| <a href="#bitmap_t_bitmap_destroy">bitmap\_destroy</a> | 销毁图片。 |
| <a href="#bitmap_t_bitmap_get_bpp">bitmap\_get\_bpp</a> | 获取图片一个像素占用的字节数。 |
| <a href="#bitmap_t_bitmap_get_bpp_of_format">bitmap\_get\_bpp\_of\_format</a> | 获取位图格式对应的颜色位数。 |
| <a href="#bitmap_t_bitmap_get_line_length">bitmap\_get\_line\_length</a> | 获取每一行占用内存的字节数。 |
| <a href="#bitmap_t_bitmap_get_pixel">bitmap\_get\_pixel</a> | 获取图片指定像素的rgba颜色值(主要用于测试程序)。 |
| <a href="#bitmap_t_bitmap_init">bitmap\_init</a> | 初始化图片。 |
| <a href="#bitmap_t_bitmap_init_from_rgba">bitmap\_init\_from\_rgba</a> | 初始化图片。 |
| <a href="#bitmap_t_bitmap_lock_buffer_for_read">bitmap\_lock\_buffer\_for\_read</a> | 为读取数据而锁定bitmap的图片缓冲区。 |
| <a href="#bitmap_t_bitmap_lock_buffer_for_write">bitmap\_lock\_buffer\_for\_write</a> | 为修改数据而锁定bitmap的图片缓冲区。 |
| <a href="#bitmap_t_bitmap_mono_dump">bitmap\_mono\_dump</a> | dump mono bitmap。 |
| <a href="#bitmap_t_bitmap_save_png">bitmap\_save\_png</a> | 把bitmap保存为png。 |
| <a href="#bitmap_t_bitmap_set_line_length">bitmap\_set\_line\_length</a> | 设置line_length。 |
| <a href="#bitmap_t_bitmap_unlock_buffer">bitmap\_unlock\_buffer</a> | 解锁图像缓冲区。 |
### 属性
<p id="bitmap_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#bitmap_t_buffer">buffer</a> | graphic\_buffer\_t* | 图片数据。 |
| <a href="#bitmap_t_flags">flags</a> | uint16\_t | 标志。请参考{bitmap_flag_t}。 |
| <a href="#bitmap_t_format">format</a> | uint16\_t | 格式。请参考{bitmap_format_t}。 |
| <a href="#bitmap_t_h">h</a> | wh\_t | 高度。 |
| <a href="#bitmap_t_line_length">line\_length</a> | uint32\_t | 每一行实际占用的内存(也称为stride或pitch)，一般情况下为w*bpp。 |
| <a href="#bitmap_t_name">name</a> | const char* | 名称。 |
| <a href="#bitmap_t_w">w</a> | wh\_t | 宽度。 |
#### bitmap\_create 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_create">创建图片对象(一般供脚本语言中使用)。

* 函数原型：

```
bitmap_t* bitmap_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bitmap\_t* | 返回bitmap对象。 |
#### bitmap\_create\_ex 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_create_ex">创建图片对象。

* 函数原型：

```
bitmap_t* bitmap_create_ex (uint32_t w, uint32_t h, uint32_t line_length, bitmap_format_t format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bitmap\_t* | 返回bitmap对象。 |
| w | uint32\_t | 宽度。 |
| h | uint32\_t | 高度。 |
| line\_length | uint32\_t | line\_length。 |
| format | bitmap\_format\_t | 格式。 |
#### bitmap\_destroy 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_destroy">销毁图片。

* 函数原型：

```
ret_t bitmap_destroy (bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bitmap | bitmap\_t* | bitmap对象。 |
#### bitmap\_get\_bpp 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_get_bpp">获取图片一个像素占用的字节数。

* 函数原型：

```
uint32_t bitmap_get_bpp (bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回一个像素占用的字节数。 |
| bitmap | bitmap\_t* | bitmap对象。 |
#### bitmap\_get\_bpp\_of\_format 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_get_bpp_of_format">获取位图格式对应的颜色位数。

* 函数原型：

```
uint32_t bitmap_get_bpp_of_format (bitmap_format_t format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 成功返回颜色位数，失败返回0。 |
| format | bitmap\_format\_t | 位图格式。 |
#### bitmap\_get\_line\_length 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_get_line_length">获取每一行占用内存的字节数。

* 函数原型：

```
ret_t bitmap_get_line_length (bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回每一行占用内存的字节数。 |
| bitmap | bitmap\_t* | bitmap对象。 |
#### bitmap\_get\_pixel 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_get_pixel">获取图片指定像素的rgba颜色值(主要用于测试程序)。

* 函数原型：

```
ret_t bitmap_get_pixel (bitmap_t* bitmap, uint32_t x, uint32_t y, rgba_t* rgba);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bitmap | bitmap\_t* | bitmap对象。 |
| x | uint32\_t | x坐标。 |
| y | uint32\_t | y坐标。 |
| rgba | rgba\_t* | 返回颜色值。 |
#### bitmap\_init 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_init">初始化图片。

* 函数原型：

```
ret_t bitmap_init (bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format, const uint8_t* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bitmap | bitmap\_t* | bitmap对象。 |
| w | uint32\_t | 宽度。 |
| h | uint32\_t | 高度。 |
| format | bitmap\_format\_t | 格式。 |
| data | const uint8\_t* | 数据，直接引用，但不负责释放。如果为空，由内部自动分配和释放。 |
#### bitmap\_init\_from\_rgba 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_init_from_rgba">初始化图片。
数据。3通道时为RGB888格式，4通道时为RGBA888格式(内部拷贝该数据，不会引用，调用者自行释放)。

* 函数原型：

```
ret_t bitmap_init_from_rgba (bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format, const uint8_t* , uint32_t comp);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bitmap | bitmap\_t* | bitmap对象。 |
| w | uint32\_t | 宽度。 |
| h | uint32\_t | 高度。 |
| format | bitmap\_format\_t | 格式。 |
|  | const uint8\_t* | a |
| comp | uint32\_t | 颜色通道数(目前支持3(rgb)和4(rgba))。 |
#### bitmap\_lock\_buffer\_for\_read 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_lock_buffer_for_read">为读取数据而锁定bitmap的图片缓冲区。

* 函数原型：

```
uint8_t* bitmap_lock_buffer_for_read (bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t* | 返回缓存区的首地址。 |
| bitmap | bitmap\_t* | bitmap对象。 |
#### bitmap\_lock\_buffer\_for\_write 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_lock_buffer_for_write">为修改数据而锁定bitmap的图片缓冲区。

* 函数原型：

```
uint8_t* bitmap_lock_buffer_for_write (bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t* | 返回缓存区的首地址。 |
| bitmap | bitmap\_t* | bitmap对象。 |
#### bitmap\_mono\_dump 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_mono_dump">dump mono bitmap。

* 函数原型：

```
ret_t bitmap_mono_dump (const uint8_t* buff, uint32_t w, uint32_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| buff | const uint8\_t* | 数据。 |
| w | uint32\_t | 宽度。 |
| h | uint32\_t | 高度。 |
#### bitmap\_save\_png 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_save_png">把bitmap保存为png。

* 函数原型：

```
bool_t bitmap_save_png (bitmap_t* bitmap, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示成功，FALSE表示失败。 |
| bitmap | bitmap\_t* | bitmap对象。 |
| filename | const char* | 文件名。 |
#### bitmap\_set\_line\_length 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_set_line_length">设置line_length。

* 函数原型：

```
ret_t bitmap_set_line_length (bitmap_t* bitmap, uint32_t line_length);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bitmap | bitmap\_t* | bitmap对象。 |
| line\_length | uint32\_t | line\_length。 |
#### bitmap\_unlock\_buffer 函数
-----------------------

* 函数功能：

> <p id="bitmap_t_bitmap_unlock_buffer">解锁图像缓冲区。

* 函数原型：

```
ret_t bitmap_unlock_buffer (bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bitmap | bitmap\_t* | bitmap对象。 |
#### buffer 属性
-----------------------
> <p id="bitmap_t_buffer">图片数据。

* 类型：graphic\_buffer\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### flags 属性
-----------------------
> <p id="bitmap_t_flags">标志。请参考{bitmap_flag_t}。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### format 属性
-----------------------
> <p id="bitmap_t_format">格式。请参考{bitmap_format_t}。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### h 属性
-----------------------
> <p id="bitmap_t_h">高度。

* 类型：wh\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### line\_length 属性
-----------------------
> <p id="bitmap_t_line_length">每一行实际占用的内存(也称为stride或pitch)，一般情况下为w*bpp。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### name 属性
-----------------------
> <p id="bitmap_t_name">名称。

* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### w 属性
-----------------------
> <p id="bitmap_t_w">宽度。

* 类型：wh\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
