## glyph\_t
### 概述
代表一个字符的字模。
----------------------------------
### 函数
<p id="glyph_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#glyph_t_glyph_clone">glyph\_clone</a> | 克隆glyph对象。 |
| <a href="#glyph_t_glyph_create">glyph\_create</a> | 创建glyph对象。 |
| <a href="#glyph_t_glyph_destroy">glyph\_destroy</a> | 销毁glyph对象。 |
### 属性
<p id="glyph_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#glyph_t_advance">advance</a> | uint16\_t | 占位宽度。 |
| <a href="#glyph_t_data">data</a> | const uint8\_t* | 数据。 |
| <a href="#glyph_t_format">format</a> | uint8\_t | 格式。 |
| <a href="#glyph_t_h">h</a> | uint16\_t | 高度。 |
| <a href="#glyph_t_pitch">pitch</a> | uint8\_t | pitch。 |
| <a href="#glyph_t_w">w</a> | uint16\_t | 宽度。 |
| <a href="#glyph_t_x">x</a> | int16\_t | x坐标。 |
| <a href="#glyph_t_y">y</a> | int16\_t | y坐标。 |
#### glyph\_clone 函数
-----------------------

* 函数功能：

> <p id="glyph_t_glyph_clone">克隆glyph对象。

* 函数原型：

```
glyph_t* glyph_clone ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | glyph\_t* | glyph对象。 |
#### glyph\_create 函数
-----------------------

* 函数功能：

> <p id="glyph_t_glyph_create">创建glyph对象。

* 函数原型：

```
glyph_t* glyph_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | glyph\_t* | glyph对象。 |
#### glyph\_destroy 函数
-----------------------

* 函数功能：

> <p id="glyph_t_glyph_destroy">销毁glyph对象。

* 函数原型：

```
ret_t glyph_destroy (glyph_t* glyph);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| glyph | glyph\_t* | glyph对象。 |
#### advance 属性
-----------------------
> <p id="glyph_t_advance">占位宽度。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### data 属性
-----------------------
> <p id="glyph_t_data">数据。

* 类型：const uint8\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### format 属性
-----------------------
> <p id="glyph_t_format">格式。

* 类型：uint8\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### h 属性
-----------------------
> <p id="glyph_t_h">高度。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### pitch 属性
-----------------------
> <p id="glyph_t_pitch">pitch。

* 类型：uint8\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### w 属性
-----------------------
> <p id="glyph_t_w">宽度。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### x 属性
-----------------------
> <p id="glyph_t_x">x坐标。

* 类型：int16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### y 属性
-----------------------
> <p id="glyph_t_y">y坐标。

* 类型：int16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
