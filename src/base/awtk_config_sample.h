
/**
 * File:   awtk_config.h
 * Author: AWTK Develop Team
 * Brief:  config
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-09-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef AWTK_CONFIG_H
#define AWTK_CONFIG_H

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
 * 如果代码在flash中，而资源在文件系统，请定义本宏指明资源所在的路径。
 * 
 * #define APP_RES_ROOT "0://awtk/"
 * 
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
 * 如果没有定义 WITH_BITMAP_BGR565 和 WITH_BITMAP_RGB565 宏，默认解析为32位色
 *
 * #define WITH_BITMAP_BGR565 1
 */

/**
 * 如果定义本宏，将不透明的PNG图片解码成RGB565格式，建议定义。
 * 另外和LCD的格式保存一致，可以大幅度提高性能。
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
 * 对于低端平台，如果内存不足以提供完整的FrameBuffer，请定义本宏启用局部FrameBuffer，可大幅度提高渲染性能。(单位是像素个数)
 *
 * #define FRAGMENT_FRAME_BUFFER_SIZE 32 * 1024
 */

/**
 * 启用widget类型检查，请定义本宏(除非编译器不支持，否则请定义它)。
 *
 * #define WITH_WIDGET_TYPE_CHECK 1
 */

/**
 * 启用输入法，但不想启用联想功能，请定义本宏。
 *
 * #define WITHOUT_SUGGEST_WORDS 1
 */

/**
 * 如果需要从zip文件中加载资源，请定义本宏。
 *
 * #define WITH_ASSET_LOADER_ZIP 1
 */

/**
 * 对于只有512K flash的平台，而且LCD格式是BGR565。如果希望进一步优化空间，去掉多余的bitmap格式支持代码。请定义本宏。
 * 其它LCD格式，可以自行修改：src/blend/soft_g2d.c 保留需要的格式即可。
 *
 * #define LCD_BGR565_LITE 1
 */

/**
 * 如果希望支持文字双向排版算法(如阿拉伯语言)，请定义本宏。
 * 
 * #define WITH_TEXT_BIDI 1
 * 
 */

/**
 * 开启自定义的离线 canvas，如果板子使用特殊的画布的话，需要定义该宏来定义特殊离线 canvas 函数
 * 
 * #define WITH_CANVAS_OFFLINE_CUSTION 1
 */

/**
 * 开启透明色背景的刷新机制，一般使用在多图层的透明背景使用
 * 
 * #define WITH_LCD_CLEAR_ALPHA 1
 */

/**
 * 如果支持多块不连续的内存块，请定义内存块的数目。
 * 
 * #define TK_MAX_MEM_BLOCK_NR 4
 */

/**
 * 用于控制窗口动画是否使用缓存。开启后可以降低内存需求(减少2倍framebuffer大小的内存)，如果内存紧缺，在下面情况可以开启：
 *  1. 界面简单
 *  2. CPU速度快
 *
 * 如果绘制速度慢，而且内存紧缺，建议关闭窗口动画。
 *
 * 限制条件：
 *  1.不支持缩放窗口动画。
 *  2.不支持对话框高亮策略。
 * 
 * #define WITHOUT_WINDOW_ANIMATOR_CACHE 1
 */

/**
 * 如果需要配置文件或者使用data_reader/data_writer，请定义本宏。
 *
 * #define WITH_DATA_READER_WRITER 1
 */

/**
 * 对于低端平台，如果不使用 fscript 模块，请定义本宏。
 *
 * #define WITHOUT_FSCRIPT 1
 */

/**
 * 对于极简键盘(3keys/5keys)，如果希望激活状态呈现不同的外观效果，请定义本宏。 
 *
 * #define WITH_STATE_ACTIVATED 1
 */

/**
 * 开启高效的 lcd 旋转功能模块（几乎无损的屏幕旋转效果）
 * 注意：
 * 1，需要重新编译资源，把相关图片预先处理旋转后的位图数据。(如果需要实现运行时动态改变旋转的话，图片资源的只能为未旋转)
 * 2，lcd 适配层需要支持。
 * 3，vg 的适配层需要支持。
 * 4，g2d 的适配层获取位图真实的数据需要通过 bitmap_get_physical_XXXX 函数来获取。
 *
 * #define WITH_FAST_LCD_PORTRAIT 1
 */

#endif /*AWTK_CONFIG_H*/
