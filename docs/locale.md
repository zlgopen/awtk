# 国际化之字符串翻译

支持多国语言的存储(Unicode)和显示(字体)是GUI必需要做的，但字符串的翻译完全可以用gettext等第三方库函数来实现。AWTK之所以选择自己实现，主要出于以下几点考虑：

* 减少不必要的第三方库的依赖。运行时需要的代码也就几十行，自己实现更简单代码更少。

* 方便支持实时切换当前语言。自己实现字符串的翻译，不要应用程序做额外的工作，即可实现实时切换当前语言。

## XML格式

我们采用XML文件(UTF-8)保存字符串的各个语言的对应关系，方便程序员和翻译人员进行编辑。如：

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

## 二进制格式

XML格式方便人类阅读和编辑，但不适合计算机处理。AWTK使用了二进制格式保存，每种语言的字符串放在独立的文件中，字符串按升序存储，查询时使用二分查找。

文件名(资源名)使用『语言』+'\_'+『国家/地区』，如en\_US，表示美国英语。如果不想搞得太精细，也可以只用语言名，如en，表示缺省的英语。在XML文件中的language的name中指定即可。

## 转换工具

我们提供了一个strgen的工具，它负责把XML的字符串文件转成紧凑高效的二进制文件(也可以直接转成资源常量)。

```
Usage: ./bin/strgen input outputidr [bin]
```

## 实时切换语言

为了支持实时切换语言，我们要保存没有翻译之前的字符串，这样在当前语言改变后，才能用该字符串重新进行翻译。这会有一点额外的内存开销，在PC上和手机上这点开销可以忽略不计，在低端嵌入式系统中这需要考虑了，所以AWTK提供一个宏WITH\_DYNAMIC\_TR来决定是否启用该功能。


## 动态字符串翻译

有些字符串是在运行时，用一个模板根据当时的数据动态生成的。这类字符串的翻译，需要程序员先用函数locale\_info\_tr获取当前语言的模板，生成真正要显示的字符串，再设置到控件中去。如：

```
  char str[64];
  const char* format = locale_info_tr(locale_info(), "value is %d");
  tk_snprintf(str, sizeof(str), format, value);
  widget_set_text_utf8(widget, str);

```

在切换当前语言时，这类字符串也需要特殊处理。注册窗口的的EVT\_LOCALE\_CHANGED事件的处理函数，然后做上面的处理即可：


```
static ret_t on_locale_changed(void* ctx, event_t* e) {
  ...
  return RET_OK;
}
...
widget_on(win, EVT_LOCALE_CHANGED, on_locale_changed, win);

```

## 图片翻译

在一些游戏中，有些文字是直接绘制在图片上的。在不同的语言中，需要加载不同的图片。这个实现起来并不难，AWTK目前暂时不支持，以后根据需求决定是否支持。

## 使用方法

如果控件的文本需要翻译，则通过函数widget\_set\_tr\_text设置文本(如果不需要翻译，则用widget\_set\_text设置):

```
widget_set_tr_text(radio_button, "Chinese");
```


## 示例

demotr提供了实时切换语言的示例，代码可以参考demos/demo\_tr\_app.c。运行下面的程序可以看到效果：

```
./bin/demotr
```

> 缺省的demo中并没有带完整的中文字体，如果翻译之后的文字无法显示，请把default.ttf换成自己的字体文件。


