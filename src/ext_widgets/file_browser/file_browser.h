/**
 * File:   file_browser.h
 * Author: AWTK Develop Team
 * Brief:  file_browser
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FILE_BROWSER_H
#define TK_FILE_BROWSER_H

#include "tkc/fs.h"
#include "tkc/str.h"
#include "tkc/darray.h"
#include "tkc/emitter.h"

BEGIN_C_DECLS

typedef struct _filebrowser_item_t {
  fs_item_t info;
  fs_file_stat_t stat;
} filebrowser_item_t;

/**
 * @class file_browser_t
 * 文件浏览器模型。
 */
typedef struct _file_browser_t {
  emitter_t emitter;
  /**
   * @property {str_t} cwd
   * 当前路径
   *
   */
  str_t cwd;

  /*private*/

  fs_t* fs;
  darray_t items;
  tk_compare_t compare;
  tk_filter_t filter;
} file_browser_t;

/**
 * @event {event_t} EVT_ITEMS_WILL_CHANGE
 * 文件项目改变
 */

/**
 * @event {progress_event_t} EVT_PROGRESS
 * 进度改变。
 */

/**
 * @method file_browser_create
 * 创建file_browser对象
 * 
 * @annotation ["constructor"]
 *
 * @return {file_browser_t*} 对象。
 */
file_browser_t* file_browser_create(fs_t* fs);

/**
 * @method file_browser_set_cwd
 * 设置当前文件路径。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 * @param {const char*} cwd 当前目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_set_cwd(file_browser_t* fb, const char* cwd);

/**
 * @method file_browser_create_dir
 * 在当前目录下创建指定子目录。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 * @param {const char*} name 当前目录下的子目录名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_create_dir(file_browser_t* fb, const char* name);

/**
 * @method file_browser_remove_dir
 * 删除当前目录下的指定子目录。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 * @param {const char*} name 当前目录下的子目录名/文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_remove_item(file_browser_t* fb, const char* name);

/**
 * @method file_browser_enter
 * 进入当前目录下的指定子目录。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 * @param {const char*} dir 当前目录下的子目录名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_enter(file_browser_t* fb, const char* dir);

/**
 * @method file_browser_up
 * 返回上一级目录。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_up(file_browser_t* fb);

/**
 * @method file_browser_set_filter
 * 设置过滤函数。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 * @param {tk_filter_t} filter 过滤函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_set_filter(file_browser_t* fb, tk_filter_t filter);

/**
 * @method file_browser_set_compare
 * 设置排序用的比较函数。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 * @param {tk_compare_t} compare 排序用的比较函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_set_compare(file_browser_t* fb, tk_compare_t compare);

/**
 * @method file_browser_get_items_nr
 * 获取当前目录下文件项个数。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 *
 * @return {uint32_t} 返回当前目录下文件项个数。
 */
uint32_t file_browser_get_items_nr(file_browser_t* fb);

/**
 * @method file_browser_get_item
 * 获取指定项序数的项目。
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 * @param {uint32_t} index 序数。
 *
 * @return {filebrowser_item_t*} 返回指定项序数的项目。
 */
filebrowser_item_t* file_browser_get_item(file_browser_t* fb, uint32_t index);

/**
 * @method file_browser_destroy
 * 销毁file_browser对象
 * 
 * @param {file_browser_t*} fb filebrowser对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_destroy(file_browser_t* fb);

END_C_DECLS

#endif /*TK_FILE_BROWSER_H*/
