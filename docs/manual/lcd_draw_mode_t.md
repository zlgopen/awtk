## lcd\_draw\_mode\_t
### 概述
LCD绘制模式常量定义。
### 常量
<p id="lcd_draw_mode_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| LCD\_DRAW\_NORMAL | 正常绘制。 |
| LCD\_DRAW\_ANIMATION | 绘制窗口动画，两个窗口无重叠。在该模式下，可以直接绘制到显存，不用绘制到framebuffer中。 |
| LCD\_DRAW\_ANIMATION\_OVERLAP | 绘制窗口动画，两个窗口有重叠。(目前无特殊用途)。 |
| LCD\_DRAW\_SWAP | 如果lcd支持swap操作，在特殊情况下，可以使用该模式提速。 |
| LCD\_DRAW\_OFFLINE | 离线模式绘制(仅适用于framebuffer)。在该模式下，绘制的内容不会显示出来，但可以用take\_snapshot取出来，主要用于窗口动画。 |
