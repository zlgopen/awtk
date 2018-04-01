## LFTK中的主题

设计漂亮的界面并非程序员的强项，LFTK通过主题提供这样一种机制，让设计漂亮的界面变得非常容易。通过主题，可以改变控件的背景颜色、边框颜色、字体颜色、字体、字体大小、背景图片、背景图片的显示方式和图标等属性。同时LFTK也提供了一些主题重用的机制，让主题文件的开发和维护变得容易。

### 一、主题的结构
LFTK的主题按控件进行分类，每种控件可以有多种不同的风格，每种风格下又有不同状态下的配置。比如：

```
<button>
  <style name="default" border-color="#a0a0a0"  text-color="black">
    <normal     bg-color="#f0f0f0" />
    <pressed    bg-color="#c0c0c0" />
    <over       bg-color="#e0e0e0" />
    <disable    bg-color="gray" text-color="#d0d0d0" />
  </style>
</button>

<button bg-image-draw-type="3patch_x" text-color="white">
  <style name="1:green_btn" >
    <normal   bg-image="green_btn_n" />
    <pressed  bg-image="green_btn_p" />
    <over     bg-image="green_btn_o" />
  </style>

  <style name="2:red_btn">
    <normal   bg-image="red_btn_n" />
    <pressed  bg-image="red_btn_p" />
    <over     bg-image="red_btn_o" />
  </style>
</button>
```

上面是按钮的主题配置(你可以自由增加自己需要的)，其中定义了三种不同的按钮风格：

* default 为缺省的按钮风格。
* 1:green_btn 是用图片实现的绿色系按钮。
* 2:red_btn 是用图片实现的红色系按钮。

> style的name分为用冒号分隔的两部分，前半部分为ID，它只能是1-255之间的整数，而且不能重复。后半部分为注释，只是给人看的。这样做的目的是，实现主题高效存储和查找的同时，保持文件的可读性。
>
> 主题的各个属性，如果出现在控件中，则为该控件下各个style的缺省值。如果出现在style中，则为该style的下各种状态的缺省值。这样可以实现类似继承的重用机制。
>
> 同一控件可以出现多次(如上面的button出现了两次)，有利于实现配置共享，让维护工作更简单。 

参考：demos/res/raw/theme/theme.xml

### 二、主题的属性

* bg\-color 背景颜色。
* fg\-color 前景颜色。用途视具体控件而定，如进度条已完成部分的颜色使用前景颜色。
* text\-color 文字的颜色
* font\-name 字体的名称。
* font\-size 字体的大小。
* border\-color 边框的颜色。
* font\-style 字体的风格(目前还不支持)，取值为italic/bold/underline。可用『,』分隔。
* text\-align\-h 文本的水平对齐方式。取值为left/center/right。
* text\-align\-v 文本的垂直对齐方式。取值为top/middle/bottom。
* bg\-image 背景图片。
* bg\-image\-draw\-type 背景图片的绘制方式。
* fg\-image 前景图片。用途视具体控件而定，如进度条已完成部分的图片使用前景图片。
* fg\-image\-draw\-type 前景图片的绘制方式。
* icon 图标。用途视具体控件而定，如check\_button的图标，按钮上的图标，对话框标题上的图标。
* margin 边距。

> 颜色可使用标准名称，#开头的16进制值和rgba合成的值。

参考： [LFTK中图片的绘制方式](docs/image_draw_type.md)

### 三、编译主题

主题用XML文件编写，然后用themegen生成C常量数据，并加入资源管理器，才能在程序中使用。

* 生成C常量数据

```
./bin/themegen input output
```

> 参考update\_res.sh

* 加入资源管理器

```
#include "res/src/theme/default.data"

...

resource_manager_add(theme_default);
```
> 参考 demos/resource.c

### 四、为控件指定style

* 在代码中，使用函数widget\_use\_style指定。

```
  ok = button_create(dialog->client, 20, 80, 80, 30);
  widget_set_text(ok, L"Go");
  widget_use_style(ok, "1:green_btn");
```

* 在xml界面描述文件中，使用属性style指定。

```
<dialog name="" icon="info" x="10" y="120" w="300" h="160" text="Dialog">
  <label name="" x="center" y="middle:-20" w="200" h="30" text="Are you ready?"/>
  <button name="ok" style="1:green_btn" x="10" y="bottom:10" w="40%" h="30" text="Yes"/>
  <button name="cancel" style="2:red_btn" x="right:10" y="bottom:10" w="40%" h="30" text="No"/>
</dialog>
```


