# 如何设置字体的全局缩放比例

有时需要支持 Accessibility，要求对字体进行全局缩放，以便实现"大字体"和"小字体"的选项。在 AWTK 中，可以调用函数 system\_info\_set\_font\_scale 来实现。

```
system_info_set_font_scale(system_info(), font_scale);
```

font\_scale 的取值：

* 1 表示正常大小。
* 大于 1 表示放大。
* 小于 1 表示缩小。
