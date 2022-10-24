# 如何使用高效的屏幕旋转

在实际的开发中，屏幕的选择会受到成本、供应商以及效果等因素的限制，某些情况下，需要旋转应用来适应屏幕。

AWTK 默认提供了一种基于图像旋转的屏幕旋转功能，该功能的兼容性好，但是效率比较低，而且很吃资源和内存。

现在 AWTK 额外提供一种全新的高效旋转机制来解决上述问题，该机制能保持较高的效率（和没有旋转时的运行效率几乎一样），但兼容性比较差，需要满足以下条件才能使用：

1. 开启 AWTK 提供的矢量画布 vgcanvas 功能，即定义宏 WITH_NANOVG_AGGE。
2. LCD 适配层基于 Framebuffer 实现，调用 AWTK 提供的 lcd_mem_t 类型创建 lcd_t 对象。
3. 如果开启 G2D 硬件加速，则需要自行实现 g2d_rotate_image_ex() 接口和 g2d_blend_image_rotate() 接口。

> 注：高效旋转机制是基于矢量计算实现的，其原理是在底层绘图之前把相关的数据通过矢量计算进行转换。

## 1 基本用法

在满足上述条件时，使用高效旋转的步骤如下：

1. 定义宏 WITH_FAST_LCD_PORTRAIT。
2. 在 AWTK Designer 的项目设置中选择旋转角度并勾选"使用快速旋转模式"，**或者**在程序初始化时手动调用相关接口，代码如下：

```c
ret_t application_init(void) {
  tk_enable_fast_lcd_portrait(TRUE);           /* 使能高效旋转 */
  tk_set_lcd_orientation(LCD_ORIENTATION_90);  /* 旋转 90 度 */
  ......

  return RET_OK;
}
```

> 注：定义宏 WITH_FAST_LCD_PORTRAIT 只是把功能相关的代码编译到程序中，开启功能仍需调用 tk_enable_fast_lcd_portrait() 接口。

### 1.1 重载 lcd 的 flush 函数

在某些情况下，用户可能会重载 lcd 的 flush 的函数，与默认的旋转机制不同，开启高效旋转时，flush 函数中不需要调用 image_rotate() 接口来旋转图像，只需调用 image_copy() 接口直接拷贝图像即可，示例代码如下：

```c
/* awtk-linux-fb/lcd_linux/lcd_linux_fb.c */

static ret_t lcd_linux_flush(lcd_t* base, int fbid) {
  ......
  /* 获取脏矩形列表 */
  dirty_rects = lcd_fb_dirty_rects_get_dirty_rects_by_fb(&(lcd->fb_dirty_rects_list), buff);
  if (dirty_rects != NULL && dirty_rects->nr > 0) {
    for (int i = 0; i < dirty_rects->nr; i++) {
      const rect_t* dr = (const rect_t*)dirty_rects->rects + i;
#ifdef WITH_FAST_LCD_PORTRAIT /* 开启高效旋转 */
      if (system_info()->flags & SYSTEM_INFO_FLAG_FAST_LCD_PORTRAIT) {
        /* 旋转脏矩形 */
        rect_t rr = lcd_orientation_rect_rotate_by_anticlockwise(dr, o, lcd_get_width(base), lcd_get_height(base));
        /* 根据旋转后的脏矩形，把 offline_fb 的数据拷贝到 online_fb 上面 */
        image_copy(&online_fb, &offline_fb, &rr, rr.x, rr.y);
      } else 
#endif
      { /* 如果没开启高效旋转，则默认调用 image_rotate() 接口旋转图像 */
        if (o == LCD_ORIENTATION_0) {
          image_copy(&online_fb, &offline_fb, dr, dr->x, dr->y);
        } else {
          image_rotate(&online_fb, &offline_fb, dr, o);
        }
      }
    }
   ......
}
```

### 1.2 注意事项

1. 为了高效贴图，打包资源时会先将图像数据旋转到指定角度（特指 data 类型的位图资源），建议采用 AWTK Designer 打包资源。
2. 在支持图片解码功能（定义宏 WITH_STB_IMAGE）时，使用图片原始数据，支持动态旋转，但解码数据会消耗一定性能。
3. 在不支持图片解码功能（没有定义宏 WITH_STB_IMAGE）时，使用 data 类型的位图资源，不支持动态旋转，打包资源时必须设置好旋转角度，并且与 LCD 旋转角度一致。

## 2 移植高效旋转

在某些特殊情况下，用户会自行适配 lcd，vgcanvas 以及 g2d，此时需要在适配层中加入相关的代码，详见下文。

在移植高效旋转需要先了解以下两个概念：

1. 图像的逻辑数据：指界面上显示出来的效果，可简单理解成图片原来未旋转时的逻辑尺寸，比如以前的 bitmap->w，bitmap->h 和 bitmap->line_length 都为逻辑数据。
2. 图像的物理数据：指图片解码后在保存在内存中的数据，可简单理解成图片解码旋转后的在内存中的物理数据，通常使用 bitmap_get_physical_xxx() 接口获取，常用的接口声明如下：

```c
/* awtk/base/bitmap.h */

/**
 * @method bitmap_get_physical_line_length
 * 获取图片真实物理的每一行占用内存的字节数。
 * 
 * @param {bitmap_t*} bitmap bitmap对象。
 *
 * @return {uint32_t} 返回每一行占用内存的字节数。
 */
uint32_t bitmap_get_physical_line_length(bitmap_t* bitmap);

/**
 * @method bitmap_get_physical_width
 * 获取图片真实物理的宽度。
 * @param {bitmap_t*} bitmap bitmap对象。
 *
 * @return {uint32_t} 返回图片宽度。
 */
uint32_t bitmap_get_physical_width(bitmap_t* bitmap);

/**
 * @method bitmap_get_physical_height
 * 获取图片真实物理的高度。
 * @param {bitmap_t*} bitmap bitmap对象。
 *
 * @return {uint32_t} 返回图片高度。
 */
uint32_t bitmap_get_physical_height(bitmap_t* bitmap);
```

