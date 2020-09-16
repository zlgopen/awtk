
# 如何使用离线的 canvas

​	canvas 在 awtk 中是画布的意思，而且用户常常是通过回调函数回去是在线 canvas 来进行绘图，但是在在线 canvas 上面画东西就会显示到屏幕上面。

​	为了解决用户离线画布的需求，所以 awtk 中提供了 canvas_offline_xxx 系列函数给用户使用。

### 一，创建离线画布 canvas

​	下面为创建离线画布 canvas 的函数，在离线画布 canvas 内置了一个离线的 bitmap，提供给用户使用。

```c
/**
 * @method canvas_offline_create
 * 创建一个离线的 canvas
 * 在 opengl 模式下 format 参数只能为 BITMAP_FMT_RGBA8888
 * 在其他模式下，离线 canvas 格式可以为 rgba，bgar，bgr565和rgb565
 * 
 * @param {uint32_t} w 离线 canvas 的宽。
 * @param {uint32_t} h 离线 canvas 的高。
 * @param {bitmap_format_t} format 离线 canvas 的格式。
 *
 * @return {canvas_t*} 成功返回 canvas ，失败返回 NULL。
 */
canvas_t* canvas_offline_create(uint32_t w, uint32_t h, bitmap_format_t format);

/**
 * @method canvas_offline_get_bitmap
 * 获取离线 canvas 的离线 bitmap。
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {bitmap_t*} 返回 bitmap_t 对象表示成功，返回 NULL 表示失败。
 */
bitmap_t* canvas_offline_get_bitmap(canvas_t* canvas);
```

下面代码，创建离线 canvas：

``` c
  static canvas_t* canvas_offline = NULL;

  /* 创建离线的 canvas */
  canvas_offline = canvas_offline_create(320, 480, BITMAP_FMT_RGBA8888);
```

> 备注：在 opengl 模式下离线 canvas 格式只能为 RGBA8888，而在 AGGE 下可以为 RGBA8888，BGRA8888，RGB565 和 BGR5656。（其他的格式暂时不支持）

### 二，绘图离线画布 canvas 

​	离线 canvas 的绘图过程和平时使用 canvas 绘图是一样的，只不过在绘制之前需要先调用 canvas_offline_begin_draw 函数，结束的时候调用 canvas_offline_end_draw  函数。

```c
/**
 * @method canvas_offline_begin_draw
 * 设置离线 canvas 开始绘图
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_begin_draw(canvas_t* canvas);

/**
 * @method canvas_offline_end_draw
 * 设置离线 canvas 结束绘图
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_end_draw(canvas_t* canvas);

/**
 * @method canvas_offline_clear_canvas
 * 把离线 canvas 清除所有数据，并把背景设置为全透明（注意：该离线 canvas 需要有透明通道）
 * 该函数调用前必须要先 canvas_offline_begin_draw 函数。
 * 该函数用来解决离线 canvas 多次绘图半透效果后导致半透效果无效的问题。
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_clear_canvas(canvas_t* canvas);

/**
 * @method canvas_offline_bitmap_move_to_new_bitmap
 * 把离线 canvas 的离线 bitmap 移动赋值给新的 bitmap。
 * 移动赋值后原来的离线 canvas 的离线 bitmap 就会被置空。
 * 备注：在移动赋值之前会先调用 canvas_offline_flush_bitmap 把数据回流到内存中。
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {bitmap_t*} 返回 bitmap_t 对象表示成功，返回 NULL 表示失败。
 */
ret_t canvas_offline_bitmap_move_to_new_bitmap(canvas_t* canvas, bitmap_t* bitmap);

/**
 * @method canvas_offline_flush_bitmap
 * 把离线 canvas 的数据放到绑定的 bitmap 中
 * 该函数只有在 opengl 模式才需要调用，是否把显存中的数据回传到内存中。
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_flush_bitmap(canvas_t* canvas);
```

下面代码把整个窗口绘制到离线 canvas 上面。

```c
  bitmap_t* offline_bitmap = NULL;

  /* 开始离线 canvas 绘图 */
  canvas_offline_begin_draw(canvas_offline);
  /* 把整个窗口绘制到离线的 canvas 中 */
  widget_paint(window_manager(), canvas_offline);
  /* 结束离线 canvas 绘图 */
  canvas_offline_end_draw(canvas_offline);

  /* 由于需要把 bitmap 保存为 png 文件，所以需要把显存中的数据回传到内存中 */
  canvas_offline_flush_bitmap(canvas_offline);

  /* 获取离线的 canvas 对应的离线 bitmap */
  offline_bitmap = canvas_offline_get_bitmap(canvas_offline);

  /* 把 bitmap 保存到 png 文件 */
  bitmap_save_png(offline_bitmap, "test.png");
```

> 备注：
>
> 1. 如果 awtk 在 opengl 模式下，同时需要操作 bitmap 中的位图数据的话，就需要调用 canvas_offline_flush_bitmap 函数把 GPU 中的显存数据回传到内存中。
> 2. canvas 的所有函数，离线  canvas 都可以调用，并且绘制数据只会放到离线 canvas 中。
> 3. 如果需要使用 vg 的话，可以通过 canvas_get_vgcanvas  函数，获取离线 canvas 的 vg 对象。

### 三，释放离线画布 canvas

​	释放离线画布只是释放对应的离线 canvas，而不会释放绑定的 bitmap，bitmap 需要用户自行管理。

```c
/**
 * @method canvas_offline_destroy
 * 释放离线 canvas 和离线 bitmap
 *
 * @param {canvas_t*} canvas 离线 canvas 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t canvas_offline_destroy(canvas_t* canvas);
```

### 四，用户自定义特殊的离线画布 canvas

​	由于有一些特别平台不能直接使用 awtk 提供的离线 canvas，只能通过平台的一些特殊的接口来实现离线画布 canvas，所以 awtk 的离线 canvas 提供了一个给外部重载离线 canvas 的函数的方法。

​	通过定义 WITH_CANVAS_OFFLINE_CUSTION 宏，并且用户自行重写下面的函数，并且返回值必须是 RET_OK 或者非 NULL：

| 函数名字                                        | 作用                                                       |
| ----------------------------------------------- | ---------------------------------------------------------- |
| canvas_offline_custom_create                    | 用户自定义 canvas_offline_create                           |
| canvas_offline_custom_clear_canvas              | 用户自定义 canvas_offline_custom_clear_canvas              |
| canvas_offline_custom_begin_draw                | 用户自定义 canvas_offline_custom_begin_draw                |
| canvas_offline_custom_end_draw                  | 用户自定义 canvas_offline_custom_end_draw                  |
| canvas_offline_custom_get_bitmap                | 用户自定义 canvas_offline_custom_get_bitmap                |
| canvas_offline_custom_bitmap_move_to_new_bitmap | 用户自定义 canvas_offline_custom_bitmap_move_to_new_bitmap |
| canvas_offline_custom_flush_bitmap              | 用户自定义 canvas_offline_custom_flush_bitmap              |
| canvas_offline_custom_destroy                   | 用户自定义 canvas_offline_custom_destroy                   |

> 备注：如果用户只需要部分函数的实现的话，可以把其他的函数返回 RET_FAIL 或者 NULL ，即可以让离线画布 canvas 函数执行回 awtk 的原来的代码逻辑。

> 具体用法请参考 
> [demo\_offline\_canvas.c](https://github.com/zlgopen/awtk/blob/master/demos/demo_canvas_offline.c)

