## matrix\_t
### 概述
2D矩阵对象。
----------------------------------
### 函数
<p id="matrix_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#matrix_t_matrix_identity">matrix\_identity</a> | 重置矩阵对象为单位矩阵。 |
| <a href="#matrix_t_matrix_init">matrix\_init</a> | 初始化矩阵对象。 |
| <a href="#matrix_t_matrix_invert">matrix\_invert</a> | 求矩阵的逆。 |
| <a href="#matrix_t_matrix_multiply">matrix\_multiply</a> | 矩阵相乘。 |
| <a href="#matrix_t_matrix_rotate">matrix\_rotate</a> | 旋转。 |
| <a href="#matrix_t_matrix_scale">matrix\_scale</a> | 缩放。 |
| <a href="#matrix_t_matrix_set">matrix\_set</a> | 设置矩阵的参数。 |
| <a href="#matrix_t_matrix_transform_point">matrix\_transform\_point</a> | 对点进行变换。 |
| <a href="#matrix_t_matrix_translate">matrix\_translate</a> | 平移。 |
#### matrix\_identity 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_identity">重置矩阵对象为单位矩阵。

* 函数原型：

```
matrix_t* matrix_identity (matrix_t* m);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
#### matrix\_init 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_init">初始化矩阵对象。

* 函数原型：

```
matrix_t* matrix_init (matrix_t* m);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
#### matrix\_invert 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_invert">求矩阵的逆。

* 函数原型：

```
matrix_t* matrix_invert (matrix_t* m);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
#### matrix\_multiply 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_multiply">矩阵相乘。

* 函数原型：

```
matrix_t* matrix_multiply (matrix_t* m, matrix_t* b);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
| b | matrix\_t* | 矩阵对象。 |
#### matrix\_rotate 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_rotate">旋转。

* 函数原型：

```
matrix_t* matrix_rotate (matrix_t* m, float rad);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
| rad | float | 角度。 |
#### matrix\_scale 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_scale">缩放。

* 函数原型：

```
matrix_t* matrix_scale (matrix_t* m, float sx, float sy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
| sx | float | sx |
| sy | float | sy |
#### matrix\_set 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_set">设置矩阵的参数。

* 函数原型：

```
matrix_t* matrix_set (matrix_t* m, float a0, float a1, float a2, float a3, float a4, float a5);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
| a0 | float | a0 |
| a1 | float | a1 |
| a2 | float | a2 |
| a3 | float | a3 |
| a4 | float | a4 |
| a5 | float | a5 |
#### matrix\_transform\_point 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_transform_point">对点进行变换。

* 函数原型：

```
matrix_t* matrix_transform_point (matrix_t* m, xy_t x, xy_t y, xy_t* out_x, xy_t* out_y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
| x | xy\_t | x |
| y | xy\_t | y |
| out\_x | xy\_t* | out\_x |
| out\_y | xy\_t* | out\_y |
#### matrix\_translate 函数
-----------------------

* 函数功能：

> <p id="matrix_t_matrix_translate">平移。

* 函数原型：

```
matrix_t* matrix_translate (matrix_t* m, float x, float y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | matrix\_t* | 矩阵对象。 |
| m | matrix\_t* | 矩阵对象。 |
| x | float | x |
| y | float | y |
