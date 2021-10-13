## vg\_gradient\_t
### 概述
渐变。
----------------------------------
### 函数
<p id="vg_gradient_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#vg_gradient_t_vg_gradient_create_linear">vg\_gradient\_create\_linear</a> | 创建线性渐变。 |
| <a href="#vg_gradient_t_vg_gradient_create_radial">vg\_gradient\_create\_radial</a> | 创建放射渐变。 |
| <a href="#vg_gradient_t_vg_gradient_destroy">vg\_gradient\_destroy</a> | 销毁gradient对象。 |
| <a href="#vg_gradient_t_vg_gradient_init_linear">vg\_gradient\_init\_linear</a> | 初始化线性。 |
| <a href="#vg_gradient_t_vg_gradient_init_radial">vg\_gradient\_init\_radial</a> | 初始化放射渐变。 |
| <a href="#vg_gradient_t_vg_gradient_init_with_gradient">vg\_gradient\_init\_with\_gradient</a> | 初始化矢量画布的渐变对象。 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager">vgcanvas\_asset\_manager</a> | 获取缺省矢量画布资源管理器。 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_add_font">vgcanvas\_asset\_manager\_add\_font</a> | 添加一个字库资源 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_add_image">vgcanvas\_asset\_manager\_add\_image</a> | 添加一个贴图资源 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_add_vg">vgcanvas\_asset\_manager\_add\_vg</a> | 添加一个矢量画布的对象 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_create">vgcanvas\_asset\_manager\_create</a> | 创建矢量画布资源管理器。 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_destroy">vgcanvas\_asset\_manager\_destroy</a> | 释放建矢量画布资源管理器对象 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_get_font_ctx">vgcanvas\_asset\_manager\_get\_font\_ctx</a> | 获取字库的相关上下文 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_get_image_specific">vgcanvas\_asset\_manager\_get\_image\_specific</a> | 获取贴图的特殊相关上下文 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_remove_font">vgcanvas\_asset\_manager\_remove\_font</a> | 移除字库资源 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_remove_image">vgcanvas\_asset\_manager\_remove\_image</a> | 移除贴图资源 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_remove_vg">vgcanvas\_asset\_manager\_remove\_vg</a> | 移除一个矢量画布的对象 |
| <a href="#vg_gradient_t_vgcanvas_asset_manager_set">vgcanvas\_asset\_manager\_set</a> | 设置缺省矢量画布资源管理器。 |
### 属性
<p id="vg_gradient_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#vg_gradient_t_info">info</a> | vg\_gradient\_info\_t | 信息。 |
#### vg\_gradient\_create\_linear 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vg_gradient_create_linear">创建线性渐变。

* 函数原型：

```
vg_gradient_t* vg_gradient_create_linear (float sx, float sy, float ex, float ey);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vg\_gradient\_t* | 返回gradient对象。 |
| sx | float | 起始点x坐标。 |
| sy | float | 起始点y坐标。 |
| ex | float | 结束点x坐标。 |
| ey | float | 结束点y坐标。 |
#### vg\_gradient\_create\_radial 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vg_gradient_create_radial">创建放射渐变。

* 函数原型：

```
vg_gradient_t* vg_gradient_create_radial (float x0, float y0, float r0, float x1, float y1, float r1);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vg\_gradient\_t* | 返回gradient对象。 |
| x0 | float | 起始圆的x坐标。 |
| y0 | float | 起始圆的y坐标。 |
| r0 | float | 起始圆的半径。 |
| x1 | float | 结束圆的x坐标。 |
| y1 | float | 结束圆的y坐标。 |
| r1 | float | 结束圆的半径。 |
#### vg\_gradient\_destroy 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vg_gradient_destroy">销毁gradient对象。

* 函数原型：

```
ret_t vg_gradient_destroy (vg_gradient_t* gradient);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| gradient | vg\_gradient\_t* | gradient对象。 |
#### vg\_gradient\_init\_linear 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vg_gradient_init_linear">初始化线性。

* 函数原型：

```
vg_gradient_t* vg_gradient_init_linear (vg_gradient_t* gradient, float sx, float sy, float ex, float ey);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vg\_gradient\_t* | 返回gradient对象。 |
| gradient | vg\_gradient\_t* | gradient对象。 |
| sx | float | 起始点x坐标。 |
| sy | float | 起始点y坐标。 |
| ex | float | 结束点x坐标。 |
| ey | float | 结束点y坐标。 |
#### vg\_gradient\_init\_radial 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vg_gradient_init_radial">初始化放射渐变。

* 函数原型：

```
vg_gradient_t* vg_gradient_init_radial (vg_gradient_t* gradient, float x0, float y0, float r0, float x1, float y1, float r1);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vg\_gradient\_t* | 返回gradient对象。 |
| gradient | vg\_gradient\_t* | gradient对象。 |
| x0 | float | 起始圆的x坐标。 |
| y0 | float | 起始圆的y坐标。 |
| r0 | float | 起始圆的半径。 |
| x1 | float | 结束圆的x坐标。 |
| y1 | float | 结束圆的y坐标。 |
| r1 | float | 结束圆的半径。 |
#### vg\_gradient\_init\_with\_gradient 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vg_gradient_init_with_gradient">初始化矢量画布的渐变对象。

* 函数原型：

```
vg_gradient_t* vg_gradient_init_with_gradient (vg_gradient_t* gradient, const rect_t* rect, const gradient_t* g);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vg\_gradient\_t* | 返回gradient对象。 |
| gradient | vg\_gradient\_t* | gradient对象。 |
| rect | const rect\_t* | 矩形区域。 |
| g | const gradient\_t* | 渐变对象。 |
#### vgcanvas\_asset\_manager 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager">获取缺省矢量画布资源管理器。

