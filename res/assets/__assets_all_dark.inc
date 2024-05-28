#include "awtk.h"
#include "base/assets_manager.h"
#if !defined(WITH_FS_RES) || defined(AWTK_WEB)
extern TK_CONST_DATA_ALIGN(const unsigned char strings_zh_CN_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char strings_en_US_default[]);
#include "dark/inc/styles/uiex_keyboard.data"
#include "dark/inc/styles/uiex.data"
#include "dark/inc/styles/uiex_dialog_confirm.data"
#include "dark/inc/styles/system_bar.data"
extern TK_CONST_DATA_ALIGN(const unsigned char style_slide_menu_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_color_tile_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_digit_clock_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_toast_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_edit_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_modeless_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_number_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_keyboard_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_color_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_bottom_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_calibration_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_list_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_gauge_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_list_view_dynamic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_default_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_system_bar_bottom_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_5key_main_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_big_font_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_top_compact_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_info_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_file_browser_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_confirm_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_image_value_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_top_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_button_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_bottom_compact_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_warn_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_image_packed_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_up_right_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_ascii_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_vgcanvas_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_rich_text1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_vpage_vtrans_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slider_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_point_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_menu_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_radial_gradient_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_left_bottom_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_auto_play_landscape_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_simple_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_bidi_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_animator_fscript_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog_highlight_update_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_progress_circle_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_accordion2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_combo_box_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_tile_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_phone_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_scrollable_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_main_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_button_view_page1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_digit_clock_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_bar_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_foo_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_lua_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_linear_gradient_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_preload_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_images_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_soft_keyboard_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_animate_widget_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_edit_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_modeless_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_close_window_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_up_left_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_locale_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_ascii_t9_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default_t9_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_animator_layout_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_int_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_control_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_animation_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_basic_fscript_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_int_floating_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_keyboard_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_hex_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_gauge_h_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_popdown_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_svg_test_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_remove_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_back_to_home_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_right_middle_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_check_button_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default_t9ext_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dragger_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_hsv_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_down_right_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_bottom_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_fruit_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_m_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h_loop_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_main_fscript_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_mutable_image_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_drop_file_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_calibration_win_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_memtest_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_sync_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_folder_chooser_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v_loop_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_language_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_float_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_htranslate_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_dynamic_list_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_draggable_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_remove_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_vh_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_rich_text_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_view_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_overlay_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_mledit_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_down_left_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_svg_image_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_list_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_animator_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_down_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_value_anim_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_gauge_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_file_chooser_for_open_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_view_h_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_dynamic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_invisible_compact_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_up_center_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_right_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_bar_h_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_spinbox_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_label_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_auto_resize_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_gif_image_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_down_center_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_test_view_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_auto_play_portrait_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_view_ym_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_system_bar_bottom_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_text_selector_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_rich_text_view_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default_5key_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_5key_main_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_big_font_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_top_compact_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog_highlight_dynamic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_spin_box_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialogs_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_selectm_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_stroke_gradient_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_left_middle_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_basic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_auto_play_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_desktop_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_file_browser_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_full_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog_highlight_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_fade_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_right_top_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_d_f_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_image_list_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_test_fscript_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_mono_test_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_select1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_animate_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_hscroll_label_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_left_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_h1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_image_animation_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_image_value_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_time_clock_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_grid_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_top_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_top_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_button_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_edit_menu_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_text_selector_fscript_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_button_fscript_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_file_chooser_for_save_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_system_bar_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_appoint_remove_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_accordion1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_right_bottom_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_vtranslate_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog_highlight_static_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_invisible_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_left_top_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_ufloat_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_button_view_page2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_remove_loop_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_up_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_rich_text2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_h_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_floating_keyboard_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_rgb_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_bottom_compact_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_popup_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_serial_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_image_packed_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_uint_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_view_v_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_animating_time_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_ascii_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_list_view_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_foo_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_slide_view_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_preload_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_window_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_color_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_edit_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_int_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_hex_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_date_time_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_sysbar_portrait_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_animate_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_layout_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_chart_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_checkbox_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_sysbar_landscape_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_button_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_progress_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dlg_center_scale_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_htranslate_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_overlay_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_down_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_right_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_selector_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dlg_popup_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dlg_fade_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dlg_popdown_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_slider_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_portrait_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_mledit_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_image_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_left_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_landscape_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_tab_ctrl_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_vtranslate_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_up_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_bar_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_popup_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_label_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_uint_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_window_default_win_anim[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_center_default_win_anim[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog_default_win_anim[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_popup_default_win_anim[]);
extern TK_CONST_DATA_ALIGN(const unsigned char xml_test_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_com_zlg_app_json_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_gpinyin_dat_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_test_dat_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_conf_test_json_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_custom_keys_json_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_suggest_words_zh_cn_dat_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_test_json_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_a_b_c_any_default[]);
#ifndef AWTK_WEB
#ifdef WITH_STB_IMAGE
#include "dark/inc/images/uiex_big_logo.res"
#include "dark/inc/images/uiex_num6.res"
#include "dark/inc/images/uiex_shifton.res"
#include "dark/inc/images/uiex_radio_d.res"
#include "dark/inc/images/uiex_arrow_left_p.res"
#include "dark/inc/images/uiex_num7.res"
#include "dark/inc/images/uiex_battery_5.res"
#include "dark/inc/images/uiex_num5.res"
#include "dark/inc/images/uiex_radio_p.res"
#include "dark/inc/images/uiex_num4.res"
#include "dark/inc/images/uiex_battery_4.res"
#include "dark/inc/images/uiex_arrow_right_n.res"
#include "dark/inc/images/uiex_battery_0.res"
#include "dark/inc/images/uiex_num0.res"
#include "dark/inc/images/uiex_load2_bg.res"
#include "dark/inc/images/uiex_num1.res"
#include "dark/inc/images/uiex_battery_1.res"
#include "dark/inc/images/uiex_editor_TextAlignleft.res"
#include "dark/inc/images/uiex_clock_bg.res"
#include "dark/inc/images/uiex_numdot.res"
#include "dark/inc/images/uiex_editor_TextUnderscore.res"
#include "dark/inc/images/uiex_battery_3.res"
#include "dark/inc/images/uiex_num3.res"
#include "dark/inc/images/uiex_num2.res"
#include "dark/inc/images/uiex_battery_2.res"
#include "dark/inc/images/uiex_List_Nextpage_n.res"
#include "dark/inc/images/uiex_texteditor_show.res"
#include "dark/inc/images/uiex_arrow_down_p.res"
#include "dark/inc/images/uiex_editor_Importpictures.res"
#include "dark/inc/images/uiex_combobox_btn_n.res"
#include "dark/inc/images/uiex_radio_checked_p.res"
#include "dark/inc/images/uiex_editor_TextLinespacing.res"
#include "dark/inc/images/uiex_arrow_up_n.res"
#include "dark/inc/images/uiex_keyboard_backspace_n.res"
#include "dark/inc/images/uiex_check_p.res"
#include "dark/inc/images/uiex_check_checked_d.res"
#include "dark/inc/images/uiex_RecordForm_bg.res"
#include "dark/inc/images/uiex_Portraitmenu_p.res"
#include "dark/inc/images/uiex_radio_checked_d.res"
#include "dark/inc/images/uiex_close_p.res"
#include "dark/inc/images/uiex_check_d.res"
#include "dark/inc/images/uiex_check_checked_p.res"
#include "dark/inc/images/uiex_editor_TextAlignBoth.res"
#include "dark/inc/images/uiex_editor_bgcolor.res"
#include "dark/inc/images/uiex_arrow_down_n.res"
#include "dark/inc/images/uiex_combobox_btn_p.res"
#include "dark/inc/images/uiex_radio_checked_n.res"
#include "dark/inc/images/uiex_arrow_up_p.res"
#include "dark/inc/images/uiex_load1_bg.res"
#include "dark/inc/images/uiex_keyboard_backspace_p.res"
#include "dark/inc/images/uiex_check_n.res"
#include "dark/inc/images/uiex_Portraitmenu_n.res"
#include "dark/inc/images/uiex_clock_Minutehand.res"
#include "dark/inc/images/uiex_keyboard_CN.res"
#include "dark/inc/images/uiex_button_Export.res"
#include "dark/inc/images/uiex_combobox_btn_d.res"
#include "dark/inc/images/uiex_close_n.res"
#include "dark/inc/images/uiex_editor_Textcolor.res"
#include "dark/inc/images/uiex_check_checked_n.res"
#include "dark/inc/images/uiex_texteditorhide.res"
#include "dark/inc/images/uiex_button_search.res"
#include "dark/inc/images/uiex_editor_TextAlignright.res"
#include "dark/inc/images/uiex_keyboard_EN.res"
#include "dark/inc/images/uiex_logo.res"
#include "dark/inc/images/uiex_arrow_left_n.res"
#include "dark/inc/images/uiex_editor_TextItalic.res"
#include "dark/inc/images/uiex_editor_TextCenteralignment.res"
#include "dark/inc/images/uiex_button_time.res"
#include "dark/inc/images/uiex_radio_n.res"
#include "dark/inc/images/uiex_button_Empty.res"
#include "dark/inc/images/uiex_arrow_right_p.res"
#include "dark/inc/images/uiex_editor_TextBold.res"
#include "dark/inc/images/uiex_num9.res"
#include "dark/inc/images/uiex_button_Import.res"
#include "dark/inc/images/uiex_num8.res"
#include "dark/inc/images/uiex_clock_Hourhand.res"
#include "dark/inc/images/uiex_List_Nextpage_p.res"
#include "dark/inc/images/uiex_shift.res"
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani6_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_checked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_7_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_6_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shift_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani7_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ania_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_anic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_earth_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_none_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bee_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_contact_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_fg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_contact_active_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dialog_title_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_anib_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_me_active_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_file_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_hand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_left_on_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_v_fg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_empty_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_rgba_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_0_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_0_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_visible_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Mydevice_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_second_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load2_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_password_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_en_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_middle_on_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bricks_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_image_packed_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_msg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load1_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_msg_active_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_expand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_checked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_active_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_zh_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_question_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_discovery_active_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shifton_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_check_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_chat_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_me_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_info_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_middle_off_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_icon_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_plus_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_collapse_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_return_up_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_indeterminate_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_muted_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Connectandshare_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_rgb_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unmuted_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_right_off_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_message_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_WLAN_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_right_on_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_pointer_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_US_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_0_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_left_off_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_progress_circle_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_dynamic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Dots_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_minus_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_text_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unchecked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_privacyprotection_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_v_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_warn_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_CN_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_unchecked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_sound_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_list_Battery_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_minute_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cross_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_fg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_avatar_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Secondhand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_image_packed_fg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_find_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani9_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_folder_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_8_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_discovery_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Moresettings_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_9_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_show_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani8_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_invisible_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Bluetooth_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_backspace_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_desktop_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_hour_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_s_default[]);
#else /*WITH_STB_IMAGE*/
#include "dark/inc/images/uiex_keyboard_CN.data"
#include "dark/inc/images/uiex_arrow_left_n.data"
#include "dark/inc/images/uiex_radio_p.data"
#include "dark/inc/images/uiex_load1_bg.data"
#include "dark/inc/images/uiex_num1.data"
#include "dark/inc/images/uiex_arrow_down_n.data"
#include "dark/inc/images/uiex_clock_Minutehand.data"
#include "dark/inc/images/uiex_shift.data"
#include "dark/inc/images/uiex_keyboard_EN.data"
#include "dark/inc/images/uiex_keyboard_backspace_n.data"
#include "dark/inc/images/uiex_num0.data"
#include "dark/inc/images/uiex_button_time.data"
#include "dark/inc/images/uiex_logo.data"
#include "dark/inc/images/uiex_Portraitmenu_p.data"
#include "dark/inc/images/uiex_radio_d.data"
#include "dark/inc/images/uiex_arrow_right_n.data"
#include "dark/inc/images/uiex_num7.data"
#include "dark/inc/images/uiex_editor_TextAlignleft.data"
#include "dark/inc/images/uiex_editor_Textcolor.data"
#include "dark/inc/images/uiex_clock_bg.data"
#include "dark/inc/images/uiex_Portraitmenu_n.data"
#include "dark/inc/images/uiex_arrow_right_p.data"
#include "dark/inc/images/uiex_texteditorhide.data"
#include "dark/inc/images/uiex_load2_bg.data"
#include "dark/inc/images/uiex_keyboard_backspace_p.data"
#include "dark/inc/images/uiex_battery_0.data"
#include "dark/inc/images/uiex_editor_TextUnderscore.data"
#include "dark/inc/images/uiex_battery_1.data"
#include "dark/inc/images/uiex_arrow_down_p.data"
#include "dark/inc/images/uiex_editor_bgcolor.data"
#include "dark/inc/images/uiex_editor_TextLinespacing.data"
#include "dark/inc/images/uiex_arrow_left_p.data"
#include "dark/inc/images/uiex_radio_n.data"
#include "dark/inc/images/uiex_editor_TextItalic.data"
#include "dark/inc/images/uiex_num6.data"
#include "dark/inc/images/uiex_check_checked_n.data"
#include "dark/inc/images/uiex_List_Nextpage_n.data"
#include "dark/inc/images/uiex_radio_checked_p.data"
#include "dark/inc/images/uiex_num5.data"
#include "dark/inc/images/uiex_shifton.data"
#include "dark/inc/images/uiex_battery_2.data"
#include "dark/inc/images/uiex_arrow_up_n.data"
#include "dark/inc/images/uiex_big_logo.data"
#include "dark/inc/images/uiex_num9.data"
#include "dark/inc/images/uiex_num8.data"
#include "dark/inc/images/uiex_battery_3.data"
#include "dark/inc/images/uiex_combobox_btn_n.data"
#include "dark/inc/images/uiex_button_Import.data"
#include "dark/inc/images/uiex_num4.data"
#include "dark/inc/images/uiex_check_n.data"
#include "dark/inc/images/uiex_radio_checked_d.data"
#include "dark/inc/images/uiex_editor_TextAlignBoth.data"
#include "dark/inc/images/uiex_numdot.data"
#include "dark/inc/images/uiex_close_p.data"
#include "dark/inc/images/uiex_editor_TextCenteralignment.data"
#include "dark/inc/images/uiex_check_checked_d.data"
#include "dark/inc/images/uiex_battery_4.data"
#include "dark/inc/images/uiex_check_p.data"
#include "dark/inc/images/uiex_clock_Hourhand.data"
#include "dark/inc/images/uiex_button_Export.data"
#include "dark/inc/images/uiex_editor_TextAlignright.data"
#include "dark/inc/images/uiex_close_n.data"
#include "dark/inc/images/uiex_texteditor_show.data"
#include "dark/inc/images/uiex_button_search.data"
#include "dark/inc/images/uiex_num3.data"
#include "dark/inc/images/uiex_combobox_btn_p.data"
#include "dark/inc/images/uiex_num2.data"
#include "dark/inc/images/uiex_arrow_up_p.data"
#include "dark/inc/images/uiex_RecordForm_bg.data"
#include "dark/inc/images/uiex_combobox_btn_d.data"
#include "dark/inc/images/uiex_editor_TextBold.data"
#include "dark/inc/images/uiex_button_Empty.data"
#include "dark/inc/images/uiex_check_checked_p.data"
#include "dark/inc/images/uiex_check_d.data"
#include "dark/inc/images/uiex_radio_checked_n.data"
#include "dark/inc/images/uiex_battery_5.data"
#include "dark/inc/images/uiex_List_Nextpage_p.data"
#include "dark/inc/images/uiex_editor_Importpictures.data"
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_image_packed_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_0_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_progress_circle_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_muted_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_file_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_plus_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_8_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_earth_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_none_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_dynamic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shift_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_discovery_active_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_WLAN_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_en_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_right_on_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_discovery_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_collapse_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_avatar_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_rgb_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load1_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_9_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_0_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_find_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_fg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_left_on_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_question_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_invisible_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Connectandshare_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_image_packed_fg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bee_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_visible_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_list_Battery_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_me_active_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_minute_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_checked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_middle_off_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_minus_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_folder_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_chat_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_zh_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani9_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_anib_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_sound_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Secondhand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Mydevice_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_empty_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_left_off_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_unchecked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_rgba_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shifton_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_msg_active_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_anic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani8_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_active_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_desktop_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_right_off_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unmuted_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_show_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_icon_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cross_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_expand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_backspace_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_second_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_contact_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bricks_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dialog_title_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_hand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani7_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_me_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_0_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Dots_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_warn_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_privacyprotection_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_contact_active_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_indeterminate_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani6_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_message_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_fg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ania_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_6_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_check_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_return_up_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_middle_on_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_checked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_v_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_hour_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_CN_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_password_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Bluetooth_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load2_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unchecked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_pointer_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_v_fg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_info_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_US_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_msg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_7_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_text_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Moresettings_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check_default[]);
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_TRUETYPE_FONT
extern TK_CONST_DATA_ALIGN(const unsigned char font_trado_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_ap_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_full_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_default[]);
#else /*WITH_TRUETYPE_FONT*/
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_20_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_16_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_18_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_96_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_32_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_24_default[]);
#endif /*WITH_TRUETYPE_FONT*/
extern TK_CONST_DATA_ALIGN(const unsigned char script_dummy_default[]);
#endif /*AWTK_WEB*/
#ifdef WITH_VGCANVAS
#include "dark/inc/images/computer.bsvg"
extern TK_CONST_DATA_ALIGN(const unsigned char image_paths_data_02_t_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_pointer_4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_htest_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ball_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shapes_polyline_01_t_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_china_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_meter_pointer_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_pservers_grad_99_b_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_tiger_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shapes_line_01_t_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_painting_stroke_05_t_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_paths_data_01_t_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_pointer_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_pointer_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shapes_circle_01_t_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shapes_rect_01_t_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_girl_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gradient_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_windmill_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shapes_ellipse_01_t_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shapes_polygon_01_t_default[]);
#endif /*WITH_VGCANVAS*/
#endif /*!defined(WITH_FS_RES) || defined(AWTK_WEB)*/

