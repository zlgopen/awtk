## gradient\_t
### 概述
渐变。
----------------------------------
### 函数
<p id="gradient_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#gradient_t_gradient_add_stop">gradient\_add\_stop</a> | 增加关键点。 |
| <a href="#gradient_t_gradient_deinit">gradient\_deinit</a> | 释放gradient对象。 |
| <a href="#gradient_t_gradient_get_color">gradient\_get\_color</a> | 获取颜色。 |
| <a href="#gradient_t_gradient_get_first_color">gradient\_get\_first\_color</a> | 获取开始的颜色。 |
| <a href="#gradient_t_gradient_get_last_color">gradient\_get\_last\_color</a> | 获取结束的颜色。 |
| <a href="#gradient_t_gradient_get_stop">gradient\_get\_stop</a> | 获取关键点。 |
| <a href="#gradient_t_gradient_init">gradient\_init</a> | 初始化gradient对象。 |
| <a href="#gradient_t_gradient_init_from_binary">gradient\_init\_from\_binary</a> | 从二进制数据初始化gradient对象。 |
| <a href="#gradient_t_gradient_init_from_str">gradient\_init\_from\_str</a> | 从字符串初始化gradient对象。 |
| <a href="#gradient_t_gradient_init_simple">gradient\_init\_simple</a> | 初始化gradient对象。 |
| <a href="#gradient_t_gradient_set_linear_degree">gradient\_set\_linear\_degree</a> | 设置线性渐变的角度。 |
| <a href="#gradient_t_gradient_set_type">gradient\_set\_type</a> | 设置类型。 |
| <a href="#gradient_t_gradient_to_binary">gradient\_to\_binary</a> | 转成二进制格式。 |
| <a href="#gradient_t_gradient_to_str">gradient\_to\_str</a> | 转换成字符串格式。 |
### 属性
<p id="gradient_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#gradient_t_degree">degree</a> | uint32\_t | 线性渐变的角度。 |
| <a href="#gradient_t_nr">nr</a> | uint32\_t | stop个数。 |
| <a href="#gradient_t_type">type</a> | gradient\_type\_t | 类型。 |
#### gradient\_add\_stop 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_add_stop">增加关键点。

* 函数原型：

```
ret_t gradient_add_stop (gradient_t* gradient, color_t color, float stop);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| gradient | gradient\_t* | gradient对象。 |
| color | color\_t | 颜色。 |
| stop | float | 位置。 |
#### gradient\_deinit 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_deinit">释放gradient对象。

* 函数原型：

```
ret_t gradient_deinit (gradient_t* gradient);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| gradient | gradient\_t* | gradient对象。 |
#### gradient\_get\_color 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_get_color">获取颜色。

* 函数原型：

```
color_t gradient_get_color (gradient_t* gradient, float offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t | 返回颜色。 |
| gradient | gradient\_t* | gradient对象。 |
| offset | float | 偏移(0-1)。 |
#### gradient\_get\_first\_color 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_get_first_color">获取开始的颜色。

* 函数原型：

```
color_t gradient_get_first_color (gradient_t* gradient);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t | 返回颜色。 |
| gradient | gradient\_t* | gradient对象。 |
#### gradient\_get\_last\_color 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_get_last_color">获取结束的颜色。

* 函数原型：

```
color_t gradient_get_last_color (gradient_t* gradient);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t | 返回颜色。 |
| gradient | gradient\_t* | gradient对象。 |
#### gradient\_get\_stop 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_get_stop">获取关键点。

* 函数原型：

```
ret_t gradient_get_stop (gradient_t* gradient, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| gradient | gradient\_t* | gradient对象。 |
| index | uint32\_t | 序数。 |
#### gradient\_init 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_init">初始化gradient对象。

* 函数原型：

```
gradient_t* gradient_init (gradient_t* gradient);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | gradient\_t* | 返回gradient对象。 |
| gradient | gradient\_t* | gradient对象。 |
#### gradient\_init\_from\_binary 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_init_from_binary">从二进制数据初始化gradient对象。

> 二进制数据必须由gradient_to_binary生成。

* 函数原型：

```
gradient_t* gradient_init_from_binary (gradient_t* gradient, const uint8_t* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | gradient\_t* | 返回gradient对象。 |
| gradient | gradient\_t* | gradient对象。 |
| data | const uint8\_t* | 数据。 |
| size | uint32\_t | 数据长度。 |
#### gradient\_init\_from\_str 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_init_from_str">从字符串初始化gradient对象。

> 字符串的格式兼容CSS，请参考相关文档。

* 函数原型：

```
gradient_t* gradient_init_from_str (gradient_t* gradient, const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | gradient\_t* | 返回gradient对象。 |
| gradient | gradient\_t* | gradient对象。 |
| str | const char* | 字符串。 |
#### gradient\_init\_simple 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_init_simple">初始化gradient对象。

* 函数原型：

```
gradient_t* gradient_init_simple (gradient_t* gradient, uint32_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | gradient\_t* | 返回gradient对象。 |
| gradient | gradient\_t* | gradient对象。 |
| color | uint32\_t | 颜色。 |
#### gradient\_set\_linear\_degree 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_set_linear_degree">设置线性渐变的角度。

* 函数原型：

```
ret_t gradient_set_linear_degree (gradient_t* gradient, uint32_t degree);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| gradient | gradient\_t* | gradient对象。 |
| degree | uint32\_t | 角度。 |
#### gradient\_set\_type 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_set_type">设置类型。

* 函数原型：

```
ret_t gradient_set_type (gradient_t* gradient, gradient_type_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| gradient | gradient\_t* | gradient对象。 |
| type | gradient\_type\_t | 类型。 |
#### gradient\_to\_binary 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_to_binary">转成二进制格式。

* 函数原型：

```
ret_t gradient_to_binary (gradient_t* gradient, wbuffer_t* wb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| gradient | gradient\_t* | gradient对象。 |
| wb | wbuffer\_t* | 二进制数据。 |
#### gradient\_to\_str 函数
-----------------------

* 函数功能：

> <p id="gradient_t_gradient_to_str">转换成字符串格式。

* 函数原型：

```
ret_t gradient_to_str (gradient_t* gradient, str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| gradient | gradient\_t* | gradient对象。 |
| str | str\_t* | 字符串。 |
#### degree 属性
-----------------------
> <p id="gradient_t_degree">线性渐变的角度。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### nr 属性
-----------------------
> <p id="gradient_t_nr">stop个数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### type 属性
-----------------------
> <p id="gradient_t_type">类型。

* 类型：gradient\_type\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
