/**
 * File:   apidoc_test.h
 * Author: AWTK Develop Team
 * Brief:  代码编辑器控件。
 *
 * Copyright (c) 2023 - 2023 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-03-03 Wang JunSheng <wangjunsheng@zlg.cn> created
 *
 */

#ifndef TK_CODE_EDIT_H
#define TK_CODE_EDIT_H

#include "base/widget.h"
#include "code_complete.h"

BEGIN_C_DECLS

/**
 * @class code_edit_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 代码编辑器控件。
 *
 * 在xml中使用"code\_edit"标签创建代码编辑器控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <code_edit name="code_edit" x="10" y="10" w="200" h="200" tab_width="2">
 *   <scroll_bar_d name="scroll_bar" x="r" y="0" w="14" h="100%" value="0"/>
 * </code_edit>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <!-- style -->
 * <code_edit>
 *   <style name="default" border_color="black" font_size="18">
 *     <normal text_color="black" />
 *   </style>
 * </code_edit>
 * ```
 */
typedef struct _code_edit_t {
  widget_t widget;

  /**
   * @property {char*} lang2
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当前的代码的语言。
   */
  char* lang;

  /**
   * @property {char*} code_theme
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当前的代码的主题名称(在资源的xml目录必须有相应的xml文件存在，格式与nodepad++的一致)。
   */
  const char* code_theme;

  /**
   * @property {char*} filename
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 文件名。
   */
  char* filename;

  /**
   * @property {bool_t} show_line_number
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否显示行号，默认显示。
   */
  bool_t show_line_number;

  /**
   * @property {bool_t} readonly
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否只读。
   */
  bool_t readonly;

  /**
   * @property {bool_t} wrap_word
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否自动折行。
   */
  bool_t wrap_word;

  /**
   * @property {uint32_t} tab_width
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * tab的宽度。
   */
  uint32_t tab_width;

  /**
   * @property {int32_t} scroll_line
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 鼠标一次滚动行数。
   */
  int32_t scroll_line;

  /**
   * @property {int32_t} zoom
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 缩放等级。
   */
  int32_t zoom;

  /**
   * @property {int32_t} line_number_margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 行号边距(默认宽度32)。
   */
  int32_t line_number_margin;

  /**
   * @property {int32_t} symbol_margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 非折叠符号边距。
   */
  int32_t symbol_margin;

  /**
   * @property {bool_t} fold
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否启用折叠。
   */
  bool_t fold;

  /**
   * @property {int32_t} fold_symbol_margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 折叠符号边距。
   */
  int32_t fold_symbol_margin;

  /**
   * @property {bool_t} end_at_last_line
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否只能滚动到最后一行。
   */
  bool_t end_at_last_line;

  /**
   * @property {bool_t} is_utf8_bom
   * @annotation ["get_prop","readable","persitent","design","scriptable"]
   * 当前文件是否 utf8_bom。
   */
  bool_t is_utf8_bom;

  /*private*/
  int32_t lexer;
  void* impl;
  str_t text;
  ca_symbols_t* symbols;
} code_edit_t;

/**
 * @method code_edit_createx
 * @annotation ["constructor", "scriptable"]
 * 创建code_edit对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} code_edit对象。
 */
static inline widget_t* code_edit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method code_edit_cast,
 * 转换为code_edit对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget code_edit对象。
 *
 * @return {widget_t*} code_edit对象。
 */
widget_t* code_edit_cast(widget_t* widget);

/**
 * @method code_edit_set_lang
 * 设置 当前的代码的语言。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} lang 当前的代码的语言。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_lang(widget_t* widget, const char* lang);

/**
 * @method code_edit_set_code_theme
 * 设置 当前的代码的主题。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} code_theme 当前的代码的语言。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_code_theme(widget_t* widget, const char* code_theme);

/**
 * @method code_edit_set_filename
 * 设置 文件名。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} filename 文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_filename(widget_t* widget, const char* filename);

/**
 * @method code_edit_set_show_line_number
 * 设置 是否显示行号。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} show_line_number 是否显示行号。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_show_line_number(widget_t* widget, bool_t show_line_number);

/**
 * @method code_edit_set_readonly
 * 设置 是否只读。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} readonly 是否只读。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_readonly(widget_t* widget, bool_t readonly);

/**
 * @method code_edit_set_tab_width
 * 设置 tab的宽度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} tab_width tab的宽度。。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_tab_width(widget_t* widget, uint32_t tab_width);

/**
 * @method code_edit_set_zoom
 * 设置 缩放等级。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} zoom 缩放等级。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_zoom(widget_t* widget, int32_t zoom);

/**
 * @method code_edit_set_line_number_margin
 * 设置 行号边距。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} margin 行号边距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_line_number_margin(widget_t* widget, int32_t margin);

/**
 * @method code_edit_set_symbol_margin
 * 设置 非折叠符号边距。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} margin 边距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_symbol_margin(widget_t* widget, int32_t margin);

/**
 * @method code_edit_set_fold_symbol_margin
 * 设置 折叠符号边距。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} margin 边距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_fold_symbol_margin(widget_t* widget, int32_t margin);

/**
 * @method code_edit_set_fold
 * 启用/禁用折叠。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} size 字体大小。
 *
 * @return {ret_t} 返回RET_OK表示成功。
 */
