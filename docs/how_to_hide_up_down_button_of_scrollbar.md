# 如何隐藏滚动条的上下按钮

在 design/default/styles/default.xml 中，去掉 scroll\_down/scroll\_up 按钮的 style 即可：

```xml
  <style name="scroll_down" border_color="#a0a0a0">
    <normal     bg_color="#f0f0f0" icon="arrow_down_n"/>
    <pressed    bg_color="#c0c0c0" icon="arrow_down_p"/>
    <over       bg_color="#e0e0e0" icon="arrow_down_o"/>
  </style>

  <style name="scroll_up" border_color="#a0a0a0">
    <normal     bg_color="#f0f0f0" icon="arrow_up_n"/>
    <pressed    bg_color="#c0c0c0" icon="arrow_up_p"/>
    <over       bg_color="#e0e0e0" icon="arrow_up_o"/>
  </style>
```
