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
#include "tkc/buffer.h"
#include "tkc/darray.h"
#include "tkc/emitter.h"

BEGIN_C_DECLS

/**
 * @class fb_item_t
 * 文件或目录的信息。
 */
typedef struct _fb_item_t {
  /**
   * @property {char*} name
   * @annotation ["readable"]
   * 名称。
   */
  char* name;
  /**
   * @property {bool_t} is_dir
   * @annotation ["readable"]
   * 是否是目录。
   */
  bool_t is_dir;
  /**
   * @property {bool_t} is_reg_file
   * @annotation ["readable"]
   * 是否是常规文件。
   */
  bool_t is_reg_file;
  /**
   * @property {uint64_t} size
   * @annotation ["readable"]
   * 文件大小。
   */
  uint64_t size;
  /**
   * @property {uint64_t} atime
   * @annotation ["readable"]
   * 最后一次访问时间。
   */
  uint64_t atime;
  /**
   * @property {uint64_t} mtime
   * @annotation ["readable"]
   * 最后修改时间。
   */
  uint64_t mtime;
  /**
   * @property {uint64_t} ctime
   * @annotation ["readable"]
   * 创建时间。
   */
  uint64_t ctime;
} fb_item_t;

/**
 * @class file_browser_t
 * 文件浏览器模型。
 */
typedef struct _file_browser_t {
  /**
   * @property {char*} cwd
   * @annotation ["readable"]
   * 当前路径。
   */
  char cwd[MAX_PATH + 1];

  /*private*/
  fs_t* fs;
  fb_item_t* items;
  uint32_t items_size;
  uint32_t items_capacity;

  void* filter_ctx;
  tk_filter_t filter;
  tk_compare_t compare;

  bool_t ignore_hidden_files;
} file_browser_t;

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
 * 设置当前目录。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {const char*} cwd 当前目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_set_cwd(file_browser_t* fb, const char* cwd);

/**
 * @method file_browser_create_dir
 * 在当前目录下创建指定子目录。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {const char*} name 当前目录下的子目录名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_create_dir(file_browser_t* fb, const char* name);

/**
 * @method file_browser_create_file
 * 在当前目录下创建指定文件。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {const char*} name 当前目录下的文件名。
 * @param {const void*} buff 文件内容。
 * @param {uint32_t} size 文件内容的长度。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_create_file(file_browser_t* fb, const char* name, const void* buff,
                               uint32_t size);

/**
 * @method file_browser_remove
 * 删除当前目录下的指定子目录或文件。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {const char*} name 当前目录下的子目录名/文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_remove(file_browser_t* fb, const char* name);

/**
 * @method file_browser_enter
 * 进入当前目录下的指定子目录。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {const char*} dir 当前目录下的子目录名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_enter(file_browser_t* fb, const char* dir);

/**
 * @method file_browser_up
 * 返回上一级目录。
 * 
 * @param {file_browser_t*} fb file browser对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_up(file_browser_t* fb);

/**
 * @method file_browser_refresh
 * 刷新。
 * 
 * @param {file_browser_t*} fb file browser对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_refresh(file_browser_t* fb);

/**
 * @method file_browser_set_filter
 * 设置过滤函数。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {tk_filter_t} filter 过滤函数。
 * @param {void*} ctx 过滤函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_set_filter(file_browser_t* fb, tk_filter_t filter, void* ctx);

/**
 * @method file_browser_set_compare
 * 设置排序用的比较函数。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {tk_compare_t} compare 排序用的比较函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_set_compare(file_browser_t* fb, tk_compare_t compare);

/**
 * @method file_browser_sort_by_name
 * 按名称排序。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {bool_t} ascending 按升序排。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_sort_by_name(file_browser_t* fb, bool_t ascending);

/**
 * @method file_browser_sort_by_type
 * 按类型排序。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {bool_t} ascending 按升序排。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_sort_by_type(file_browser_t* fb, bool_t ascending);

/**
 * @method file_browser_sort_by_size
 * 按大小排序。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {bool_t} ascending 按升序排。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_sort_by_size(file_browser_t* fb, bool_t ascending);

/**
 * @method file_browser_sort_by_mtime
 * 按修改时间排序。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {bool_t} ascending 按升序排。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_sort_by_mtime(file_browser_t* fb, bool_t ascending);

/**
 * @method file_browser_get_items_nr
 * 获取当前目录下文件项个数。
 * 
 * @param {file_browser_t*} fb file browser对象。
 *
 * @return {uint32_t} 返回当前目录下文件项个数。
 */
uint32_t file_browser_get_items_nr(file_browser_t* fb);

/**
 * @method file_browser_get_item
 * 获取指定项序数的项目。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {uint32_t} index 序数。
 *
 * @return {fb_item_t*} 返回指定项序数的项目。
 */
fb_item_t* file_browser_get_item(file_browser_t* fb, uint32_t index);

/**
 * @method file_browser_set_ignore_hidden_files
 * 设置是否忽略隐藏文件。
 * 
 * @param {file_browser_t*} fb file browser对象。
 * @param {bool_t} ignore_hidden_files 是否忽略隐藏文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_set_ignore_hidden_files(file_browser_t* fb, bool_t ignore_hidden_files);

/**
 * @method file_browser_destroy
 * 销毁file_browser对象
 * 
 * @param {file_browser_t*} fb file browser对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_browser_destroy(file_browser_t* fb);

/*public for test*/
bool_t fb_filter_files_only(void* ctx, const void* data);
bool_t fb_filter_by_ext_names(void* ctx, const void* data);
bool_t fb_filter_directories_only(void* ctx, const void* data);

int fb_compare_by_name(const void* a, const void* b);
int fb_compare_by_type(const void* a, const void* b);
int fb_compare_by_size(const void* a, const void* b);
int fb_compare_by_mtime(const void* a, const void* b);
int fb_compare_by_name_dec(const void* a, const void* b);
int fb_compare_by_type_dec(const void* a, const void* b);
int fb_compare_by_size_dec(const void* a, const void* b);
int fb_compare_by_mtime_dec(const void* a, const void* b);

END_C_DECLS

#endif /*TK_FILE_BROWSER_H*/