ret_t code_edit_set_fold(widget_t* widget, bool_t fold);

/**
 * @method code_edit_set_end_at_last_line
 * 设置 是否只能滚动到最后一行。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} end_at_last_line 是否只能滚动到最后一行。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_end_at_last_line(widget_t* widget, bool_t end_at_last_line);

/**
 * @method code_edit_set_wrap_word
 * 设置 是否自动折行。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} wrap_word 是否自动折行。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_wrap_word(widget_t* widget, bool_t wrap_word);

/**
 * @method code_edit_set_scroll_line
 * 设置 滚动速度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} scroll_line 滚动行数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_scroll_line(widget_t* widget, int32_t scroll_line);

/**
 * @method code_edit_insert_text
 * 插入一段文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} offset 插入的偏移位置。
 * @param {const char*} text 待插入的文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_insert_text(widget_t* widget, int32_t offset, const char* text);

/**
 * @method code_edit_replace_sel
 * 替换选中文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} text 待替换的文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_replace_sel(widget_t* widget, const char* text);

/**
 * @method code_edit_set_sel
 * 设置选中位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} anchor 锚点位置。
 * @param {int64_t} caret 光标位置。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_sel(widget_t* widget, int64_t anchor, int64_t caret);

/**
 * @method code_edit_delete_range
 * 删除选中范围的文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} start 起始位置。
 * @param {int64_t} length 文本长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_delete_range(widget_t* widget, int64_t start, int64_t length);
/**
 * @method code_edit_redo
 * 重做。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_redo(widget_t* widget);

/**
 * @method code_edit_undo
 * 撤销。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_undo(widget_t* widget);

/**
 * @method code_edit_empty_undo_buffer
 * 清空undo缓存。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_empty_undo_buffer(widget_t* widget);

/**
 * @method code_edit_set_save_point
 * 设置save point。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_save_point(widget_t* widget);

/**
 * @method code_edit_copy
 * 拷贝选中的文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_copy(widget_t* widget);

/**
 * @method code_edit_cut
 * 剪切。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_cut(widget_t* widget);

/**
 * @method code_edit_paste
 * 粘贴。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_paste(widget_t* widget);

/**
 * @method code_edit_clear
 * 清除内容。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_clear(widget_t* widget);

/**
 * @method code_edit_clear_selection
 * 清除选中内容。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_clear_selection(widget_t* widget);

/**
 * @method code_edit_clear_all
 * 清除所有内容。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_clear_all(widget_t* widget);

/**
 * @method code_edit_select_none
 * 取消选择。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_select_none(widget_t* widget);

/**
 * @method code_edit_select_all
 * 全选。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_select_all(widget_t* widget);

/**
 * @method code_edit_can_redo
 * 检查是否可以重做。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {bool_t} 返回TRUE表示能，否则表示不能。
 */
bool_t code_edit_can_redo(widget_t* widget);

/**
 * @method code_edit_can_undo
 * 检查是否可以撤销。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {bool_t} 返回TRUE表示能，否则表示不能。
 */
bool_t code_edit_can_undo(widget_t* widget);

/**
 * @method code_edit_can_copy
 * 检查是否可以拷贝。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {bool_t} 返回TRUE表示能，否则表示不能。
 */
bool_t code_edit_can_copy(widget_t* widget);

/**
 * @method code_edit_can_cut
 * 检查是否可以剪切。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {bool_t} 返回TRUE表示能，否则表示不能。
 */
bool_t code_edit_can_cut(widget_t* widget);

/**
 * @method code_edit_can_paste
 * 检查是否可以粘贴。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {bool_t} 返回TRUE表示能，否则表示不能。
 */
