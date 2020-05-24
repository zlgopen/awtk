## text\_edit\_t
### 概述

----------------------------------
### 函数
<p id="text_edit_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#text_edit_t_text_edit_click">text\_edit\_click</a> | 处理点击事件。 |
| <a href="#text_edit_t_text_edit_copy">text\_edit\_copy</a> | 拷贝文本。 |
| <a href="#text_edit_t_text_edit_create">text\_edit\_create</a> | 创建text_edit对象 |
| <a href="#text_edit_t_text_edit_cut">text\_edit\_cut</a> | 剪切文本。 |
| <a href="#text_edit_t_text_edit_destroy">text\_edit\_destroy</a> | 销毁text_edit对象。 |
| <a href="#text_edit_t_text_edit_drag">text\_edit\_drag</a> | 处理拖动事件。 |
| <a href="#text_edit_t_text_edit_get_cursor">text\_edit\_get\_cursor</a> | 获取光标位置。 |
| <a href="#text_edit_t_text_edit_get_state">text\_edit\_get\_state</a> | 获取编辑器的状态。 |
| <a href="#text_edit_t_text_edit_invert_caret_visible">text\_edit\_invert\_caret\_visible</a> | 如果caret可见，将其设置为不可见。 如果caret不可见，将其设置为可见。 |
| <a href="#text_edit_t_text_edit_key_down">text\_edit\_key\_down</a> | 处理按键事件。 |
| <a href="#text_edit_t_text_edit_key_up">text\_edit\_key\_up</a> | 处理按键事件。 |
| <a href="#text_edit_t_text_edit_layout">text\_edit\_layout</a> | 重新排版。 |
| <a href="#text_edit_t_text_edit_paint">text\_edit\_paint</a> | 绘制文本。 |
| <a href="#text_edit_t_text_edit_paste">text\_edit\_paste</a> | 粘贴文本。 |
| <a href="#text_edit_t_text_edit_preedit">text\_edit\_preedit</a> | 进入预编辑状态。 |
| <a href="#text_edit_t_text_edit_preedit_abort">text\_edit\_preedit\_abort</a> | 取消预编辑的文本，并退出预编辑状态。 |
| <a href="#text_edit_t_text_edit_preedit_clear">text\_edit\_preedit\_clear</a> | 清除预编辑文本。 |
| <a href="#text_edit_t_text_edit_preedit_confirm">text\_edit\_preedit\_confirm</a> | 提交预编辑的文本，并退出预编辑状态。 |
| <a href="#text_edit_t_text_edit_select_all">text\_edit\_select\_all</a> | 全选。 |
| <a href="#text_edit_t_text_edit_set_canvas">text\_edit\_set\_canvas</a> | 设置canvas对象。 |
| <a href="#text_edit_t_text_edit_set_caret_visible">text\_edit\_set\_caret\_visible</a> | 设置光标的看见性。 |
| <a href="#text_edit_t_text_edit_set_cursor">text\_edit\_set\_cursor</a> | 设置光标位置。 |
| <a href="#text_edit_t_text_edit_set_mask">text\_edit\_set\_mask</a> | 设置是否马赛克字符(用于密码)。 |
| <a href="#text_edit_t_text_edit_set_mask_char">text\_edit\_set\_mask\_char</a> | 设置马赛克字符。 |
| <a href="#text_edit_t_text_edit_set_max_rows">text\_edit\_set\_max\_rows</a> | 设置最大行数。 |
| <a href="#text_edit_t_text_edit_set_offset">text\_edit\_set\_offset</a> | 设置滚动偏移。 |
| <a href="#text_edit_t_text_edit_set_on_state_changed">text\_edit\_set\_on\_state\_changed</a> | 设置状态改变回调函数。 |
| <a href="#text_edit_t_text_edit_set_select">text\_edit\_set\_select</a> | 选择指定范围的文本。 |
| <a href="#text_edit_t_text_edit_set_tips">text\_edit\_set\_tips</a> | 设置提示信息。 |
| <a href="#text_edit_t_text_edit_set_wrap_word">text\_edit\_set\_wrap\_word</a> | 设置是否自动折行。 |
| <a href="#text_edit_t_text_edit_unselect">text\_edit\_unselect</a> | 取消选择。 |
#### text\_edit\_click 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_click">处理点击事件。

* 函数原型：

