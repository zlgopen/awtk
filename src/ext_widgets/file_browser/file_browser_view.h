/**
 * File:   file_browser_view.h
 * Author: AWTK Develop Team
 * Brief:  file manager/browser/choosor
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
 * 文件管理/浏览/选择控件。
 *
 * file\_browser\_view\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于file\_browser\_view\_t控件。
 *
 * 考虑到文件浏览器界面呈现的多样性，界面呈现工作完全有子控件来完成。
 * 
 * file\_browser\_view\_t负责关联文件/文件夹数据到子控件上，子控件需要特定的规范命名。
 * 
 * * 名为 "cwd" 的子控件用于显示当前路径。
 *
 * * 名为 "selected_file" 的子控件用于显示当前选择的文件。
 *
 * * 名为 "file" 的子控件用于显示文件项的模板控件。 
 *
 * * 名为 "folder" 的子控件用于显示文件夹项的模板控件。
 *
 * * 名为 "return_up" 的子控件用于返回上一级文件夹的模板控件。
 *
 * * 名为 "container" 的子控件为容器控件，通常是scrollview。
 *
 * * 名为 "name" 的子控件用于显示文件和文件夹的名称(放在列表项目内)。
 
 * * 名为 "size" 的子控件用于显示文件和文件夹的大小(放在列表项目内)。
 *
 * * 名为 "mtime" 的子控件用于显示文件和文件夹的修改时间(放在列表项目内)。
 *
 * * 名为 "ctime" 的子控件用于显示文件和文件夹的创建时间(放在列表项目内)。
 *
 * * 名为 "icon" 的子控件用于显示文件和文件夹的图标(放在列表项目内)。
 *
 * * 类型为 "check_button" 的子控件用于选择(放在列表项目内)。
 *
 * 完整示例请参考：
 *
 * https://github.com/zlgopen/awtk/blob/master/design/default/ui/file_chooser_for_open.xml
 *
 */
typedef struct _file_browser_view_t {
  widget_t widget;

  /**
   * @property {char*} init_dir
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 初始文件夹。
   */
  char* init_dir;

  /**
   * @property {char*} filter
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 过滤规则。
   */
  char* filter;

  /**
   * @property {bool_t} ignore_hidden_files
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否忽略隐藏文件。
   */
  bool_t ignore_hidden_files;

  /**
   * @property {bool_t} sort_ascending
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否为升序排序。
   */
  bool_t sort_ascending;

  /**
   * @property {bool_t} show_check_button
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否显示checkbutton。
   */
  bool_t show_check_button;

  /**
   * @property {char*} sort_by
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 排序方式。可选值(name, size, mtime, type)。
   */
  char* sort_by;

  /*private*/
  bool_t inited;
  file_browser_t* fb;

  widget_t* cwd;
  widget_t* selected_file;
  widget_t* container;
  widget_t* file_template;
  widget_t* folder_template;
  widget_t* return_up_template;

  darray_t selected_items;
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
 * 设置 初始文件夹。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} init_dir 初始文件夹。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_init_dir(widget_t* widget, const char* init_dir);

/**
 * @method file_browser_view_set_filter
 * 设置 过滤规则。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} filter 过滤规则。 
 *> files_only 表示只列出文件，dir_only 表示只列出目录，其它表示只列出满足扩展名文件集合(如：.jpg.png.gif)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_filter(widget_t* widget, const char* filter);

/**
 * @method file_browser_view_reload
 * 重新加载。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_reload(widget_t* widget);

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
 * @method file_browser_view_set_show_check_button
 * 设置 是否显示checkbutton。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} show_check_button 是否显示checkbutton。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_show_check_button(widget_t* widget, bool_t show_check_button);

/**
 * @method file_browser_view_set_sort_by
 * 设置 排序方式。可选值(name, size, mtime, type)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} sort_by 排序方式。可选值(name, size, mtime, type)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_set_sort_by(widget_t* widget, const char* sort_by);

/**
 * @method file_browser_view_get_selected_items
 * 返回当前选中的项目。
 *
 * > 在返回数组中，每个元素是一个fb\_item\_t对象。
 * > 调用者无需释放返回值，返回值只在当次调用有效。
 *
 * @param {widget_t*} widget widget对象。
 *
 * @return {darray_t*} 返回当前选中的项目。
 */
darray_t* file_browser_view_get_selected_items(widget_t* widget);

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
 * @method file_browser_view_create_dir
 * 在当前文件夹创建子文件夹。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} name 子文件夹名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_view_create_dir(widget_t* widget, const char* name);

/**
 * @method file_browser_view_create_file
 * 在当前文件夹创建文件。
 *
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
#define FILE_BROWSER_VIEW_PROP_SHOW_CHECK_BUTTON "show_check_button"
#define FILE_BROWSER_VIEW_PROP_IGNORE_HIDDEN_FILES "ignore_hidden_files"

#define WIDGET_TYPE_FILE_BROWSER_VIEW "file_browser_view"

#define FILE_BROWSER_VIEW(widget) ((file_browser_view_t*)(file_browser_view_cast(WIDGET(widget))))

ret_t file_browser_view_register(void);

/*特殊子控件的名字*/

/*用于显示当前路径的控件*/
#define FILE_BROWSER_VIEW_CWD "cwd"
#define FILE_BROWSER_VIEW_SELECTED_FILE "selected_file"

/*用于显示名称的控件*/
#define FILE_BROWSER_VIEW_NAME "name"
/*用于显示文件大小的控件*/
#define FILE_BROWSER_VIEW_SIZE "size"
/*用于显示文件修改时间的控件*/
#define FILE_BROWSER_VIEW_MTIME "mtime"
/*用于显示文件创建时间的控件*/
#define FILE_BROWSER_VIEW_CTIME "ctime"
/*用于显示文件图标的控件*/
#define FILE_BROWSER_VIEW_ICON "icon"

/*用于显示文件项的模板控件*/
#define FILE_BROWSER_VIEW_FILE "file"
/*用于显示文件夹项的模板控件*/
#define FILE_BROWSER_VIEW_FOLDER "folder"
/*用于显示返回上一级文件夹的模板控件*/
#define FILE_BROWSER_VIEW_RETURN_UP "return_up"
/*容器控件，通常是scrollview*/
#define FILE_BROWSER_VIEW_CONTAINER "container"

#define STR_FILTER_FILES_ONLY "files_only"
#define STR_FILTER_DIR_ONLY "dir_only"

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(file_browser_view);

END_C_DECLS

#endif /*TK_FILE_BROWSER_VIEW_H*/
