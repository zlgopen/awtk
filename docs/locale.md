# 国际化之字符串翻译

字符串翻译是 GUI 国际化时必须实现的功能之一，AWTK 没有选择 gettext 等第三方库，而是选择自己实现，主要出于以下几点考虑：

* 减少不必要的第三方库的依赖。运行时需要的代码也就几十行，自己实现更简单代码更少。

* 方便支持实时切换当前语言。自己实现字符串的翻译，不需要应用程序做额外的工作，即可实现实时切换当前语言。

## 一、XML 格式

我们采用 XML 文件 (UTF-8) 保存字符串的各个语言的对应关系，方便程序员和翻译人员进行编辑。如：

```
<string name="ok">
<language name="en_US">OK</language>
<language name="zh_CN">确定</language>
</string>

<string name="cancel">
<language name="en_US">Cancel</language>
<language name="zh_CN">取消</language>
</string>

<string name="value is %d">
<language name="en_US">value is %d</language>
<language name="zh_CN">值为%d</language>
</string>
```

> 在实际工作中，由程序员提供一个模板，让翻译人员翻译，翻译好之后交给程序员放到项目中。

## 二、二进制格式

XML 格式方便人类阅读和编辑，但不适合计算机处理。AWTK 使用了二进制格式保存，每种语言的字符串放在独立的文件中，字符串按升序存储，查询时使用二分查找。

文件名（资源名）使用『语言』+'\_'+『国家/地区』，如 en\_US，表示美国英语。如果不想搞得太精细，也可以只用语言名，如 en，表示缺省的英语。在 XML 文件中的 language 的 name 中指定即可。

## 三、转换工具

我们提供了一个 strgen 的工具，它负责把 XML 的字符串文件转成紧凑高效的二进制文件（也可以直接转成资源常量）。

```
Usage: ./bin/strgen input outputidr [bin]
```

## 四、实时切换语言

为了支持实时切换语言，我们要保存没有翻译之前的字符串，这样在当前语言改变后，才能用该字符串重新进行翻译。这会有一点额外的内存开销，在 PC 上和手机上这点开销可以忽略不计，在低端嵌入式系统中这需要考虑了，所以 AWTK 提供一个宏 WITH\_DYNAMIC\_TR 来决定是否启用该功能。

## 五、动态字符串翻译

有些字符串是在运行时动态生成的，用一个模板根据当时的数据动态生成。这类字符串的翻译，需要程序员先用函数 locale\_info\_tr 获取当前语言的模板，生成真正要显示的字符串，再设置到控件中去。如：

```
  char str[64];
  const char* format = locale_info_tr(locale_info(), "value is %d");
  tk_snprintf(str, sizeof(str), format, value);
  widget_set_text_utf8(widget, str);

```

在切换当前语言时，这类字符串也需要特殊处理。注册窗口的的 EVT\_LOCALE\_CHANGED 事件的处理函数，然后做上面的处理即可：

```
static ret_t on_locale_changed(void* ctx, event_t* e) {
  ...
  return RET_OK;
}
...
widget_on(win, EVT_LOCALE_CHANGED, on_locale_changed, win);

```

## 六、图片翻译

在一些应用程序中，有些文字是直接绘制在图片上的。所以在切换到不同的语言时，需要加载不同的图片。这时只要在图片名称中包含『$locale$』即可，加载时自动替换成当前的语言。

如：图片名称为『flag\_$locale$』，当前语言为 en\_US，加载图片时会按下列顺序查找：

* flag\_en\_US
* flag_en
* flag_

XML 中的用法：

```
<image style="border" image="flag_$locale$" draw_type="icon" />
```

```
image_set_image(image, "flag_$locale$");
```

> 也可以使用 [资源名称的高级用法](assets_manager.md) 来实现同样的功能。

## 七、数字时钟月份和星期的翻译
 
星期的格式用"W"，如：

```
<digit_clock style="green" format="W"/>
```

月份的格式用"MMM"，如：
```
<digit_clock style="green" format="MMM"/>
```

星期要翻译的字符串如下：

```
<string name="Mon">
<language name="en_US">Mon</language>
<language name="zh_CN">星期一</language>
</string>

<string name="Tues">
<language name="en_US">Tues</language>
<language name="zh_CN">星期二</language>
</string>

<string name="Wed">
<language name="en_US">Wed</language>
<language name="zh_CN">星期三</language>
</string>

<string name="Thur">
<language name="en_US">Thur</language>
<language name="zh_CN">星期四</language>
</string>

<string name="Fri">
<language name="en_US">Fri</language>
<language name="zh_CN">星期五</language>
</string>

<string name="Sat">
<language name="en_US">Sat</language>
<language name="zh_CN">星期六</language>
</string>

<string name="Sun">
<language name="en_US">Sun</language>
<language name="zh_CN">星期天</language>
</string>
```

月份要翻译的字符串如下：

```
<string name="Jan">
<language name="en_US">Jan</language>
<language name="zh_CN">一月</language>
</string>

<string name="Feb">
<language name="en_US">Feb</language>
<language name="zh_CN">二月</language>
</string>

<string name="Mar">
<language name="en_US">Mar</language>
<language name="zh_CN">三月</language>
</string>

<string name="Apr">
<language name="en_US">Apr</language>
<language name="zh_CN">四月</language>
</string>

<string name="May">
<language name="en_US">May</language>
<language name="zh_CN">五月</language>
</string>

<string name="Jun">
<language name="en_US">Jun</language>
<language name="zh_CN">六月</language>
</string>

<string name="Jul">
<language name="en_US">Jul</language>
<language name="zh_CN">七月</language>
</string>

<string name="Aug">
<language name="en_US">Aug</language>
<language name="zh_CN">八月</language>
</string>

<string name="Sept">
<language name="en_US">Sept</language>
<language name="zh_CN">九月</language>
</string>

<string name="Oct">
<language name="en_US">Oct</language>
<language name="zh_CN">十月</language>
</string>

<string name="Nov">
<language name="en_US">Nov</language>
<language name="zh_CN">十一月</language>
</string>

<string name="Dec">
<language name="en_US">Dec</language>
<language name="zh_CN">十二月</language>
</string>
```

## 八、使用方法

* XML 中用 tr_text 属性指定。如：

```
  <label ... tr_text="ok"/>
```

* C 代码通过函数 widget\_set\_tr\_text 设置文本（如果不需要翻译，则用 widget\_set\_text 设置）:

```
widget_set_tr_text(radio_button, "Chinese");
```

## 九、示例

demotr 提供了实时切换语言的示例，代码可以参考 demos/demo\_tr\_app.c。运行下面的程序可以看到效果：

```
./bin/demotr
```

> 缺省的 demo 中并没有带完整的中文字体，如果翻译之后的文字无法显示，请把 default.ttf 换成自己的字体文件。

## 十、注意事项

* 1. 在源文件中最好不要用非英文字符串。如果一定要用，源文件的格式一定要用 UTF8-BOM 编码，这个可以用 vscode 或记事本转换，否则在 Windows 平台可能出现乱码。

* 2. 在 XML 中，以下字符需要转义成 HTML 实体

|  字符   | 转义实体  |
|  ----  | ----  |
|  <  | &amp;lt; |
|  >  | &amp;gt; |
|  "  | &amp;quot; |

示例：

```xml
<string name="&lt;&quot;en&amp;zh&quot;&gt;">
<language name="en_US">&lt;&quot;en&amp;zh&quot;&gt;</language>
<language name="zh_CN">&lt;&quot;英文&amp;中文&quot;&gt;</language>
</string>
```