bool_t code_edit_can_paste(widget_t* widget);

/**
 * @method code_edit_save
 * save
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} filename 文件名。
 * @param {bool_t} with_utf8_bom bom
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_save(widget_t* widget, const char* filename, bool_t with_utf8_bom);

/**
 * @method code_edit_load
 * load
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} filename 文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_load(widget_t* widget, const char* filename);

/**
 * @method code_edit_is_modified
 * 检查文档是否变化。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示否。
 */
bool_t code_edit_is_modified(widget_t* widget);

/**
 * @method code_edit_scroll_range
 * 检查文档是否变化。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} secondary 次位置。
 * @param {int64_t} primary 主位置
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_scroll_range(widget_t* widget, int64_t secondary, int64_t primary);

/**
 * @method code_edit_set_x_caret_policy
 * 设置x轴浮标策略。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} policy 策略
 * @param {int32_t} slop
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_x_caret_policy(widget_t* widget, int32_t policy, int32_t slop);

/**
 * @method code_edit_get_current_pos
 * 获取当前位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {int64_t} 返回当前位置。
 */
int64_t code_edit_get_current_pos(widget_t* widget);

/**
 * @method code_edit_word_start
 * 获取当前位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} pos 位置
 *
 * @return {int64_t} 词的起始位置。
 */
int64_t code_edit_word_start(widget_t* widget, int64_t pos);

/**
 * @method code_edit_word_end
 * 获取当前位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {int64_t} 词的结束位置。
 */
int64_t code_edit_word_end(widget_t* widget, int64_t pos);

/**
 * @method code_edit_get_text_range
 * 获取指定区域的文本。
 * @annotation ["scriptable1"]
 * @param {widget_t} widget widget对象。
 * @param {int64_t} start 起始位置
 * @param {int64_t} end 结束位置
 * @param {char*} buf 存放文本
 * @param {int32_t} buf_capacity buf的容量
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_get_text_range(widget_t* widget, int64_t start, int64_t end, char* buf,
                               int32_t buf_capacity);

/**
 * @method code_edit_get_text_bom
 * 返回所有文本。 如果存在 utf8_bom, 返回的文本附带 bom 信息
 * 使用后请调用 code_edit_free 释放
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t*} len 返回文本的长度。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
char* code_edit_get_text_bom(widget_t* widget, int64_t* len);

/**
 * @method code_edit_free
 * 释放 s
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {void*} s 待释放的内容
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_free(void* s);

/**
 * @method code_edit_get_word
 * 获取pos所在的单词
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} pos 位置偏移
 * @param {char*} buf 存放文本
 * @param {int32_t} buf_capacity buf的容量
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_get_word(widget_t* widget, int64_t pos, char* buf, int32_t buf_capacity);

/**
 * @method code_edit_get_line
 * 获取pos所在行的内容
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} pos 位置偏移
 * @param {char*} buf 存放文本
 * @param {int32_t} buf_capacity buf的容量
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_get_line(widget_t* widget, int64_t pos, char* buf, int32_t buf_capacity);

/**
 * @method code_edit_get_text_length
 * 获取文本的长度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {int64_t} 返回文本的长度。
 */
int64_t code_edit_get_text_length(widget_t* widget);

/**
 * @method code_edit_set_current_pos
 * 设置当前位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} pos 位置
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_current_pos(widget_t* widget, int64_t pos);

/**
 * @method code_edit_get_anchor
 * 获取当前位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {int64_t} 返回当前位置。
 */
int64_t code_edit_get_anchor(widget_t* widget);

/**
 * @method code_edit_set_anchor
 * 设置当前位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} pos 位置
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_anchor(widget_t* widget, int64_t pos);

/**
 * @method code_edit_scroll_caret
 * 将视图滚动到浮标所在的位置（根据策略不同位置有所不同）。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_scroll_caret(widget_t* widget);

/**
 * @method code_edit_get_first_visible_line
 * 获取当前第一个可见的行号。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {int64_t} 返回当前第一个可见的行号。
 */
int64_t code_edit_get_first_visible_line(widget_t* widget);

/**
 * @method code_edit_set_first_visible_line
 * 设置当前第一个可见的行号。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} line 行号
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_set_first_visible_line(widget_t* widget, int64_t line);

/**
 * @method code_edit_is_line_visible
 * 指定行是否可见。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} line 行号
 *
 * @return {bool_t} 返回line行是否可见。
 */
