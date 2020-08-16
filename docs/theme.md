## AWTK 中的主题

设计漂亮的界面并非程序员的强项，AWTK 通过主题提供这样一种机制，让设计漂亮的界面变得非常容易。通过主题，可以改变控件的背景颜色、边框颜色、字体颜色、字体、字体大小、背景图片、背景图片的显示方式和图标等属性。同时 AWTK 也提供了一些主题重用的机制，让主题文件的开发和维护变得容易。

### 一、主题的结构
AWTK 的主题按控件进行分类，每种控件可以有多种不同的风格，每种风格下又有不同状态下的配置。比如：

```
<button bg_image_draw_type="3patch_x" text_color="blue">
  <style name="default" border_color="#a0a0a0"  text_color="black">
    <normal     bg_color="#f0f0f0" />
    <pressed    bg_color="#c0c0c0" />
    <over       bg_color="#e0e0e0" />
    <disable    bg_color="gray" text_color="#d0d0d0" />
  </style>
  <style name="green_btn" >
    <normal   bg_image="green_btn_n" />
    <pressed  bg_image="green_btn_p" />
    <over     bg_image="green_btn_o" />
  </style>

  <style name="red_btn">
    <normal   bg_image="red_btn_n" />
    <pressed  bg_image="red_btn_p" />
    <over     bg_image="red_btn_o" />
  </style>
</button>
```

上面是按钮的主题配置（你可以自由增加自己需要的），其中定义了三种不同的按钮风格：

* default 为缺省的按钮风格。
* green_btn 是用图片实现的绿色系按钮。
* red_btn 是用图片实现的红色系按钮。

> 主题的各个属性，如果出现在控件中，则为该控件下各个 style 的缺省值。如果出现在 style 中，则为该 style 的下各种状态的缺省值。这样可以实现类似继承的重用机制。
>
> 同一控件可以出现多次（如上面的 button 出现了两次），有利于实现配置共享，让维护工作更简单。 

参考：

