## color\_t
### 概述
 颜色。


### 函数
<p id="color_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#color_t_color_init">color\_init</a> | 初始化颜色对象。 |
### 属性
<p id="color_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#color_t_color">color</a> | uint32\_t | 颜色的数值。 |
| <a href="#color_t_rgba">rgba</a> | rgba\_t | 颜色的RGBA值。 |
#### color\_init 函数
-----------------------

* 函数功能：

> <p id="color_t_color_init"> 初始化颜色对象。




* 函数原型：

```
color_t color_init (uint8_t r, uint8_t g, uint8_t b, uint8_t a);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t | 颜色对象。 |
| r | uint8\_t | 红色。 |
| g | uint8\_t | 绿色。 |
| b | uint8\_t | 蓝色。 |
| a | uint8\_t | alpha。 |
#### color 属性
-----------------------
> <p id="color_t_color"> 颜色的数值。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 是 |
#### rgba 属性
-----------------------
> <p id="color_t_rgba"> 颜色的RGBA值。



* 类型：rgba\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 是 |
