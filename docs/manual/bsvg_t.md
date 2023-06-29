## bsvg\_t
### 概述

----------------------------------
### 函数
<p id="bsvg_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#bsvg_t_bsvg_draw">bsvg\_draw</a> | 绘制bsvg。 |
| <a href="#bsvg_t_bsvg_draw_path">bsvg\_draw\_path</a> | 绘制bsvg路径。 |
| <a href="#bsvg_t_bsvg_init">bsvg\_init</a> | 初始化bsvg对象。 |
#### bsvg\_draw 函数
-----------------------

* 函数功能：

> <p id="bsvg_t_bsvg_draw">绘制bsvg。

* 函数原型：

```
ret_t bsvg_draw (bsvg_t* svg, vgcanvas_t* canvas);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| svg | bsvg\_t* | SVG对象。 |
| canvas | vgcanvas\_t* | vgcanvas对象。 |
#### bsvg\_draw\_path 函数
-----------------------

* 函数功能：

> <p id="bsvg_t_bsvg_draw_path">绘制bsvg路径。

* 函数原型：

```
ret_t bsvg_draw_path (bsvg_draw_ctx_t* ctx, const svg_path_t* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | bsvg\_draw\_ctx\_t* | 绘制上下文。 |
| path | const svg\_path\_t* | path对象。 |
#### bsvg\_init 函数
-----------------------

* 函数功能：

> <p id="bsvg_t_bsvg_init">初始化bsvg对象。

* 函数原型：

```
bsvg_t* bsvg_init (bsvg_t* svg, const uint32_t* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bsvg\_t* | 返回解析后的svg。 |
| svg | bsvg\_t* | svg对象。 |
| data | const uint32\_t* | svg数据。 |
| size | uint32\_t | data长度。 |
