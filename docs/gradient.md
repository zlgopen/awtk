## 渐变色

渐变色在设计漂亮美观的界面时十分实用，相较于纯色，渐变色看起来更有质感，也更新潮一些。目前，AWTK 中支持将控件的背景色和前景色设置为渐变色。

### 一、主要限制

AWTK 的渐变色设置格式参考了 [HTML CSS 的格式](https://developer.mozilla.org/en-US/docs/Web/CSS/gradient/linear-gradient())，但是目前只实现了它的一个子集。主要有以下限制：

1. 只支持 180 度的 linear-gradient，也就是垂直从上到下的线性渐变。
2. 只支持两种颜色的渐变，也就是从某一种颜色渐变到另一种颜色。

需要注意的是，目前 AWTK 只支持将控件风格（样式）中的 bg\_color 属性和 fg\_color 属性设置为渐变色，即只允许控件背景色和前景色为渐变色。

> 备注：AWTK 中已实现渐变色的填充，后续会不断优化完善，支持更多的渐变色设置格式并让更多控件风格支持设置渐变色。

### 二、示例用法

#### 2.1 窗体样式文件中设置渐变色

例如，此处在窗体样式文件中创建名为 gradient_button 的按钮样式，设置其不同状态下的背景色为渐变色：

```xml
<button>
  ...
  <style name="gradient_button" border_color="#a0a0a0"  text_color="black" >
    <normal bg_color="linear-gradient(180deg, red 60%, cyan);" />
    <over bg_color="linear-gradient(180deg, orange 60%, red);" />
    <pressed bg_color="linear-gradient(180deg, orange 60%, blue);" />
    <focused bg_color="linear-gradient(180deg, orange 60%, green);" />
  </style>
</button>
```

上面按钮样式中 normal 状态的 bg_color 属性设置表示如下：

* **linear-gradient**: 表示线性渐变；
* **180deg**: 表示垂直从上到下的渐变；
* **red 60%, cyan**: 表示从高度为 60% 的位置开始红色渐变最后以为蓝绿色结束。

不填写开始高度，则默认从 0% 的位置开始，例如直接从红色垂直渐变为蓝色，代码如下：

```xml
    <normal bg_color="linear-gradient(180deg, red, cyan);" />
```

并且，开始和结束的颜色可通过颜色编码表示，代码如下：

```xml
    <normal bg_color="linear-gradient(180deg, #FF0000, #0000FF);" />
```

#### 2.2 代码中设置渐变色

在 C 代码中设置控件背景色为渐变色只需调用 widget\_set\_style\_str 接口即可，代码如下：

```c
widget_set_style_str(button, "normal:bg_color", "linear-gradient(180deg, red, blue);");
```

> 备注：调用 widget\_set\_style\_xxx 接口在代码中设置的样式为 inline style，会占用额外的内存，具体详见 [theme.md](theme.md)。
