## mledit\_t
### 概述
![image](images/mledit_t_0.png)

多行编辑器控件。

mledit\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于mledit\_t控件。

在xml中使用"mledit"标签创建多行编辑器控件。如：

```xml
<mledit x="c" y="m" w="300" h="300" />
```

> 更多用法请参考：[mledit.xml](
https://github.com/zlgopen/awtk/blob/master/design/default/ui/mledit.xml)

在c代码中使用函数mledit\_create创建多行编辑器控件。如：

```c
widget_t* tc = mledit_create(win, 10, 10, 240, 240);
```

> 完整示例请参考：[mledit demo](
https://github.com/zlgopen/awtk-c-demos/blob/master/demos/mledit.c)

time\_clock一般不需要设置style。
----------------------------------
### 函数
<p id="mledit_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#mledit_t_mledit_cast">mledit\_cast</a> | 转换为mledit对象(供脚本语言使用)。 |
| <a href="#mledit_t_mledit_create">mledit\_create</a> | 创建mledit对象 |
| <a href="#mledit_t_mledit_get_cursor">mledit\_get\_cursor</a> | 获取编辑器光标位置。 |
| <a href="#mledit_t_mledit_get_selected_text">mledit\_get\_selected\_text</a> | 获取选中的文本。 |
| <a href="#mledit_t_mledit_scroll_to_offset">mledit\_scroll\_to\_offset</a> | 设置编辑器滚动到指定偏移位置。 |
| <a href="#mledit_t_mledit_set_cancelable">mledit\_set\_cancelable</a> | 设置编辑器是否为可撤销修改。 |
| <a href="#mledit_t_mledit_set_close_im_when_blured">mledit\_set\_close\_im\_when\_blured</a> | 设置编辑器是否在失去焦点时关闭输入法。 |
| <a href="#mledit_t_mledit_set_cursor">mledit\_set\_cursor</a> | 设置编辑器光标位置。 |
| <a href="#mledit_t_mledit_set_focus">mledit\_set\_focus</a> | 设置为焦点。 |
| <a href="#mledit_t_mledit_set_keyboard">mledit\_set\_keyboard</a> | 设置自定义软键盘名称。 |
| <a href="#mledit_t_mledit_set_max_chars">mledit\_set\_max\_chars</a> | 设置编辑器的最大字符数（0 为不限制字符数）。 |
| <a href="#mledit_t_mledit_set_max_lines">mledit\_set\_max\_lines</a> | 设置编辑器的最大行数。 |
| <a href="#mledit_t_mledit_set_open_im_when_focused">mledit\_set\_open\_im\_when\_focused</a> | 设置编辑器是否在获得焦点时打开输入法。 |
| <a href="#mledit_t_mledit_set_readonly">mledit\_set\_readonly</a> | 设置编辑器是否为只读。 |
| <a href="#mledit_t_mledit_set_scroll_line">mledit\_set\_scroll\_line</a> | 设置编辑器滚动速度。 |
| <a href="#mledit_t_mledit_set_select">mledit\_set\_select</a> | 选择编辑器中指定范围的文本。 |
| <a href="#mledit_t_mledit_set_tips">mledit\_set\_tips</a> | 设置编辑器的输入提示。 |
| <a href="#mledit_t_mledit_set_tr_tips">mledit\_set\_tr\_tips</a> | 获取翻译之后的文本，然后调用mledit_set_tips。 |
| <a href="#mledit_t_mledit_set_wrap_word">mledit\_set\_wrap\_word</a> | 设置编辑器是否自动折行。 |
### 属性
<p id="mledit_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#mledit_t_cancelable">cancelable</a> | bool\_t | 是否支持撤销编辑。如果为TRUE，在失去焦点之前可以撤销所有修改(恢复获得焦点之前的内容)。 |
| <a href="#mledit_t_close_im_when_blured">close\_im\_when\_blured</a> | bool\_t | 是否在失去焦点时关闭输入法(默认是)。 |
| <a href="#mledit_t_keyboard">keyboard</a> | char* | 自定义软键盘名称。 |
| <a href="#mledit_t_max_chars">max\_chars</a> | uint32\_t | 最大字符数。 |
| <a href="#mledit_t_max_lines">max\_lines</a> | uint32\_t | 最大行数。 |
| <a href="#mledit_t_open_im_when_focused">open\_im\_when\_focused</a> | bool\_t | 获得焦点时打开输入法。 |
| <a href="#mledit_t_readonly">readonly</a> | bool\_t | 编辑器是否为只读。 |
| <a href="#mledit_t_scroll_line">scroll\_line</a> | uint32\_t | 鼠标一次滚动行数。 |
| <a href="#mledit_t_tips">tips</a> | char* | 输入提示。 |
| <a href="#mledit_t_tr_tips">tr\_tips</a> | char* | 保存用于翻译的提示信息。 |
| <a href="#mledit_t_wrap_word">wrap\_word</a> | bool\_t | 是否自动折行。 |
### 事件
<p id="mledit_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
| EVT\_VALUE\_CHANGING | event\_t | 文本正在改变事件(编辑中)。 |
| EVT\_VALUE\_CHANGED | event\_t | 文本改变事件。 |
#### mledit\_cast 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_cast">转换为mledit对象(供脚本语言使用)。

* 函数原型：

```
widget_t* mledit_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | mledit对象。 |
| widget | widget\_t* | mledit对象。 |
#### mledit\_create 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_create">创建mledit对象

* 函数原型：

```
widget_t* mledit_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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
#### mledit\_get\_cursor 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_get_cursor">获取编辑器光标位置。

* 函数原型：

```
uint32_t mledit_get_cursor (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回光标位置。 |
| widget | widget\_t* | widget对象。 |
#### mledit\_get\_selected\_text 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_get_selected_text">获取选中的文本。
使用完后需调用 TKMEM_FREE() 进行释放文本占有内存。

* 函数原型：

```
char* mledit_get_selected_text (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回选中文本。 |
| widget | widget\_t* | widget对象。 |
#### mledit\_scroll\_to\_offset 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_scroll_to_offset">设置编辑器滚动到指定偏移位置。

* 函数原型：

```
ret_t mledit_scroll_to_offset (widget_t* widget, uint32_t offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| offset | uint32\_t | 偏移位置。 |
#### mledit\_set\_cancelable 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_cancelable">设置编辑器是否为可撤销修改。

* 函数原型：

```
ret_t mledit_set_cancelable (widget_t* widget, bool_t cancelable);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| cancelable | bool\_t | 是否为可撤销修。 |
#### mledit\_set\_close\_im\_when\_blured 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_close_im_when_blured">设置编辑器是否在失去焦点时关闭输入法。

* 函数原型：

```
ret_t mledit_set_close_im_when_blured (widget_t* widget, bool_t close_im_when_blured);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| close\_im\_when\_blured | bool\_t | 是否是否在失去焦点时关闭输入法。在失去焦点时关闭输入法。 |
#### mledit\_set\_cursor 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_cursor">设置编辑器光标位置。

* 函数原型：

```
ret_t mledit_set_cursor (widget_t* widget, uint32_t cursor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| cursor | uint32\_t | 光标位置。 |
#### mledit\_set\_focus 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_focus">设置为焦点。

* 函数原型：

```
ret_t mledit_set_focus (widget_t* widget, bool_t focus);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| focus | bool\_t | 是否为焦点。 |
#### mledit\_set\_keyboard 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_keyboard">设置自定义软键盘名称。

* 函数原型：

```
ret_t mledit_set_keyboard (widget_t* widget, char* keyboard);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| keyboard | char* | 键盘名称(相应UI资源必须存在)。 |
#### mledit\_set\_max\_chars 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_max_chars">设置编辑器的最大字符数（0 为不限制字符数）。

* 函数原型：

```
ret_t mledit_set_max_chars (widget_t* widget, uint32_t max_chars);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| max\_chars | uint32\_t | 最大字符数。 |
#### mledit\_set\_max\_lines 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_max_lines">设置编辑器的最大行数。

* 函数原型：

```
ret_t mledit_set_max_lines (widget_t* widget, uint32_t max_lines);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| max\_lines | uint32\_t | 最大行数。 |
#### mledit\_set\_open\_im\_when\_focused 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_open_im_when_focused">设置编辑器是否在获得焦点时打开输入法。

> * 设置默认焦点时，打开窗口时不弹出软键盘。
> * 用键盘切换焦点时，编辑器获得焦点时不弹出软键盘。

* 函数原型：

```
ret_t mledit_set_open_im_when_focused (widget_t* widget, bool_t open_im_when_focused);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| open\_im\_when\_focused | bool\_t | 是否在获得焦点时打开输入法。 |
#### mledit\_set\_readonly 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_readonly">设置编辑器是否为只读。

* 函数原型：

```
ret_t mledit_set_readonly (widget_t* widget, bool_t readonly);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| readonly | bool\_t | 只读。 |
#### mledit\_set\_scroll\_line 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_scroll_line">设置编辑器滚动速度。

* 函数原型：

```
ret_t mledit_set_scroll_line (widget_t* widget, uint32_t scroll_line);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| scroll\_line | uint32\_t | 滚动行数。 |
#### mledit\_set\_select 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_select">选择编辑器中指定范围的文本。

* 函数原型：

```
ret_t mledit_set_select (widget_t* widget, uint32_t start, uint32_t end);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| start | uint32\_t | 起始偏移。 |
| end | uint32\_t | 结束偏移。 |
#### mledit\_set\_tips 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_tips">设置编辑器的输入提示。

* 函数原型：

```
ret_t mledit_set_tips (widget_t* widget, char* tips);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| tips | char* | 输入提示。 |
#### mledit\_set\_tr\_tips 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_tr_tips">获取翻译之后的文本，然后调用mledit_set_tips。

* 函数原型：

```
ret_t mledit_set_tr_tips (widget_t* widget, const char* tr_tips);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| tr\_tips | const char* | 提示信息。 |
#### mledit\_set\_wrap\_word 函数
-----------------------

* 函数功能：

> <p id="mledit_t_mledit_set_wrap_word">设置编辑器是否自动折行。

* 函数原型：

```
ret_t mledit_set_wrap_word (widget_t* widget, bool_t wrap_word);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| wrap\_word | bool\_t | 是否自动折行。 |
#### cancelable 属性
-----------------------
> <p id="mledit_t_cancelable">是否支持撤销编辑。如果为TRUE，在失去焦点之前可以撤销所有修改(恢复获得焦点之前的内容)。

> * 1.一般配合keyboard的"cancel"按钮使用。
> * 2.为TRUE时，如果内容有变化，会设置编辑器的状态为changed，所以此时编辑器需要支持changed状态的style。

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
#### close\_im\_when\_blured 属性
-----------------------
> <p id="mledit_t_close_im_when_blured">是否在失去焦点时关闭输入法(默认是)。

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
#### keyboard 属性
-----------------------
> <p id="mledit_t_keyboard">自定义软键盘名称。

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
#### max\_chars 属性
-----------------------
> <p id="mledit_t_max_chars">最大字符数。

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
#### max\_lines 属性
-----------------------
> <p id="mledit_t_max_lines">最大行数。

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
#### open\_im\_when\_focused 属性
-----------------------
> <p id="mledit_t_open_im_when_focused">获得焦点时打开输入法。

> 主要用于没有指针设备的情况，否则每次切换焦点时都打开输入法。

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
#### readonly 属性
-----------------------
> <p id="mledit_t_readonly">编辑器是否为只读。

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
#### scroll\_line 属性
-----------------------
> <p id="mledit_t_scroll_line">鼠标一次滚动行数。

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
#### tips 属性
-----------------------
> <p id="mledit_t_tips">输入提示。

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
#### tr\_tips 属性
-----------------------
> <p id="mledit_t_tr_tips">保存用于翻译的提示信息。

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
#### wrap\_word 属性
-----------------------
> <p id="mledit_t_wrap_word">是否自动折行。

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
