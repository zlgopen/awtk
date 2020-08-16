# 如何设置字体的全局缩放比例

有时需要支持Accessibility，要求对字体进行全局缩放，以便实现"大字体"和"小字体"的选项。在AWTK中，可以调用函数system\_info\_set\_font\_scale来实现。

```
system_info_set_font_scale(system_info(), font_scale);
```

font\_scale的取值：

* 1 表示正常大小。
* 大于1 表示放大。
* 小于1 表示缩小。
