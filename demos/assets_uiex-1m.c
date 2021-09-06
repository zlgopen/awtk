#include "awtk.h"
#include "base/assets_manager.h"

#ifndef APP_THEME
#define APP_THEME "default"
#endif /*APP_THEME*/

/*** default ***/
#ifndef WITH_FS_RES
#include "../res/assets/default/inc/strings/en_US.data"
#include "../res/assets/default/inc/strings/zh_CN.data"
#include "../res/assets/default/inc/styles/dialog_toast.data"
#include "../res/assets/default/inc/styles/uiex.data"
#include "../res/assets/default/inc/styles/uiex_dialog_confirm.data"
#include "../res/assets/default/inc/styles/uiex_dialog_info.data"
#include "../res/assets/default/inc/styles/uiex_dialog_warn.data"
#include "../res/assets/default/inc/styles/uiex_keyboard.data"
#include "../res/assets/default/inc/ui/uiex/cbp_fast_food.data"
#include "../res/assets/default/inc/ui/uiex/cbp_font.data"
#include "../res/assets/default/inc/ui/uiex/cbp_font_size.data"
#include "../res/assets/default/inc/ui/uiex/cbp_week.data"
#include "../res/assets/default/inc/ui/uiex/dlg_center_scale.data"
#include "../res/assets/default/inc/ui/uiex/dlg_fade.data"
#include "../res/assets/default/inc/ui/uiex/dlg_popdown.data"
#include "../res/assets/default/inc/ui/uiex/dlg_popup.data"
#include "../res/assets/default/inc/ui/uiex/htranslate.data"
#include "../res/assets/default/inc/ui/uiex/kb_ascii.data"
#include "../res/assets/default/inc/ui/uiex/kb_default.data"
#include "../res/assets/default/inc/ui/uiex/kb_foo.data"
#include "../res/assets/default/inc/ui/uiex/kb_hex.data"
#include "../res/assets/default/inc/ui/uiex/kb_int.data"
#include "../res/assets/default/inc/ui/uiex/kb_uint.data"
#include "../res/assets/default/inc/ui/uiex/menu_bar.data"
#include "../res/assets/default/inc/ui/uiex/menu_landscape.data"
#include "../res/assets/default/inc/ui/uiex/menu_portrait.data"
#include "../res/assets/default/inc/ui/uiex/overlay.data"
#include "../res/assets/default/inc/ui/uiex/page_animate.data"
#include "../res/assets/default/inc/ui/uiex/page_button.data"
#include "../res/assets/default/inc/ui/uiex/page_chart.data"
#include "../res/assets/default/inc/ui/uiex/page_checkbox.data"
#include "../res/assets/default/inc/ui/uiex/page_color.data"
#include "../res/assets/default/inc/ui/uiex/page_date_time.data"
#include "../res/assets/default/inc/ui/uiex/page_edit.data"
#include "../res/assets/default/inc/ui/uiex/page_image.data"
#include "../res/assets/default/inc/ui/uiex/page_label.data"
#include "../res/assets/default/inc/ui/uiex/page_layout.data"
#include "../res/assets/default/inc/ui/uiex/page_list_view.data"
#include "../res/assets/default/inc/ui/uiex/page_mledit.data"
#include "../res/assets/default/inc/ui/uiex/page_progress.data"
#include "../res/assets/default/inc/ui/uiex/page_selector.data"
#include "../res/assets/default/inc/ui/uiex/page_slider.data"
#include "../res/assets/default/inc/ui/uiex/page_slide_view.data"
#include "../res/assets/default/inc/ui/uiex/page_tab_ctrl.data"
#include "../res/assets/default/inc/ui/uiex/page_window.data"
#include "../res/assets/default/inc/ui/uiex/popup.data"
#include "../res/assets/default/inc/ui/uiex/preload.data"
#include "../res/assets/default/inc/ui/uiex/slide_down.data"
#include "../res/assets/default/inc/ui/uiex/slide_left.data"
#include "../res/assets/default/inc/ui/uiex/slide_right.data"
#include "../res/assets/default/inc/ui/uiex/slide_up.data"
#include "../res/assets/default/inc/ui/uiex/sysbar_landscape.data"
#include "../res/assets/default/inc/ui/uiex/sysbar_portrait.data"
#include "../res/assets/default/inc/ui/uiex/vtranslate.data"
#ifdef WITH_STB_IMAGE
#include "../res/assets/default/inc/images/uiex_arrow_down_n.res"
#include "../res/assets/default/inc/images/uiex_arrow_down_p.res"
#include "../res/assets/default/inc/images/uiex_arrow_up_n.res"
#include "../res/assets/default/inc/images/uiex_arrow_up_p.res"
#include "../res/assets/default/inc/images/uiex_big_logo.res"
#include "../res/assets/default/inc/images/uiex_Boot_loading.res"
#include "../res/assets/default/inc/images/uiex_button_Empty.res"
#include "../res/assets/default/inc/images/uiex_button_Export.res"
#include "../res/assets/default/inc/images/uiex_button_Import.res"
#include "../res/assets/default/inc/images/uiex_button_search.res"
#include "../res/assets/default/inc/images/uiex_button_time.res"
#include "../res/assets/default/inc/images/uiex_check.res"
#include "../res/assets/default/inc/images/uiex_check_checked_d.res"
#include "../res/assets/default/inc/images/uiex_check_checked_n.res"
#include "../res/assets/default/inc/images/uiex_check_checked_p.res"
#include "../res/assets/default/inc/images/uiex_check_d.res"
#include "../res/assets/default/inc/images/uiex_check_n.res"
#include "../res/assets/default/inc/images/uiex_check_p.res"
#include "../res/assets/default/inc/images/uiex_close_n.res"
#include "../res/assets/default/inc/images/uiex_close_p.res"
#include "../res/assets/default/inc/images/uiex_combobox_btn_d.res"
#include "../res/assets/default/inc/images/uiex_combobox_btn_n.res"
#include "../res/assets/default/inc/images/uiex_combobox_btn_p.res"
#include "../res/assets/default/inc/images/uiex_Configuration_nor.res"
#include "../res/assets/default/inc/images/uiex_Configuration_sel.res"
#include "../res/assets/default/inc/images/uiex_Coverwindow.res"
#include "../res/assets/default/inc/images/uiex_editor_bgcolor.res"
#include "../res/assets/default/inc/images/uiex_editor_Importpictures.res"
#include "../res/assets/default/inc/images/uiex_editor_TextAlignBoth.res"
#include "../res/assets/default/inc/images/uiex_editor_TextAlignleft.res"
#include "../res/assets/default/inc/images/uiex_editor_TextAlignright.res"
#include "../res/assets/default/inc/images/uiex_editor_TextBold.res"
#include "../res/assets/default/inc/images/uiex_editor_TextCenteralignment.res"
#include "../res/assets/default/inc/images/uiex_editor_Textcolor.res"
#include "../res/assets/default/inc/images/uiex_editor_TextItalic.res"
#include "../res/assets/default/inc/images/uiex_editor_TextLinespacing.res"
#include "../res/assets/default/inc/images/uiex_editor_TextUnderscore.res"
#include "../res/assets/default/inc/images/uiex_keyboard_backspace_n.res"
#include "../res/assets/default/inc/images/uiex_keyboard_backspace_p.res"
#include "../res/assets/default/inc/images/uiex_keyboard_CN.res"
#include "../res/assets/default/inc/images/uiex_keyboard_EN.res"
#include "../res/assets/default/inc/images/uiex_loading_repeat_bg.res"
#include "../res/assets/default/inc/images/uiex_logo.res"
#include "../res/assets/default/inc/images/uiex_Portraitmenu_n.res"
#include "../res/assets/default/inc/images/uiex_Portraitmenu_p.res"
#include "../res/assets/default/inc/images/uiex_radio_checked_d.res"
#include "../res/assets/default/inc/images/uiex_radio_checked_n.res"
#include "../res/assets/default/inc/images/uiex_radio_checked_p.res"
#include "../res/assets/default/inc/images/uiex_radio_d.res"
#include "../res/assets/default/inc/images/uiex_radio_n.res"
#include "../res/assets/default/inc/images/uiex_radio_p.res"
#include "../res/assets/default/inc/images/uiex_send_nor.res"
#include "../res/assets/default/inc/images/uiex_send_sel.res"
#include "../res/assets/default/inc/images/uiex_shift.res"
#include "../res/assets/default/inc/images/uiex_shifton.res"
#include "../res/assets/default/inc/images/uiex_Sliding_dot.res"
#include "../res/assets/default/inc/images/uiex_Sliding_Square.res"
#include "../res/assets/default/inc/images/uiex_switch.res"
#include "../res/assets/default/inc/images/uiex_takeover_nor.res"
#include "../res/assets/default/inc/images/uiex_takeover_sel.res"
#include "../res/assets/default/inc/images/uiex_texteditorhide.res"
#include "../res/assets/default/inc/images/uiex_texteditor_show.res"
#include "../res/assets/default/inc/images/uiex_Universal_nor.res"
#include "../res/assets/default/inc/images/uiex_Universal_sel.res"
#include "../res/assets/default/inc/images/uiex_Windowclosed_n.res"
#include "../res/assets/default/inc/images/uiex_Windowclosed_p.res"
#else
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
#include "../res/assets/default/inc/fonts/default.res"
#else  /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*WITH_FS_RES*/

