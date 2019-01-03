## image\_animation\_t
### 概述
![image](images/image_animation_t_0.png)

 图片动画控件，指定一个图片前缀，依次显示指定序列的图片，从而形成动画效果。
### 函数
<p id="image_animation_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#image_animation_t_image_animation_cast">image\_animation\_cast</a> | 转换为image_animation对象(供脚本语言使用)。 |
| <a href="#image_animation_t_image_animation_create">image\_animation\_create</a> | 创建image_animation对象 |
| <a href="#image_animation_t_image_animation_pause">image\_animation\_pause</a> | 暂停。 |
| <a href="#image_animation_t_image_animation_play">image\_animation\_play</a> | 播放。 |
| <a href="#image_animation_t_image_animation_set_auto_play">image\_animation\_set\_auto\_play</a> | 设置是否自动播放。 |
| <a href="#image_animation_t_image_animation_set_delay">image\_animation\_set\_delay</a> | 设置延迟播放时间(仅适用于自动播放)。 |
| <a href="#image_animation_t_image_animation_set_image">image\_animation\_set\_image</a> | 设置图片前缀。 |
| <a href="#image_animation_t_image_animation_set_interval">image\_animation\_set\_interval</a> | 设置播放间隔时间。 |
| <a href="#image_animation_t_image_animation_set_loop">image\_animation\_set\_loop</a> | 设置是否循环播放。 |
| <a href="#image_animation_t_image_animation_set_sequence">image\_animation\_set\_sequence</a> | 设置播放序列。比如image为"fire"，sequence为"123", 将依次播放"fire1", "fire2", "fire3"。 |
| <a href="#image_animation_t_image_animation_stop">image\_animation\_stop</a> | 停止(并重置index为0)。 |
### 属性
<p id="image_animation_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#image_animation_t_auto_play">auto\_play</a> | bool\_t | 是否自动播放。 |
| <a href="#image_animation_t_delay">delay</a> | uint32\_t | 自动播放时延迟播放的时间(毫秒)。 |
| <a href="#image_animation_t_image">image</a> | char* | 图片名称的前缀。 |
| <a href="#image_animation_t_interval">interval</a> | uint32\_t | 每张图片播放的时间(毫秒)。 |
| <a href="#image_animation_t_loop">loop</a> | bool\_t | 是否循环播放。 |
| <a href="#image_animation_t_sequence">sequence</a> | char* | 播放的序列，字符可选值为:0-9,a-z,A-Z。 |
#### image\_animation\_cast 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_cast"> 转换为image_animation对象(供脚本语言使用)。



* 函数原型：

```
widget_t* image_animation_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | image\_animation对象。 |
| widget | widget\_t* | image\_animation对象。 |
#### image\_animation\_create 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_create"> 创建image_animation对象



* 函数原型：

```
widget_t* image_animation_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
#### image\_animation\_pause 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_pause"> 暂停。



* 函数原型：

```
ret_t image_animation_pause (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
#### image\_animation\_play 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_play"> 播放。



* 函数原型：

```
ret_t image_animation_play (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
#### image\_animation\_set\_auto\_play 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_set_auto_play"> 设置是否自动播放。



* 函数原型：

```
ret_t image_animation_set_auto_play (widget_t* widget, bool_t auto_play);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
| auto\_play | bool\_t | 是否自动播放。 |
#### image\_animation\_set\_delay 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_set_delay"> 设置延迟播放时间(仅适用于自动播放)。



* 函数原型：

```
ret_t image_animation_set_delay (widget_t* widget, uint32_t delay);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
| delay | uint32\_t | 延迟播放时间(毫秒)。 |
#### image\_animation\_set\_image 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_set_image"> 设置图片前缀。



* 函数原型：

```
ret_t image_animation_set_image (widget_t* widget, const char* image);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
| image | const char* | 图片前缀。 |
#### image\_animation\_set\_interval 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_set_interval"> 设置播放间隔时间。



* 函数原型：

```
ret_t image_animation_set_interval (widget_t* widget, uint32_t interval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
| interval | uint32\_t | 间隔时间(毫秒)。 |
#### image\_animation\_set\_loop 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_set_loop"> 设置是否循环播放。



* 函数原型：

```
ret_t image_animation_set_loop (widget_t* widget, bool_t loop);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
| loop | bool\_t | 是否循环播放。 |
#### image\_animation\_set\_sequence 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_set_sequence"> 设置播放序列。比如image为"fire"，sequence为"123", 将依次播放"fire1", "fire2", "fire3"。



* 函数原型：

```
ret_t image_animation_set_sequence (widget_t* widget, const char* sequence);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
| sequence | const char* | 播放序列。 |
#### image\_animation\_stop 函数
-----------------------

* 函数功能：

> <p id="image_animation_t_image_animation_stop"> 停止(并重置index为0)。



* 函数原型：

```
ret_t image_animation_stop (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image\_animation对象。 |
#### auto\_play 属性
-----------------------
> <p id="image_animation_t_auto_play"> 是否自动播放。


* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### delay 属性
-----------------------
> <p id="image_animation_t_delay"> 自动播放时延迟播放的时间(毫秒)。


* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### image 属性
-----------------------
> <p id="image_animation_t_image"> 图片名称的前缀。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### interval 属性
-----------------------
> <p id="image_animation_t_interval"> 每张图片播放的时间(毫秒)。


* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### loop 属性
-----------------------
> <p id="image_animation_t_loop"> 是否循环播放。


* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### sequence 属性
-----------------------
> <p id="image_animation_t_sequence"> 播放的序列，字符可选值为:0-9,a-z,A-Z。


* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
