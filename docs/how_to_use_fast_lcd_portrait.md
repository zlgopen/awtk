# 如何使用高效的屏幕旋转

  在实际的开发中，屏幕的选择会受到成本，供应商以及效果而影响，会导致横屏的应用无法使用横屏的屏幕，所以就需要高效的屏幕旋转功能来解决该问题。

  AWTK 默认提供了一种基于图像旋转的屏幕旋转功能，这一套逻辑优点是兼容性好，但是缺点就是效率比较低和吃内存。

  现在提供一种全新的高效旋转机制来解决上面说的所有问题，同时保持高效（和没有旋转的运行效率几乎一样），但是由于这套机制兼容性比较差一点，对 lcd 层，vgcanvas 层和 g2d 的适配层的都有所要求，所以使用的时候需要注意一下。

>新的旋转机制是一套矢量计算的旋转机制，所以在底层绘图之前需要把相关的数据通过矢量计算转换，所以对 lcd 层，vgcanvas 层和 g2d 的适配层有要求。


## 一、基本用法

  由于该机制需要 lcd 层和 vgcanvas 层配合，所以如果用户是使用 AWTK 提供了的 lcd 适配层（lcd_mem_XXX_create 的函数创建的 lcd ）和 vgcanvas 适配层（定义 WITH_NANOVG_AGGE 宏），则只需要定义 **WITH_FAST_LCD_PORTRAIT 宏**，以及在程序运行前调用 tk_enable_fast_lcd_portrait 函数设置开启高效屏幕旋转模式，然后在代码中调用 tk_set_lcd_orientation 函数就可以使用了。

~~~c
/* awtk_global.h */

/**
 * @method tk_set_lcd_orientation
 * 设置屏幕的旋转方向(XXX:目前仅支持0度,90度,180度和270度，旋转方向为逆时针方向)。
 * @param {int} orientation 旋转方向。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_set_lcd_orientation(lcd_orientation_t orientation);

/**
 * @method tk_enable_fast_lcd_portrait
 * 设置是否开启快速旋转功能。（开启这个功能需要定义 WITH_FAST_LCD_PORTRAIT 宏）
 * 备注：需要在 tk_set_lcd_orientation 函数之前调用
 * @param {bool_t} enable 是否开启。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_enable_fast_lcd_portrait(bool_t enable);
~~~

#### 注意实现：

  1. 为了可以高效贴图，所以贴图在加载到内存前就会被旋转到指定的角度了，所以使用  data 格式的位图时候需要提前调用脚本命令来生成资源。（最后一个参数为旋转角度，单位为角度，支持 0 度，90 度，180 度和 270 度）

     ~~~shell
     python .\scripts\update_res.py all x1 bgra+bgr565 0
     ~~~

  2. 如果 data 格式的位图的旋转角度为 0 度的话（在 Desiger 工具中可以取消勾选快速旋转模式来达到设置旋转角度为 0 的效果），可以支持动态 lcd 旋转，但是效率会下降，而使用文件系统或者 res 格式的位图数据则不会降低效率。

  3. 在没有定义 WITH_STB_IMAGE 宏（使用 data 格式的位图数据）的情况下，并且是位图旋转角度不为 0 度的话，是**不支持程序动态旋转**，需要在程序开始前就需要设置好旋转的角度，同时旋转角度应该和资源保持一致。

  4. 在使用的时候，需要特别注意 bitmap_t，lcd_t 和 graphic_buffer_t 类型是分为有**逻辑数据**和**真实的物理数据**的，所有的真实的物理数据只能通过接口获取，一般名称都会带有 **“physical”** 的字眼。

  5.  WITH_FAST_LCD_PORTRAIT 宏只是把功能增加到工程中，还需要用户自行调用 tk_enable_fast_lcd_portrait 来开启，如果没有调用 tk_enable_fast_lcd_portrait 函数的话，默认不启用，退化为以前就的 lcd 旋转方案。

> 如果是使用 Desiger 工具打包的话，第 1 点和第 3 点都会处理好的，用户只要注意动态 lcd 旋转的问题。


## 二、功能移植

  由于在某些情况下，用户会自行适配 lcd， vgcanvas 或者 g2d，所以用户需要在自行适配的层中加入相关的代码既可。

  在贴图的时候，需要注意的是为了高效的贴图，所以贴图在加载到内存前就会被旋转到指定的角度了（又名为已旋转的贴图），如果 vgcanvas 层不支持显示旋转后的图片的话，需要定义 **WITHOUT_FAST_LCD_PORTRAIT_FOR_IMAGE宏**，来让贴图不要旋转，如果贴图为 data 格式的位图数据话，这生成位图数据时候把 LCD_ORIENTATION 设置为 0 度。

> 如果使用  data 格式的位图数据的话，屏幕的旋转角度一定要和图片旋转角度一致否则会出现断言。

  已旋转的贴图的旋转角度一般和 lcd 旋转的角度一致，为了高效贴图而不需要旋转贴图，以及以前的 bitmap->w，bitmap->h 和 bitmap->line_length 都为逻辑数据，逻辑数据则为图片原来未旋转时的数据，下列是获取位图的真实物理数据的接口：

~~~c
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
~~~

> 除了上述的接口，其他接口获取出来都是逻辑数据。

### 1. LCD 层适配

  需要在各个的 lcd 层的绘图函数中加入坐标数据转化的机制，例如：

~~~c
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

~~~

  在 WITH_FAST_LCD_PORTRAIT 宏中，我们可以看到把填充的矩形坐标数据通过 lcd_orientation_rect_rotate_by_anticlockwise 函数转化为新的一个矩形坐标，然后再调用相关的填充函数来填充颜色。

~~~c
/* lcd_orientation_helper.inc */

/* 输入一个矩形数据通过旋转计算后返回的一个矩形数据 */
rect_t lcd_orientation_rect_rotate_by_anticlockwise(const rect_t* rect, lcd_orientation_t o, wh_t src_limit_w, wh_t src_limit_h);
~~~

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

### 2. VGCANVAS 层适配

  而 vgcanvas 层的适配和 lcd 层的适配基本是大同小异的都是适配绘图的函数就好了，但是**需要特别注意的就是贴图的旋转的问题**，因为有一些矢量画布库是不支持使用以及旋转的贴图。

### 3. G2D 层适配

  而 g2d 层的适配其实主要是增加了两个适配函数：

~~~c
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
~~~

备注：

1. 如果不实现这两个函数的话，则会调用 soft_rotate_image_ex 和 soft_blend_image_rotate 进行软件旋转绘制图片。
2. g2d 层的这两个函数主要是用来处理图片旋转角度为 0 的 lcd 旋转的情况，如果可以保证图片旋转角度和 lcd 旋转角度一样的话，这两个函数可以不实现。