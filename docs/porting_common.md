## 嵌入式平台移植注意事项

### 一、几个常用的宏

* 1.WITH\_BITMAP\_FONT 如果需要使用位图字体请定义该宏。

* 2.WITH\_STB\_FONT 如果需要使用truetype字体请定义该宏(请确保stack不小于16KB)。一般WITH\_STB\_FONT和WITH\_BITMAP\_FONT任选一个。

* 3.WITH\_STB\_IMAGE 如果需要使用png/jpg图片请定义该宏(请确保stack不小于16KB)。由于解码图片需要较大内存，所以在内存较
少的情况下，我们直接把png/jpg转换成位图数据，当作常量编译到代码中，此时不需要定义该宏。 

* 4.WITH\_FS\_RES 如果支持从文件系统中加载资源请定义该宏。需要有兼容POSIX标准的FS接口，如果没有则需要自己实现src/base/fs.c中的函数。

* 5.USE\_GUI\_MAIN 嵌入式系统有自己的main函数时请定义该宏。

* 6.WITH\_WCSXXX 如果出现wcsxxx之类的函数没有定义时请定义该宏。

* 7.HAS\_FAST\_MEMCPY 如果有高效的memcpy函数请定义该宏。

* 8.WITH\_BITMAP\_RGB565 如果支持rgb565格式请定义该宏。除了使用nanovg渲染，一般情况都可以定义该宏。

* 9.WITH\_BITMAP\_BGRA 缺省图片加载之后的格式为RGBA，但有的硬件加速需要使用BGRA格式(如stm32 dma2d)，此时需要定义该宏。

* 10.WITH\_LOW\_RES 资源有限时请定义该宏。

* 11.WITH\_DYNAMIC\_TR 如果需要动态翻译字符串时请定义该宏。

系统内存小余8M时，推荐定义：WITH\_BITMAP\_FONT,USE\_GUI\_MAIN,HAS\_FAST\_MEMCPY,WITH\_BITMAP\_RGB565,WITH\_LOW\_RES
系统内存超过8M时，推荐定义：WITH\_STB\_FONT,WITH\_STB\_IMAGE,USE\_GUI\_MAIN,HAS\_FAST\_MEMCPY,WITH\_BITMAP\_RGB565,WITH\_DYNAMIC\_TR

### 二、include路径

* 1.基本路径

```
awtk
awtk\src
awtk\3rd
```

* 2.picasso的路径(如果启用)

```
awtk\3rd\picasso\src
awtk\3rd\picasso\src\gfx
awtk\3rd\picasso\include
awtk\3rd\picasso\src\include
awtk\3rd\picasso\build
```

* 3.agg的路径(如果启用)

```
3rd/agg/include
```

### 三、要加入到工程的文件

* src/base 全部加入。
* src/font 全部加入。
* src/main\_loop/main\_loop\_simple.*
* src/window\_animators/ 如果需要窗口动画而且支持framebuffer加入window\_animator\_fb.c，否则加入window\_animator\_null.c。
* src/image_loader 全部加入。
* src/platforms/raw/mutex.c 加入本文件或者自己实现mutex。
* src/ui\_loader 全部加入。
* src/window_animators 全部加入。
* src/blend 使用framebuffer时全部加入。
* src/lcd 根据LCD类型选择相应的文件。
* src/tk.c
* src/tk.h 
* src/vgcanvas/vgcanvas\_null.c 如果不使用vgcanvas请加入本文件。
* src/xml 可选。
* 3rd/picasso 可选。
* 3rd/agg 可选。

### 四、其它

> 使用keil编译时，请在C/C++ -> Misc Controls中加上**--gnu**标志。