bool_t code_edit_is_line_visible(widget_t* widget, int64_t line);

/**
 * @method code_edit_goto_pos
 * 跳转到指定位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} pos 位置
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t code_edit_goto_pos(widget_t* widget, int64_t pos);

/**
 * @method code_edit_get_line_from_position
 * 获取指定位置所在的行。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} pos 位置
 *
 * @return {int64_t} 返回获取指定位置所在的行。
 */
int64_t code_edit_get_line_from_position(widget_t* widget, int64_t pos);

/**
 * @method code_edit_get_line_start
 * 获取指定行的起始偏移。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * 不可以的
 * @param {int64_t} line 行号
 *
 * @return {int64_t} 行的起始位置。
 */
int64_t code_edit_get_line_start(widget_t* widget, int64_t line);

/**
 * @method code_edit_get_line_end
 * 获取指定行的结束偏移。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} line 行号
 *
 * @return {int64_t} 行的结束位置。
 */
int64_t code_edit_get_line_end(widget_t* widget, int64_t line);

/**
 * @method code_edit_get_lines_on_screen
 * 获取当前屏幕中可见的行数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {int64_t} 返回当前屏幕中可见的行数。
 */
int64_t code_edit_get_lines_on_screen(widget_t* widget);

/**
 * @method code_edit_set_all_font_size
 * 设置代码编辑器的默认字体大小。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} size 字体大小。
 *
 * @return {ret_t} 返回RET_OK表示成功。
 */
ret_t code_edit_set_all_font_size(widget_t* widget, uint32_t size);

/**
 * @method code_edit_set_reloaded_text
 * 设置代码编辑器文本作为reload的字符串。与setText的区别是不会回滚到第一行。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功。
 */
ret_t code_edit_set_reloaded_text(widget_t* widget, const char* text);

/**
 * @method code_edit_vertical_centre_caret
 * 光标位置垂直居中。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功。
 */
ret_t code_edit_vertical_centre_caret(widget_t* widget);

/**
 * @method code_edit_show_autoc
 * 显示补全。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {ca_symbols_t*} symbols 补全信息
 *
 * @return {ret_t} 返回RET_OK表示成功。
 */
ret_t code_edit_show_autoc(widget_t* widget, ca_symbols_t* symbols);

/**
 * @method code_edit_cancel_autoc
 * 取消补全。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功。
 */
ret_t code_edit_cancel_autoc(widget_t* widget);

/**
 * @method code_edit_set_autoc_visible_lines
 * 设置推荐列表最大可见行数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} lines 推荐列表可见的行数。
 *
 * @return {ret_t} 返回RET_OK表示成功。
 */
ret_t code_edit_set_autoc_visible_lines(widget_t* widget, int32_t lines);

/**
 * @method code_edit_is_comment
 * 指定位置是否是注释
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int64_t} pos 文件的偏移。
 *
 * @return {bool_t} 是注释返回TRUE，否则返回 FALSE
 */
bool_t code_edit_is_comment(widget_t* widget, int64_t pos);

/**
 * @enum code_edit_prop_t
 * @annotation ["scriptable", "string1"]
 * @prefix CODE_EDIT_PROP_
 * 控件的属性。
 */

/**
 * @const CODE_EDIT_PROP_LANG1
 * 语言。
 */
#define CODE_EDIT_PROP_LANG "lang"

/**
 * @const CODE_EDIT_PROP_FILENAME
 * 文件名称。
 */
#define CODE_EDIT_PROP_FILENAME "filename"

/**
 * @const CODE_EDIT_PROP_TAB_WIDTH
 * tab宽度。
 */
#define CODE_EDIT_PROP_TAB_WIDTH "tab_width"

/**
 * @const CODE_EDIT_PROP_CODE_THEME
 * 代码样式主题。
 */
#define CODE_EDIT_PROP_CODE_THEME "code_theme"

/**
 * @const CODE_EDIT_PROP_SHOW_LINE_NUMBER
 * 显示行号。
 */
#define CODE_EDIT_PROP_SHOW_LINE_NUMBER "show_line_number"

/**
 * @const CODE_EDIT_PROP_ZOOM
 * ZOOM。
 */
#define CODE_EDIT_PROP_ZOOM "zoom"

/**
 * @const CODE_EDIT_PROP_LINE_NUMBER_MARGIN
 * LINE_NUMBER_MARGIN。
 */
#define CODE_EDIT_PROP_LINE_NUMBER_MARGIN "line_number_margin"

