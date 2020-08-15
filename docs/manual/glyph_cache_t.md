## glyph\_cache\_t
### 概述
glyph cache
----------------------------------
### 函数
<p id="glyph_cache_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#glyph_cache_t_glyph_cache_add">glyph\_cache\_add</a> | 增加一个glyph对象到cache。 |
| <a href="#glyph_cache_t_glyph_cache_deinit">glyph\_cache\_deinit</a> | 释放全部cache。 |
| <a href="#glyph_cache_t_glyph_cache_init">glyph\_cache\_init</a> | 初始化glyph_cache对象。 |
| <a href="#glyph_cache_t_glyph_cache_lookup">glyph\_cache\_lookup</a> | 查找glyph对象。 |
| <a href="#glyph_cache_t_glyph_cache_shrink">glyph\_cache\_shrink</a> | 释放部分glyph。 |
#### glyph\_cache\_add 函数
-----------------------

* 函数功能：

> <p id="glyph_cache_t_glyph_cache_add">增加一个glyph对象到cache。

* 函数原型：

```
ret_t glyph_cache_add (glyph_cache_t* cache, wchar_t code, font_size_t size, glyph_t g);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cache | glyph\_cache\_t* | cache对象。 |
| code | wchar\_t | 字符。 |
| size | font\_size\_t | 字体大小。 |
| g | glyph\_t | glyph对象。 |
#### glyph\_cache\_deinit 函数
-----------------------

* 函数功能：

> <p id="glyph_cache_t_glyph_cache_deinit">释放全部cache。

* 函数原型：

```
ret_t glyph_cache_deinit (glyph_cache_t* cache);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cache | glyph\_cache\_t* | cache对象。 |
#### glyph\_cache\_init 函数
-----------------------

* 函数功能：

> <p id="glyph_cache_t_glyph_cache_init">初始化glyph_cache对象。

* 函数原型：

```
glyph_cache_t* glyph_cache_init (glyph_cache_t* cache, uint32_t capacity, tk_destroy_t destroy_glyph);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | glyph\_cache\_t* | 返回cache对象。 |
| cache | glyph\_cache\_t* | cache对象。 |
| capacity | uint32\_t | 最大容量。 |
| destroy\_glyph | tk\_destroy\_t | 销毁函数。 |
#### glyph\_cache\_lookup 函数
-----------------------

* 函数功能：

> <p id="glyph_cache_t_glyph_cache_lookup">查找glyph对象。

* 函数原型：

```
ret_t glyph_cache_lookup (glyph_cache_t* cache, wchar_t code, font_size_t size, glyph_t g);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cache | glyph\_cache\_t* | cache对象。 |
| code | wchar\_t | 字符。 |
| size | font\_size\_t | 字体大小。 |
| g | glyph\_t | glyph对象(用于返回glyph)。 |
#### glyph\_cache\_shrink 函数
-----------------------

* 函数功能：

> <p id="glyph_cache_t_glyph_cache_shrink">释放部分glyph。

* 函数原型：

```
ret_t glyph_cache_shrink (glyph_cache_t* cache, uint32_t cache_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cache | glyph\_cache\_t* | cache对象。 |
| cache\_size | uint32\_t | 保留cache个数。 |