ret_t assets_init_dark(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "dark");

#if defined(WITH_FS_RES) && !defined(AWTK_WEB)
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else /*defined(WITH_FS_RES) && !defined(AWTK_WEB)*/
  assets_manager_add(am, strings_zh_CN_default);
  assets_manager_add(am, strings_en_US_default);
  assets_manager_add(am, style_uiex_keyboard_dark);
  assets_manager_add(am, style_uiex_dark);
  assets_manager_add(am, style_uiex_dialog_confirm_dark);
  assets_manager_add(am, style_system_bar_dark);
  assets_manager_add(am, style_slide_menu_default);
  assets_manager_add(am, style_color_tile_default);
  assets_manager_add(am, style_digit_clock_default);
  assets_manager_add(am, style_dialog_toast_default);
  assets_manager_add(am, style_edit_default);
  assets_manager_add(am, style_modeless_default);
  assets_manager_add(am, style_number_default);
  assets_manager_add(am, style_keyboard_default);
  assets_manager_add(am, style_color_default);
  assets_manager_add(am, style_tab_bottom_default);
  assets_manager_add(am, style_calibration_default);
  assets_manager_add(am, style_tab_list_default);
  assets_manager_add(am, style_gauge_default);
  assets_manager_add(am, style_list_view_dynamic_default);
  assets_manager_add(am, style_default_default);
  assets_manager_add(am, style_system_bar_bottom_default);
  assets_manager_add(am, style_5key_main_default);
  assets_manager_add(am, style_big_font_default);
  assets_manager_add(am, style_tab_top_compact_default);
  assets_manager_add(am, style_dialog_info_default);
  assets_manager_add(am, style_file_browser_default);
  assets_manager_add(am, style_dialog_confirm_default);
  assets_manager_add(am, style_image_value_default);
  assets_manager_add(am, style_dialog_default);
  assets_manager_add(am, style_tab_top_default);
  assets_manager_add(am, style_button_default);
  assets_manager_add(am, style_tab_bottom_compact_default);
  assets_manager_add(am, style_dialog_warn_default);
  assets_manager_add(am, style_image_packed_default);
  assets_manager_add(am, ui_menu_up_right_default);
  assets_manager_add(am, ui_kb_ascii_default);
  assets_manager_add(am, ui_vgcanvas_default);
  assets_manager_add(am, ui_rich_text1_default);
  assets_manager_add(am, ui_vpage_vtrans_default);
  assets_manager_add(am, ui_slider_default);
  assets_manager_add(am, ui_menu_point_default);
  assets_manager_add(am, ui_slide_menu_default);
  assets_manager_add(am, ui_radial_gradient_default);
  assets_manager_add(am, ui_menu_left_bottom_default);
  assets_manager_add(am, ui_auto_play_landscape_default);
  assets_manager_add(am, ui_color_picker_simple_default);
  assets_manager_add(am, ui_bidi_default);
  assets_manager_add(am, ui_animator_fscript_default);
  assets_manager_add(am, ui_slide_view_v3_default);
  assets_manager_add(am, ui_dialog_highlight_update_bg_default);
  assets_manager_add(am, ui_progress_circle_default);
  assets_manager_add(am, ui_accordion2_default);
  assets_manager_add(am, ui_combo_box_default);
  assets_manager_add(am, ui_color_tile_default);
  assets_manager_add(am, ui_kb_phone_default);
  assets_manager_add(am, ui_tab_scrollable_default);
  assets_manager_add(am, ui_main_default);
  assets_manager_add(am, ui_tab_button_view_page1_default);
  assets_manager_add(am, ui_digit_clock_default);
  assets_manager_add(am, ui_scroll_bar_default);
  assets_manager_add(am, ui_kb_foo_default);
  assets_manager_add(am, ui_lua_default);
  assets_manager_add(am, ui_linear_gradient_default);
  assets_manager_add(am, ui_preload_default);
  assets_manager_add(am, ui_images_default);
  assets_manager_add(am, ui_soft_keyboard_default);
  assets_manager_add(am, ui_animate_widget_default);
  assets_manager_add(am, ui_edit_default);
  assets_manager_add(am, ui_modeless_default);
  assets_manager_add(am, ui_close_window_default);
  assets_manager_add(am, ui_menu_up_left_default);
  assets_manager_add(am, ui_locale_default);
  assets_manager_add(am, ui_kb_ascii_t9_default);
  assets_manager_add(am, ui_kb_default_t9_default);
  assets_manager_add(am, ui_animator_layout_default);
  assets_manager_add(am, ui_kb_int_default);
  assets_manager_add(am, ui_tab_control_default);
  assets_manager_add(am, ui_slide_view_v2_default);
  assets_manager_add(am, ui_animation_default);
  assets_manager_add(am, ui_basic_fscript_default);
  assets_manager_add(am, ui_kb_int_floating_default);
  assets_manager_add(am, ui_keyboard_default);
  assets_manager_add(am, ui_kb_hex_default);
  assets_manager_add(am, ui_gauge_h_default);
  assets_manager_add(am, ui_popdown_default);
  assets_manager_add(am, ui_svg_test_default);
  assets_manager_add(am, ui_slide_view_remove_default);
  assets_manager_add(am, ui_color_default);
  assets_manager_add(am, ui_color_picker_default);
  assets_manager_add(am, ui_switch_default);
  assets_manager_add(am, ui_back_to_home_default);
  assets_manager_add(am, ui_menu_right_middle_default);
  assets_manager_add(am, ui_list_view_check_button_default);
  assets_manager_add(am, ui_kb_default_t9ext_default);
  assets_manager_add(am, ui_dragger_default);
  assets_manager_add(am, ui_dialog2_default);
  assets_manager_add(am, ui_color_picker_hsv_default);
  assets_manager_add(am, ui_menu_down_right_default);
  assets_manager_add(am, ui_tab_bottom_default);
  assets_manager_add(am, ui_fruit_default);
  assets_manager_add(am, ui_list_view_m_default);
  assets_manager_add(am, ui_slide_view_h_loop_default);
  assets_manager_add(am, ui_main_fscript_default);
  assets_manager_add(am, ui_mutable_image_default);
  assets_manager_add(am, ui_drop_file_default);
  assets_manager_add(am, ui_calibration_win_default);
  assets_manager_add(am, ui_memtest_default);
  assets_manager_add(am, ui_list_view_sync_default);
  assets_manager_add(am, ui_slide_view_h2_default);
  assets_manager_add(am, ui_slide_view_h3_default);
  assets_manager_add(am, ui_folder_chooser_default);
  assets_manager_add(am, ui_slide_view_v_loop_default);
  assets_manager_add(am, ui_language_default);
  assets_manager_add(am, ui_kb_float_default);
  assets_manager_add(am, ui_htranslate_default);
  assets_manager_add(am, ui_tab_dynamic_list_default);
  assets_manager_add(am, ui_draggable_default);
  assets_manager_add(am, ui_tab_remove_default);
  assets_manager_add(am, ui_list_view_vh_default);
  assets_manager_add(am, ui_rich_text_default);
  assets_manager_add(am, ui_scroll_view_default);
  assets_manager_add(am, ui_overlay_default);
  assets_manager_add(am, ui_mledit_default);
  assets_manager_add(am, ui_menu_down_left_default);
  assets_manager_add(am, ui_svg_image_default);
  assets_manager_add(am, ui_tab_list_default);
  assets_manager_add(am, ui_animator_default);
  assets_manager_add(am, ui_slide_down_default);
  assets_manager_add(am, ui_value_anim_default);
  assets_manager_add(am, ui_gauge_default);
  assets_manager_add(am, ui_file_chooser_for_open_default);
  assets_manager_add(am, ui_scroll_view_h_default);
  assets_manager_add(am, ui_list_view_dynamic_default);
  assets_manager_add(am, ui_tab_invisible_compact_default);
  assets_manager_add(am, ui_menu_up_center_default);
  assets_manager_add(am, ui_slide_right_default);
  assets_manager_add(am, ui_scroll_bar_h_default);
  assets_manager_add(am, ui_spinbox_default);
  assets_manager_add(am, ui_label_default);
  assets_manager_add(am, ui_list_view_auto_resize_default);
  assets_manager_add(am, ui_gif_image_default);
  assets_manager_add(am, ui_menu_down_center_default);
  assets_manager_add(am, ui_test_view_default);
  assets_manager_add(am, ui_auto_play_portrait_default);
  assets_manager_add(am, ui_scroll_view_ym_default);
  assets_manager_add(am, ui_system_bar_bottom_default);
  assets_manager_add(am, ui_text_selector_default);
  assets_manager_add(am, ui_rich_text_view_default);
  assets_manager_add(am, ui_kb_default_5key_default);
  assets_manager_add(am, ui_5key_main_default);
  assets_manager_add(am, ui_big_font_default);
  assets_manager_add(am, ui_tab_top_compact_default);
  assets_manager_add(am, ui_dialog_highlight_dynamic_default);
  assets_manager_add(am, ui_slide_view_h_default);
  assets_manager_add(am, ui_spin_box_default);
  assets_manager_add(am, ui_dialogs_default);
  assets_manager_add(am, ui_selectm_default);
  assets_manager_add(am, ui_list_view_default);
  assets_manager_add(am, ui_stroke_gradient_default);
  assets_manager_add(am, ui_menu_left_middle_default);
  assets_manager_add(am, ui_slide_view_h1_default);
  assets_manager_add(am, ui_basic_default);
  assets_manager_add(am, ui_auto_play_default);
  assets_manager_add(am, ui_desktop_default);
  assets_manager_add(am, ui_file_browser_default);
  assets_manager_add(am, ui_color_picker_full_default);
  assets_manager_add(am, ui_dialog1_default);
  assets_manager_add(am, ui_dialog_highlight_default);
  assets_manager_add(am, ui_fade_default);
  assets_manager_add(am, ui_menu_right_top_default);
  assets_manager_add(am, ui_list_view_d_f_default);
  assets_manager_add(am, ui_image_list_default);
  assets_manager_add(am, ui_slide_view_v1_default);
  assets_manager_add(am, ui_test_fscript_default);
  assets_manager_add(am, ui_mono_test_default);
  assets_manager_add(am, ui_select1_default);
  assets_manager_add(am, ui_list_view_animate_default);
  assets_manager_add(am, ui_hscroll_label_default);
  assets_manager_add(am, ui_slide_left_default);
  assets_manager_add(am, ui_list_view_h1_default);
  assets_manager_add(am, ui_image_animation_default);
  assets_manager_add(am, ui_image_value_default);
  assets_manager_add(am, ui_slide_view_default);
  assets_manager_add(am, ui_time_clock_default);
  assets_manager_add(am, ui_grid_default);
  assets_manager_add(am, ui_tab_top_default);
  assets_manager_add(am, ui_slide_view_v_default);
  assets_manager_add(am, ui_top_default);
  assets_manager_add(am, ui_button_default);
  assets_manager_add(am, ui_edit_menu_default);
  assets_manager_add(am, ui_menu_default);
  assets_manager_add(am, ui_text_selector_fscript_default);
  assets_manager_add(am, ui_button_fscript_default);
  assets_manager_add(am, ui_file_chooser_for_save_default);
  assets_manager_add(am, ui_system_bar_default);
  assets_manager_add(am, ui_slide_view_appoint_remove_default);
  assets_manager_add(am, ui_accordion1_default);
  assets_manager_add(am, ui_menu_right_bottom_default);
  assets_manager_add(am, ui_list_view_d_default);
  assets_manager_add(am, ui_vtranslate_default);
  assets_manager_add(am, ui_dialog_highlight_static_default);
  assets_manager_add(am, ui_tab_invisible_default);
  assets_manager_add(am, ui_menu_left_top_default);
  assets_manager_add(am, ui_kb_ufloat_default);
  assets_manager_add(am, ui_kb_default_default);
  assets_manager_add(am, ui_tab_button_view_page2_default);
  assets_manager_add(am, ui_slide_view_remove_loop_default);
  assets_manager_add(am, ui_slide_up_default);
  assets_manager_add(am, ui_rich_text2_default);
  assets_manager_add(am, ui_list_view_h_default);
  assets_manager_add(am, ui_floating_keyboard_default);
  assets_manager_add(am, ui_color_picker_rgb_default);
  assets_manager_add(am, ui_tab_bottom_compact_default);
  assets_manager_add(am, ui_popup_default);
  assets_manager_add(am, ui_serial_default);
  assets_manager_add(am, ui_image_packed_default);
  assets_manager_add(am, ui_kb_uint_default);
  assets_manager_add(am, ui_scroll_view_v_default);
  assets_manager_add(am, ui_slide_view_animating_time_default);
  assets_manager_add(am, ui_kb_ascii_default_uiex);
  assets_manager_add(am, ui_page_list_view_default_uiex);
  assets_manager_add(am, ui_kb_foo_default_uiex);
  assets_manager_add(am, ui_page_slide_view_default_uiex);
  assets_manager_add(am, ui_preload_default_uiex);
  assets_manager_add(am, ui_page_window_default_uiex);
  assets_manager_add(am, ui_page_color_default_uiex);
  assets_manager_add(am, ui_page_edit_default_uiex);
  assets_manager_add(am, ui_kb_int_default_uiex);
  assets_manager_add(am, ui_kb_hex_default_uiex);
  assets_manager_add(am, ui_page_date_time_default_uiex);
  assets_manager_add(am, ui_sysbar_portrait_default_uiex);
  assets_manager_add(am, ui_page_animate_default_uiex);
  assets_manager_add(am, ui_page_layout_default_uiex);
  assets_manager_add(am, ui_page_chart_default_uiex);
  assets_manager_add(am, ui_page_checkbox_default_uiex);
  assets_manager_add(am, ui_sysbar_landscape_default_uiex);
  assets_manager_add(am, ui_page_button_default_uiex);
  assets_manager_add(am, ui_page_progress_default_uiex);
  assets_manager_add(am, ui_dlg_center_scale_default_uiex);
  assets_manager_add(am, ui_htranslate_default_uiex);
  assets_manager_add(am, ui_overlay_default_uiex);
  assets_manager_add(am, ui_slide_down_default_uiex);
  assets_manager_add(am, ui_slide_right_default_uiex);
  assets_manager_add(am, ui_page_selector_default_uiex);
  assets_manager_add(am, ui_dlg_popup_default_uiex);
  assets_manager_add(am, ui_dlg_fade_default_uiex);
  assets_manager_add(am, ui_dlg_popdown_default_uiex);
  assets_manager_add(am, ui_page_slider_default_uiex);
  assets_manager_add(am, ui_menu_portrait_default_uiex);
  assets_manager_add(am, ui_page_mledit_default_uiex);
  assets_manager_add(am, ui_page_image_default_uiex);
  assets_manager_add(am, ui_slide_left_default_uiex);
  assets_manager_add(am, ui_menu_landscape_default_uiex);
  assets_manager_add(am, ui_page_tab_ctrl_default_uiex);
  assets_manager_add(am, ui_vtranslate_default_uiex);
  assets_manager_add(am, ui_kb_default_default_uiex);
  assets_manager_add(am, ui_slide_up_default_uiex);
  assets_manager_add(am, ui_menu_bar_default_uiex);
  assets_manager_add(am, ui_popup_default_uiex);
  assets_manager_add(am, ui_page_label_default_uiex);
  assets_manager_add(am, ui_kb_uint_default_uiex);
  assets_manager_add(am, ui_window_default_win_anim);
  assets_manager_add(am, ui_center_default_win_anim);
  assets_manager_add(am, ui_dialog_default_win_anim);
  assets_manager_add(am, ui_popup_default_win_anim);
  assets_manager_add(am, xml_test_default);
  assets_manager_add(am, data_com_zlg_app_json_default);
  assets_manager_add(am, data_gpinyin_dat_default);
  assets_manager_add(am, data_test_dat_default);
  assets_manager_add(am, data_conf_test_json_default);
  assets_manager_add(am, data_custom_keys_json_default);
  assets_manager_add(am, data_suggest_words_zh_cn_dat_default);
  assets_manager_add(am, data_test_json_default);
  assets_manager_add(am, data_a_b_c_any_default);
