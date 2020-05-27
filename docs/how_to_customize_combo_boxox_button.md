# 如何定制 combo_box 控件中按钮的风格和大小

## 定制 combo_box 控件中按钮的风格

打开 style 文件，找到按钮的 style，修改名为 combobox_down 的 style 即可。

```xml
<button>
  <style name="combobox_down" border_color="#a0a0a0">
    <normal     bg_color="#f0f0f0" icon="arrow_down_n"/>
    <pressed    bg_color="#c0c0c0" icon="arrow_down_p"/>
    <over       bg_color="#e0e0e0" icon="arrow_down_o"/>
  </style>
</button>

```

## 定制 combo_box 控件中按钮的大小

此时需要在 combo_box 控件中，增加一个按钮子控件，可以设置它的大小、位置和 style 的名称。

```xml
  <combo_box left_margin="6" x="10" y="5" w="200" h="30" text="left" options="left;center;right;">
    <button style="combobox_down" x="right:5" y="middle" w="20" h="20"/>
  </combo_box>
```