```
ret_t text_edit_click (text_edit_t* text_edit, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### text\_edit\_copy 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_copy">拷贝文本。

* 函数原型：

```
ret_t text_edit_copy (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_create 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_create">创建text_edit对象

* 函数原型：

```
widget_t* text_edit_create (widget_t* widget, boo_t single_line);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| widget | widget\_t* | 控件 |
| single\_line | boo\_t | 是否是单行编辑器。 |
#### text\_edit\_cut 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_cut">剪切文本。

* 函数原型：

```
ret_t text_edit_cut (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_destroy 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_destroy">销毁text_edit对象。

* 函数原型：

```
ret_t text_edit_destroy (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_drag 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_drag">处理拖动事件。

* 函数原型：

```
ret_t text_edit_drag (text_edit_t* text_edit, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### text\_edit\_get\_cursor 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_get_cursor">获取光标位置。

* 函数原型：

```
uin32_t text_edit_get_cursor (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uin32\_t | 返回光标的位置。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_get\_state 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_get_state">获取编辑器的状态。

* 函数原型：

```
ret_t text_edit_get_state (text_edit_t* text_edit, text_edit_state_t* state);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| state | text\_edit\_state\_t* | 状态。 |
#### text\_edit\_invert\_caret\_visible 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_invert_caret_visible">如果caret可见，将其设置为不可见。 如果caret不可见，将其设置为可见。

* 函数原型：

```
ret_t text_edit_invert_caret_visible (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_key\_down 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_key_down">处理按键事件。

* 函数原型：

```
ret_t text_edit_key_down (text_edit_t* text_edit, key_event_t* evt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| evt | key\_event\_t* | event |
#### text\_edit\_key\_up 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_key_up">处理按键事件。

* 函数原型：

```
ret_t text_edit_key_up (text_edit_t* text_edit, key_event_t* evt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| evt | key\_event\_t* | event |
#### text\_edit\_layout 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_layout">重新排版。

* 函数原型：

```
ret_t text_edit_layout (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_paint 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_paint">绘制文本。

* 函数原型：

```
ret_t text_edit_paint (text_edit_t* text_edit, canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| c | canvas\_t* | canvas对象。 |
#### text\_edit\_paste 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_paste">粘贴文本。

* 函数原型：

```
ret_t text_edit_paste (text_edit_t* text_edit, const wchar_t* str, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| str | const wchar\_t* | 文本。 |
| size | uint32\_t | 文本长度。 |
#### text\_edit\_preedit 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_preedit">进入预编辑状态。

* 函数原型：

```
ret_t text_edit_preedit (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_preedit\_abort 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_preedit_abort">取消预编辑的文本，并退出预编辑状态。

* 函数原型：

```
ret_t text_edit_preedit_abort (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_preedit\_clear 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_preedit_clear">清除预编辑文本。

* 函数原型：

```
ret_t text_edit_preedit_clear (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_preedit\_confirm 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_preedit_confirm">提交预编辑的文本，并退出预编辑状态。

* 函数原型：

```
ret_t text_edit_preedit_confirm (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_select\_all 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_select_all">全选。

* 函数原型：

```
ret_t text_edit_select_all (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
#### text\_edit\_set\_canvas 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_canvas">设置canvas对象。

* 函数原型：

```
ret_t text_edit_set_canvas (text_edit_t* text_edit, canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| c | canvas\_t* | canvas对象。 |
#### text\_edit\_set\_caret\_visible 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_caret_visible">设置光标的看见性。

* 函数原型：

```
ret_t text_edit_set_caret_visible (text_edit_t* text_edit, bool_t caret_visible);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| caret\_visible | bool\_t | 是否可见。 |
#### text\_edit\_set\_cursor 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_cursor">设置光标位置。

* 函数原型：

```
ret_t text_edit_set_cursor (text_edit_t* text_edit, uint32_t cursor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| cursor | uint32\_t | 光标偏移。 |
#### text\_edit\_set\_mask 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_mask">设置是否马赛克字符(用于密码)。

* 函数原型：

```
ret_t text_edit_set_mask (text_edit_t* text_edit, bool_t mask);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| mask | bool\_t | 是否马赛克字符。 |
#### text\_edit\_set\_mask\_char 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_mask_char">设置马赛克字符。

* 函数原型：

```
ret_t text_edit_set_mask_char (text_edit_t* text_edit, wchar_t mask_char);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| mask\_char | wchar\_t | 马赛克字符。 |
#### text\_edit\_set\_max\_rows 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_max_rows">设置最大行数。

* 函数原型：

```
ret_t text_edit_set_max_rows (text_edit_t* text_edit, uint32_t max_rows);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| max\_rows | uint32\_t | 最大行数。 |
#### text\_edit\_set\_offset 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_offset">设置滚动偏移。

* 函数原型：

```
ret_t text_edit_set_offset (text_edit_t* text_edit, int32_t ox, int32_t oy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| ox | int32\_t | x偏移量。 |
| oy | int32\_t | y偏移量。 |
#### text\_edit\_set\_on\_state\_changed 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_on_state_changed">设置状态改变回调函数。

* 函数原型：

```
ret_t text_edit_set_on_state_changed (text_edit_t* text_edit, text_edit_on_state_changed_t on_state_changed, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| on\_state\_changed | text\_edit\_on\_state\_changed\_t | 回调函数。 |
| ctx | void* | 回调函数上下文。 |
#### text\_edit\_set\_select 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_select">选择指定范围的文本。

* 函数原型：

```
ret_t text_edit_set_select (text_edit_t* text_edit, uint32_t start, uint32_t end);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| start | uint32\_t | 起始偏移。 |
| end | uint32\_t | 结束偏移。 |
#### text\_edit\_set\_tips 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_tips">设置提示信息。

* 函数原型：

```
ret_t text_edit_set_tips (text_edit_t* text_edit, const char* tips);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| tips | const char* | 提示信息。 |
#### text\_edit\_set\_wrap\_word 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_set_wrap_word">设置是否自动折行。

* 函数原型：

```
ret_t text_edit_set_wrap_word (text_edit_t* text_edit, bool_t wrap_word);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
| wrap\_word | bool\_t | 是否自动折行。 |
#### text\_edit\_unselect 函数
-----------------------

* 函数功能：

> <p id="text_edit_t_text_edit_unselect">取消选择。

* 函数原型：

```
ret_t text_edit_unselect (text_edit_t* text_edit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| text\_edit | text\_edit\_t* | text\_edit对象。 |
