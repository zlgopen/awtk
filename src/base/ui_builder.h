/**
 * File:   ui_builder.h
 * Author: AWTK Develop Team
 * Brief:  ui_builder
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

#ifndef TK_UI_BUILDER_H
#define TK_UI_BUILDER_H

#include "base/widget.h"
#include "base/layout.h"

BEGIN_C_DECLS

struct _ui_builder_t;
typedef struct _ui_builder_t ui_builder_t;

typedef struct _widget_desc_t {
  char type[TK_NAME_LEN + 1];
  rect_t layout;
} widget_desc_t;

typedef ret_t (*ui_builder_on_start_t)(ui_builder_t* b);
typedef ret_t (*ui_builder_on_widget_start_t)(ui_builder_t* b, const widget_desc_t* desc);
typedef ret_t (*ui_builder_on_widget_prop_t)(ui_builder_t* b, const char* name, const char* value);
typedef ret_t (*ui_builder_on_widget_prop_end_t)(ui_builder_t* b);
typedef ret_t (*ui_builder_on_widget_end_t)(ui_builder_t* b);
typedef ret_t (*ui_builder_on_end_t)(ui_builder_t* b);

/**
 * @class ui_builder_t
 *
 * 界面构造器接口。
 *
 * 负责把ui\_loader解析的数据进一步处理，至于具体做什么，由具体实现决定。常见的实现有：
 *
 * * 构建widget对象树。
 *
 * * 生成二进制的UI描述数据。
 *
 * * 生成XML格式的UI描述数据。
 *
 * ```graphviz
 *   [default_style]
 *
 *   ui_builder_defualt_t -> ui_builder_t[arrowhead = "empty"]
 *   ui_xml_writer_t -> ui_builder_t[arrowhead = "empty"]
 *   ui_binary_writer_t -> ui_builder_t[arrowhead = "empty"]
 * ```
 */
struct _ui_builder_t {
  ui_builder_on_start_t on_start;
  ui_builder_on_widget_start_t on_widget_start;
  ui_builder_on_widget_prop_t on_widget_prop;
  ui_builder_on_widget_prop_end_t on_widget_prop_end;
  ui_builder_on_widget_end_t on_widget_end;
  ui_builder_on_end_t on_end;
  widget_t* root;
  widget_t* widget;
  const char* name;
};

/**
 * @method ui_builder_on_widget_start
 * ui\_loader在解析到widget时，调用本函数进一步处理。
 *
 * @param {ui_builder_t*} builder builder对象。
 * @param {const widget_desc_t*} desc widget描述信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t ui_builder_on_widget_start(ui_builder_t* builder, const widget_desc_t* desc);

/**
 * @method ui_builder_on_widget_prop
 * ui\_loader在解析到widget的属性时，调用本函数进一步处理。
 *
 * @param {ui_builder_t*} builder builder对象。
 * @param {const char*} name 属性名。
 * @param {const char*} value 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t ui_builder_on_widget_prop(ui_builder_t* builder, const char* name, const char* value);

/**
 * @method ui_builder_on_widget_prop_end
 * ui\_loader在解析到widget全部属性结束时，调用本函数进一步处理。
 *
 * @param {ui_builder_t*} builder builder对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t ui_builder_on_widget_prop_end(ui_builder_t* builder);

/**
 * @method ui_builder_on_widget_end
 * ui\_loader在解析到widget结束时，调用本函数进一步处理。
 *
 * @param {ui_builder_t*} builder builder对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t ui_builder_on_widget_end(ui_builder_t* builder);

/**
 * @method ui_builder_on_start
 * ui\_loader在解析开始时，调用本函数做些准备工作。
 *
 * @param {ui_builder_t*} builder builder对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t ui_builder_on_start(ui_builder_t* builder);

/**
 * @method ui_builder_on_end
 * ui\_loader在解析结束时，调用本函数做些收尾工作。
 *
 * @param {ui_builder_t*} builder builder对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t ui_builder_on_end(ui_builder_t* builder);

#define UI_DATA_MAGIC 0x11221212

END_C_DECLS

#endif /*TK_UI_BUILDER_H*/
