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
 * 文件管理/浏览/选择控件。
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
   * @property {bool_t} ignore_hidden_files
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 忽略隐藏文件。
   */
  bool_t ignore_hidden_files;

  /**
   * @property {bool_t} sort_ascending
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否为升序排序。
   */
  bool_t sort_ascending;

  /**
   * @property {char*} sort_by
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 排序方式。可选值(name, size, mtime)。
   */
  char* sort_by;

  /**
   * @property {char*} value
   * @fake
   * @annotation ["set_prop","get_prop"]
   * 当前选中项的完整路径。
   */

  /*private*/
  bool_t inited;
  file_browser_t* fb;

  widget_t* cut;
  widget_t* copy;
  widget_t* paste;
  widget_t* remove;

  widget_t* cwd;
  widget_t* container;
  widget_t* file_template;
  widget_t* folder_template;
  widget_t* return_up_template;

  darray_t file_items_cache;
  darray_t folder_items_cache;
  darray_t selected_items;

  str_t value;
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
 * @method file_browser_view_set_ignore_hidden_files
 * 设置 忽略隐藏文件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} ignore_hidden_files 忽略隐藏文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_ignore_hidden_files(widget_t* widget, bool_t ignore_hidden_files);

/**
 * @method file_browser_view_set_sort_ascending
 * 设置 是否为升序排序。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} sort_ascending 是否为升序排序。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_sort_ascending(widget_t* widget, bool_t sort_ascending);

/**
 * @method file_browser_view_set_sort_by
 * 设置 排序方式。可选值(name, size, mtime)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} sort_by 排序方式。可选值(name, size, mtime)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_sort_by(widget_t* widget, const char* sort_by);

/**
 * @method file_browser_view_get_selected_items_nr
 * 返回当前选中的项目的个数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {uint32_t} 返回返回当前选中的项目的个数。
 */
uint32_t file_browser_view_get_selected_items_nr(widget_t* widget);

/**
 * @method file_browser_view_get_selected_item
 * 获取第index个选中的项目的名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} index 序号。
 *
 * @return {const char*} 返回第index个选中的项目的名称。
 */
const char* file_browser_view_get_selected_item(widget_t* widget, uint32_t index);

/**
 * @method file_browser_view_get_cwd
 * 获取当前路径。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {const char*} 返回当前路径。
 */
const char* file_browser_view_get_cwd(widget_t* widget);

/**
 * @method file_browser_view_copy
 * 拷贝当前选中的项目。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_copy(widget_t* widget);

/**
 * @method file_browser_view_cut
 * 剪切当前选中的项目。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_cut(widget_t* widget);

/**
 * @method file_browser_view_remove
 * 删除当前选中的项目。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_remove(widget_t* widget);

/**
 * @method file_browser_view_paste
 * 粘贴之前拷贝或剪切的项目到当前目录。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_paste(widget_t* widget);

/**
 * @method file_browser_view_can_paste
 * 检查是否可以粘贴(之前是否拷贝和剪切)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {bool_t} 返回FALSE表示不可以粘贴，否则表示可以。
 */
bool_t file_browser_view_can_paste(widget_t* widget);

/**
 * @method file_browser_view_create_dir
 * 在当前目录创建子目录。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} name 子目录名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_create_dir(widget_t* widget, const char* name);

/**
 * @method file_browser_view_create_file
 * 在当前目录创建文件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} name 文件名。
 * @param {const char*} data 数据。
 * @param {uint32_t} size 数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_create_file(widget_t* widget, const char* name, const char* data,
                                    uint32_t size);

#define FILE_BROWSER_VIEW_PROP_SORT_BY "sort_by"
#define FILE_BROWSER_VIEW_PROP_INIT_DIR "init_dir"
#define FILE_BROWSER_VIEW_PROP_SORT_ASCENDING "sort_ascending"
#define FILE_BROWSER_VIEW_PROP_IGNORE_HIDDEN_FILES "ignore_hidden_files"

#define WIDGET_TYPE_FILE_BROWSER_VIEW "file_browser_view"

#define FILE_BROWSER_VIEW(widget) ((file_browser_view_t*)(file_browser_view_cast(WIDGET(widget))))

ret_t file_browser_view_register(void);

/*特殊子控件的名字*/
#define FILE_BROWSER_VIEW_CWD "cwd"
#define FILE_BROWSER_VIEW_NAME "name"
#define FILE_BROWSER_VIEW_ICON "icon"
#define FILE_BROWSER_VIEW_FILE "file"
#define FILE_BROWSER_VIEW_FOLDER "folder"
#define FILE_BROWSER_VIEW_RETURN_UP "return_up"
#define FILE_BROWSER_VIEW_CONTAINER "container"

#define FILE_BROWSER_VIEW_CUT "cut"
#define FILE_BROWSER_VIEW_COPY "copy"
#define FILE_BROWSER_VIEW_PASTE "paste"
#define FILE_BROWSER_VIEW_REMOVE "remove"

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(file_browser_view);

END_C_DECLS

#endif /*TK_FILE_BROWSER_VIEW_H*/
