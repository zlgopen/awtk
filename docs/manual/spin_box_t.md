## spin\_box\_t
### 概述
![image](images/spin_box_t_0.png)

spinbox控件。

一个特殊的数值编辑器，将edit\_t和button\_t进行组合，方便编辑数值。

点击向上的按钮将数值增加一个step，点击向下的按钮将数值减小一个step。
step的值可以通过step属性进行设置。

spin_box\_t是[edit\_t](edit_t.md)的子类控件，edit\_t的函数均适用于spin\_box\_t控件。

在xml中使用"spin_box"标签创建spinbox控件。如：

```xml
<spin_box w="70%" input_type="int" min="-100" max="100" step="5">
```

>
更多用法请参考：[spin_box.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/spinbox.xml)

在c代码中使用函数spin_box\_create创建spinbox控件。如：

```c
widget_t* spin_box = spin_box_create(win, 10, 10, 128, 30);
edit_set_input_type(spin_box, type);
```

> 创建之后:
>
> 可以用edit相关函数去设置它的各种属性。

> 完整示例请参考：[spin_box
demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/spin_box.c)

可用通过style来设置控件的显示风格，如背景和边框等。如：

```xml
<spin_box>
<style name="default" border_color="#a0a0a0"  text_color="black" text_align_h="left">
<normal     bg_color="#f0f0f0" />
<focused    bg_color="#f0f0f0" border_color="black"/>
<disable    bg_color="gray" text_color="#d0d0d0" />
<error      bg_color="#f0f0f0" text_color="red" />
<empty      bg_color="#f0f0f0" text_color="#a0a0a0" />
</style>
</spin_box>
```

> 更多用法请参考：[theme
default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L128)
----------------------------------
### 函数
<p id="spin_box_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#spin_box_t_spin_box_cast">spin\_box\_cast</a> | 转换为spin_box对象(供脚本语言使用)。 |
| <a href="#spin_box_t_spin_box_create">spin\_box\_create</a> | 创建spin_box对象 |
| <a href="#spin_box_t_spin_box_get_widget_vtable">spin\_box\_get\_widget\_vtable</a> | 获取 spin_box 虚表。 |
| <a href="#spin_box_t_spin_box_set_button_position">spin\_box\_set\_button\_position</a> | 设置按钮位置样式。 |
| <a href="#spin_box_t_spin_box_set_easy_touch_mode">spin\_box\_set\_easy\_touch\_mode</a> | 设置是否启用易点击模式。 |
| <a href="#spin_box_t_spin_set_repeat">spin\_set\_repeat</a> | 设置连击的时间间隔。 |
### 属性
<p id="spin_box_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#spin_box_t_button_position">button\_position</a> | char* | 按钮位置样式选择，优先级高于easy_touch_mode，各模式对应样式如下,默认为none。 |
| <a href="#spin_box_t_easy_touch_mode">easy\_touch\_mode</a> | bool\_t | 是否启用易点击模式(在电容屏设备上建议启用)。 |
| <a href="#spin_box_t_repeat">repeat</a> | int32\_t | 重复触发EVT\_CLICK事件的时间间隔。 |
#### spin\_box\_cast 函数
-----------------------

* 函数功能：

> <p id="spin_box_t_spin_box_cast">转换为spin_box对象(供脚本语言使用)。

* 函数原型：

```
widget_t* spin_box_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | spin\_box对象。 |
| widget | widget\_t* | spin\_box对象。 |
#### spin\_box\_create 函数
-----------------------

* 函数功能：

> <p id="spin_box_t_spin_box_create">创建spin_box对象

* 函数原型：

```
widget_t* spin_box_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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
#### spin\_box\_get\_widget\_vtable 函数
-----------------------

* 函数功能：

> <p id="spin_box_t_spin_box_get_widget_vtable">获取 spin_box 虚表。

* 函数原型：

```
const widget_vtable_t* spin_box_get_widget_vtable ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const widget\_vtable\_t* | 成功返回 spin\_box 虚表。 |
#### spin\_box\_set\_button\_position 函数
-----------------------

* 函数功能：

> <p id="spin_box_t_spin_box_set_button_position">设置按钮位置样式。

* 函数原型：

```
ret_t spin_box_set_button_position (widget_t* widget, const char* button_position);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| button\_position | const char* | 按钮位置样式。 |
#### spin\_box\_set\_easy\_touch\_mode 函数
-----------------------

* 函数功能：

> <p id="spin_box_t_spin_box_set_easy_touch_mode">设置是否启用易点击模式。

* 函数原型：

```
ret_t spin_box_set_easy_touch_mode (widget_t* widget, bool_t easy_touch_mode);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| easy\_touch\_mode | bool\_t | 易点击模式。 |
#### spin\_set\_repeat 函数
-----------------------

* 函数功能：

> <p id="spin_box_t_spin_set_repeat">设置连击的时间间隔。
备注：时间间隔越低，速度越快。

* 函数原型：

```
ret_t spin_set_repeat (widget_t* widget, int32_t repeat);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| repeat | int32\_t | 连击的时间间隔。 |
#### button\_position 属性
-----------------------
> <p id="spin_box_t_button_position">按钮位置样式选择，优先级高于easy_touch_mode，各模式对应样式如下,默认为none。
none：按照easy_touch_mode选择样式
default：inc按钮在右上角，dec按钮在右下角。
left_right：dec按钮在左边，inc按钮在右边。
top_bottom：inc按钮在顶部，dec按钮在底部。

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
#### easy\_touch\_mode 属性
-----------------------
> <p id="spin_box_t_easy_touch_mode">是否启用易点击模式(在电容屏设备上建议启用)。
> 在该模式下：
> * 1.当高度大于font size的3倍时，inc按钮在顶部(style名为spinbox_top)，dec按钮在底部(style名为spinbox_bottom)。
> * 2.当高度正常时，dec按钮在左边(style名为spinbox_left)，inc按钮在右边(style名为spinbox_right)。
> 不在该模式下：
> inc按钮在右上角(style名为spinbox_up)，dec按钮在右下角(style名为spinbox_down)。

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
#### repeat 属性
-----------------------
> <p id="spin_box_t_repeat">重复触发EVT\_CLICK事件的时间间隔。

为0则不重复触发EVT\_CLICK事件。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
