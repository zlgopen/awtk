## fps\_t
### 概述
用于计算FPS(帧率)。
----------------------------------
### 函数
<p id="fps_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fps_t_fps_get">fps\_get</a> | 获取FPS(帧率)。 |
| <a href="#fps_t_fps_inc">fps\_inc</a> | 增加帧数。 |
| <a href="#fps_t_fps_init">fps\_init</a> | 初始化。 |
#### fps\_get 函数
-----------------------

* 函数功能：

> <p id="fps_t_fps_get">获取FPS(帧率)。

* 函数原型：

```
uint32_t fps_get (fps_t* fps);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回FPS(帧率)。 |
| fps | fps\_t* | fps对象。 |
#### fps\_inc 函数
-----------------------

* 函数功能：

> <p id="fps_t_fps_inc">增加帧数。

* 函数原型：

```
ret_t fps_inc (fps_t* fps);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fps | fps\_t* | fps对象。 |
#### fps\_init 函数
-----------------------

* 函数功能：

> <p id="fps_t_fps_init">初始化。

* 函数原型：

```
ret_t fps_init (fps_t* fps);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fps | fps\_t* | fps对象。 |
