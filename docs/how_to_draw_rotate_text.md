# 如何绘制旋转文字

要旋转文字只能用 [vgcanvas](https://github.com/zlgopen/awtk/blob/master/docs/manual/vgcanvas_t.md) 绘制文字。在不同的情况下，AWTK 底层采用不同的 [vgcanvas](https://github.com/zlgopen/awtk/blob/master/docs/manual/vgcanvas_t.md) 引擎，遗憾的是部分 [vgcanvas](https://github.com/zlgopen/awtk/blob/master/docs/manual/vgcanvas_t.md) 引擎并不支持绘制文字。事实上只能在用 OpenGL 的情况下，才支持绘制文字。

不过没有关系，毕竟在嵌入式平台，旋转文字的需求不多，如果确实需要，也是有解决方案的。本文介绍一种跨平台的绘制旋转的文字方法。

字体引擎（比如 stb/freetype) 会把矢量字体转换成一种特殊位图，这个位图只有透明通道 (alpha) 信息，在 AWTK 中用 glyph\_t 表示。[vgcanvas](https://github.com/zlgopen/awtk/blob/master/docs/manual/vgcanvas_t.md) 引擎都支持贴图功能，只要把 glyph\_t 转换成正常 bitmap\_t 对象，就可以利用 vgcanvas 各种变换效果了。

下面这个函数可以把字符串对应的 glyph 转换成位图对象（拿去用就好了）：

```c
static bitmap_t* bitmap_from_str(canvas_t* canvas, wchar_t* str, color_t tc) {
  glyph_t g;
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t i = 0;
  uint32_t w = 0;
  uint32_t h = 0;
  uint32_t ox = 0;
  font_vmetrics_t vm;
  uint32_t* p = NULL;
  uint32_t baseline = 0;
  bitmap_t* bitmap = NULL;
  uint32_t font_size = canvas->font_size;
  font_manager_t* fm = canvas->font_manager;
  font_t* font = font_manager_get_font(fm, canvas->font_name, font_size);
  return_value_if_fail(font != NULL, NULL);

  vm = font_get_vmetrics(font, font_size);
  h = vm.ascent - vm.descent;
  return_value_if_fail(h > 0, NULL);

  baseline = vm.ascent;
  for (i = 0; str[i]; i++) {
    return_value_if_fail(font_get_glyph(font, str[i], font_size, &g) == RET_OK, NULL);
    w += g.advance + 1;
  }

  bitmap = bitmap_create_ex(w, h, 0, BITMAP_FMT_RGBA8888);
  return_value_if_fail(bitmap != NULL, NULL);

  p = (uint32_t*)bitmap_lock_buffer_for_write(bitmap);

  memset(p, 0x00, w * h * 4);

  for (i = 0; str[i]; i++) {
    return_value_if_fail(font_get_glyph(font, str[i], font_size, &g) == RET_OK, NULL);

    for (y = 0; y < g.h; y++) {
      for (x = 0; x < g.w; x++) {
        int32_t dx = ox + g.x + x;
        int32_t dy = baseline + g.y + y;

        uint32_t* d = p +  dy * w + dx;
        const uint8_t* s = g.data + y * g.w + x;

        tc.rgba.a = *s;
        *d = tc.color;
      }
    }
    ox += g.advance + 1;
  }
  bitmap_unlock_buffer(bitmap);
  bitmap->flags |= BITMAP_FLAG_CHANGED;

  return bitmap;
}
```

然后？然后就和画图一样了：

```c
  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);

  vgcanvas_translate(vg, cx, cy);
  vgcanvas_rotate(vg, info->rotation);
  vgcanvas_scale(vg, info->scale, info->scale);
  vgcanvas_translate(vg, -cx, -cy);

  canvas_widget_draw_image_center(widget, vg, text_img);

  vgcanvas_restore(vg);
  ```

完整代码请参考：[vgcanvas_draw_text](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/vgcanvas_draw_text.c)

vgcanvas 的 API 基本上是参考 [HTML5 Canvas API](https://www.w3schools.com/tags/ref_canvas.asp) 定义的，这个 API 是我见过最好的 2D 绘图 API。不过，[vgcanvas](https://github.com/zlgopen/awtk/blob/master/docs/manual/vgcanvas_t.md) 在没有 GPU 的情况下，性能远远不如 [canvas](https://github.com/zlgopen/awtk/blob/master/docs/manual/canvas_t.md)，除非有旋转之类的特殊需求，尽量使用 [canvas](https://github.com/zlgopen/awtk/blob/master/docs/manual/canvas_t.md) 的 API。
