# 如何使用软键盘上的 action 按钮

android/iphone 等手机的软键盘上有一个特殊的键，这个键的功能和文字与当前的编辑密切相关，在不同的编辑器上，可能显示“发送”、“下一个”、“回车”，“关闭”，“清除”等。

## 1. 软键盘

AWTK 也支持类似的功能，我们把它称为 action 键，在软键盘中可以这样定义：

```xml
<button name="action" style="highlight" w="20%" text="Return"/>
```

> 完整代码请参考 kb_default.xml

## 2. 编辑器

编辑器可以指定 action_text 属性，来修改软键盘上的 action 键上的文本：
```xml
<edit w="70%" right_margin="30" input_type="password" action_text="next">
```

action\_text 可以通过 strings.xml 中文本进行翻译，所以用小写英文字母即可。action\_text 有几个特殊的取值：

* next 将焦点切换到下一个控件。
* done 完成，关闭软键盘。

## 3. 事件处理

如果你想自己处理 action 按键按下的事件，可以注册编辑器的 EVT\_IM\_ACTION 事件。

如：

```c
static ret_t on_action_event(void* ctx, event_t* evt) {
  widget_t* target = WIDGET(evt->target);

  log_debug("%s action event\n", target->name);

  return RET_OK;
}

widget_on(edit1, EVT_IM_ACTION, on_action_event, NULL);
```

> 完整代码请参考：https://github.com/zlgopen/awtk-c-demos/blob/master/demos/edit.c