#ifndef AWTK_WEB
  assets_manager_add(am, image_uiex_big_logo_dark);
  assets_manager_add(am, image_uiex_num6_dark);
  assets_manager_add(am, image_uiex_shifton_dark);
  assets_manager_add(am, image_uiex_radio_d_dark);
  assets_manager_add(am, image_uiex_arrow_left_p_dark);
  assets_manager_add(am, image_uiex_num7_dark);
  assets_manager_add(am, image_uiex_battery_5_dark);
  assets_manager_add(am, image_uiex_num5_dark);
  assets_manager_add(am, image_uiex_radio_p_dark);
  assets_manager_add(am, image_uiex_num4_dark);
  assets_manager_add(am, image_uiex_battery_4_dark);
  assets_manager_add(am, image_uiex_arrow_right_n_dark);
  assets_manager_add(am, image_uiex_battery_0_dark);
  assets_manager_add(am, image_uiex_num0_dark);
  assets_manager_add(am, image_uiex_load2_bg_dark);
  assets_manager_add(am, image_uiex_num1_dark);
  assets_manager_add(am, image_uiex_battery_1_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignleft_dark);
  assets_manager_add(am, image_uiex_clock_bg_dark);
  assets_manager_add(am, image_uiex_numdot_dark);
  assets_manager_add(am, image_uiex_editor_TextUnderscore_dark);
  assets_manager_add(am, image_uiex_battery_3_dark);
  assets_manager_add(am, image_uiex_num3_dark);
  assets_manager_add(am, image_uiex_num2_dark);
  assets_manager_add(am, image_uiex_battery_2_dark);
  assets_manager_add(am, image_uiex_List_Nextpage_n_dark);
  assets_manager_add(am, image_uiex_texteditor_show_dark);
  assets_manager_add(am, image_uiex_arrow_down_p_dark);
  assets_manager_add(am, image_uiex_editor_Importpictures_dark);
  assets_manager_add(am, image_uiex_combobox_btn_n_dark);
  assets_manager_add(am, image_uiex_radio_checked_p_dark);
  assets_manager_add(am, image_uiex_editor_TextLinespacing_dark);
  assets_manager_add(am, image_uiex_arrow_up_n_dark);
  assets_manager_add(am, image_uiex_keyboard_backspace_n_dark);
  assets_manager_add(am, image_uiex_check_p_dark);
  assets_manager_add(am, image_uiex_check_checked_d_dark);
  assets_manager_add(am, image_uiex_RecordForm_bg_dark);
  assets_manager_add(am, image_uiex_Portraitmenu_p_dark);
  assets_manager_add(am, image_uiex_radio_checked_d_dark);
  assets_manager_add(am, image_uiex_close_p_dark);
  assets_manager_add(am, image_uiex_check_d_dark);
  assets_manager_add(am, image_uiex_check_checked_p_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignBoth_dark);
  assets_manager_add(am, image_uiex_editor_bgcolor_dark);
  assets_manager_add(am, image_uiex_arrow_down_n_dark);
  assets_manager_add(am, image_uiex_combobox_btn_p_dark);
  assets_manager_add(am, image_uiex_radio_checked_n_dark);
  assets_manager_add(am, image_uiex_arrow_up_p_dark);
  assets_manager_add(am, image_uiex_load1_bg_dark);
  assets_manager_add(am, image_uiex_keyboard_backspace_p_dark);
  assets_manager_add(am, image_uiex_check_n_dark);
  assets_manager_add(am, image_uiex_Portraitmenu_n_dark);
  assets_manager_add(am, image_uiex_clock_Minutehand_dark);
  assets_manager_add(am, image_uiex_keyboard_CN_dark);
  assets_manager_add(am, image_uiex_button_Export_dark);
  assets_manager_add(am, image_uiex_combobox_btn_d_dark);
  assets_manager_add(am, image_uiex_close_n_dark);
  assets_manager_add(am, image_uiex_editor_Textcolor_dark);
  assets_manager_add(am, image_uiex_check_checked_n_dark);
  assets_manager_add(am, image_uiex_texteditorhide_dark);
  assets_manager_add(am, image_uiex_button_search_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignright_dark);
  assets_manager_add(am, image_uiex_keyboard_EN_dark);
  assets_manager_add(am, image_uiex_logo_dark);
  assets_manager_add(am, image_uiex_arrow_left_n_dark);
  assets_manager_add(am, image_uiex_editor_TextItalic_dark);
  assets_manager_add(am, image_uiex_editor_TextCenteralignment_dark);
  assets_manager_add(am, image_uiex_button_time_dark);
  assets_manager_add(am, image_uiex_radio_n_dark);
  assets_manager_add(am, image_uiex_button_Empty_dark);
  assets_manager_add(am, image_uiex_arrow_right_p_dark);
  assets_manager_add(am, image_uiex_editor_TextBold_dark);
  assets_manager_add(am, image_uiex_num9_dark);
  assets_manager_add(am, image_uiex_button_Import_dark);
  assets_manager_add(am, image_uiex_num8_dark);
  assets_manager_add(am, image_uiex_clock_Hourhand_dark);
  assets_manager_add(am, image_uiex_List_Nextpage_p_dark);
  assets_manager_add(am, image_uiex_shift_dark);
  assets_manager_add(am, image_ani6_default);
  assets_manager_add(am, image_checked_default);
  assets_manager_add(am, image_num_7_default);
  assets_manager_add(am, image_close_p_default);
  assets_manager_add(am, image_num_6_default);
  assets_manager_add(am, image_bg_landscape_1_default);
  assets_manager_add(am, image_shift_default);
  assets_manager_add(am, image_ani7_default);
  assets_manager_add(am, image_ania_default);
  assets_manager_add(am, image_anic_default);
  assets_manager_add(am, image_earth_default);
  assets_manager_add(am, image_ani5_default);
  assets_manager_add(am, image_flag_none_default);
  assets_manager_add(am, image_bee_default);
  assets_manager_add(am, image_debug_step_over_d_default);
  assets_manager_add(am, image_contact_default);
  assets_manager_add(am, image_bg_landscape_3_default);
  assets_manager_add(am, image_battery_4_default);
  assets_manager_add(am, image_num_4_default);
  assets_manager_add(am, image_slider_fg_default);
  assets_manager_add(am, image_contact_active_default);
  assets_manager_add(am, image_uiex_Configuration_nor_default);
  assets_manager_add(am, image_close_d_default);
  assets_manager_add(am, image_dialog_title_default);
  assets_manager_add(am, image_uiex_Boot_loading_default);
  assets_manager_add(am, image_battery_5_default);
  assets_manager_add(am, image_num_5_default);
  assets_manager_add(am, image_bg_landscape_2_default);
  assets_manager_add(am, image_ani4_default);
  assets_manager_add(am, image_anib_default);
  assets_manager_add(am, image_me_active_default);
  assets_manager_add(am, image_file_default);
  assets_manager_add(am, image_arrow_up_n_default);
  assets_manager_add(am, image_uiex_Universal_sel_default);
  assets_manager_add(am, image_cursor_hand_default);
  assets_manager_add(am, image_left_on_default);
  assets_manager_add(am, image_num_1_default);
  assets_manager_add(am, image_battery_1_default);
  assets_manager_add(am, image_arrow_right_p_default);
  assets_manager_add(am, image_slider_v_fg_default);
  assets_manager_add(am, image_empty_default);
  assets_manager_add(am, image_rgba_default);
  assets_manager_add(am, image_num_0_default);
  assets_manager_add(am, image_battery_0_default);
  assets_manager_add(am, image_edit_clear_p_default);
  assets_manager_add(am, image_arrow_up_o_default);
  assets_manager_add(am, image_visible_default);
  assets_manager_add(am, image_ani1_default);
  assets_manager_add(am, image_uiex_List_Mydevice_default);
  assets_manager_add(am, image_arrow_left_o_default);
  assets_manager_add(am, image_ani3_default);
  assets_manager_add(am, image_debug_pause_o_default);
  assets_manager_add(am, image_debug_stop_d_default);
  assets_manager_add(am, image_clock_second_default);
  assets_manager_add(am, image_debug_resume_n_default);
  assets_manager_add(am, image_debug_step_return_d_default);
  assets_manager_add(am, image_uiex_load2_loading_default);
  assets_manager_add(am, image_battery_2_default);
  assets_manager_add(am, image_num_2_default);
  assets_manager_add(am, image_uiex_Sliding_dot_default);
  assets_manager_add(am, image_uiex_List_password_default);
  assets_manager_add(am, image_en_default);
  assets_manager_add(am, image_slider_drag_o_default);
  assets_manager_add(am, image_battery_3_default);
  assets_manager_add(am, image_num_3_default);
  assets_manager_add(am, image_debug_resume_o_default);
  assets_manager_add(am, image_uiex_Coverwindow_default);
  assets_manager_add(am, image_debug_pause_n_default);
  assets_manager_add(am, image_ani2_default);
  assets_manager_add(am, image_arrow_left_n_default);
  assets_manager_add(am, image_middle_on_default);
  assets_manager_add(am, image_app3_default);
  assets_manager_add(am, image_bricks_default);
  assets_manager_add(am, image_image_packed_bg_default);
  assets_manager_add(am, image_msg_default);
  assets_manager_add(am, image_uiex_Windowclosed_n_default);
  assets_manager_add(am, image_play_o_default);
  assets_manager_add(am, image_play_n_default);
  assets_manager_add(am, image_uiex_load1_loading_default);
  assets_manager_add(am, image_msg_active_default);
  assets_manager_add(am, image_img_slider_bg_default);
  assets_manager_add(am, image_app2_default);
  assets_manager_add(am, image_expand_default);
  assets_manager_add(am, image_radio_checked_default);
  assets_manager_add(am, image_active_dot_default);
  assets_manager_add(am, image_zh_default);
  assets_manager_add(am, image_question_default);
  assets_manager_add(am, image_discovery_active_default);
  assets_manager_add(am, image_dot_default);
  assets_manager_add(am, image_gauge_bg_default);
  assets_manager_add(am, image_shifton_default);
  assets_manager_add(am, image_app1_default);
  assets_manager_add(am, image_app5_default);
  assets_manager_add(am, image_check_default);
  assets_manager_add(am, image_uiex_Information_chat_bg_default);
  assets_manager_add(am, image_uiex_Sliding_Square_default);
  assets_manager_add(am, image_uiex_takeover_sel_default);
  assets_manager_add(am, image_cursor_default);
  assets_manager_add(am, image_uiex_send_nor_default);
  assets_manager_add(am, image_me_default);
  assets_manager_add(am, image_uiex_loading_repeat_bg_default);
  assets_manager_add(am, image_info_default);
  assets_manager_add(am, image_red_btn_p_default);
  assets_manager_add(am, image_app4_default);
  assets_manager_add(am, image_middle_off_default);
  assets_manager_add(am, image_debug_restart_d_default);
  assets_manager_add(am, image_img_slider_icon_default);
  assets_manager_add(am, image_arrow_down_n_default);
  assets_manager_add(am, image_debug_step_into_d_default);
  assets_manager_add(am, image_num_plus_default);
  assets_manager_add(am, image_collapse_default);
  assets_manager_add(am, image_return_up_default);
  assets_manager_add(am, image_green_btn_o_default);
  assets_manager_add(am, image_green_btn_n_default);
  assets_manager_add(am, image_debug_start_d_default);
  assets_manager_add(am, image_indeterminate_default);
  assets_manager_add(am, image_muted_default);
  assets_manager_add(am, image_uiex_List_Connectandshare_default);
  assets_manager_add(am, image_arrow_down_o_default);
  assets_manager_add(am, image_rgb_default);
  assets_manager_add(am, image_num_dot_default);
  assets_manager_add(am, image_unmuted_default);
  assets_manager_add(am, image_right_off_default);
  assets_manager_add(am, image_message_default);
  assets_manager_add(am, image_uiex_Windowclosed_p_default);
  assets_manager_add(am, image_uiex_Universal_nor_default);
  assets_manager_add(am, image_3_default);
  assets_manager_add(am, image_play_p_default);
  assets_manager_add(am, image_2_default);
  assets_manager_add(am, image_slider_bg_default);
  assets_manager_add(am, image_uiex_List_WLAN_default);
  assets_manager_add(am, image_right_on_default);
  assets_manager_add(am, image_gauge_pointer_default);
  assets_manager_add(am, image_bg2_s_default);
  assets_manager_add(am, image_flag_US_default);
  assets_manager_add(am, image_uiex_check_default);
  assets_manager_add(am, image_0_default);
  assets_manager_add(am, image_left_off_default);
  assets_manager_add(am, image_1_default);
  assets_manager_add(am, image_progress_circle_default);
  assets_manager_add(am, image_logo_dynamic_default);
  assets_manager_add(am, image_debug_restart_n_default);
  assets_manager_add(am, image_red_btn_o_default);
  assets_manager_add(am, image_debug_step_into_n_default);
  assets_manager_add(am, image_uiex_clock_Dots_default);
  assets_manager_add(am, image_slider_drag_default);
  assets_manager_add(am, image_num_minus_default);
  assets_manager_add(am, image_debug_start_o_default);
  assets_manager_add(am, image_bg_portrait_1_default);
  assets_manager_add(am, image_debug_start_n_default);
  assets_manager_add(am, image_bg4_s_default);
  assets_manager_add(am, image_cursor_text_default);
  assets_manager_add(am, image_unchecked_default);
  assets_manager_add(am, image_uiex_List_privacyprotection_default);
  assets_manager_add(am, image_debug_step_into_o_default);
  assets_manager_add(am, image_red_btn_n_default);
  assets_manager_add(am, image_debug_restart_o_default);
  assets_manager_add(am, image_uiex_Configuration_sel_default);
  assets_manager_add(am, image_arrow_down_p_default);
  assets_manager_add(am, image_slider_v_bg_default);
  assets_manager_add(am, image_warn_default);
  assets_manager_add(am, image_bg_portrait_3_default);
  assets_manager_add(am, image_green_btn_p_default);
  assets_manager_add(am, image_bg_portrait_2_default);
  assets_manager_add(am, image_flag_CN_default);
  assets_manager_add(am, image_repeat3_default);
  assets_manager_add(am, image_switch_default);
  assets_manager_add(am, image_radio_unchecked_default);
  assets_manager_add(am, image_bg1_default);
  assets_manager_add(am, image_uiex_List_sound_default);
  assets_manager_add(am, image_uiex_takeover_nor_default);
  assets_manager_add(am, image_debug_step_over_n_default);
  assets_manager_add(am, image_bg1_s_default);
  assets_manager_add(am, image_close_n_default);
  assets_manager_add(am, image_close_o_default);
  assets_manager_add(am, image_uiex_list_Battery_default);
  assets_manager_add(am, image_clock_bg_default);
  assets_manager_add(am, image_uiex_send_sel_default);
  assets_manager_add(am, image_clock_minute_default);
  assets_manager_add(am, image_debug_step_over_o_default);
  assets_manager_add(am, image_cross_default);
  assets_manager_add(am, image_repeat2_default);
  assets_manager_add(am, image_bg2_default);
  assets_manager_add(am, image_img_slider_fg_default);
  assets_manager_add(am, image_uiex_Information_avatar_default);
  assets_manager_add(am, image_clock_default);
  assets_manager_add(am, image_uiex_clock_Secondhand_default);
  assets_manager_add(am, image_bg3_s_default);
  assets_manager_add(am, image_image_packed_fg_default);
  assets_manager_add(am, image_bg3_default);
  assets_manager_add(am, image_repeat1_default);
  assets_manager_add(am, image_find_default);
  assets_manager_add(am, image_ani9_default);
  assets_manager_add(am, image_debug_stop_n_default);
  assets_manager_add(am, image_arrow_up_p_default);
  assets_manager_add(am, image_debug_step_return_n_default);
  assets_manager_add(am, image_edit_clear_o_default);
  assets_manager_add(am, image_folder_default);
  assets_manager_add(am, image_debug_resume_d_default);
  assets_manager_add(am, image_num_8_default);
  assets_manager_add(am, image_arrow_right_n_default);
  assets_manager_add(am, image_discovery_default);
  assets_manager_add(am, image_arrow_right_o_default);
  assets_manager_add(am, image_uiex_List_Moresettings_default);
  assets_manager_add(am, image_num_9_default);
  assets_manager_add(am, image_edit_clear_n_default);
  assets_manager_add(am, image_debug_step_return_o_default);
  assets_manager_add(am, image_uiex_List_show_default);
  assets_manager_add(am, image_debug_stop_o_default);
  assets_manager_add(am, image_ani8_default);
  assets_manager_add(am, image_debug_pause_d_default);
  assets_manager_add(am, image_repeat4_default);
  assets_manager_add(am, image_uiex_switch_default);
  assets_manager_add(am, image_bg4_default);
  assets_manager_add(am, image_logo_default);
  assets_manager_add(am, image_invisible_default);
  assets_manager_add(am, image_slider_drag_p_default);
  assets_manager_add(am, image_uiex_List_Bluetooth_default);
  assets_manager_add(am, image_backspace_default);
  assets_manager_add(am, image_uiex_List_desktop_default);
  assets_manager_add(am, image_bg5_default);
  assets_manager_add(am, image_arrow_left_p_default);
  assets_manager_add(am, image_clock_hour_default);
  assets_manager_add(am, image_bg5_s_default);
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_trado_default);
  assets_manager_add(am, font_ap_default);
  assets_manager_add(am, font_default_full_default);
  assets_manager_add(am, font_default_default);