/**
 * @const CODE_EDIT_PROP_SYMBOL_MARGIN
 * SYMBOL_MARGIN。
 */
#define CODE_EDIT_PROP_SYMBOL_MARGIN "symbol_margin"

/**
 * @const CODE_EDIT_PROP_FOLD
 * FOLD。
 */
#define CODE_EDIT_PROP_FOLD "fold"

/**
 * @const CODE_EDIT_PROP_FOLD_SYMBOL_MARGIN
 * FOLD_SYMBOL_MARGIN。
 */
#define CODE_EDIT_PROP_FOLD_SYMBOL_MARGIN "fold_symbol_margin"

/**
 * @const CODE_EDIT_PROP_WRAP_WORD
 * 是否折行。
 */
#define CODE_EDIT_PROP_WRAP_WORD "wrap_word"

/**
 * @const CODE_EDIT_PROP_SCROLL_LINE
 * 鼠标一次滚动行数。
 */
#define CODE_EDIT_PROP_SCROLL_LINE "scroll_line"

/**
 * @const CODE_EDIT_PROP_CURRENT_POSITION
 * 当前位置。
 */
#define CODE_EDIT_PROP_CURRENT_POSITION "current_position"

/**
 * @const CODE_EDIT_PROP_ANCHOR
 * 锚点位置。
 */
#define CODE_EDIT_PROP_ANCHOR "anchor"

/**
 * @const CODE_EDIT_PROP_CURSOR
 * 光标位置（同步锚点位置）。
 */
#define CODE_EDIT_PROP_CURSOR "cursor"

/**
 * @const CODE_EDIT_PROP_SELECTION_START
 * 选择的文本的起始位置。
 */
#define CODE_EDIT_PROP_SELECTION_START "selection_start"

/**
 * @const CODE_EDIT_PROP_SELECTION_END
 * 选择文本的结束位置。
 */
#define CODE_EDIT_PROP_SELECTION_END "selection_end"

/**
 * @enum widget_type_t
 * @annotation ["scriptable", "string"]
 * @prefix WIDGET_TYPE_
 * 控件的类型。
 */

/**
 * @const WIDGET_TYPE_CODE_EDIT
 * 代码编辑器。
 */
#define WIDGET_TYPE_CODE_EDIT "code_edit"

#define CODE_EDIT(widget) ((code_edit_t*)(code_edit_cast(WIDGET(widget))))

/**public for ScintillaAWTK*/
#define PROP_CODE_EDITOR_FONT_SIZE \
  "CODE_EDITOR_FONT_SIZE"  // designer设置了代码编辑器的字体大小后，要设置值为字号的此属性到wm。
#define EVT_GLOBAL_CODE_EDITOR_FONT_SIZE_CHANGED \
  EVT_USER_START + 1  // designer设置了代码编辑器的字体大小后，要从wm派发这个事件。
#define _SC_MARGE_LINENUMBER 0  // 第0条marge为行号
#define _SC_MARGE_SYBOLE 1      // 第1条marge为断点等
#define _SC_MARGE_FOLDER 2      // 第2条marge为折叠符号
#define _CODE_EDIT_DEFAULT_LINE_NUMBER_WITH 32

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(code_edit);

/*public for vtable*/
ret_t code_edit_on_destroy(widget_t* widget);
ret_t code_edit_on_event(widget_t* widget, event_t* e);
ret_t code_edit_on_paint_self(widget_t* widget, canvas_t* c);
ret_t code_edit_on_add_child(widget_t* widget, widget_t* child);
ret_t code_edit_get_prop(widget_t* widget, const char* name, value_t* v);
ret_t code_edit_set_prop(widget_t* widget, const char* name, const value_t* v);

/**
 * @enum ret_t
 * @annotation ["scriptable"]
 * 函数返回值常量定义。
 */
typedef enum _ret_t {
  /**
   * @const RET_OK
   * 成功。
   */
  RET_OK = 0,
  /**
   * @const RET_OOM
   * Out of memory。
   */
  RET_OOM,
} ret_t;

/**
 * @enum kind_t
 * @annotation ["scriptable"]
 * @prefix 
 * 函数返回值常量定义。
 */
typedef enum _kind_t {
  /**
   * @const KIND_OK
   * 成功。
   */
  KIND_OK = 0,
  /**
   * @const KIND_OOM
   * Out of memory。
   */
  KIND_OOM,
} kind_t;

END_C_DECLS

#endif /*TK_CODE_EDIT_H*/