ret_t assets_init_default(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "default");

#ifdef WITH_FS_RES
  assets_manager_preload(am, ASSET_TYPE_FONT, "default");
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else
  assets_manager_add(am, strings_en_US);
  assets_manager_add(am, strings_zh_CN);
  assets_manager_add(am, style_dialog_toast);
  assets_manager_add(am, style_uiex);
  assets_manager_add(am, style_uiex_dialog_confirm);
  assets_manager_add(am, style_uiex_dialog_info);
  assets_manager_add(am, style_uiex_dialog_warn);
  assets_manager_add(am, style_uiex_keyboard);
  assets_manager_add(am, ui_uiex_cbp_fast_food);
  assets_manager_add(am, ui_uiex_cbp_font);
  assets_manager_add(am, ui_uiex_cbp_font_size);
  assets_manager_add(am, ui_uiex_cbp_week);
  assets_manager_add(am, ui_uiex_dlg_center_scale);
  assets_manager_add(am, ui_uiex_dlg_fade);
  assets_manager_add(am, ui_uiex_dlg_popdown);
  assets_manager_add(am, ui_uiex_dlg_popup);
  assets_manager_add(am, ui_uiex_htranslate);
  assets_manager_add(am, ui_uiex_kb_ascii);
  assets_manager_add(am, ui_uiex_kb_default);
  assets_manager_add(am, ui_uiex_kb_foo);
  assets_manager_add(am, ui_uiex_kb_hex);
  assets_manager_add(am, ui_uiex_kb_int);
  assets_manager_add(am, ui_uiex_kb_uint);
  assets_manager_add(am, ui_uiex_menu_bar);
  assets_manager_add(am, ui_uiex_menu_landscape);
  assets_manager_add(am, ui_uiex_menu_portrait);
  assets_manager_add(am, ui_uiex_overlay);
  assets_manager_add(am, ui_uiex_page_animate);
  assets_manager_add(am, ui_uiex_page_button);
  assets_manager_add(am, ui_uiex_page_chart);
  assets_manager_add(am, ui_uiex_page_checkbox);
  assets_manager_add(am, ui_uiex_page_color);
  assets_manager_add(am, ui_uiex_page_date_time);
  assets_manager_add(am, ui_uiex_page_edit);
  assets_manager_add(am, ui_uiex_page_image);
  assets_manager_add(am, ui_uiex_page_label);
  assets_manager_add(am, ui_uiex_page_layout);
  assets_manager_add(am, ui_uiex_page_list_view);
  assets_manager_add(am, ui_uiex_page_mledit);
  assets_manager_add(am, ui_uiex_page_progress);
  assets_manager_add(am, ui_uiex_page_selector);
  assets_manager_add(am, ui_uiex_page_slider);
  assets_manager_add(am, ui_uiex_page_slide_view);
  assets_manager_add(am, ui_uiex_page_tab_ctrl);
  assets_manager_add(am, ui_uiex_page_window);
  assets_manager_add(am, ui_uiex_popup);
  assets_manager_add(am, ui_uiex_preload);
  assets_manager_add(am, ui_uiex_slide_down);
  assets_manager_add(am, ui_uiex_slide_left);
  assets_manager_add(am, ui_uiex_slide_right);
  assets_manager_add(am, ui_uiex_slide_up);
  assets_manager_add(am, ui_uiex_sysbar_landscape);
  assets_manager_add(am, ui_uiex_sysbar_portrait);
  assets_manager_add(am, ui_uiex_vtranslate);
  assets_manager_add(am, image_uiex_arrow_down_n);
  assets_manager_add(am, image_uiex_arrow_down_p);
  assets_manager_add(am, image_uiex_arrow_up_n);
  assets_manager_add(am, image_uiex_arrow_up_p);
  assets_manager_add(am, image_uiex_big_logo);
  assets_manager_add(am, image_uiex_Boot_loading);
  assets_manager_add(am, image_uiex_button_Empty);
  assets_manager_add(am, image_uiex_button_Export);
  assets_manager_add(am, image_uiex_button_Import);
  assets_manager_add(am, image_uiex_button_search);
  assets_manager_add(am, image_uiex_button_time);
  assets_manager_add(am, image_uiex_check);
  assets_manager_add(am, image_uiex_check_checked_d);
  assets_manager_add(am, image_uiex_check_checked_n);
  assets_manager_add(am, image_uiex_check_checked_p);
  assets_manager_add(am, image_uiex_check_d);
  assets_manager_add(am, image_uiex_check_n);
  assets_manager_add(am, image_uiex_check_p);
  assets_manager_add(am, image_uiex_close_n);
  assets_manager_add(am, image_uiex_close_p);
  assets_manager_add(am, image_uiex_combobox_btn_d);
  assets_manager_add(am, image_uiex_combobox_btn_n);
  assets_manager_add(am, image_uiex_combobox_btn_p);
  assets_manager_add(am, image_uiex_Configuration_nor);
  assets_manager_add(am, image_uiex_Configuration_sel);
  assets_manager_add(am, image_uiex_Coverwindow);
  assets_manager_add(am, image_uiex_editor_bgcolor);
  assets_manager_add(am, image_uiex_editor_Importpictures);
  assets_manager_add(am, image_uiex_editor_TextAlignBoth);
  assets_manager_add(am, image_uiex_editor_TextAlignleft);
  assets_manager_add(am, image_uiex_editor_TextAlignright);
  assets_manager_add(am, image_uiex_editor_TextBold);
  assets_manager_add(am, image_uiex_editor_TextCenteralignment);
  assets_manager_add(am, image_uiex_editor_Textcolor);
  assets_manager_add(am, image_uiex_editor_TextItalic);
  assets_manager_add(am, image_uiex_editor_TextLinespacing);
  assets_manager_add(am, image_uiex_editor_TextUnderscore);
  assets_manager_add(am, image_uiex_keyboard_backspace_n);
  assets_manager_add(am, image_uiex_keyboard_backspace_p);
  assets_manager_add(am, image_uiex_keyboard_CN);
  assets_manager_add(am, image_uiex_keyboard_EN);
  assets_manager_add(am, image_uiex_loading_repeat_bg);
  assets_manager_add(am, image_uiex_logo);
  assets_manager_add(am, image_uiex_Portraitmenu_n);
  assets_manager_add(am, image_uiex_Portraitmenu_p);
  assets_manager_add(am, image_uiex_radio_checked_d);
  assets_manager_add(am, image_uiex_radio_checked_n);
  assets_manager_add(am, image_uiex_radio_checked_p);
  assets_manager_add(am, image_uiex_radio_d);
  assets_manager_add(am, image_uiex_radio_n);
  assets_manager_add(am, image_uiex_radio_p);
  assets_manager_add(am, image_uiex_send_nor);
  assets_manager_add(am, image_uiex_send_sel);
  assets_manager_add(am, image_uiex_shift);
  assets_manager_add(am, image_uiex_shifton);
  assets_manager_add(am, image_uiex_Sliding_dot);
  assets_manager_add(am, image_uiex_Sliding_Square);
  assets_manager_add(am, image_uiex_switch);
  assets_manager_add(am, image_uiex_takeover_nor);
  assets_manager_add(am, image_uiex_takeover_sel);
  assets_manager_add(am, image_uiex_texteditorhide);
  assets_manager_add(am, image_uiex_texteditor_show);
  assets_manager_add(am, image_uiex_Universal_nor);
  assets_manager_add(am, image_uiex_Universal_sel);
  assets_manager_add(am, image_uiex_Windowclosed_n);
  assets_manager_add(am, image_uiex_Windowclosed_p);
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_default);
#else  /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif

  tk_init_assets();
  return RET_OK;
}
/*** default end ***/

