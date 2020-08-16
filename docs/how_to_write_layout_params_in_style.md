# 如何在主题文件中写控件布局参数

一个控件在多个界面出现，而且它的布局参数是一样的，可以把它布局参数放到 style 中，这样可以提高可维护性。

## 一、在 style 中指定布局参数

* 控件自身布局参数使用 self_layout 属性指定。请参考 [控件布局参数文档](layout.md)

* 子控件布局参数使用 children_layout 属性指定。请参考 [控件布局参数文档](layout.md)

示例：

```xml
<column>
  <style name="buttons" children_layout="default(row=4,col=1,spacing=10)"                        self_layout="default(x=center,y=10,w=50%,h=148)">
    <normal />
  </style>
</column>
```

> 完整示例请参考 [styles/button.xml](https://github.com/zlgopen/awtk/blob/master/design/default/styles/button.xml)

## 二、在 UI 文件中引用

示例：

```xml
  <column style="buttons">
    <button name="open:dialog1" style="round" enable_long_press="true" text="Long Press"/>
    <button style="red_btn" text="Text"/>
    <button style="icon" text="Fullscreen" name="fullscreen"/>
    <button >
      <label y="0" x="0" w="100%" h="100%" text="Text"/>
      <image image="earth" draw_type="icon" y="0" x="right" w="30" h="100%"/>
    </button>
  </column>
```

> 完整示例请参考 [ui/button.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/button.xml)
