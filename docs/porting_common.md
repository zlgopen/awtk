## 嵌入式平台移植注意事项

### 一、配置的宏

> awtk_config.h

```
/**
 * 嵌入式系统有自己的 main 函数时，请定义本宏。
 *
 * #define USE_GUI_MAIN 1
 */

/**
 * 如果需要支持预先解码的位图字体，请定义本宏。一般只在 RAM 极小时，才启用本宏。
 * #define WITH_BITMAP_FONT 1
 */

/**
 * 如果支持 png/jpeg 图片，请定义本宏
 *
 * #define WITH_STB_IMAGE 1
 */

/**
 * 如果用 stb 支持 Truetype 字体，请定义本宏
 *
 * #define WITH_STB_FONT 1
 */

/**
 * 如果用 freetype 支持 Truetype 字体，请定义本宏。
 *
 * #define WITH_FT_FONT 1
 */

/**
 * 如果支持从文件系统加载资源，请定义本宏
 *
 * #define WITH_FS_RES 1
 */

/**
 * 如果定义本宏，使用标准的 UNICODE 换行算法，除非资源极为有限，请定义本宏。
 *
 * #define WITH_UNICODE_BREAK 1
 */

/**
 * 如果定义本宏，将图片解码成 BGRA8888 格式，否则解码成 RGBA8888 的格式。
 * 当硬件的 2D 加速需要 BGRA 格式时，请启用本宏。
 *
 * #define WITH_BITMAP_BGRA 1
 */

/**
 * 如果定义本宏，将不透明的 PNG 图片解码成 BGR565 格式，建议定义。
 * 另外和 LCD 的格式保存一致，可以大幅度提高性能。
 * 如果没有定义 WITH_BITMAP_BGR565 和 WITH_BITMAP_RGB565 宏，默认解析为32位色
 *
 * #define WITH_BITMAP_BGR565 1
 */

 /**
 * 如果定义本宏，将不透明的 PNG 图片解码成 RGB565 格式，建议定义。
 * 另外和 LCD 的格式保存一致，可以大幅度提高性能。
 * 如果没有定义 WITH_BITMAP_BGR565 和 WITH_BITMAP_RGB565 宏，默认解析为32位色
 *
 * #define WITH_BITMAP_RGB565 1
 */

/**
 * 如果不需输入法，请定义本宏
 *
 * #define WITH_NULL_IM 1
 */

/**
 * 如果有标准的 malloc/free/calloc 等函数，请定义本宏
 *
 * #define HAS_STD_MALLOC 1
 */

/**
 * 如果有标准的 fopen/fclose 等函数，请定义本宏
 *
 * #define HAS_STDIO 1
 */

/**
 * 如果有标准的 pthread 等函数，请定义本宏
 *
 * #define HAS_PTHREAD 1
 */

/**
 * 如果有优化版本的 memcpy 函数，请定义本宏
 *
 * #define HAS_FAST_MEMCPY 1
 */

/**
 * 如果出现 wcsxxx 之类的函数没有定义时，请定义该宏
 *
 * #define WITH_WCSXXX 1
 */

/**
 * 如果启用 STM32 G2D 硬件加速，请定义本宏
 *
 * #define WITH_STM32_G2D 1
 */

/**
 * 如果启用 NXP PXP 硬件加速，请定义本宏
 *
 * #define WITH_PXP_G2D 1
 */

/**
 * 在没有 GPU 时，如果启用 agge 作为 nanovg 的后端（较 agg 作为后端：小，快，图形质量稍差），请定义本宏。
 *
 * #define WITH_NANOVG_AGGE 1
 */

/**
 * 在没有 GPU 时，如果启用 agg 作为 nanovg 的后端（较 agge 作为后端：大，慢，图形质量好），请定义本宏。
 * 注意：agg 是以 GPL 协议开源。
 *
 * #define WITH_NANOVG_AGG 1
 */

/**
 * 如果启用鼠标指针，请定义本宏
 *
 * #define ENABLE_CURSOR 1
 */

/**
 * 对于低端平台，如果不使用控件动画，请定义本宏。
 *
 * #define WITHOUT_WIDGET_ANIMATORS 1
 */

/**
 * 对于低端平台，如果不使用窗口动画，请定义本宏。
 *
 * #define WITHOUT_WINDOW_ANIMATORS 1
 */

/**
 * 对于低端平台，如果不使用对话框高亮策略，请定义本宏。
 *
 * #define WITHOUT_DIALOG_HIGHLIGHTER 1
 */

/**
 * 对于低端平台，如果不使用扩展控件，请定义本宏。
 *
 * #define WITHOUT_EXT_WIDGETS 1
 */

/**
 * 对于低端平台，如果内存不足以提供完整的 FrameBuffer，请定义本宏启用局部 FrameBuffer，可大幅度提高渲染性能。
 *
 * #define FRAGMENT_FRAME_BUFFER_SIZE 32 * 1024
 */

 /**
 * 启用widget类型检查，请定义本宏(除非编译器不支持，否则请定义它)。
 *
 * #define WITH_WIDGET_TYPE_CHECK 1
 */
```

