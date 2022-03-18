#ifndef TK_LCD_FB_DIRTY_RECTS_H
#define TK_LCD_FB_DIRTY_RECTS_H

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/darray.h"
#include "base/lcd.h"
#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @class lcd_fb_dirty_rects_t
 * lcd fb脏矩形。
 *
 */
typedef struct _lcd_fb_dirty_rects_t {
  wh_t lcd_w;
  wh_t lcd_h;
  darray_t fb_dirty_list;
} lcd_fb_dirty_rects_t;

/**
 * @method lcd_fb_dirty_rects_get_dirty_rects_by_fb
 * 根据 fb 来获取脏矩形对象
 * @export none
 * @param {lcd_fb_dirty_rects_t*} lcd_fb_dirty_rects lcd_fb_dirty_rects_t对象。
 * @param {uint8_t*} fb fb 地址。
 * 
 * @return {const dirty_rects_t*} 成功返回脏矩形对象，失败返回 NULL。
 */
const dirty_rects_t* lcd_fb_dirty_rects_get_dirty_rects_by_fb(
    lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, uint8_t* fb);

/**
 * @method lcd_fb_dirty_rects_reset_dirty_rects_by_fb
 * 清除对应 fb 的脏矩形列表。
 * @export none
 * @param {lcd_fb_dirty_rects_t*} lcd_fb_dirty_rects lcd_fb_dirty_rects_t对象。
 * @param {uint8_t*} fb fb 地址。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_fb_dirty_rects_reset_dirty_rects_by_fb(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects,
                                                 uint8_t* fb);

/**
 * @method lcd_fb_dirty_rects_update_all_fb_dirty_rects
 * 把当前脏矩形列表合并到所有 fb 的脏矩形中。
 * @export none
 * @param {lcd_fb_dirty_rects_t*} lcd_fb_dirty_rects lcd_fb_dirty_rects_t对象。
 * @param {const dirty_rects_t*} dirty_rects 当前脏矩形列表。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_fb_dirty_rects_update_all_fb_dirty_rects(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects,
                                                   const dirty_rects_t* dirty_rects);

/**
 * @method lcd_fb_dirty_rects_update_all_fb_dirty_rect
 * 把当前脏矩形合并到所有 fb 的脏矩形中。
 * @export none
 * @param {lcd_fb_dirty_rects_t*} lcd_fb_dirty_rects lcd_fb_dirty_rects_t对象。
 * @param {const rect_t*} dirty_rect 当前脏矩形。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_fb_dirty_rects_update_all_fb_dirty_rect(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects,
                                                  const rect_t* dirty_rect);

/**
 * @method lcd_fb_dirty_rects_add_fb_info
 * 添加新的 fb 信息
 * 如果已经存在的话，就不会重新添加
 * @export none
 * @param {lcd_fb_dirty_rects_t*} lcd_fb_dirty_rects lcd_fb_dirty_rects_t对象。
 * @param {uint8_t*} fb fb 地址。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_fb_dirty_rects_add_fb_info(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, uint8_t* fb);

/**
 * @method lcd_fb_dirty_rects_deinit
 * 清除 lcd_fb_dirty_rects_t 对象数据
 * @export none
 * @param {lcd_fb_dirty_rects_t*} lcd_fb_dirty_rects lcd_fb_dirty_rects_t对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_fb_dirty_rects_deinit(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects);

/**
 * @method lcd_fb_dirty_rects_init
 * 初始化 lcd_fb_dirty_rects_t 对象
 * @export none
 * @param {lcd_fb_dirty_rects_t*} lcd_fb_dirty_rects lcd_fb_dirty_rects_t对象。
 * @param {wh_t} lcd_w lcd 的宽。
 * @param {wh_t} lcd_h lcd 的高。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_fb_dirty_rects_init(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, wh_t lcd_w, wh_t lcd_h);

/**
 * @method lcd_fb_dirty_rects_reinit
 * 修改 lcd_fb_dirty_rects_t 对象的宽高
 * @export none
 * @param {lcd_fb_dirty_rects_t*} lcd_fb_dirty_rects lcd_fb_dirty_rects_t对象。
 * @param {wh_t} lcd_w lcd 的宽。
 * @param {wh_t} lcd_h lcd 的高。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lcd_fb_dirty_rects_reinit(lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, wh_t lcd_w, wh_t lcd_h);

END_C_DECLS

#endif /*TK_LCD_FB_DIRTY_RECTS_H*/
