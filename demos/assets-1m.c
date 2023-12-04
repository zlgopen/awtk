#include "awtk.h"
#include "base/assets_manager.h"

#ifndef APP_THEME
#define APP_THEME "default"
#endif /*APP_THEME*/

/*** default ***/
#ifndef WITH_FS_RES
#include "../res/assets/default/inc/strings/en_US.data"
#include "../res/assets/default/inc/styles/dialog_toast.data"
#include "../res/assets/default/inc/styles/uiex.data"
#include "../res/assets/default/inc/styles/uiex_dialog.data"
#include "../res/assets/default/inc/styles/uiex_keyboard.data"
#include "../res/assets/default/inc/ui/uiex/dlg_center_scale.data"
#include "../res/assets/default/inc/ui/uiex/dlg_fade.data"
#include "../res/assets/default/inc/ui/uiex/dlg_popdown.data"
#include "../res/assets/default/inc/ui/uiex/dlg_popup.data"
#include "../res/assets/default/inc/ui/uiex/htranslate.data"
#include "../res/assets/default/inc/ui/uiex/kb_ascii.data"
#include "../res/assets/default/inc/ui/uiex/kb_default.data"
#include "../res/assets/default/inc/ui/uiex/kb_foo.data"
#include "../res/assets/default/inc/ui/uiex/kb_uint.data"
#include "../res/assets/default/inc/ui/uiex/menu_bar.data"
#include "../res/assets/default/inc/ui/uiex/menu_landscape.data"
#include "../res/assets/default/inc/ui/uiex/overlay.data"
#include "../res/assets/default/inc/ui/uiex/page_animate.data"
#include "../res/assets/default/inc/ui/uiex/page_button.data"
#include "../res/assets/default/inc/ui/uiex/page_chart.data"
#include "../res/assets/default/inc/ui/uiex/page_checkbox.data"
#include "../res/assets/default/inc/ui/uiex/page_color.data"
#include "../res/assets/default/inc/ui/uiex/page_date_time.data"
#include "../res/assets/default/inc/ui/uiex/page_edit.data"
#include "../res/assets/default/inc/ui/uiex/page_label.data"
#include "../res/assets/default/inc/ui/uiex/page_layout.data"
#include "../res/assets/default/inc/ui/uiex/page_mledit.data"
#include "../res/assets/default/inc/ui/uiex/page_progress.data"
#include "../res/assets/default/inc/ui/uiex/page_selector.data"
#include "../res/assets/default/inc/ui/uiex/page_slider.data"
#include "../res/assets/default/inc/ui/uiex/page_tab_ctrl.data"
#include "../res/assets/default/inc/ui/uiex/page_window.data"
#include "../res/assets/default/inc/ui/uiex/popup.data"
#include "../res/assets/default/inc/ui/uiex/preload.data"
#include "../res/assets/default/inc/ui/uiex/sysbar_landscape.data"
#include "../res/assets/default/inc/ui/uiex/sysbar_portrait.data"
#include "../res/assets/default/inc/ui/uiex/vtranslate.data"
#ifdef WITH_STB_IMAGE
#include "../res/assets/default/inc/images/uiex_arrow_down_n.res"
#include "../res/assets/default/inc/images/uiex_arrow_down_p.res"
#include "../res/assets/default/inc/images/uiex_arrow_up_n.res"
#include "../res/assets/default/inc/images/uiex_arrow_up_p.res"
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
#include "../res/assets/default/inc/images/uiex_shift.res"
#include "../res/assets/default/inc/images/uiex_shifton.res"
#include "../res/assets/default/inc/images/uiex_Sliding_dot.res"
#include "../res/assets/default/inc/images/uiex_Sliding_Square.res"
#include "../res/assets/default/inc/images/uiex_switch.res"
#include "../res/assets/default/inc/images/uiex_texteditorhide.res"
#include "../res/assets/default/inc/images/uiex_texteditor_show.res"
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
  assets_manager_add(am, strings_en_US_default);
  assets_manager_add(am, style_dialog_toast_default);
  assets_manager_add(am, style_uiex_default);
  assets_manager_add(am, style_uiex_dialog_default);
  assets_manager_add(am, style_uiex_keyboard_default);
  assets_manager_add(am, ui_dlg_center_scale_default_uiex);
  assets_manager_add(am, ui_dlg_fade_default_uiex);
  assets_manager_add(am, ui_dlg_popdown_default_uiex);
  assets_manager_add(am, ui_dlg_popup_default_uiex);
  assets_manager_add(am, ui_htranslate_default_uiex);
  assets_manager_add(am, ui_kb_ascii_default_uiex);
  assets_manager_add(am, ui_kb_default_default_uiex);
  assets_manager_add(am, ui_kb_foo_default_uiex);
  assets_manager_add(am, ui_kb_uint_default_uiex);
  assets_manager_add(am, ui_menu_bar_default_uiex);
  assets_manager_add(am, ui_menu_landscape_default_uiex);
  assets_manager_add(am, ui_overlay_default_uiex);
  assets_manager_add(am, ui_page_animate_default_uiex);
  assets_manager_add(am, ui_page_button_default_uiex);
  assets_manager_add(am, ui_page_chart_default_uiex);
  assets_manager_add(am, ui_page_checkbox_default_uiex);
  assets_manager_add(am, ui_page_color_default_uiex);
  assets_manager_add(am, ui_page_date_time_default_uiex);
  assets_manager_add(am, ui_page_edit_default_uiex);
  assets_manager_add(am, ui_page_label_default_uiex);
  assets_manager_add(am, ui_page_layout_default_uiex);
  assets_manager_add(am, ui_page_mledit_default_uiex);
  assets_manager_add(am, ui_page_progress_default_uiex);
  assets_manager_add(am, ui_page_selector_default_uiex);
  assets_manager_add(am, ui_page_slider_default_uiex);
  assets_manager_add(am, ui_page_tab_ctrl_default_uiex);
  assets_manager_add(am, ui_page_window_default_uiex);
  assets_manager_add(am, ui_popup_default_uiex);
  assets_manager_add(am, ui_preload_default_uiex);
  assets_manager_add(am, ui_sysbar_landscape_default_uiex);
  assets_manager_add(am, ui_sysbar_portrait_default_uiex);
  assets_manager_add(am, ui_vtranslate_default_uiex);
  assets_manager_add(am, image_uiex_arrow_down_n_default);
  assets_manager_add(am, image_uiex_arrow_down_p_default);
  assets_manager_add(am, image_uiex_arrow_up_n_default);
  assets_manager_add(am, image_uiex_arrow_up_p_default);
  assets_manager_add(am, image_uiex_Boot_loading_default);
  assets_manager_add(am, image_uiex_button_Empty_default);
  assets_manager_add(am, image_uiex_button_Export_default);
  assets_manager_add(am, image_uiex_button_Import_default);
  assets_manager_add(am, image_uiex_button_search_default);
  assets_manager_add(am, image_uiex_button_time_default);
  assets_manager_add(am, image_uiex_check_default);
  assets_manager_add(am, image_uiex_check_checked_d_default);
  assets_manager_add(am, image_uiex_check_checked_n_default);
  assets_manager_add(am, image_uiex_check_checked_p_default);
  assets_manager_add(am, image_uiex_check_d_default);
  assets_manager_add(am, image_uiex_check_n_default);
  assets_manager_add(am, image_uiex_check_p_default);
  assets_manager_add(am, image_uiex_close_n_default);
  assets_manager_add(am, image_uiex_close_p_default);
  assets_manager_add(am, image_uiex_combobox_btn_d_default);
  assets_manager_add(am, image_uiex_combobox_btn_n_default);
  assets_manager_add(am, image_uiex_combobox_btn_p_default);
  assets_manager_add(am, image_uiex_Coverwindow_default);
  assets_manager_add(am, image_uiex_editor_bgcolor_default);
  assets_manager_add(am, image_uiex_editor_Importpictures_default);
  assets_manager_add(am, image_uiex_editor_TextAlignBoth_default);
  assets_manager_add(am, image_uiex_editor_TextAlignleft_default);
  assets_manager_add(am, image_uiex_editor_TextAlignright_default);
  assets_manager_add(am, image_uiex_editor_TextBold_default);
  assets_manager_add(am, image_uiex_editor_TextCenteralignment_default);
  assets_manager_add(am, image_uiex_editor_Textcolor_default);
  assets_manager_add(am, image_uiex_editor_TextItalic_default);
  assets_manager_add(am, image_uiex_editor_TextLinespacing_default);
  assets_manager_add(am, image_uiex_editor_TextUnderscore_default);
  assets_manager_add(am, image_uiex_keyboard_backspace_n_default);
  assets_manager_add(am, image_uiex_keyboard_backspace_p_default);
  assets_manager_add(am, image_uiex_loading_repeat_bg_default);
  assets_manager_add(am, image_uiex_logo_default);
  assets_manager_add(am, image_uiex_Portraitmenu_n_default);
  assets_manager_add(am, image_uiex_Portraitmenu_p_default);
  assets_manager_add(am, image_uiex_radio_checked_d_default);
  assets_manager_add(am, image_uiex_radio_checked_n_default);
  assets_manager_add(am, image_uiex_radio_checked_p_default);
  assets_manager_add(am, image_uiex_radio_d_default);
  assets_manager_add(am, image_uiex_radio_n_default);
  assets_manager_add(am, image_uiex_radio_p_default);
  assets_manager_add(am, image_uiex_shift_default);
  assets_manager_add(am, image_uiex_shifton_default);
  assets_manager_add(am, image_uiex_Sliding_dot_default);
  assets_manager_add(am, image_uiex_Sliding_Square_default);
  assets_manager_add(am, image_uiex_switch_default);
  assets_manager_add(am, image_uiex_texteditorhide_default);
  assets_manager_add(am, image_uiex_texteditor_show_default);
  assets_manager_add(am, image_uiex_Windowclosed_n_default);
  assets_manager_add(am, image_uiex_Windowclosed_p_default);
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_default_default);
#else  /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif

  tk_init_assets();
  return RET_OK;
}
/*** default end ***/

bool_t assets_has_theme(const char* name) {
  return_value_if_fail(name != NULL, FALSE);

  if (tk_str_eq(name, "default")) {
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
  } else {
    log_debug("%s not support.\n", theme);
    return RET_NOT_IMPL;
  }
}

ret_t assets_init(void) {
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