> 请参考：src/base/awtk\_config\_sample.h

### 二、include 路径

```
awtk/src
awtk/src/ext_widgets
awtk/3rd
awtk/3rd/agge
awtk/3rd/nanovg
awtk/3rd/nanovg/base
awtk/3rd/gpinyin/include
awtk/3rd/libunibreak
awtk-port
```

### 三、要加入到工程的文件

| 文件                        |      说明    | 
|-----------------------------|:-------------|
| src/awtk\_global.c          | 加入       
| src/tkc                     | 全部加入
| src/base                    | 全部加入
| src/widgets                 | 全部加入
| src/xml                     | 全部加入
| src/ui\_loader              | 全部加入
| src/svg                     | 支持 vgcanvas 时全部加入
| src/blend                   | 使用 framebuffer 时全部加入
| src/ext\_widgets            | 使用扩展控件时全部加入
| src/font\_loader            | 推荐全部加入（可只加需要的）
| src/image\_loader           | 推荐全部加入（可只加需要的）
| src/input\_engines          | 不需要输入法时加入 input\_engine\_null.c，否则加入 input\_engine\_pinyin.cpp
| src/input\_methods          | 加入 input\_method\_creator.c，根据宏决定启用何种输入法
| src/lcd                     | 根据 LCD 类型 (OpenGL/FrameBuffer/Register）选择相应的文件
| src/main\_loop              | 嵌入式系统一般使用 main\_loop\_simple.c
| src/misc                    | 如果没有标准的内存管理函数请加入
| src/native\_window               | 一般加入 native\_window\_raw.*
| src/window\_manager              | 一般加入 window\_manager\_default.*
| src/graphic\_buffer              | 一般加入 graphic\_buffer\_default.*
| src/platforms/raw/fs\_os.c       | 如果没有文件系统请加入，否则请自行实现 fs 接口
| src/platforms/raw/mutex\_null.c  | 如果没有 mutex 请加入，否则请自行实现 mutex 接口
| src/platforms/raw/sys\_tick.c    | 如果需要自己实现 sys tick 中断，可以加入
| src/vgcanvas                     | 如果需要矢量图或图片旋转（不支持 lcd\_reg) 请加入 vgcanvas\_nanovg_soft.c，否则加入 vgcanvas\_null.c
| src/widget\_animators            | 除了使用 LCD\_REG 的平台外推荐全部加入
| src/window\_animators            | 启用 OpenGL(PC/Android/iOS) 时加入 window\_animator\_gpu.c，使用 Framebuffer（一般嵌入式）时加入 window\_animator\_fb.c，低端嵌入式平台请加入 window\_animator\_null.c
| src/ubjson                       | 不用不加。
| src/streams                      | 一般不加。
| awtk/3rd/agge                    | 在 framebuffer 模式启用 vgcanvas 时加入
| awtk/3rd/gpinyin/src             | 启用拼音输入法时加入
| awtk/3rd/nanovg/base             | 在启用 vgcanvas 时加入
| awtk/3rd/nanovg/agge             | 在启用 vgcanvas 时加入
| awtk/3rd/libunibreak             | 建议加入
| awtk-port                        | 加入各个平台自己的移植代码。
| awtk/demos                       | 在使用 ui demos 可以加入：assets.c demo\_main.c demo\_ui\_app.c。

### 四、其它

* 1. 如果要加载 png/jpg 图片格式 StackSize 不小于 32K。 
* 2. 使用 keil 编译时，请在 C/C++ -> Misc Controls 中加上**--gnu **标志。
* 3. 如果系统有 FPU，初始化时一定要开启 FPU。
* 4. 在工程中定义宏 HAS\_AWTK\_CONFIG。
