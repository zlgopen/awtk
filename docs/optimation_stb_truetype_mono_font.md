# 优化 stb\_truetype 在 mono 模式下的字体显示效果

stb\_truetype 解析 ttf 字体文件时，输出的字模为灰度图，如果需要在mono模式下使用stb\_truetype 则需要另外编写二值化算法。

## 一、stb\_truetype 的二值化算法

AWTK 通过给定阈值实现 stb\_truetype 的二值化算法，代码如下：

```c
//awtk/src/font_loader/font_loader_stb.c
static ret_t font_stb_gray_to_mono_by_threshold(const glyph_t* gray, glyph_t* mono,
                                                uint32_t threshold) {
  return_value_if_fail(gray != NULL && mono != NULL && gray->format == GLYPH_FMT_ALPHA,
                       RET_BAD_PARAMS);
  uint32_t i = 0;
  uint32_t j = 0;
  uint16_t h = gray->h;
  uint16_t w = gray->w;

  mono->format = GLYPH_FMT_MONO;
  mono->h = h;
  mono->w = w;
  mono->pitch = ((mono->w + 15) >> 4) << 1;
  mono->x = gray->x;
  mono->y = gray->y;
  mono->advance = gray->advance;

  uint32_t nmemb = mono->pitch * h;
  uint8_t* bitmap = TKMEM_CALLOC(nmemb, sizeof(uint8_t));
  return_value_if_fail(bitmap != NULL, RET_OOM);
  for (j = 0; j < h; ++j) {
    for (i = 0; i < w; ++i) {
      if (gray->data[w * j + i] > threshold) {
        uint32_t offset = j * mono->pitch + (i >> 3);
        uint32_t offset_bit = 7 - (i % 8);
        bitmap[offset] += 0x1 << offset_bit;
      }
    }
  }
  mono->data = bitmap;
  return RET_OK;
}
```

## 二、优化二值化阈值

经测试发现二值化阈值与 ttf 字体文件和字号有关，因此，对 stb\_truetype 获取的灰度图字模进行二值化的阈值做了调整，在不同字号区间使用不同的阈值，代码如下：

```c
//awtk/src/font_loader/font_loader_stb.c
static ret_t font_stb_get_glyph(font_t* f, wchar_t c, font_size_t font_size, glyph_t* g) {
  ...
  if (bitmap != NULL) {
    if (font->mono) {
      glyph_t* gray_g = glyph_clone(g);
      gray_g->data = bitmap;
      uint32_t threshold = font_size > 24? (font_size > 48 ? (font_size > 71 ? (font_size > 95 ? 195 : 175) : 160) : 118) : 95;
      font_stb_gray_to_mono_by_threshold(gray_g, g, threshold);
      STBTT_free(bitmap, NULL);
      glyph_destroy(gray_g);
    } else {
      g->data = bitmap;
    }
    ...
  }

  return g->data != NULL || c == ' ' ? RET_OK : RET_NOT_FOUND;
}
```

可以看出 stb\_truetype 在 mono 模式下的字体显示效果主要由阈值 threshold 决定，这些阈值是通过测试不断调整得到的，用户也可针对自己使用矢量字体文件进行调整。

