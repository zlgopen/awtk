## dialog\_highlighter\_t
### 概述
对话框高亮策略。

> 高亮策略的基本思路是对背景进行处理，比如将背景变暗或变模糊。
----------------------------------
### 函数
<p id="dialog_highlighter_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#dialog_highlighter_t_dialog_highlighter_create">dialog\_highlighter\_create</a> | 创建对话框高亮策略对象。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_destroy">dialog\_highlighter\_destroy</a> | 销毁对话框高亮策略对象。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_draw">dialog\_highlighter\_draw</a> | 绘制背景。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_draw_mask">dialog\_highlighter\_draw\_mask</a> | 绘制背景高亮部分。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_get_alpha">dialog\_highlighter\_get\_alpha</a> | 获取高亮的透明度。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_is_dynamic">dialog\_highlighter\_is\_dynamic</a> | 是否是动态绘制(方便外层优化)。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_prepare">dialog\_highlighter\_prepare</a> | 初始化。在绘制完背景，在截图前调用。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_prepare_ex">dialog\_highlighter\_prepare\_ex</a> | 初始化。在绘制完背景，在截图前调用。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_set_bg">dialog\_highlighter\_set\_bg</a> | 设置背景图片。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_set_bg_clip_rect">dialog\_highlighter\_set\_bg\_clip\_rect</a> | 设置背景图片的显示裁减区。 |
| <a href="#dialog_highlighter_t_dialog_highlighter_set_system_bar_alpha">dialog\_highlighter\_set\_system\_bar\_alpha</a> | 设置 sytem_bar 的高亮透明值。 |
### 属性
<p id="dialog_highlighter_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#dialog_highlighter_t_canvas">canvas</a> | canvas\_t* | 画布。 |
| <a href="#dialog_highlighter_t_clip_rect">clip\_rect</a> | rect\_t* | 截图的显示裁减区 |
| <a href="#dialog_highlighter_t_dialog">dialog</a> | widget\_t* | 对应的对话框。 |
| <a href="#dialog_highlighter_t_img">img</a> | bitmap\_t | 底层窗口的截图。 |
#### dialog\_highlighter\_create 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_create">创建对话框高亮策略对象。

>供子类构造函数用。

* 函数原型：

```
dialog_highlighter_t* dialog_highlighter_create (const dialog_highlighter_vtable_t* vt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | dialog\_highlighter\_t* | 返回对话框高亮策略对象 |
| vt | const dialog\_highlighter\_vtable\_t* | 虚表对象。 |
#### dialog\_highlighter\_destroy 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_destroy">销毁对话框高亮策略对象。

* 函数原型：

```
ret_t dialog_highlighter_destroy (dialog_highlighter_t* h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
#### dialog\_highlighter\_draw 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_draw">绘制背景。

* 函数原型：

```
ret_t dialog_highlighter_draw (dialog_highlighter_t* h, float_t percent);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
| percent | float\_t | 动画进度(0-1)，1表示打开已经完成。 |
#### dialog\_highlighter\_draw\_mask 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_draw_mask">绘制背景高亮部分。

* 函数原型：

```
ret_t dialog_highlighter_draw_mask (dialog_highlighter_t* h, canvas_t* c, float_t percent);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
| c | canvas\_t* | 画布。 |
| percent | float\_t | 高亮的百分比。 |
#### dialog\_highlighter\_get\_alpha 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_get_alpha">获取高亮的透明度。

* 函数原型：

```
uint8_t dialog_highlighter_get_alpha (dialog_highlighter_t* h, float_t percent);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t | 返回透明度。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
| percent | float\_t | 高亮的百分比。 |
#### dialog\_highlighter\_is\_dynamic 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_is_dynamic">是否是动态绘制(方便外层优化)。

* 函数原型：

```
bool_t dialog_highlighter_is_dynamic (dialog_highlighter_t* h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示动态绘制，否则表示不是动态绘制。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
#### dialog\_highlighter\_prepare 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_prepare">初始化。在绘制完背景，在截图前调用。

* 函数原型：

```
ret_t dialog_highlighter_prepare (dialog_highlighter_t* h, canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
| c | canvas\_t* | 画布。 |
#### dialog\_highlighter\_prepare\_ex 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_prepare_ex">初始化。在绘制完背景，在截图前调用。

* 函数原型：

```
ret_t dialog_highlighter_prepare_ex (dialog_highlighter_t* h, canvas_t* c, canvas_t* canvas_offline);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
| c | canvas\_t* | 画布。 |
| canvas\_offline | canvas\_t* | 离线画布。 |
#### dialog\_highlighter\_set\_bg 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_set_bg">设置背景图片。

* 函数原型：

```
ret_t dialog_highlighter_set_bg (dialog_highlighter_t* h, bitmap_t* img);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
| img | bitmap\_t* | 背景截图。 |
#### dialog\_highlighter\_set\_bg\_clip\_rect 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_set_bg_clip_rect">设置背景图片的显示裁减区。

* 函数原型：

```
ret_t dialog_highlighter_set_bg_clip_rect (dialog_highlighter_t* h, rect_t* clip_rect);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
| clip\_rect | rect\_t* | 背景显示裁减区。 |
#### dialog\_highlighter\_set\_system\_bar\_alpha 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_t_dialog_highlighter_set_system_bar_alpha">设置 sytem_bar 的高亮透明值。

* 函数原型：

```
ret_t dialog_highlighter_set_system_bar_alpha (dialog_highlighter_t* h, uint8_t alpha);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| h | dialog\_highlighter\_t* | 对话框高亮策略对象。 |
| alpha | uint8\_t | 设置 sytem\_bar 的高亮透明值。 |
#### canvas 属性
-----------------------
> <p id="dialog_highlighter_t_canvas">画布。

* 类型：canvas\_t*

#### clip\_rect 属性
-----------------------
> <p id="dialog_highlighter_t_clip_rect">截图的显示裁减区

* 类型：rect\_t*

#### dialog 属性
-----------------------
> <p id="dialog_highlighter_t_dialog">对应的对话框。

* 类型：widget\_t*

#### img 属性
-----------------------
> <p id="dialog_highlighter_t_img">底层窗口的截图。

* 类型：bitmap\_t

