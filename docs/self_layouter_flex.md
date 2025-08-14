# 弹性自身布局器 (self\_layouter\_flex) 介绍

## 概述

弹性自身布局器 (self\_layouter\_flex) 是基于 [缺省的控件自身布局器(self\_layouter\_default)](https://github.com/zlgopen/awtk/blob/master/docs/self_layouter_default.md) 布局器作扩展，用法上是兼容缺省的控件自身布局器的。

## 扩展功能

### 一、根据子控件的宽高调整自身的宽高。 

该功能可避免容器固定大小导致内容溢出或留白过多，可用于在需要动态展示内容（如列表项、弹窗内容、动态文本组合）的场景。

注意：使用该功能后，子控件只能`像素方式`布局自身，不能使用`百分比`等其他方式布局。

#### 示例：

1. 根据子控件的高度调整自身的高度：

由于只作用于 y 轴方向上，x 轴还是可以使用`百分比`等其他方式布局。

* 在 XML 界面描述文件中：

```xml
<view self_layout="flex(h=flex)">
  <button name="button1" text="button1" x="c" y="0" w="50%" h="30"/>  
  <button name="button2" text="button2" x="c" y="50" w="50%" h="40"/>
  <button name="button3" text="button3" x="c" y="100" w="50%" h="50"/>
</view>
```

* 在代码中：

```c
widget_set_self_layout(view, "flex(h=flex)");
widget_set_self_layout_params(button1, "c", "0", "50%", "30");
widget_set_self_layout_params(button2, "c", "50", "50%", "40");
widget_set_self_layout_params(button3, "c", "100", "50%", "50");
widget_layout(view);
```

可以结合 [list_view](https://github.com/zlgopen/awtk/blob/master/src/ext_widgets/scroll_view/list_view.h) 控件使用，实现列表项的自适应高度，详情请看[示例](https://github.com/zlgopen/awtk/blob/master/design/default/ui/uiex/page_button.xml)。

2. 根据子控件的宽度调整自身的宽度：

由于只作用于 x 轴方向上，y 轴还是可以使用`百分比`等其他方式布局。

* 在 XML 界面描述文件中：

```xml
<view self_layout="flex(w=flex)">
  <button name="button1" text="button1" x="0" y="m" w="30" h="50%"/>  
  <button name="button2" text="button2" x="50" y="m" w="40" h="50%"/>
  <button name="button3" text="button3" x="100" y="m" w="50" h="50%"/>
</view>
```

* 在代码中：

```c
widget_set_self_layout(view, "flex(w=flex)");
widget_set_self_layout_params(button1, "0", "m", "30", "50%");
widget_set_self_layout_params(button2, "50", "m", "40", "50%");
widget_set_self_layout_params(button3, "100", "m", "50", "50%");
widget_layout(view);
```

同样可以结合 [list_view_h](https://github.com/zlgopen/awtk/blob/master/src/ext_widgets/scroll_view/list_view_h.h) 控件使用。

3. 根据子控件的宽高调整自身的宽高：

* 在 XML 界面描述文件中：

```xml
<view self_layout="flex(w=flex,h=flex)">
  <button name="button1" text="button1" x="0" y="0" w="30" h="30"/>  
  <button name="button2" text="button2" x="50" y="50" w="40" h="40"/>
  <button name="button3" text="button3" x="100" y="50" w="50" h="50"/>
</view>
```

* 在代码中：

```c
widget_set_self_layout(view, "flex(w=flex,h=flex)");
widget_set_self_layout_params(button1, "0", "0", "30", "30");
widget_set_self_layout_params(button2, "50", "0", "40", "40");
widget_set_self_layout_params(button3, "100", "0", "50", "50");
widget_layout(view);
```

## 示例

[list_view 示例](https://github.com/zlgopen/awtk/blob/master/design/default/ui/uiex/page_button.xml)

[dialog 示例](https://github.com/zlgopen/awtk/blob/master/design/default/ui/dialog_flex.xml)
