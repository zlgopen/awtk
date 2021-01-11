# 如何隐藏滚动条的上下按钮

在design/default/styles/default.xml中，去掉scroll\_down/scroll\_up按钮的style即可：

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

