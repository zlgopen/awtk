/**
 * File:   file_browser_view.h
 * Author: AWTK Develop Team
 * Brief:  文件管理/浏览/选择控件
 *
 * Copyright (c) 2020 - 2020 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FILE_BROWSER_VIEW_H
#define TK_FILE_BROWSER_VIEW_H

#include "base/widget.h"
#include "file_browser/file_browser.h"

BEGIN_C_DECLS
/**
 * @class file_browser_view_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 文件管理/浏览/选择控件
 */
typedef struct _file_browser_view_t {
  widget_t widget;

  /**
   * @property {char*} init_dir
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 初始目录。
   */
  char* init_dir;

  /**
   * @property {char*} file_icon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 文件项的图标。
   */
  char* file_icon;

  /**
   * @property {char*} folder_icon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 目录项的图标。
   */
  char* folder_icon;

  /**
   * @property {char*} return_up_icon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 返回上一级目录的图标。
   */
  char* return_up_icon;

  /*private*/
  file_browser_t* fb;

  widget_t* container;
  widget_t* file_template;
  widget_t* folder_template;
  widget_t* return_up_template;

  darray_t file_items_cache;
  darray_t folder_items_cache;
} file_browser_view_t;

/**
 * @method file_browser_view_create
 * @annotation ["constructor", "scriptable"]
 * 创建file_browser_view对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* file_browser_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method file_browser_view_cast
 * 转换为file_browser_view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget file_browser_view对象。
 *
 * @return {widget_t*} file_browser_view对象。
 */
widget_t* file_browser_view_cast(widget_t* widget);

/**
 * @method file_browser_view_set_init_dir
 * 设置 初始目录。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} init_dir 初始目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_init_dir(widget_t* widget, const char* init_dir);

/**
 * @method file_browser_view_set_file_icon
 * 设置 文件项的图标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} file_icon 文件项的图标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_file_icon(widget_t* widget, const char* file_icon);

/**
 * @method file_browser_view_set_folder_icon
 * 设置 目录项的图标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} folder_icon 目录项的图标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_folder_icon(widget_t* widget, const char* folder_icon);

/**
 * @method file_browser_view_set_return_up_icon
 * 设置 返回上一级目录的图标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} return_up_icon 返回上一级目录的图标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_return_up_icon(widget_t* widget, const char* return_up_icon);

#define FILE_BROWSER_VIEW_PROP_INIT_DIR "init_dir"
#define FILE_BROWSER_VIEW_PROP_FILE_ICON "file_icon"
#define FILE_BROWSER_VIEW_PROP_FOLDER_ICON "folder_icon"
#define FILE_BROWSER_VIEW_PROP_RETURN_UP_ICON "return_up_icon"

#define WIDGET_TYPE_FILE_BROWSER_VIEW "file_browser_view"

#define FILE_BROWSER_VIEW(widget) ((file_browser_view_t*)(file_browser_view_cast(WIDGET(widget))))

ret_t file_browser_view_register(void);

#define FILE_BROWSER_VIEW_NAME "name"
#define FILE_BROWSER_VIEW_ICON "icon"
#define FILE_BROWSER_VIEW_FILE "file"
#define FILE_BROWSER_VIEW_FOLDER "folder"
#define FILE_BROWSER_VIEW_RETURN_UP "return_up"
#define FILE_BROWSER_VIEW_CONTAINER "container"

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(file_browser_view);

END_C_DECLS

#endif /*TK_FILE_BROWSER_VIEW_H*/
