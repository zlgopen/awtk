## 嵌入式平台移植注意事项

### 一、配置的宏

```
/**
 * 嵌入式系统有自己的main函数时，请定义本宏。
 *
 * #define USE_GUI_MAIN 1
 */

/**
 * 如果需要支持预先解码的位图字体，请定义本宏。一般只在RAM极小时，才启用本宏。
 * #define WITH_BITMAP_FONT 1
 */

/**
 * 如果支持png/jpeg图片，请定义本宏
 *
 * #define WITH_STB_IMAGE 1
 */

/**
 * 如果用stb支持Truetype字体，请定义本宏
 *
 * #define WITH_STB_FONT 1
 */

/**
 * 如果用freetype支持Truetype字体，请定义本宏。
 *
 * #define WITH_FT_FONT 1
 */

/**
 * 如果支持从文件系统加载资源，请定义本宏
 *
 * #define WITH_FS_RES 1
 */

/**
 * 如果定义本宏，使用标准的UNICODE换行算法，除非资源极为有限，请定义本宏。
 *
 * #define WITH_UNICODE_BREAK 1
 */

/**
 * 如果定义本宏，将图片解码成BGRA8888格式，否则解码成RGBA8888的格式。
 * 当硬件的2D加速需要BGRA格式时，请启用本宏。
 *
 * #define WITH_BITMAP_BGRA 1
 */

/**
 * 如果定义本宏，将不透明的PNG图片解码成BGR565格式，建议定义。
 * 另外和LCD的格式保存一致，可以大幅度提高性能。
 *
 * #define WITH_BITMAP_BGR565 1
 */

/**
 * 如果不需输入法，请定义本宏
 *
 * #define WITH_NULL_IM 1
 */

/**
 * 如果有标准的malloc/free/calloc等函数，请定义本宏
 *
 * #define HAS_STD_MALLOC 1
 */

/**
 * 如果有标准的fopen/fclose等函数，请定义本宏
 *
 * #define HAS_STDIO 1
 */

/**
 * 如果有标准的pthread等函数，请定义本宏
 *
 * #define HAS_PTHREAD 1
 */

/**
 * 如果有优化版本的memcpy函数，请定义本宏
 *
 * #define HAS_FAST_MEMCPY 1
 */

/**
 * 如果出现wcsxxx之类的函数没有定义时，请定义该宏
 *
 * #define WITH_WCSXXX 1
 */

/**
 * 如果启用STM32 G2D硬件加速，请定义本宏
 *
 * #define WITH_STM32_G2D 1
 */

/**
 * 如果启用NXP PXP硬件加速，请定义本宏
 *
 * #define WITH_PXP_G2D 1
 */

/**
 * 在没有GPU时，如果启用agge作为nanovg的后端(较agg作为后端：小，快，图形质量稍差)，请定义本宏。
 *
 * #define WITH_NANOVG_AGGE 1
 */

/**
 * 在没有GPU时，如果启用agg作为nanovg的后端(较agge作为后端：大，慢，图形质量好)，请定义本宏。
 * 注意：agg是以GPL协议开源。
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
 * 对于低端平台，如果内存不足以提供完整的FrameBuffer，请定义本宏启用局部FrameBuffer，可大幅度提高渲染性能。
 *
 * #define FRAGMENT_FRAME_BUFFER_SIZE 32 * 1024
 */
```

> 请参考：src/base/awtk\_config\_sample.h

### 二、include路径

```
awtk/src
awtk/src/ext_widgets
awtk/3rd
awtk/3rd/agge
awtk/3rd/nanovg/
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
| src/svg                     | 支持vgcanvas时全部加入
| src/blend                   | 使用framebuffer时全部加入
| src/ext\_widgets            | 使用扩展控件时全部加入
| src/font\_loader            | 推荐全部加入(可只加需要的)
| src/image\_loader           | 推荐全部加入(可只加需要的)
| src/input\_engines          | 不需要输入法时加入input\_engine\_null.c，否则加入input\_engine\_pinyin.cpp
| src/input\_methods          | 加入input\_method\_creator.c，根据宏决定启用何种输入法
| src/lcd                     | 根据LCD类型(OpenGL/FrameBuffer/Register）选择相应的文件
| src/main\_loop              | 嵌入式系统一般使用main\_loop\_simple.c
| src/misc                    | 如果没有标准的内存管理函数请加入
| src/platforms/raw/fs\_os.c       | 如果没有文件系统请加入，否则请自行实现fs接口
| src/platforms/raw/mutex\_null.c  | 如果没有mutex请加入，否则请自行实现mutex接口
| src/platforms/raw/sys\_tick.c    | 如果需要自己实现sys tick中断，可以加入
| src/vgcanvas                     | 如果需要矢量图或图片旋转(不支持lcd\_reg)请加入vgcanvas\_nanovg_soft.c，否则加入vgcanvas\_null.c
| src/widget\_animators            | 除了使用LCD\_REG的平台外推荐全部加入
| src/window\_animators            | 启用OpenGL(PC/Android/iOS)时加入window\_animator\_gpu.c，使用Framebuffer(一般嵌入式)时加入window\_animator\_fb.c，低端嵌入式平台请加入window\_animator\_null.c
| awtk/3rd/agge                    | 在framebuffer模式启用vgcanvas时加入
| awtk/3rd/gpinyin/src             | 启用拼音输入法时加入
| awtk/3rd/nanovg/base             | 在启用vgcanvas时加入
| awtk/3rd/nanovg/agge             | 在启用vgcanvas时加入
| awtk/3rd/libunibreak             | 建议加入
| awtk-port                        | 加入各个平台自己的移植代码。
| awtk/demos                       | 在使用ui demos可以加入：assets.c demo\_main.c demo\_ui\_app.c。

### 四、其它

* 1.如果要加载png/jpg图片格式StackSize不小于32K。 
* 2.使用keil编译时，请在C/C++ -> Misc Controls中加上**--gnu**标志。
* 3.如果系统有FPU，初始化时一定要开启FPU。


