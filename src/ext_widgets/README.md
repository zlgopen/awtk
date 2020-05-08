# 扩展控件

### 1.使用注意事项

* 1. 部分控件不适合在低端平台使用。
* 2. 部分控件需要 vgcanvas 支持。
* 3. 部分控件需要 widget 动画效果配合。

### 2.扩展控件在低端平台的使用情况

​	测试平台为 stm32f103 ，ram 为 64 kb，flash 为 512 kb，采用**片段式显存的方案**来测试（*片段式显存方案不支持 vgcanvas*）并且**开启了 widget 动画效果**，该测试不考虑实际效果，因为如果控件刷新面积很大，会导致刷新次数变多和刷新效率减低，影响实际的效果，所以本测试只确定扩展控件是否正常工作，而且在低端平台使用扩展控件需要注意比较多的问题，最好是先了解该扩张控件的代码原理才使用，否则容易出现奇怪的问题，所以低端平台慎用扩展控件。

| 控件文件夹名字  | 是否正常工作 | 注意事项                                                     |
| --------------- | ------------ | ------------------------------------------------------------ |
| canvas_widget   | 是           | 需要注意平台是否支持 vgcanvas ，否则调用 vgcanvas 对应的接口会失效。 |
| color_picker    | 是           | 需要注意平台的内存是否足够，color_component 控件需要开辟一块和该控件宽高一样的32位色的图片内存。 |
| combo_box_ex    | 是           |                                                              |
| features        | 是           |                                                              |
| file_browser    | 否           | 需要文件系统支持。                                           |
| gif_image       | 否           | 因为 imagegen 工具暂时不支持生成 gif 格式的资源。            |
| guage           | 否           | 因为需要 vgcanvas 支持。                                     |
| image_animation | 是           |                                                              |
| image_value     | 是           |                                                              |
| keyboard        | 是           | 需要注意内存是否足够，因为软键盘会创建大量的 button 控件。   |
| mledit          | 是           |                                                              |
| mutable_image   | 是           | 但是旋转部分没有了，因为旋转需要支持 vgcanvas 。             |
| progress_circle | 否           | 因为需要 vgcanvas 支持。                                     |
| rich_text       | 是           | 但是需要 widget 动画效果配合。                               |
| scroll_label    | 是           | 但是需要 widget 动画效果配合。                               |
| scroll_view     | 是           | 但是需要 widget 动画效果配合。                               |
| slide_menu      | 是           | 但是需要 widget 动画效果配合。                               |
| slide_view      | 是           | 但是需要 widget 动画效果配合， slide_indicator 控件只使用  stroke_rect  和  fill_rect 效果，不能使用画圆形的效果，因为画圆需要 vgcanvas 支持。 |
| svg_image       | 否           | 因为需要 vgcanvas 支持。                                     |
| switch          | 是           | 但是没有圆角的效果，因为圆角效果需要 vgcanvas 支持。         |
| text_selector   | 是           | 但是需要 widget 动画效果配合。                               |
| time_clock      | 否           | 因为需要 vgcanvas 支持。                                     |

> 备注：在低端平台中 widget 动画效果和扩展控件一般都会被禁止的，所以需要手动把 WITHOUT_WIDGET_ANIMATORS 和 WITHOUT_EXT_WIDGETS 宏取消定义可以正常使用。