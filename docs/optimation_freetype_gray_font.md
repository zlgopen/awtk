# 优化 freetype 解析矢量字体的显示效果

## 一、freetype 的缺陷

矢量字体（TrueType）通过数学曲线来描述字形，包含字形边界上的关键点、连线的导数信息等。

freetype 解析矢量字体的过程是将字形关键点按照规则连线变成字符轮廓，最后再将轮廓填充成点阵字。

很明显，在这个过程中，字形关键点是字体轮廓精准的度量，而位图则可以看成网格状态的，单位为像素，那么将坐标点转化为位图的过程势必会导致精度丢失，比如点坐标为(1.2, 2.8)，那么转换成位图则变成了(1, 3)。

并且由于 freetype 为了提高运算速度，采用位移的方式进行数学计算势必会导致精度丢失更加严重，在处理一些小字号字体或像素字体时，容易发生像素块粘连的现象，因此，AWTK 针对该问题做了一些优化。

> 像素字体也是矢量字体，只是该字体由正方形的像素块组成，其字体轮廓只有直线，像素块与像素块之间的间距比较小，具体效果可上网搜索“方正像素18”、“方正像素24”等关键字。

## 二、针对 freetype 缺陷所做优化

### 3.1 提高 freetype 解析字体的精度

虽然 freetype  解析字体时精度丢失无法避免，但提高运算精度依然可以优化显示效果。

freetype  提供两种计算二次贝塞尔曲线的精度，低精度 0.5 个像素，高精度 0.0625 个像素。其默认采用低精度进行运算，通过设置标志位 FT\_OUTLINE\_HIGH\_PRECISION 可让 freetype  采用高精度运算。

```c
//awtk/src/font_loader/font_loader_ft.c
...
uint32_t flags = FT_LOAD_DEFAULT | FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT | FT_OUTLINE_HIGH_PRECISION;
...
```

### 3.2 关闭 auto\_hint

要了解 auto\_hint 首先需要知道什么是 hinting 。

hinting 用来优化字体显示的方法。由于屏幕像素有限，矢量字体的缩放需要有更多的考虑，例如当一条线在两个像素格子中间时，该取左边的格子还是右边的格子？如果这方面没做好，就常常会出现字形衬线没对齐、或小字号字形歪七扭八、或像素块粘连到一起的情况。

> hinting 是额外的信息，它告诉 渲染器 renderer 该如何处理这些细节部分，使矢量字在小字号时显示效果更好，因此，hinting 是非常费时费人力的工作，TrueType 字体很多，但是有良好 hinting 的不多，拙劣的 hinting 会让字变得很难看。

为了改善以上问题，freetype 提供了 auto\_hint 功能，并默认开启该功能，它可以自动为没有 hinting 的字体做 hinting 工作，auto\_hint 肯定无法做得像人力 hinting  一样好，对于许多笔画复杂的文字（如中文）效果不佳，并且对于小字号的复杂字形，经常造成一系列反效果，最典型问题的就是像素粘连。

因此，经过多次测试，AWTK 决定关闭 freetype 的 auto\_hint 功能以对大多数矢量字体达到更好的效果，通过设置标志位 FT\_LOAD\_NO\_AUTOHINT 可关闭该功能。

```c
//awtk/src/font_loader/font_loader_ft.c
...
uint32_t flags = FT_LOAD_DEFAULT | FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT | FT_OUTLINE_HIGH_PRECISION;
...
```

