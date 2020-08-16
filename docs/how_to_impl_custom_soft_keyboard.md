# 如何实现自定义的软键盘

有时需要把软键盘嵌入到窗口内部(比如计算器和密码输入等)，这时可以使用自定义软键盘。

### 一、编辑器设置input_type为"custom"(它会禁止内置的软键盘)。

```
<edit x="c" y="10" w="90%" h="30" focused="true" input_type="custom" text="" />
```

> 如果希望初始化时编辑器自动获的焦点，可以设置focused为true。

### 二、软键盘的按钮放入一个view(任何容器控件均可)中，并将view的is\_keyboard设置为true。

```
  <view y="60" x="c" w="90%" h="-60" is_keyboard="true" 
    children_layout="default(r=4,c=4,m=5,s=5)" >
    <button name="key" text="0" />
    <button name="key" text="1" />
    <button name="key" text="2" />
    <button name="key" text="3" />
    <button name="key" text="4" />
    <button name="key" text="5" />
    <button name="key" text="6" />
    <button name="key" text="7" />
    <button name="key" text="8" />
    <button name="key" text="9" />
    <button name="key" text="#" />
    <button name="backspace" text="<=" />
  </view>
```

### 三、处理按钮事件

#### 1.处理正常按键

```
static ret_t on_send_key(void* ctx, event_t* e) {
  widget_t* button = WIDGET(e->target);
  char text[2];
  text[0] = (char)button->text.str[0];
  text[1] = '\0';

  input_method_commit_text(input_method(), text);

  return RET_OK;
}

```

#### 2.处理删除键

```
static ret_t on_backspace(void* ctx, event_t* e) {
  input_method_dispatch_key(input_method(), TK_KEY_BACKSPACE);

  return RET_OK;
}
```


>如果你不希望出现编辑器的光标，可以使用label控件代替edit控件，输入和删除时直接操作label的text。


参考：

* [demo\_ui\_app.c](https://github.com/zlgopen/awtk/blob/master/demos/demo_ui_app.c)
* [soft\_keyboard.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/soft_keyboard.xml)
