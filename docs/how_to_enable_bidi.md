# 如何启用文本双向排版 (Unicode Bidirectional Algorithm)

## 1. 启用文本双向排版算法

定义宏 WITH\_TEXT\_BIDI 即可，嵌入式版本请修改 awtk_config.h:

```c
/**
 * 如果希望支持文字双向排版算法（如阿拉伯语言），请定义本宏。
 * 
 * #define WITH_TEXT_BIDI 1
 * 
 */
 ```

>PC　版本默认已经定义。

 ### 2. 加入相关文件

```
3rd/fribidi/fribidi-arabic.c
3rd/fribidi/fribidi-bidi-types.c
3rd/fribidi/fribidi-bidi.c
3rd/fribidi/fribidi-brackets.c
3rd/fribidi/fribidi-deprecated.c
3rd/fribidi/fribidi-joining-types.c
3rd/fribidi/fribidi-joining.c
3rd/fribidi/fribidi-mirroring.c
3rd/fribidi/fribidi-run.c
3rd/fribidi/fribidi-shape.c
3rd/fribidi/fribidi.c
src/base/bidi.c
```

>PC　版本默认已经加。

 ### 3. 控件的bidi属性

 默认情况下，文字的方向自动确定，如果在某些情况下，需要自己指定，可以通过bidi属性来实现。

 示例：

 ```xml
  <label text="1. جامعة كولومبيا عام." style:font_size="30" style:font_name="trado" bidi="lro"/>
```

bidi的可选值有：

* rtl Right-To-Left letter。
* ltr Left-To-Right letter。
* auto Auto Detection
* wrtl Weak Right To Left paragraph。
* wltr Weak Left To Right paragraph。
* lro Left-To-Right letter Override。
* rlo Right-To-Left letter Override。


