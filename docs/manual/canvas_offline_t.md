## canvas\_offline\_t
### 概述
离线画布 canvas。
----------------------------------
### 函数
<p id="canvas_offline_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#canvas_offline_t_canvas_offline_begin_draw">canvas\_offline\_begin\_draw</a> | 设置离线 canvas 开始绘图 |
| <a href="#canvas_offline_t_canvas_offline_create">canvas\_offline\_create</a> | 创建一个离线的 canvas |
| <a href="#canvas_offline_t_canvas_offline_destroy">canvas\_offline\_destroy</a> | 释放离线 canvas 和离线 bitmap |
| <a href="#canvas_offline_t_canvas_offline_end_draw">canvas\_offline\_end\_draw</a> | 设置离线 canvas 结束绘图 |
| <a href="#canvas_offline_t_canvas_offline_flush_bitmap">canvas\_offline\_flush\_bitmap</a> | 把离线 canvas 的数据放到绑定的 bitmap 中 |
| <a href="#canvas_offline_t_canvas_offline_get_bitmap">canvas\_offline\_get\_bitmap</a> | 获取离线 canvas 的离线 bitmap。 |
### 属性
<p id="canvas_offline_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#canvas_offline_t_bitmap">bitmap</a> | bitmap\_t* | 绑定的离线 bitmap |
#### canvas\_offline\_begin\_draw 函数
-----------------------

* 函数功能：

> <p id="canvas_offline_t_canvas_offline_begin_draw">设置离线 canvas 开始绘图

* 函数原型：

```
ret_t canvas_offline_begin_draw (canvas_t* canvas);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| canvas | canvas\_t* | 离线 canvas 对象。 |
#### canvas\_offline\_create 函数
-----------------------

* 函数功能：

> <p id="canvas_offline_t_canvas_offline_create">创建一个离线的 canvas
在 opengl 模式下 format 参数只能为 BITMAP_FMT_RGBA8888
在其他模式下，离线 canvas 格式可以为 rgba，bgar，bgr565和rgb565

* 函数原型：

```
canvas_t* canvas_offline_create (uint32_t w, uint32_t h, bitmap_format_t format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | canvas\_t* | 成功返回 canvas ，失败返回 NULL。 |
| w | uint32\_t | 离线 canvas 的宽。 |
| h | uint32\_t | 离线 canvas 的高。 |
| format | bitmap\_format\_t | 离线 canvas 的格式。 |
#### canvas\_offline\_destroy 函数
-----------------------

* 函数功能：

> <p id="canvas_offline_t_canvas_offline_destroy">释放离线 canvas 和离线 bitmap

* 函数原型：

```
ret_t canvas_offline_destroy (canvas_t* canvas);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| canvas | canvas\_t* | 离线 canvas 对象。 |
#### canvas\_offline\_end\_draw 函数
-----------------------

* 函数功能：

> <p id="canvas_offline_t_canvas_offline_end_draw">设置离线 canvas 结束绘图

* 函数原型：

```
ret_t canvas_offline_end_draw (canvas_t* canvas);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| canvas | canvas\_t* | 离线 canvas 对象。 |
#### canvas\_offline\_flush\_bitmap 函数
-----------------------

* 函数功能：

> <p id="canvas_offline_t_canvas_offline_flush_bitmap">把离线 canvas 的数据放到绑定的 bitmap 中
该函数只有在 opengl 模式才需要调用，是否把显存中的数据回传到内存中。

* 函数原型：

```
ret_t canvas_offline_flush_bitmap (canvas_t* canvas);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| canvas | canvas\_t* | 离线 canvas 对象。 |
#### canvas\_offline\_get\_bitmap 函数
-----------------------

* 函数功能：

> <p id="canvas_offline_t_canvas_offline_get_bitmap">获取离线 canvas 的离线 bitmap。

* 函数原型：

```
bitmap_t* canvas_offline_get_bitmap (canvas_t* canvas);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bitmap\_t* | 返回 bitmap\_t 对象表示成功，返回 NULL 表示失败。 |
| canvas | canvas\_t* | 离线 canvas 对象。 |
#### bitmap 属性
-----------------------
> <p id="canvas_offline_t_bitmap">绑定的离线 bitmap

* 类型：bitmap\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
