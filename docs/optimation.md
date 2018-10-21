## 优化技巧

* memcpy是最快的贴图方式，所以对于不透明的图片，尽量保持与Framebuffer一致的格式，这样可以直接使用memcpy贴图。

> 比如FrameBuffer的格式是BGR565，请定义宏WITH\_BITMAP\_BGR565。

* 如果支持硬件加速，确保(半)透明的图片解码成硬件支持的格式。

> 比如对于PXP/DMA2D硬件加速，请定义宏WITH\_BITMAP\_BGRA。

* memcpy的速度对于AWTK非常重要。如果显示速度不理想，请测试一下memcpy的速度。

> 如果memcpy的速度不理想，请检查一下GUI使用的内存是否配置正确(如cache等)。

* 绘制一帧需要的时间是一个重要指标。

> 可以在src/base/window\_manager.c:window\_manager\_paint\_normal中查看wm->last\_paint\_cost的值。

* 排除法有时能快速定位性能瓶颈。

> 1.怀疑字体渲染太慢? 让lcd\_draw\_glyph直接返回，看看效果如何。

> 2.怀疑图片渲染太慢? lcd\_draw\_image直接返回，看看效果如何。

> 3.怀疑填充颜色太慢? lcd\_fill\_rect直接返回，看看效果如何。

> 4.怀疑硬件加速没有效果，用纯软件渲染试试，对比一下渲染一帧需要的时间。

* 猜测往往是错误的，请用函数time\_now\_ms去度量你怀疑的地方。
