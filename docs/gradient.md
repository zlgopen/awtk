# 渐变色

AWTK 渐变色格式兼容 [HTML CSS 的格式](https://developer.mozilla.org/en-US/docs/Web/CSS/gradient/linear-gradient()), 但是目前只实现了它的一个子集。主要限制有：

* 只支持 180 度的 linear-gradient，也就是垂直从上到下的渐变。

```xml
  <style name="close" border_color="#a0a0a0"  text_color="black" >
    <normal bg_color="linear-gradient(180deg, red 60%, cyan);" />
    <over bg_color="linear-gradient(180deg, orange 60%, red);" />
    <pressed bg_color="linear-gradient(180deg, orange 60%, blue);" />
    <focused bg_color="linear-gradient(180deg, orange 60%, green);" />
  </style>
```

## TODO
* 优化和完善。
