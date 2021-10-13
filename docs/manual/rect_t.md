## rect\_t
### 概述
矩形。包括一个x坐标、y坐标、宽度和高度。
----------------------------------
### 函数
<p id="rect_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#rect_t_rect_cast">rect\_cast</a> | 转换为rect对象。 |
| <a href="#rect_t_rect_contains">rect\_contains</a> | 判断指定的点在rect范围内。 |
| <a href="#rect_t_rect_create">rect\_create</a> | 创建rect对象。 |
| <a href="#rect_t_rect_destroy">rect\_destroy</a> | 销毁rect对象。 |
| <a href="#rect_t_rect_fix">rect\_fix</a> | 确保rect在指定的大小范围内。 |
| <a href="#rect_t_rect_from_rectf">rect\_from\_rectf</a> | rectf 类型转换到 rect 类型。 |
| <a href="#rect_t_rect_has_intersect">rect\_has\_intersect</a> | 判断两个rect的是否存在交集。 |
| <a href="#rect_t_rect_init">rect\_init</a> | 初始化rect对象。 |
| <a href="#rect_t_rect_intersect">rect\_intersect</a> | 求两个rect的交集。 |
| <a href="#rect_t_rect_merge">rect\_merge</a> | 合并两个rect对象。 |
| <a href="#rect_t_rect_scale">rect\_scale</a> | 缩放rect对象。 |
| <a href="#rect_t_rect_set">rect\_set</a> | 设置rect对象的xywh。 |
| <a href="#rect_t_rect_to_rectf">rect\_to\_rectf</a> | rect 类型转换到 rectf 类型。 |
| <a href="#rect_t_rectf_scale">rectf\_scale</a> | 缩放rectf对象。 |
### 属性
<p id="rect_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#rect_t_h">h</a> | wh\_t | 高度。 |
| <a href="#rect_t_w">w</a> | wh\_t | 宽度。 |
| <a href="#rect_t_x">x</a> | xy\_t | x坐标。 |
| <a href="#rect_t_y">y</a> | xy\_t | y坐标。 |
#### rect\_cast 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_cast">转换为rect对象。

> 供脚本语言使用。

* 函数原型：

```
rect_t* rect_cast (rect_t* rect);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rect\_t* | rect对象。 |
| rect | rect\_t* | rect对象。 |
#### rect\_contains 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_contains">判断指定的点在rect范围内。

* 函数原型：

```
bool_t rect_contains (const rect_t* r, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回在rect范围内。 |
| r | const rect\_t* | rect对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### rect\_create 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_create">创建rect对象。

> 主要供脚本语言使用。

* 函数原型：

```
rect_t* rect_create (xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rect\_t* | rect对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | wh\_t | 宽度。 |
| h | wh\_t | 高度。 |
#### rect\_destroy 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_destroy">销毁rect对象。

> 主要供脚本语言使用。

* 函数原型：

```
ret_t rect_destroy (rect_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| r | rect\_t* | rect对象。 |
#### rect\_fix 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_fix">确保rect在指定的大小范围内。

* 函数原型：

```
rect_t rect_fix (rect_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rect\_t | 返回修复之后的rect对象。 |
| r | rect\_t* | rect对象。 |
#### rect\_from\_rectf 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_from_rectf">rectf 类型转换到 rect 类型。

* 函数原型：

```
rect_t rect_from_rectf (const rectf_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rect\_t | 返回 rect 对象。 |
| r | const rectf\_t* | rectf 对象。 |
#### rect\_has\_intersect 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_has_intersect">判断两个rect的是否存在交集。

* 函数原型：

```
bool_t rect_has_intersect (const rect_t* r1, const rect_t* r2);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| r1 | const rect\_t* | rect对象。 |
| r2 | const rect\_t* | rect对象。 |
#### rect\_init 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_init">初始化rect对象。

* 函数原型：

```
rect_t rect_init (xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rect\_t | 返回rect对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | wh\_t | 宽度。 |
| h | wh\_t | 高度。 |
#### rect\_intersect 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_intersect">求两个rect的交集。

* 函数原型：

```
rect_t rect_intersect (const rect_t* r1, const rect_t* r2);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rect\_t | 返回交集。 |
| r1 | const rect\_t* | rect对象。 |
| r2 | const rect\_t* | rect对象。 |
#### rect\_merge 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_merge">合并两个rect对象。

* 函数原型：

```
ret_t rect_merge (rect_t* dst_r, const rect_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dst\_r | rect\_t* | rect对象。 |
| r | const rect\_t* | rect对象。 |
#### rect\_scale 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_scale">缩放rect对象。

* 函数原型：

```
rect_t* rect_scale (rect_t* r, float_t scale);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rect\_t* | 返回rect对象。 |
| r | rect\_t* | rect对象。 |
| scale | float\_t | 缩放比例。 |
#### rect\_set 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_set">设置rect对象的xywh。

> 主要供脚本语言使用。

* 函数原型：

```
rect_t* rect_set (rect_t* rect, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rect\_t* | rect对象。 |
| rect | rect\_t* | rect对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | wh\_t | 宽度。 |
| h | wh\_t | 高度。 |
#### rect\_to\_rectf 函数
-----------------------

* 函数功能：

> <p id="rect_t_rect_to_rectf">rect 类型转换到 rectf 类型。

* 函数原型：

```
rectf_t rect_to_rectf (const rect_t* r1);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rectf\_t | 返回 rectf\_t 对象。 |
| r1 | const rect\_t* | rect 对象。 |
#### rectf\_scale 函数
-----------------------

* 函数功能：

> <p id="rect_t_rectf_scale">缩放rectf对象。

* 函数原型：

```
rectf_t* rectf_scale (rectf_t* r, float_t scale);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rectf\_t* | 返回rect对象。 |
| r | rectf\_t* | rect对象。 |
| scale | float\_t | 缩放比例。 |
#### h 属性
-----------------------
> <p id="rect_t_h">高度。

* 类型：wh\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### w 属性
-----------------------
> <p id="rect_t_w">宽度。

* 类型：wh\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### x 属性
-----------------------
> <p id="rect_t_x">x坐标。

* 类型：xy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### y 属性
-----------------------
> <p id="rect_t_y">y坐标。

* 类型：xy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
