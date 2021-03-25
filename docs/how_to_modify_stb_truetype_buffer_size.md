# 如何修改 stb\_truetype 获取字模时申请的缓冲区大小

stb\_truetype 是一个用来解析 ttf 字体文件的开源库，功能强大，体积小巧。

在 AWTK 中，使用 AGGE 渲染模式时，默认采用 stb\_truetype 解析 ttf 字体文件（实际上由宏 WITH\_STB\_FONT 决定）。

stb\_truetype 在获取字模时，会 malloc 一块缓冲区保存字模位图中每行像素的变化信息，生成字模后，该缓冲区被释放，由于频繁申请释放内存容易产生内存碎片，因此，AWTK 默认设置该缓冲区大小约为 11 KB。

若由于内存不足 stb\_truetype 获取字模失败，则会打印以下提示信息：

```
STBTT get glyph failed : Cannot allocate memory!
```

此时，用户可通过定义宏 STBTT\_MAX\_SCANLINE\_EDGES 来设置缓冲区大小，该宏表示字模位图中每行像素的变化信息的最大数量，比如一个宽为 1000 像素的字模，如果它的某一行每个像素点都在变化，那么最大值则为1000，此时可设置该宏为1000，代码如下：

```c
#define STBTT_MAX_SCANLINE_EDGES 1000  //缓冲区大小约为 1000 * 28 字节（约27KB）
```

当设置宏 STBTT\_MAX\_SCANLINE\_EDGES 为 0 时，stb\_truetype 的缓冲区将动态扩张，但容易产生内存碎片，代码如下：

```c
#define STBTT_MAX_SCANLINE_EDGES 0   //缓冲区将动态扩张
```