* [完整示例(default.xml)](https://github.com/zlgopen/awtk/blob/master/demos/assets/raw/styles/default.xml)

* [控件状态定义](manual/widget_state_t.md)

### 二、主题的属性

#### 基本属性

* bg\_color 背景颜色。
* fg\_color 前景颜色。用途视具体控件而定，如进度条已完成部分的颜色使用前景颜色。
* text\_color 文字的颜色
* tips\_text\_color 提示文字的颜色
* border\_color 边框颜色
* border 边框类型，取值为 left/right/top/bottom 和 all，以及它们的组合。
* font\_name 字体的名称。
* font\_size 字体的大小。
* font\_style 字体的风格（目前还不支持），取值为：italic/bold/underline。可用『,』分隔。
* text\_align\_h 文本的水平对齐方式。取值为 left/center/right。
* text\_align\_v 文本的垂直对齐方式。取值为 top/middle/bottom。
* bg\_image 背景图片。
* bg\_image\_draw\_type 背景图片的 [绘制方式](image_draw_type.md)。
* fg\_image 前景图片。用途视具体控件而定，如进度条已完成部分的图片使用前景图片。
* fg\_image\_draw\_type 前景图片的 [绘制方式](image_draw_type.md)。
* icon 图标。用途视具体控件而定，如 check\_button 的图标，按钮上的图标，对话框标题上的图标。
* active\_icon active 图标。用途视具体控件而定，目前 slideview 的页面指示器会用到。
* icon\_at 图标的位置，取值为 left/right/top/bottom。
* x\_offset 在 X 坐标方向上的偏移（可用来实现按下的效果）。 
* y\_offset 在 Y 坐标方向上的偏移（可用来实现按下的效果）。
* margin 边距（边距目前只影响 icon/text，不影响子控件，子控件的边距由布局算法参数决定）。
* margin_top 上边距。
* margin_bottom 下边距。
* margin_left 左边距。
* margin_right 右边距。
* spacer 间距（目前仅用于文本和图标之间）。
* round\_radius 背景和边框的圆角半径（仅在定义 WITH\_VGCANVAS 时有效）。
* border\_width 边框线宽（仅在定义 WITH\_VGCANVAS 时有效）。
* self\_layout 自身布局参数。请参考 [控件布局参数文档](layout.md)
* children\_layout 子控件布局参数。请参考 [控件布局参数文档](layout.md)

> 颜色可使用标准名称，#开头的 16 进制值和 rgba 合成的值。

参考： 

* [AWTK 中图片的绘制方式](image_draw_type.md)
* [如何在主题文件中写控件布局参数](how_to_write_layout_params_in_style.md)

#### 扩展属性

第三方扩展控件可以扩展控件特有的属性，遵循下列规则即可。

* 名称长度不超过 TK\_NAME\_LEN。
* 名称带有 color 视为颜色格式的值。
* 名称带有 name/image 视为字符串格式的值。
* 其它视为整数格式的值。

### 三、编译主题

主题用 XML 文件编写，然后用 themegen 生成 C 常量数据，并加入资源管理器，才能在程序中使用。

* 生成 C 常量数据

```
./bin/themegen input output
```

> 参考 update\_res.sh

* 加入资源管理器

```
#include "res/src/theme/default.data"

...

resource_manager_add(theme_default);
```
> 参考 demos/resource.c

### 四、为控件指定 style

* 在代码中，使用函数 widget\_use\_style 指定。

```
  ok = button_create(dialog->client, 20, 80, 80, 30);
  widget_set_text(ok, L"Go");
  widget_use_style(ok, "green_btn");
```

* 在 xml 界面描述文件中，使用属性 style 指定。

```
<dialog name="" icon="info" x="10" y="120" w="300" h="160" text="Dialog">
  <label name="" x="center" y="middle:-20" w="200" h="30" text="Are you ready?"/>
  <button name="ok" style="green_btn" x="10" y="bottom:10" w="40%" h="30" text="Yes"/>
  <button name="cancel" style="red_btn" x="right:10" y="bottom:10" w="40%" h="30" text="No"/>
</dialog>
```

### 五、每个窗口支持独立的主题

像微信小程序那样，AWTK 中每个窗口（包括对话框和其它窗口）可以有自己的主题文件。

* 通过窗口的 theme 属性来指定窗口的主题文件名（方便多个窗口共用一个主题文件）。

* 如果没有指定 theme 属性，以窗口的 name 属性作为窗口的主题文件名。

* 以窗口自己的主题文件优先，其次为缺省的主题文件。

> 参考：dialog1.xml

### 六、inline style

主题数据是只读的，它的好处是速度快，占用内存少。但在一些特殊情况下，我们希望通过函数直接修改控件的 style，或者在 UI 描述的 XML 文件中直接写 style。我们把这类 style 称为 inline style，具体用法如下：

* 在 XML UI 描述文件中使用 inline style。

控件的属性名以『style:』开头表示这是一个 inline 属性：

```
style: 状态：名称
```

下面表示设置正常状态的字体大小为 16：

```
style:normal:font_size="16"
```

状态可以省略，如果省略，表示正常状态 (normal)，下面这个和上面的功能一样：

```
style:font_size="16"
```

完整示例：

```
<label x="0" y="0" w="100%" h="100%" text="Basic Controls" style:font_size="24" style:text_color="green"/>
<button name="dec_value" text="Dec" focusable="true" style:focused:text_color="red"/>

```

* 在 C 代码中使用 inline style。

在 C 代码中可以使用下列函数设置 style：

```
/**
 * @method widget_set_style_int
 * 设置整数类型的 style。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} state_and_name 状态和名字，用英文的冒号分隔。
 * @param {int32_t} value 值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_style_int(widget_t* widget, const char* state_and_name, int32_t value);

/**
 * @method widget_set_style_str
 * 设置字符串类型的 style。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} state_and_name 状态和名字，用英文的冒号分隔。
 * @param {const char*} value 值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_style_str(widget_t* widget, const char* state_and_name, const char* value);

/**
 * @method widget_set_style_color
 * 设置颜色类型的 style。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} state_and_name 状态和名字，用英文的冒号分隔。
 * @param {uint32_t} value 值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */

```

示例：

```
widget_set_style_int(b, "font_size", 24);
widget_set_style_str(b, "style:focused:text_color", "red");
```

> inline style 会消耗更多内存，而且不方便切换主题，一般应该尽量避免使用。

### 七、相关文档

* [AWTK 中的颜色格式](color_format.md)

* [控件状态定义](manual/widget_state_t.md)
