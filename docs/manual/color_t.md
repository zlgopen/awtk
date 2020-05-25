## color\_t
### 概述
颜色。
----------------------------------
### 函数
<p id="color_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#color_t_color_a">color\_a</a> | 获取alpha通道的值。 |
| <a href="#color_t_color_b">color\_b</a> | 获取蓝色通道的值。 |
| <a href="#color_t_color_cast">color\_cast</a> | 转换为color对象。 |
| <a href="#color_t_color_create">color\_create</a> | 创建color对象。 |
| <a href="#color_t_color_destroy">color\_destroy</a> | 销毁color对象。 |
| <a href="#color_t_color_from_str">color\_from\_str</a> | 创建color对象。 |
| <a href="#color_t_color_g">color\_g</a> | 获取绿色通道的值。 |
| <a href="#color_t_color_hex_str">color\_hex\_str</a> | 获取16进制格式表示的颜色 |
| <a href="#color_t_color_init">color\_init</a> | 初始化颜色对象。 |
| <a href="#color_t_color_r">color\_r</a> | 获取红色通道的值。 |
| <a href="#color_t_color_rgba_str">color\_rgba\_str</a> | 获取rgba格式表示的颜色 |
### 属性
<p id="color_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#color_t_color">color</a> | uint32\_t | 颜色的数值。 |
| <a href="#color_t_rgba">rgba</a> | rgba\_t | 颜色的RGBA值。 |
#### color\_a 函数
-----------------------

* 函数功能：

> <p id="color_t_color_a">获取alpha通道的值。

> 主要供脚本语言使用。

* 函数原型：

```
uint8_t color_a (color_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t | 返回alpha通道的值。 |
| c | color\_t* | color对象。 |
#### color\_b 函数
-----------------------

* 函数功能：

> <p id="color_t_color_b">获取蓝色通道的值。

> 主要供脚本语言使用。

* 函数原型：

```
uint8_t color_b (color_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t | 返回蓝色通道的值。 |
| c | color\_t* | color对象。 |
#### color\_cast 函数
-----------------------

* 函数功能：

> <p id="color_t_color_cast">转换为color对象。

> 供脚本语言使用。

* 函数原型：

```
color_t* color_cast (color_t* color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t* | color对象。 |
| color | color\_t* | color对象。 |
#### color\_create 函数
-----------------------

* 函数功能：

> <p id="color_t_color_create">创建color对象。

> 主要供脚本语言使用。

* 函数原型：

```
color_t* color_create (uint8_t r, uint8_t b, uint8_t g, uint8_t a);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t* | color对象。 |
| r | uint8\_t | 红色通道。 |
| b | uint8\_t | 蓝色通道。 |
| g | uint8\_t | 绿色通道。 |
| a | uint8\_t | alpha通道。 |
#### color\_destroy 函数
-----------------------

* 函数功能：

> <p id="color_t_color_destroy">销毁color对象。
> 主要供脚本语言使用。

* 函数原型：

```
ret_t color_destroy (color_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | color\_t* | color对象。 |
#### color\_from\_str 函数
-----------------------

* 函数功能：

> <p id="color_t_color_from_str">创建color对象。

> 主要供脚本语言使用。

* 函数原型：

```
color_t* color_from_str (color_t* c, const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t* | color对象。 |
| c | color\_t* | color对象。 |
| str | const char* | css类似的颜色值。 |
#### color\_g 函数
-----------------------

* 函数功能：

> <p id="color_t_color_g">获取绿色通道的值。

> 主要供脚本语言使用。

* 函数原型：

```
uint8_t color_g (color_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t | 返回绿色通道的值。 |
| c | color\_t* | color对象。 |
#### color\_hex\_str 函数
-----------------------

* 函数功能：

> <p id="color_t_color_hex_str">获取16进制格式表示的颜色

* 函数原型：

```
char* color_hex_str (color_t* c, char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 获取16进制字符串表示的颜色。 |
| c | color\_t* | color对象。 |
| str | char* | 内存。 |
#### color\_init 函数
-----------------------

* 函数功能：

> <p id="color_t_color_init">初始化颜色对象。

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
#### color\_r 函数
-----------------------

* 函数功能：

> <p id="color_t_color_r">获取红色通道的值。

> 主要供脚本语言使用。

* 函数原型：

```
uint8_t color_r (color_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t | 返回红色通道的值。 |
| c | color\_t* | color对象。 |
#### color\_rgba\_str 函数
-----------------------

* 函数功能：

> <p id="color_t_color_rgba_str">获取rgba格式表示的颜色

* 函数原型：

```
char* color_rgba_str (color_t* c, char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 获取rgba格式表示的颜色。 |
| c | color\_t* | color对象。 |
| str | char* | 内存。 |
#### color 属性
-----------------------
> <p id="color_t_color">颜色的数值。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 是 |
| 可脚本化   | 是 |
#### rgba 属性
-----------------------
> <p id="color_t_rgba">颜色的RGBA值。

* 类型：rgba\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 是 |