#else /*WITH_TRUETYPE_FONT*/
  assets_manager_add(am, font_default_20_default);
  assets_manager_add(am, font_default_16_default);
  assets_manager_add(am, font_default_18_default);
  assets_manager_add(am, font_default_96_default);
  assets_manager_add(am, font_default_32_default);
  assets_manager_add(am, font_default_24_default);
#endif /*WITH_TRUETYPE_FONT*/
  assets_manager_add(am, script_dummy_default);
#endif /*AWTK_WEB*/
#ifdef WITH_VGCANVAS
  assets_manager_add(am, image_computer_dark);
  assets_manager_add(am, image_paths_data_02_t_default);
  assets_manager_add(am, image_pointer_4_default);
  assets_manager_add(am, image_htest_default);
  assets_manager_add(am, image_ball_default);
  assets_manager_add(am, image_shapes_polyline_01_t_default);
  assets_manager_add(am, image_china_default);
  assets_manager_add(am, image_meter_pointer_default);
  assets_manager_add(am, image_pservers_grad_99_b_default);
  assets_manager_add(am, image_tiger_default);
  assets_manager_add(am, image_shapes_line_01_t_default);
  assets_manager_add(am, image_painting_stroke_05_t_default);
  assets_manager_add(am, image_paths_data_01_t_default);
  assets_manager_add(am, image_pointer_1_default);
  assets_manager_add(am, image_pointer_default);
  assets_manager_add(am, image_shapes_circle_01_t_default);
  assets_manager_add(am, image_shapes_rect_01_t_default);
  assets_manager_add(am, image_girl_default);
  assets_manager_add(am, image_gradient_default);
  assets_manager_add(am, image_windmill_default);
  assets_manager_add(am, image_shapes_ellipse_01_t_default);
  assets_manager_add(am, image_shapes_polygon_01_t_default);
#endif /*WITH_VGCANVAS*/
#endif /*defined(WITH_FS_RES) && !defined(AWTK_WEB)*/

  tk_init_assets();
  return RET_OK;
}