/*** dark ***/
#ifndef WITH_FS_RES
extern TK_CONST_DATA_ALIGN(const unsigned char strings_en_US[]);
extern TK_CONST_DATA_ALIGN(const unsigned char strings_zh_CN[]);
#include "../res/assets/dark/inc/styles/uiex.data"
#include "../res/assets/dark/inc/styles/uiex_dialog_confirm.data"
#include "../res/assets/dark/inc/styles/uiex_dialog_info.data"
#include "../res/assets/dark/inc/styles/uiex_dialog_warn.data"
#include "../res/assets/dark/inc/styles/uiex_keyboard.data"
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_toast[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_cbp_fast_food[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_cbp_font[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_cbp_font_size[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_cbp_week[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_dlg_center_scale[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_dlg_fade[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_dlg_popdown[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_dlg_popup[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_htranslate[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_ascii[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_foo[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_hex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_int[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_uint[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_menu_bar[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_menu_landscape[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_menu_portrait[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_overlay[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_animate[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_button[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_chart[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_checkbox[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_color[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_date_time[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_edit[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_image[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_label[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_layout[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_list_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_mledit[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_progress[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_selector[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_slider[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_slide_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_tab_ctrl[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_window[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_popup[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_preload[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_slide_down[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_slide_left[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_slide_right[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_slide_up[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_sysbar_landscape[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_sysbar_portrait[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_vtranslate[]);
#ifdef WITH_STB_IMAGE
#include "../res/assets/dark/inc/images/uiex_arrow_down_n.res"
#include "../res/assets/dark/inc/images/uiex_arrow_down_p.res"
#include "../res/assets/dark/inc/images/uiex_arrow_up_n.res"
#include "../res/assets/dark/inc/images/uiex_arrow_up_p.res"
#include "../res/assets/dark/inc/images/uiex_big_logo.res"
#include "../res/assets/dark/inc/images/uiex_button_Empty.res"
#include "../res/assets/dark/inc/images/uiex_button_Export.res"
#include "../res/assets/dark/inc/images/uiex_button_Import.res"
#include "../res/assets/dark/inc/images/uiex_button_search.res"
#include "../res/assets/dark/inc/images/uiex_button_time.res"
#include "../res/assets/dark/inc/images/uiex_check_checked_d.res"
#include "../res/assets/dark/inc/images/uiex_check_checked_n.res"
#include "../res/assets/dark/inc/images/uiex_check_checked_p.res"
#include "../res/assets/dark/inc/images/uiex_check_d.res"
#include "../res/assets/dark/inc/images/uiex_check_n.res"
#include "../res/assets/dark/inc/images/uiex_check_p.res"
#include "../res/assets/dark/inc/images/uiex_close_n.res"
#include "../res/assets/dark/inc/images/uiex_close_p.res"
#include "../res/assets/dark/inc/images/uiex_combobox_btn_d.res"
#include "../res/assets/dark/inc/images/uiex_combobox_btn_n.res"
#include "../res/assets/dark/inc/images/uiex_combobox_btn_p.res"
#include "../res/assets/dark/inc/images/uiex_editor_bgcolor.res"
#include "../res/assets/dark/inc/images/uiex_editor_Importpictures.res"
#include "../res/assets/dark/inc/images/uiex_editor_TextAlignBoth.res"
#include "../res/assets/dark/inc/images/uiex_editor_TextAlignleft.res"
#include "../res/assets/dark/inc/images/uiex_editor_TextAlignright.res"
#include "../res/assets/dark/inc/images/uiex_editor_TextBold.res"
#include "../res/assets/dark/inc/images/uiex_editor_TextCenteralignment.res"
#include "../res/assets/dark/inc/images/uiex_editor_Textcolor.res"
#include "../res/assets/dark/inc/images/uiex_editor_TextItalic.res"
#include "../res/assets/dark/inc/images/uiex_editor_TextLinespacing.res"
#include "../res/assets/dark/inc/images/uiex_editor_TextUnderscore.res"
#include "../res/assets/dark/inc/images/uiex_keyboard_backspace_n.res"
#include "../res/assets/dark/inc/images/uiex_keyboard_backspace_p.res"
#include "../res/assets/dark/inc/images/uiex_keyboard_CN.res"
#include "../res/assets/dark/inc/images/uiex_keyboard_EN.res"
#include "../res/assets/dark/inc/images/uiex_logo.res"
#include "../res/assets/dark/inc/images/uiex_Portraitmenu_n.res"
#include "../res/assets/dark/inc/images/uiex_Portraitmenu_p.res"
#include "../res/assets/dark/inc/images/uiex_radio_checked_d.res"
#include "../res/assets/dark/inc/images/uiex_radio_checked_n.res"
#include "../res/assets/dark/inc/images/uiex_radio_checked_p.res"
#include "../res/assets/dark/inc/images/uiex_radio_d.res"
#include "../res/assets/dark/inc/images/uiex_radio_n.res"
#include "../res/assets/dark/inc/images/uiex_radio_p.res"
#include "../res/assets/dark/inc/images/uiex_shift.res"
#include "../res/assets/dark/inc/images/uiex_shifton.res"
#include "../res/assets/dark/inc/images/uiex_texteditorhide.res"
#include "../res/assets/dark/inc/images/uiex_texteditor_show.res"
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p[]);
#else
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
extern TK_CONST_DATA_ALIGN(const unsigned char font_default[]);
#else  /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*WITH_FS_RES*/

ret_t assets_init_dark(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "dark");

#ifdef WITH_FS_RES
  assets_manager_preload(am, ASSET_TYPE_FONT, "default");
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else
  assets_manager_add(am, strings_en_US);
  assets_manager_add(am, strings_zh_CN);
  assets_manager_add(am, style_uiex_dark);
  assets_manager_add(am, style_uiex_dialog_confirm_dark);
  assets_manager_add(am, style_uiex_dialog_info_dark);
  assets_manager_add(am, style_uiex_dialog_warn_dark);
  assets_manager_add(am, style_uiex_keyboard_dark);
  assets_manager_add(am, style_dialog_toast);
  assets_manager_add(am, ui_uiex_cbp_fast_food);
  assets_manager_add(am, ui_uiex_cbp_font);
  assets_manager_add(am, ui_uiex_cbp_font_size);
  assets_manager_add(am, ui_uiex_cbp_week);
  assets_manager_add(am, ui_uiex_dlg_center_scale);
  assets_manager_add(am, ui_uiex_dlg_fade);
  assets_manager_add(am, ui_uiex_dlg_popdown);
  assets_manager_add(am, ui_uiex_dlg_popup);
  assets_manager_add(am, ui_uiex_htranslate);
  assets_manager_add(am, ui_uiex_kb_ascii);
  assets_manager_add(am, ui_uiex_kb_default);
  assets_manager_add(am, ui_uiex_kb_foo);
  assets_manager_add(am, ui_uiex_kb_hex);
  assets_manager_add(am, ui_uiex_kb_int);
  assets_manager_add(am, ui_uiex_kb_uint);
  assets_manager_add(am, ui_uiex_menu_bar);
  assets_manager_add(am, ui_uiex_menu_landscape);
  assets_manager_add(am, ui_uiex_menu_portrait);
  assets_manager_add(am, ui_uiex_overlay);
  assets_manager_add(am, ui_uiex_page_animate);
  assets_manager_add(am, ui_uiex_page_button);
  assets_manager_add(am, ui_uiex_page_chart);
  assets_manager_add(am, ui_uiex_page_checkbox);
  assets_manager_add(am, ui_uiex_page_color);
  assets_manager_add(am, ui_uiex_page_date_time);
  assets_manager_add(am, ui_uiex_page_edit);
  assets_manager_add(am, ui_uiex_page_image);
  assets_manager_add(am, ui_uiex_page_label);
  assets_manager_add(am, ui_uiex_page_layout);
  assets_manager_add(am, ui_uiex_page_list_view);
  assets_manager_add(am, ui_uiex_page_mledit);
  assets_manager_add(am, ui_uiex_page_progress);
  assets_manager_add(am, ui_uiex_page_selector);
  assets_manager_add(am, ui_uiex_page_slider);
  assets_manager_add(am, ui_uiex_page_slide_view);
  assets_manager_add(am, ui_uiex_page_tab_ctrl);
  assets_manager_add(am, ui_uiex_page_window);
  assets_manager_add(am, ui_uiex_popup);
  assets_manager_add(am, ui_uiex_preload);
  assets_manager_add(am, ui_uiex_slide_down);
  assets_manager_add(am, ui_uiex_slide_left);
  assets_manager_add(am, ui_uiex_slide_right);
  assets_manager_add(am, ui_uiex_slide_up);
  assets_manager_add(am, ui_uiex_sysbar_landscape);
  assets_manager_add(am, ui_uiex_sysbar_portrait);
  assets_manager_add(am, ui_uiex_vtranslate);
  assets_manager_add(am, image_uiex_arrow_down_n_dark);
  assets_manager_add(am, image_uiex_arrow_down_p_dark);
  assets_manager_add(am, image_uiex_arrow_up_n_dark);
  assets_manager_add(am, image_uiex_arrow_up_p_dark);
  assets_manager_add(am, image_uiex_big_logo_dark);
  assets_manager_add(am, image_uiex_button_Empty_dark);
  assets_manager_add(am, image_uiex_button_Export_dark);
  assets_manager_add(am, image_uiex_button_Import_dark);
  assets_manager_add(am, image_uiex_button_search_dark);
  assets_manager_add(am, image_uiex_button_time_dark);
  assets_manager_add(am, image_uiex_check_checked_d_dark);
  assets_manager_add(am, image_uiex_check_checked_n_dark);
  assets_manager_add(am, image_uiex_check_checked_p_dark);
  assets_manager_add(am, image_uiex_check_d_dark);
  assets_manager_add(am, image_uiex_check_n_dark);
  assets_manager_add(am, image_uiex_check_p_dark);
  assets_manager_add(am, image_uiex_close_n_dark);
  assets_manager_add(am, image_uiex_close_p_dark);
  assets_manager_add(am, image_uiex_combobox_btn_d_dark);
  assets_manager_add(am, image_uiex_combobox_btn_n_dark);
  assets_manager_add(am, image_uiex_combobox_btn_p_dark);
  assets_manager_add(am, image_uiex_editor_bgcolor_dark);
  assets_manager_add(am, image_uiex_editor_Importpictures_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignBoth_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignleft_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignright_dark);
  assets_manager_add(am, image_uiex_editor_TextBold_dark);
  assets_manager_add(am, image_uiex_editor_TextCenteralignment_dark);
  assets_manager_add(am, image_uiex_editor_Textcolor_dark);
  assets_manager_add(am, image_uiex_editor_TextItalic_dark);
  assets_manager_add(am, image_uiex_editor_TextLinespacing_dark);
  assets_manager_add(am, image_uiex_editor_TextUnderscore_dark);
  assets_manager_add(am, image_uiex_keyboard_backspace_n_dark);
  assets_manager_add(am, image_uiex_keyboard_backspace_p_dark);
  assets_manager_add(am, image_uiex_keyboard_CN_dark);
  assets_manager_add(am, image_uiex_keyboard_EN_dark);
  assets_manager_add(am, image_uiex_logo_dark);
  assets_manager_add(am, image_uiex_Portraitmenu_n_dark);
  assets_manager_add(am, image_uiex_Portraitmenu_p_dark);
  assets_manager_add(am, image_uiex_radio_checked_d_dark);
  assets_manager_add(am, image_uiex_radio_checked_n_dark);
  assets_manager_add(am, image_uiex_radio_checked_p_dark);
  assets_manager_add(am, image_uiex_radio_d_dark);
  assets_manager_add(am, image_uiex_radio_n_dark);
  assets_manager_add(am, image_uiex_radio_p_dark);
  assets_manager_add(am, image_uiex_shift_dark);
  assets_manager_add(am, image_uiex_shifton_dark);
  assets_manager_add(am, image_uiex_texteditorhide_dark);
  assets_manager_add(am, image_uiex_texteditor_show_dark);
  assets_manager_add(am, image_uiex_Boot_loading);
  assets_manager_add(am, image_uiex_check);
  assets_manager_add(am, image_uiex_Configuration_nor);
  assets_manager_add(am, image_uiex_Configuration_sel);
  assets_manager_add(am, image_uiex_Coverwindow);
  assets_manager_add(am, image_uiex_loading_repeat_bg);
  assets_manager_add(am, image_uiex_send_nor);
  assets_manager_add(am, image_uiex_send_sel);
  assets_manager_add(am, image_uiex_Sliding_dot);
  assets_manager_add(am, image_uiex_Sliding_Square);
  assets_manager_add(am, image_uiex_switch);
  assets_manager_add(am, image_uiex_takeover_nor);
  assets_manager_add(am, image_uiex_takeover_sel);
  assets_manager_add(am, image_uiex_Universal_nor);
  assets_manager_add(am, image_uiex_Universal_sel);
  assets_manager_add(am, image_uiex_Windowclosed_n);
  assets_manager_add(am, image_uiex_Windowclosed_p);
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_default);
#else  /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif

  tk_init_assets();
  return RET_OK;
}
/*** dark end ***/

bool_t assets_has_theme(const char* name) {
  return_value_if_fail(name != NULL, FALSE);

  if (tk_str_eq(name, "default")) {
    return TRUE;
  } else if (tk_str_eq(name, "dark")) {
    return TRUE;
  } else {
    return FALSE;
  }
}

static ret_t assets_init_internal(const char* theme) {
  assets_manager_t* am = assets_manager();
  return_value_if_fail(theme != NULL && am != NULL, RET_BAD_PARAMS);

  assets_manager_set_theme(am, theme);

  if (tk_str_eq(theme, "default")) {
    return assets_init_default();
  } else if (tk_str_eq(theme, "dark")) {
    return assets_init_dark();
  } else {
    log_debug("%s not support.\n", theme);
    return RET_NOT_IMPL;
  }
}

extern ret_t on_adject_win_in_1m_assets(void* ctx, event_t* e);
ret_t assets_init(void) {
  widget_on(window_manager(), EVT_WINDOW_WILL_OPEN, on_adject_win_in_1m_assets, NULL);
  return assets_init_internal(APP_THEME);
}

#ifndef WITH_FS_RES
static ret_t widget_set_theme_without_file_system(widget_t* widget, const char* name) {
  const asset_info_t* info = NULL;
  event_t e = event_init(EVT_THEME_CHANGED, NULL);
  widget_t* wm = widget_get_window_manager(widget);
  font_manager_t* fm = widget_get_font_manager(widget);
  image_manager_t* imm = widget_get_image_manager(widget);
  assets_manager_t* am = widget_get_assets_manager(widget);
  locale_info_t* locale_info = widget_get_locale_info(widget);

  return_value_if_fail(am != NULL && name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(assets_has_theme(name), RET_BAD_PARAMS);

  font_manager_unload_all(fm);
  image_manager_unload_all(imm);
  assets_manager_clear_all(am);
  widget_reset_canvas(widget);

  assets_init_internal(name);
  locale_info_reload(locale_info);

  info = assets_manager_ref(am, ASSET_TYPE_STYLE, "default");
  if (info != NULL) {
    assets_manager_unref(assets_manager(), info);
    theme_set_theme_data(theme(), info->data);
  }

  widget_dispatch(wm, &e);
  widget_invalidate_force(wm, NULL);

  log_debug("theme changed: %s\n", name);

  return RET_OK;
}
#endif /*WITH_FS_RES*/

ret_t assets_set_global_theme(const char* name) {
#ifdef WITH_FS_RES
  return widget_set_theme(window_manager(), name);
#else  /*WITH_FS_RES*/
  return widget_set_theme_without_file_system(window_manager(), name);
#endif /*WITH_FS_RES*/
}