> 除了上述的接口，其他接口获取出来都是逻辑数据。

在自行适配 lcd，vgcanvas 以及 g2d 时，如果需要支持高效旋转，在绘图时统一采用图像的**物理数据**即可，此外在使用脏矩形时也需要调用相关接口旋转脏矩形坐标，具体详见下文。

### 2.1 LCD 层适配

需要在各个的 lcd 层的绘图函数中加入坐标数据转化的机制，例如：

```c
/* awtk/lcd/lcd_mem.inc */

/* 填充矩形 */
static ret_t lcd_mem_fill_rect_with_color(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h, color_t c) {
  bitmap_t fb;

#ifdef WITH_FAST_LCD_PORTRAIT
  rect_t r = rect_init(x, y, w, h);
  system_info_t* info = system_info();
  rect_t rr = lcd_orientation_rect_rotate_by_anticlockwise(&r, info->lcd_orientation, lcd_get_width(lcd), lcd_get_height(lcd));
#else
  rect_t rr = rect_init(x, y, w, h);
#endif

  c.rgba.a = (c.rgba.a * lcd->global_alpha) / 0xff;

  lcd_mem_init_drawing_fb(lcd, &fb);
  return image_fill(&fb, &rr, c);
}
```

在 WITH_FAST_LCD_PORTRAIT 宏中，我们可以看到把填充的矩形坐标数据通过 lcd_orientation_rect_rotate_by_anticlockwise 函数转化为新的一个矩形坐标，然后再调用相关的填充函数来填充颜色。

```c
/* lcd_orientation_helper.inc */

/* 输入一个矩形数据通过旋转计算后返回的一个矩形数据 */
rect_t lcd_orientation_rect_rotate_by_anticlockwise(const rect_t* rect, lcd_orientation_t o, wh_t src_limit_w, wh_t src_limit_h);
```

lcd 适配层一共需要适配下面的函数：（适配的思路可以查看 lcd_mem.inc 中的代码）

| 函数                | 作用             |
| ------------------- | ---------------- |
| fill_rect           | 填充矩形区域颜色 |
| clear_rect          | 清除矩形区域颜色 |
| draw_image          | 绘制贴图         |
| draw_image_matrix   | 绘制贴图         |
| draw_glyph          | 绘制字体         |
| get_point_color     | 获取坐标对应颜色 |
| get_physical_width  | 获取真实物理宽度 |
| get_physical_height | 获取真实物理高度 |

上面的基本的适配函数，可能在实际运行的时候，还需要适配其他的函数（例如：set_orientation 函数）。

### 2.2 VGCANVAS 层适配

vgcanvas 层的适配和 lcd 层的适配大同小异，都是适配绘图函数即可，但是**需要特别注意的是贴图旋转的问题**，有一些矢量画布库是不支持旋转贴图的。

> 注：为了高效贴图，图像数据在加载到内存前就会被旋转到指定的角度了（又称已旋转的贴图），如果 vgcanvas 层不支持显示旋转后的图片，则需要定义宏 WITHOUT_FAST_LCD_PORTRAIT_FOR_IMAGE，此时如果使用 data 位图数据，则打包资源时需要指定旋转角度为 0。

### 2.3 G2D 层适配

g2d 层的适配主要是增加了这两个适配函数：

```c
/* awtk/base/g2d.h */

/**
 * @method image_rotate_ex
 * @export none
 * 把图片指定的区域进行旋转。
 * @param {bitmap_t*} dst 目标图片对象。
 * @param {bitmap_t*} src 源图片对象。
 * @param {const rect_t*} src_r 要旋转并拷贝的区域。
 * @param {xy_t} dx 目标位置的x坐标。（坐标原点为旋转后的坐标系原点，并非是 dst 的左上角）
 * @param {xy_t} dy 目标位置的y坐标。（坐标原点为旋转后的坐标系原点，并非是 dst 的左上角）
 * @param {lcd_orientation_t} o 旋转角度(一般支持90度即可，旋转方向为逆时针)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败，返回失败则上层用软件实现。
 */
ret_t g2d_rotate_image_ex(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, xy_t dx, xy_t dy, lcd_orientation_t o);

/**
 * @method g2d_blend_image_rotate
 * @export none
 * 把图片指定的区域渲染到framebuffer指定的区域，src的大小和dst的大小不一致则进行缩放以及旋转。
 *
 * @param {bitmap_t*} dst 目标图片对象。
 * @param {bitmap_t*} src 源图片对象。
 * @param {const rectf_t*} dst_r 目的区域。（坐标原点为旋转后的坐标系原点，并非是 dst 的左上角）
 * @param {const rectf_t*} src_r 源区域。
 * @param {uint8_t} global_alpha 全局alpha。
 * @param {lcd_orientation_t} o 旋转角度(一般支持90度即可，旋转方向为逆时针)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败，返回失败则上层用软件实现。
 */
ret_t g2d_blend_image_rotate(bitmap_t* dst, bitmap_t* src, const rectf_t* dst_r, const rectf_t* src_r,
                       uint8_t alpha, lcd_orientation_t o);
```

> 备注：
> 1. 如果不实现这两个函数的话，则会调用 soft_rotate_image_ex 和 soft_blend_image_rotate 进行软件旋转绘制图片。
> 2. g2d 层的这两个函数主要是用来处理图片旋转角度为 0 的 lcd 旋转的情况，如果可以保证图片旋转角度和 lcd 旋转角度一样的话，这两个函数可以不实现。
