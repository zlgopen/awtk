# 控件焦点相关问题

## 一、启用焦点停留 (tab stop)

除了 edit 控件外，其它控件如果需要焦点停留功能，可以指定控件的 focusable 属性为 true 来实现。

在 XML 中，可以这样指定：

```
 <button ... focusable="true"/>
```

在 C 代码中，可以这样指定：

```
 widget_set_prop_bool(widget, WIDGET_PROP_FOCUSABLE, TRUE);
```

> 如果指定了 fucusable 属性为 true，请确保控件的 style 中定义了 focused 状态的数据，否则会因为 focused 状态没有 style 数据而无法显示。如：

```
  <style name="default" border_color="#a0a0a0"  text_color="black">
    <normal     bg_color="#f0f0f0" />
    <pressed    bg_color="#c0c0c0" x_offset="1" y_offset="1"/>
    <over       bg_color="#e0e0e0" />
    <focused    bg_color="#e0e0e0" />
    <disable    bg_color="gray" text_color="#d0d0d0" />
  </style>    <focused    bg_color="#e0e0e0" />
```

## 二、前后切换焦点的按键

### 1. 缺省用 tab 键循环切换焦点。

```
#ifndef TK_KEY_MOVE_FOCUS_NEXT
#define TK_KEY_MOVE_FOCUS_NEXT "tab"
#endif /*TK_KEY_MOVE_FOCUS_NEXT*/
```

### 2. 可以为当前窗口指定的向前和向后移动焦点的键值。

* move\_focus\_prev\_key 指定向前移动焦点的键值。 
* move\_focus\_next\_key 指定向后移动焦点的键值。

```
<window anim_hint="htranslate" move_focus_prev_key="up" move_focus_next_key="down">
```

> 在这个例子中，方向键 up 移动到前一个焦点控件，方向键 down 移动到下一个焦点控件。

## 三、上下左右切换焦点的按键

在一些特殊的硬件设备上，没有触摸屏，只有上、下、左、右、确定和取消六个键。

为了快速切换焦点，AWTK 支持通过左右键切换水平焦点，通过上下键切换垂直焦点。可以通过窗口的下列属性来设置：

* move\_focus\_up\_key 向上移动焦点的键。
* move\_focus\_down\_key 向下移动焦点的键。
* move\_focus\_left\_key 向左移动焦点的键。
* move\_focus\_right\_key 向右移动焦点的键。

示例：

```
<window text="Custom Soft Keyboard" anim_hint="htranslate"
  move_focus_up_key="up" move_focus_down_key="down" move_focus_left_key="left" move_focus_right_key="right">

  <edit name="edit" x="c" y="10" w="90%" h="30" focused="true" input_type="custom" text="" tips="custom"/>
  <view y="60" x="c" w="90%" h="-60" is_keyboard="true" grab_keys="true"
    children_layout="default(r=4,c=4,m=5,s=5)" >
    <button focusable="true" name="key0" text="0" />
    <button focusable="true" name="key1" text="1" />
    <button focusable="true" name="key2" text="2" />
    <button focusable="true" name="key3" text="3" />
    <button focusable="true" name="key4" text="4" />
    <button focusable="true" name="key5" text="5" />
    <button focusable="true" name="key6" text="6" />
    <button focusable="true" name="key7" text="7" />
    <button focusable="true" name="key8" text="8" />
    <button focusable="true" name="key9" text="9" />
    <button focusable="true" name="key#" text="#" />
    <button focusable="true" name="backspace" text="<=" />
  </view>
</window>
```

> 在这个例子中，方向键 up 移动到上方的焦点控件，方向键 down 移动到下方的焦点控件。
> 方向键 left 移动到左方的焦点控件，方向键 right 移动到右方的焦点控件。

> 软键盘本身不能得到焦点，为了收到按键消息，需要指定属性 grab_keys="true"。

## 四、设置初始焦点

可以指定控件的 focused 属性为 true 将控件设置为初始焦点控件。

在 XML 中，可以这样指定：

```
 <button ... focused="true"/>
```

在 C 中，可以这样指定：

```
 widget_set_prop_bool(widget, WIDGET_PROP_FOCUSED, TRUE);
```

## 五、参考
 
* [键名的定义：https://github.com/zlgopen/awtk/blob/master/src/base/enums.c](https://github.com/zlgopen/awtk/blob/master/src/base/enums.c#L99)。

* [键值映射：https://github.com/zlgopen/awtk/blob/master/docs/map_key.md](https://github.com/zlgopen/awtk/blob/master/docs/map_key.md)