* 函数原型：

```
vgcanvas_asset_manager* vgcanvas_asset_manager ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vgcanvas\_asset\_manager* | 返回矢量画布资源管理器对象。 |
#### vgcanvas\_asset\_manager\_add\_font 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_add_font">添加一个字库资源

* 函数原型：

```
ret_t vgcanvas_asset_manager_add_font (vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, const char* font_name, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
| vg | void* | 使用该字库的矢量画布对象 |
| font\_name | const char* | 字库名称。 |
| ctx | void* | 上下文参数。 |
#### vgcanvas\_asset\_manager\_add\_image 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_add_image">添加一个贴图资源

* 函数原型：

```
ret_t vgcanvas_asset_manager_add_image (vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, bitmap_t* img, void* specific);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
| vg | void* | 使用该字库的矢量画布对象 |
| img | bitmap\_t* | 贴图对象。 |
| specific | void* | 贴图特殊上下文参数。 |
#### vgcanvas\_asset\_manager\_add\_vg 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_add_vg">添加一个矢量画布的对象

* 函数原型：

```
ret_t vgcanvas_asset_manager_add_vg (vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, vgcanvas_asset_manager_bitmap_destroy_t bitmap_destroy, vgcanvas_asset_manager_font_destroy_t font_destroy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
| vg | void* | 矢量画布对象。 |
| bitmap\_destroy | vgcanvas\_asset\_manager\_bitmap\_destroy\_t | 矢量画布对象释放贴图资源的回调函数。 |
| font\_destroy | vgcanvas\_asset\_manager\_font\_destroy\_t | 矢量画布对象释放字库资源的回调函数。 |
#### vgcanvas\_asset\_manager\_create 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_create">创建矢量画布资源管理器。

* 函数原型：

```
vgcanvas_asset_manager_t* vgcanvas_asset_manager_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vgcanvas\_asset\_manager\_t* | 返回矢量画布资源管理器对象。 |
#### vgcanvas\_asset\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_destroy">释放建矢量画布资源管理器对象

* 函数原型：

```
ret_t vgcanvas_asset_manager_destroy (vgcanvas_asset_manager_t* vgcanvas_asset_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
#### vgcanvas\_asset\_manager\_get\_font\_ctx 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_get_font_ctx">获取字库的相关上下文

* 函数原型：

```
const void* vgcanvas_asset_manager_get_font_ctx (vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, const char* font_name, ret_t* reuslt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const void* | 返回上下文。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
| vg | void* | 使用该字库的矢量画布对象 |
| font\_name | const char* | 字库名称。 |
| reuslt | ret\_t* | 返回获取结果是否成功，成功返回 RET\_OK，否则表示失败。 |
#### vgcanvas\_asset\_manager\_get\_image\_specific 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_get_image_specific">获取贴图的特殊相关上下文

* 函数原型：

```
const void* vgcanvas_asset_manager_get_image_specific (vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, bitmap_t* img, ret_t* reuslt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const void* | 返回特殊上下文。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
| vg | void* | 使用该字库的矢量画布对象 |
| img | bitmap\_t* | 贴图对象。 |
| reuslt | ret\_t* | 返回获取结果是否成功，成功返回 RET\_OK，否则表示失败。 |
#### vgcanvas\_asset\_manager\_remove\_font 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_remove_font">移除字库资源
备注：如果字库名字为 NULL，则移除该 vg 对象下的所有字库。

* 函数原型：

```
ret_t vgcanvas_asset_manager_remove_font (vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, const char* font_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
| vg | void* | 使用该字库的矢量画布对象 |
| font\_name | const char* | 字库名称。 |
#### vgcanvas\_asset\_manager\_remove\_image 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_remove_image">移除贴图资源
备注：如果 bitmap 对象为 NULL，则移除该 vg 对象下的所有贴图。

* 函数原型：

```
ret_t vgcanvas_asset_manager_remove_image (vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg, bitmap_t* img);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
| vg | void* | 使用该字库的矢量画布对象 |
| img | bitmap\_t* | 贴图对象。 |
#### vgcanvas\_asset\_manager\_remove\_vg 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_remove_vg">移除一个矢量画布的对象

* 函数原型：

```
ret_t vgcanvas_asset_manager_remove_vg (vgcanvas_asset_manager_t* vgcanvas_asset_manager, void* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器。 |
| vg | void* | 矢量画布对象。 |
#### vgcanvas\_asset\_manager\_set 函数
-----------------------

* 函数功能：

> <p id="vg_gradient_t_vgcanvas_asset_manager_set">设置缺省矢量画布资源管理器。

* 函数原型：

```
ret_t vgcanvas_asset_manager_set (vgcanvas_asset_manager_t* vgcanvas_asset_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vgcanvas\_asset\_manager | vgcanvas\_asset\_manager\_t* | 矢量画布资源管理器对象。 |
#### info 属性
-----------------------
> <p id="vg_gradient_t_info">信息。

* 类型：vg\_gradient\_info\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
