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

### 二、根据父控件剩余空间调整自身的宽高。

该功能使子控件填充父控件中的剩余空间：扣除其他非 fill 兄弟控件占据的空间、兄弟控件之间的间距以及边距之后，将剩余空间按权重比例分配给所有使用 `fill` 的兄弟控件。适用于工具栏、状态栏、自适应表单等需要某些控件自动占据剩余空间的场景。

注意：间距 `s`、边距 `x`/`y` 以及 `keep_invisible`/`keep_disable` 等参数从父控件的 [children_layout](https://github.com/zlgopen/awtk/blob/master/docs/children_layouter_default.md) 中读取，因此通常需要为父控件配置 children_layout（如 `default(r=1,c=0,s=10,x=10)`）。不可见、浮动或被 `keep_invisible`/`keep_disable` 过滤的兄弟控件不参与分配。若父控件没有 children_layout，则间距和边距按 0 处理。

#### 语法

在 `w` 或 `h` 上使用 `fill` 或 `fill:权重`，权重默认为 1，可为小数（如 `1.5`）：

* `flex(w=fill)`：宽度填充，权重为 1。
* `flex(w=fill:2)`：宽度填充，权重为 2。
* `flex(h=fill:1.5)`：高度填充，权重为 1.5。
* `flex(w=fill,h=fill:2)`：宽高分别按各自权重填充。

#### 计算方式

设父控件在该方向上的布局尺寸为 `L`，非 fill 兄弟控件在该方向占据的空间之和为 `O`，参与布局的子控件（含本控件）数量为 `N`，间距为 `s`，边距为 `m`，本控件的权重为 `W`，所有 fill 兄弟控件的权重之和为 `T`，则本控件在该方向的尺寸为：

```
size = (L - O - s * (N - 1) - m * 2) * (W / T)
```

#### 示例：

1. 多个 fill 子控件按权重平分剩余宽度：

父控件宽 410，children_layout 间距 `s=10`、边距 `x=10`；两个权重为 1 的 fill 子控件平分剩余空间：`(410 - 0 - 10*1 - 10*2) / 2 = 190`，各得 190。

* 在 XML 界面描述文件中：

```xml
<view children_layout="default(r=1,c=0,s=10,x=10)" w="410" h="300">
  <button name="button1" text="button1" self_layout="flex(w=fill)"/>
  <button name="button2" text="button2" self_layout="flex(w=fill)"/>
</view>
```

* 在代码中：

```c
widget_set_children_layout(view, "default(r=1,c=0,s=10,x=10)");
widget_set_self_layout(button1, "flex(w=fill)");
widget_set_self_layout(button2, "flex(w=fill)");
widget_layout(view);
/* button1->w == 190, button2->w == 190 */
```

2. 固定控件与 fill 控件混排，fill 吸收剩余空间：

非 fill 兄弟控件按其自身尺寸参与布局，fill 控件获得扣除它们之后的剩余空间。父控件宽 410、`s=10`、`x=10`，固定控件宽 50 与 100，fill 控件宽 = `410 - (50 + 100) - 10*2 - 10*2 = 220`。

* 在 XML 界面描述文件中：

```xml
<view children_layout="default(r=1,c=0,s=10,x=10)" w="410" h="300">
  <button name="button1" text="fixed" w="50" h="50"/>
  <button name="button2" text="fill" self_layout="flex(w=fill)"/>
  <button name="button3" text="fixed" w="100" h="50"/>
</view>
```

* 在代码中：

```c
widget_set_children_layout(view, "default(r=1,c=0,s=10,x=10)");
widget_set_self_layout_params(button1, "0", "0", "50", "50");
widget_set_self_layout(button2, "flex(w=fill)");
widget_set_self_layout_params(button3, "0", "0", "100", "50");
widget_layout(view);
/* button2->w == 220 */
```

3. 按权重分配剩余空间：

父控件宽 410、`s=0`、`x=0`（无 children_layout），固定控件宽 50 与 60，两个 fill 控件权重 1:3。剩余宽度 = `410 - 50 - 60 = 300`，按 1:3 分配 → 权重 1 得 75、权重 3 得 225。

* 在 XML 界面描述文件中：

```xml
<view w="410" h="300">
  <button name="button1" text="50" self_layout="default(x=0,y=0,w=50,h=40)"/>
  <button name="button2" text="60" self_layout="default(x=0,y=0,w=60,h=50)"/>
  <button name="button4" text="fill1" self_layout="flex(w=fill)"/>
  <button name="button5" text="fill3" self_layout="flex(w=fill:3)"/>
</view>
```

* 在代码中：

```c
widget_set_self_layout_params(button1, "0", "0", "50", "40");
widget_set_self_layout_params(button2, "0", "0", "60", "50");
widget_set_self_layout(button4, "flex(w=fill)");
widget_set_self_layout(button5, "flex(w=fill:3)");
widget_layout(view);
/* button4->w == 75, button5->w == 225 */
```

## 示例

[list_view 示例](https://github.com/zlgopen/awtk/blob/master/design/default/ui/uiex/page_button.xml)

[fill 示例](https://github.com/zlgopen/awtk/blob/master/design/default/ui/uiex/page_mledit.xml)

[dialog 示例](https://github.com/zlgopen/awtk/blob/master/design/default/ui/dialog_flex.xml)
