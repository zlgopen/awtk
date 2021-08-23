## dirty\_rects\_t
### 概述
支持多个脏矩形。
> 在通常情况下，脏矩形的个数并不多，而且一般都是不重叠的，
> 所以为了降低计算开销、避免内存分配和简化实现。这里采用下列措施：

* 如果新的脏矩形是独立的，直接加入进来。
* 如果新的脏矩形与某个脏矩形有重叠，则合并到该脏矩形。
* 如果脏矩形的个数超出最大个数则进行合并。
----------------------------------
### 函数
<p id="dirty_rects_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#dirty_rects_t_dirty_rects_add">dirty\_rects\_add</a> | 增加脏矩形。 |
| <a href="#dirty_rects_t_dirty_rects_deinit">dirty\_rects\_deinit</a> | 释放dirty_rects对象。 |
| <a href="#dirty_rects_t_dirty_rects_fix">dirty\_rects\_fix</a> | 脏矩形之间可能重叠，合并重叠的脏矩形。 |
| <a href="#dirty_rects_t_dirty_rects_init">dirty\_rects\_init</a> | 初始化dirty_rects对象。 |
| <a href="#dirty_rects_t_dirty_rects_remove">dirty\_rects\_remove</a> | 删除指定序数的矩形。 |
| <a href="#dirty_rects_t_dirty_rects_reset">dirty\_rects\_reset</a> | 重置dirty_rects对象。 |
| <a href="#dirty_rects_t_dirty_rects_set_debug">dirty\_rects\_set\_debug</a> | 设置是否开启调试模式。 |
| <a href="#dirty_rects_t_dirty_rects_set_disable_multiple">dirty\_rects\_set\_disable\_multiple</a> | 设置是否关闭多脏矩形模式(方便对比测试和调试)。 |
| <a href="#dirty_rects_t_dirty_rects_set_profile">dirty\_rects\_set\_profile</a> | 设置是否开启profile模式。 |
### 属性
<p id="dirty_rects_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#dirty_rects_t_max">max</a> | rect\_t | 最大脏矩形。 |
| <a href="#dirty_rects_t_nr">nr</a> | uint32\_t | 个数。 |
| <a href="#dirty_rects_t_rects">rects</a> | rect\_t | 脏矩形。 |
#### dirty\_rects\_add 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_add">增加脏矩形。

* 函数原型：

```
ret_t dirty_rects_add (dirty_rects_t* dirty_rects, rect_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
| r | rect\_t* | 脏矩形。 |
#### dirty\_rects\_deinit 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_deinit">释放dirty_rects对象。

* 函数原型：

```
ret_t dirty_rects_deinit (dirty_rects_t* dirty_rects);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
#### dirty\_rects\_fix 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_fix">脏矩形之间可能重叠，合并重叠的脏矩形。

* 函数原型：

```
ret_t dirty_rects_fix (dirty_rects_t* dirty_rects);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
#### dirty\_rects\_init 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_init">初始化dirty_rects对象。

* 函数原型：

```
ret_t dirty_rects_init (dirty_rects_t* dirty_rects);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
#### dirty\_rects\_remove 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_remove">删除指定序数的矩形。

* 函数原型：

```
ret_t dirty_rects_remove (dirty_rects_t* dirty_rects, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
| index | uint32\_t | 序数。 |
#### dirty\_rects\_reset 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_reset">重置dirty_rects对象。

* 函数原型：

```
ret_t dirty_rects_reset (dirty_rects_t* dirty_rects);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
#### dirty\_rects\_set\_debug 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_set_debug">设置是否开启调试模式。

* 函数原型：

```
ret_t dirty_rects_set_debug (dirty_rects_t* dirty_rects, bool_t debug);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
| debug | bool\_t | 是否开启调试模式。 |
#### dirty\_rects\_set\_disable\_multiple 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_set_disable_multiple">设置是否关闭多脏矩形模式(方便对比测试和调试)。

* 函数原型：

```
ret_t dirty_rects_set_disable_multiple (dirty_rects_t* dirty_rects, bool_t disable_multiple);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
| disable\_multiple | bool\_t | 是否关闭多脏矩形模式。 |
#### dirty\_rects\_set\_profile 函数
-----------------------

* 函数功能：

> <p id="dirty_rects_t_dirty_rects_set_profile">设置是否开启profile模式。

* 函数原型：

```
ret_t dirty_rects_set_profile (dirty_rects_t* dirty_rects, bool_t profile);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dirty\_rects | dirty\_rects\_t* | dirty\_rects对象。 |
| profile | bool\_t | 是否开启profile模式。 |
#### max 属性
-----------------------
> <p id="dirty_rects_t_max">最大脏矩形。

* 类型：rect\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### nr 属性
-----------------------
> <p id="dirty_rects_t_nr">个数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### rects 属性
-----------------------
> <p id="dirty_rects_t_rects">脏矩形。

* 类型：rect\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
