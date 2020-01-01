/**
 * File:   ui_loader.h
 * Author: AWTK Develop Team
 * Brief:  ui_loader interface
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UI_LOADER_H
#define TK_UI_LOADER_H

#include "base/ui_builder.h"

BEGIN_C_DECLS

struct _ui_loader_t;
typedef struct _ui_loader_t ui_loader_t;

typedef ret_t (*ui_loader_load_t)(ui_loader_t* loader, const uint8_t* data, uint32_t size,
                                  ui_builder_t* b);

/**
 * @class ui_loader_t
 *
 * UI加载器。
 *
 * 负责从资源加载界面描述数据，然后分发给ui\_builder，由ui\_builder进一步处理。
 *
 * 常见的实现有两种：
 *
 * * XML格式的加载器。
 *
 * * 二进制格式的加载器。
 *
 * ```graphviz
 *   [default_style]
 *
 *   ui_loader_defualt_t -> ui_loader_t[arrowhead = "empty"]
 *   ui_loader_xml_t -> ui_loader_t[arrowhead = "empty"]
 *
 * ```
 *
 */
struct _ui_loader_t {
  ui_loader_load_t load;
};

/**
 * @method ui_loader_load
 * 加载界面描述数据，然后分发给ui\_builder，由ui\_builder进一步处理。
 *
 * @param {ui_loader_t*} loader loader对象。
 * @param {const uint8_t*} data 数据。
 * @param {uint32_t} size 数据长度。
 * @param {ui_builder_t*} builder 负责进一步处理，比如构建UI界面。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t ui_loader_load(ui_loader_t* loader, const uint8_t* data, uint32_t size,
                     ui_builder_t* builder);

/**
 * @method ui_loader_load_widget
 *
 * 从指定的资源加载widget对象，通常用于加载非窗口的widget。
 *
 *>需要自己将widget加入窗口后，再调用widget\_layout。
 *
 * @param {const char*} name 资源名。
 *
 * @return {widget_t*} 返回widget对象。
 *
 */
widget_t* ui_loader_load_widget(const char* name);

END_C_DECLS

#endif /*TK_UI_LOADER_H*/
