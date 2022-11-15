# Scalable Vector Graphics

AWTK把SVG转换成二进制格式的BSVG，可以大幅提升解析速度和降低文件体积。

支持SVGTiny1.1标准，参考 https://www.w3.org/TR/SVGTiny ，其中部分特性不支持，部分特性提供额外支持。

## 以下元素不支持：

* 不支持use元素
* 不支持a元素
* 不支持switch元素
* 不支持image元素
* 不支持foreignObject元素

## 以下属性不支持：

* svg元素不支持preserveAspectRatio、zoomAndPan属性
* path、rect、circle、ellipse、line、polyline、polygon元素不支持fill-rule、stroke-linecap、stroke-linejoin、stroke-miterlimit、stroke-dasharray、stroke-dashoffset、display、visibility属性
* rect元素不支持rx、ry属性
* text元素不支持transform、rotate、text-anchor、font-family、font-size、font-style、font-weight、xml:space属性
* text元素的x、y属性，不支持多个数值
* 颜色属性（如fill、stroke），不支持currentColor、系统颜色

## 以下功能不支持：

* 不支持字体
* 不支持动画
* 不支持交互
* **path元素显示时，不支持镂空**
* XML语法不支持ENTITY

## 以下功能额外支持：

* path、rect、circle、ellipse、line、polyline、polygon、text元素支持style属性
* path、rect、circle、ellipse、line、polyline、polygon元素的fill属性，支持渐变
* path元素的d属性，支持A/a命令

## 渐变支持：

* 支持linearGradient元素
* linearGradient元素支持x1、y1、x2、y2、gradientUnits、xlink:href属性
* **每个渐变支持最多2个stop元素**
* stop元素支持offset、stop-color属性


