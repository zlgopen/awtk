#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
extern TK_CONST_DATA_ALIGN(const unsigned char strings_zh_CN[]);
extern TK_CONST_DATA_ALIGN(const unsigned char strings_en_US[]);
#include "dark/inc/styles/uiex_dialog_warn.data"
#include "dark/inc/styles/uiex_keyboard.data"
#include "dark/inc/styles/uiex_dialog_info.data"
#include "dark/inc/styles/uiex.data"
#include "dark/inc/styles/uiex_dialog_confirm.data"
#include "dark/inc/styles/system_bar.data"
extern TK_CONST_DATA_ALIGN(const unsigned char style_slide_menu[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_color_tile[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_digit_clock[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_toast[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_edit[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_keyboard[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_color[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_bottom[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_calibration[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_list[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_gauge[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_list_view_dynamic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_system_bar_bottom[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_big_font[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_top_compact[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_info[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_file_browser[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_confirm[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_image_value[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_top[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_button[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_tab_bottom_compact[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_warn[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_image_packed[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_up_right[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_ascii[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_vgcanvas[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_rich_text1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_vpage_vtrans[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slider[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_point[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_menu[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_radial_gradient[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_left_bottom[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_auto_play_landscape[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_simple[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_bidi[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_progress_circle[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_combo_box[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_tile[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_phone[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_scrollable[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_main[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_button_view_page1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_digit_clock[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_bar[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_foo[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_lua[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_linear_gradient[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_preload[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_images[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_soft_keyboard[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_animate_widget[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_edit[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_up_left[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_locale[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_ascii_t9[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default_t9[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_int[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_control[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_animation[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_basic_fscript[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_int_floating[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_keyboard[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_hex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_gauge_h[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_popdown[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_remove[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_switch[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_back_to_home[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_right_middle[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default_t9ext[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dragger[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_hsv[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_down_right[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_bottom[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_m[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h_loop[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_main_fscript[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_mutable_image[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_calibration_win[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_memtest[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_folder_chooser[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v_loop[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_language[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_float[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_htranslate[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_dynamic_list[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_draggable[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_remove[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_vh[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_rich_text[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_overlay[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_mledit[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_down_left[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_svg_image[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_list[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_animator[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_down[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_value_anim[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_gauge[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_file_chooser_for_open[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_view_h[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_dynamic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_up_center[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_right[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_bar_h[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_spinbox[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_label[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_auto_resize[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_gif_image[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_down_center[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_test_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_auto_play_portrait[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_system_bar_bottom[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_text_selector[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_rich_text_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_big_font[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_top_compact[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialogs[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_selectm[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_stroke_gradient[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_left_middle[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_h1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_basic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_auto_play[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_desktop[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_file_browser[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_full[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dialog1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_fade[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_right_top[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_d_f[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_image_list[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_test_fscript[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_mono_test[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_select1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_hscroll_label[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_left[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_h1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_image_animation[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_image_value[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_time_clock[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_top[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_v[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_top[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_button[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_text_selector_fscript[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_button_fscript[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_file_chooser_for_save[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_system_bar[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_right_bottom[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_vtranslate[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_left_top[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_ufloat[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_button_view_page2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_view_remove_loop[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_up[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_rich_text2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_list_view_h[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_floating_keyboard[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_color_picker_rgb[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_tab_bottom_compact[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_popup[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_image_packed[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_uint[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_scroll_view_v[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_ascii[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_list_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_foo[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_slide_view[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_preload[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_window[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_cbp_fast_food[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_color[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_edit[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_int[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_hex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_date_time[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_cbp_week[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_sysbar_portrait[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_animate[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_layout[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_chart[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_checkbox[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_sysbar_landscape[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_button[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_progress[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_dlg_center_scale[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_cbp_font[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_htranslate[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_overlay[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_slide_down[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_slide_right[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_selector[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_dlg_popup[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_dlg_fade[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_dlg_popdown[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_cbp_font_size[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_slider[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_menu_portrait[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_mledit[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_image[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_slide_left[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_menu_landscape[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_tab_ctrl[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_vtranslate[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_slide_up[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_menu_bar[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_popup[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_page_label[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_uiex_kb_uint[]);
extern TK_CONST_DATA_ALIGN(const unsigned char xml_test[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_com_zlg_app_json[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_gpinyin_dat[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_test_dat[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_conf_test_json[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_suggest_words_zh_cn_dat[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_test_json[]);
extern TK_CONST_DATA_ALIGN(const unsigned char data_a_b_c_any[]);
#ifdef WITH_STB_IMAGE
#include "dark/inc/images/uiex_big_logo.res"
#include "dark/inc/images/uiex_num6.res"
#include "dark/inc/images/uiex_shifton.res"
#include "dark/inc/images/uiex_radio_d.res"
#include "dark/inc/images/uiex_num7.res"
#include "dark/inc/images/uiex_battery_5.res"
#include "dark/inc/images/uiex_num5.res"
#include "dark/inc/images/uiex_radio_p.res"
#include "dark/inc/images/uiex_num4.res"
#include "dark/inc/images/uiex_battery_4.res"
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
#include "dark/inc/images/uiex_editor_TextItalic.res"
#include "dark/inc/images/uiex_editor_TextCenteralignment.res"
#include "dark/inc/images/uiex_button_time.res"
#include "dark/inc/images/uiex_radio_n.res"
#include "dark/inc/images/uiex_button_Empty.res"
#include "dark/inc/images/uiex_editor_TextBold.res"
#include "dark/inc/images/uiex_num9.res"
#include "dark/inc/images/uiex_button_Import.res"
#include "dark/inc/images/uiex_num8.res"
#include "dark/inc/images/uiex_clock_Hourhand.res"
#include "dark/inc/images/uiex_List_Nextpage_p.res"
#include "dark/inc/images/uiex_shift.res"
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani6[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_checked[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_7[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_6[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shift[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani7[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ania[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_anic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_earth[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_none[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bee[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_contact[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_fg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_contact_active[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dialog_title[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_anib[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_me_active[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_file[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_hand[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_left_on[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_v_fg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_empty[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_rgba[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_0[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_0[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_visible[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Mydevice[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_second[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load2_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_password[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_en[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_middle_on[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bricks[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_image_packed_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_msg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load1_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_msg_active[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_checked[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_active_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_zh[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_question[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_discovery_active[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shifton[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_check[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_chat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_me[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_info[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_middle_off[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_icon[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_plus[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_return_up[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_muted[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Connectandshare[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_rgb[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unmuted[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_right_off[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_message[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_WLAN[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_right_on[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_pointer[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_US[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_0[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_left_off[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_progress_circle[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_dynamic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Dots[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_minus[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_text[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unchecked[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_privacyprotection[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_v_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_warn[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_CN[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_switch[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_unchecked[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_sound[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_list_Battery[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_minute[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cross[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_fg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_avatar[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Secondhand[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_image_packed_fg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_find[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani9[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_folder[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_8[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_discovery[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Moresettings[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_9[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_show[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani8[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_invisible[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Bluetooth[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_backspace[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_desktop[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_hour[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_s[]);
#else
#include "dark/inc/images/uiex_keyboard_CN.data"
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
#include "dark/inc/images/uiex_num7.data"
#include "dark/inc/images/uiex_editor_TextAlignleft.data"
#include "dark/inc/images/uiex_editor_Textcolor.data"
#include "dark/inc/images/uiex_clock_bg.data"
#include "dark/inc/images/uiex_Portraitmenu_n.data"
#include "dark/inc/images/uiex_texteditorhide.data"
#include "dark/inc/images/uiex_load2_bg.data"
#include "dark/inc/images/uiex_keyboard_backspace_p.data"
#include "dark/inc/images/uiex_battery_0.data"
#include "dark/inc/images/uiex_editor_TextUnderscore.data"
#include "dark/inc/images/uiex_battery_1.data"
#include "dark/inc/images/uiex_arrow_down_p.data"
#include "dark/inc/images/uiex_editor_bgcolor.data"
#include "dark/inc/images/uiex_editor_TextLinespacing.data"
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
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_image_packed_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_0[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_progress_circle[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_muted[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_file[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_plus[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_8[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_earth[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_none[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_dynamic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shift[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_discovery_active[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_WLAN[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_en[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_right_on[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_discovery[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_avatar[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_rgb[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load1_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_9[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_0[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_find[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_fg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_left_on[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_question[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_invisible[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Connectandshare[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_image_packed_fg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bee[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_visible[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_list_Battery[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_me_active[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_minute[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_switch[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_checked[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_middle_off[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_minus[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_folder[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_chat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_zh[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani9[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_anib[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_sound[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Secondhand[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Mydevice[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_empty[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_left_off[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_unchecked[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_rgba[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shifton[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_msg_active[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_anic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani8[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_active_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_desktop[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_close_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_right_off[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unmuted[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_show[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_icon[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cross[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_backspace[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_second[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_contact[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bricks[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dialog_title[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_hand[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani7[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_me[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_0[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Dots[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_warn[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_privacyprotection[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_contact_active[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_portrait_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg_landscape_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani6[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_message[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_img_slider_fg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ania[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_6[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_check[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_return_up[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_middle_on[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_checked[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_v_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_clock_hour[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_CN[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_password[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Bluetooth[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load2_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unchecked[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_pointer[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_green_btn_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_red_btn_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_drag_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_slider_v_fg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_info[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_flag_US[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_msg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_num_7[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_text[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Moresettings[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_battery_2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check[]);
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_VGCANVAS
#include "dark/inc/images/computer.bsvg"
extern TK_CONST_DATA_ALIGN(const unsigned char image_pointer_4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_htest[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ball[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_china[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_meter_pointer[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_pointer_1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_pointer[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_girl[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_windmill[]);
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
extern TK_CONST_DATA_ALIGN(const unsigned char font_trado[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_ap[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_full[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default[]);
#else /*WITH_TRUETYPE_FONT*/
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_20[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_16[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_18[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_96[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_32[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_24[]);
#endif /*WITH_TRUETYPE_FONT*/
extern TK_CONST_DATA_ALIGN(const unsigned char script_dummy[]);
#endif /*WITH_FS_RES*/

ret_t assets_init_dark(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "dark");

#ifdef WITH_FS_RES
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else
  assets_manager_add(am, strings_zh_CN);
  assets_manager_add(am, strings_en_US);
  assets_manager_add(am, style_uiex_dialog_warn_dark);
  assets_manager_add(am, style_uiex_keyboard_dark);
  assets_manager_add(am, style_uiex_dialog_info_dark);
  assets_manager_add(am, style_uiex_dark);
  assets_manager_add(am, style_uiex_dialog_confirm_dark);
  assets_manager_add(am, style_system_bar_dark);
  assets_manager_add(am, style_slide_menu);
  assets_manager_add(am, style_color_tile);
  assets_manager_add(am, style_digit_clock);
  assets_manager_add(am, style_dialog_toast);
  assets_manager_add(am, style_edit);
  assets_manager_add(am, style_keyboard);
  assets_manager_add(am, style_color);
  assets_manager_add(am, style_tab_bottom);
  assets_manager_add(am, style_calibration);
  assets_manager_add(am, style_tab_list);
  assets_manager_add(am, style_gauge);
  assets_manager_add(am, style_list_view_dynamic);
  assets_manager_add(am, style_default);
  assets_manager_add(am, style_system_bar_bottom);
  assets_manager_add(am, style_big_font);
  assets_manager_add(am, style_tab_top_compact);
  assets_manager_add(am, style_dialog_info);
  assets_manager_add(am, style_file_browser);
  assets_manager_add(am, style_dialog_confirm);
  assets_manager_add(am, style_image_value);
  assets_manager_add(am, style_dialog);
  assets_manager_add(am, style_tab_top);
  assets_manager_add(am, style_button);
  assets_manager_add(am, style_tab_bottom_compact);
  assets_manager_add(am, style_dialog_warn);
  assets_manager_add(am, style_image_packed);
  assets_manager_add(am, ui_menu_up_right);
  assets_manager_add(am, ui_kb_ascii);
  assets_manager_add(am, ui_vgcanvas);
  assets_manager_add(am, ui_rich_text1);
  assets_manager_add(am, ui_vpage_vtrans);
  assets_manager_add(am, ui_slider);
  assets_manager_add(am, ui_menu_point);
  assets_manager_add(am, ui_slide_menu);
  assets_manager_add(am, ui_radial_gradient);
  assets_manager_add(am, ui_menu_left_bottom);
  assets_manager_add(am, ui_auto_play_landscape);
  assets_manager_add(am, ui_color_picker_simple);
  assets_manager_add(am, ui_bidi);
  assets_manager_add(am, ui_progress_circle);
  assets_manager_add(am, ui_combo_box);
  assets_manager_add(am, ui_color_tile);
  assets_manager_add(am, ui_kb_phone);
  assets_manager_add(am, ui_tab_scrollable);
  assets_manager_add(am, ui_main);
  assets_manager_add(am, ui_tab_button_view_page1);
  assets_manager_add(am, ui_digit_clock);
  assets_manager_add(am, ui_scroll_bar);
  assets_manager_add(am, ui_kb_foo);
  assets_manager_add(am, ui_lua);
  assets_manager_add(am, ui_linear_gradient);
  assets_manager_add(am, ui_preload);
  assets_manager_add(am, ui_images);
  assets_manager_add(am, ui_soft_keyboard);
  assets_manager_add(am, ui_animate_widget);
  assets_manager_add(am, ui_edit);
  assets_manager_add(am, ui_menu_up_left);
  assets_manager_add(am, ui_locale);
  assets_manager_add(am, ui_kb_ascii_t9);
  assets_manager_add(am, ui_kb_default_t9);
  assets_manager_add(am, ui_kb_int);
  assets_manager_add(am, ui_tab_control);
  assets_manager_add(am, ui_slide_view_v2);
  assets_manager_add(am, ui_animation);
  assets_manager_add(am, ui_basic_fscript);
  assets_manager_add(am, ui_kb_int_floating);
  assets_manager_add(am, ui_keyboard);
  assets_manager_add(am, ui_kb_hex);
  assets_manager_add(am, ui_gauge_h);
  assets_manager_add(am, ui_popdown);
  assets_manager_add(am, ui_slide_view_remove);
  assets_manager_add(am, ui_color);
  assets_manager_add(am, ui_color_picker);
  assets_manager_add(am, ui_switch);
  assets_manager_add(am, ui_back_to_home);
  assets_manager_add(am, ui_menu_right_middle);
  assets_manager_add(am, ui_kb_default_t9ext);
  assets_manager_add(am, ui_dragger);
  assets_manager_add(am, ui_dialog2);
  assets_manager_add(am, ui_color_picker_hsv);
  assets_manager_add(am, ui_menu_down_right);
  assets_manager_add(am, ui_tab_bottom);
  assets_manager_add(am, ui_list_view_m);
  assets_manager_add(am, ui_slide_view_h_loop);
  assets_manager_add(am, ui_main_fscript);
  assets_manager_add(am, ui_mutable_image);
  assets_manager_add(am, ui_calibration_win);
  assets_manager_add(am, ui_memtest);
  assets_manager_add(am, ui_slide_view_h2);
  assets_manager_add(am, ui_folder_chooser);
  assets_manager_add(am, ui_slide_view_v_loop);
  assets_manager_add(am, ui_language);
  assets_manager_add(am, ui_kb_float);
  assets_manager_add(am, ui_htranslate);
  assets_manager_add(am, ui_tab_dynamic_list);
  assets_manager_add(am, ui_draggable);
  assets_manager_add(am, ui_tab_remove);
  assets_manager_add(am, ui_list_view_vh);
  assets_manager_add(am, ui_rich_text);
  assets_manager_add(am, ui_scroll_view);
  assets_manager_add(am, ui_overlay);
  assets_manager_add(am, ui_mledit);
  assets_manager_add(am, ui_menu_down_left);
  assets_manager_add(am, ui_svg_image);
  assets_manager_add(am, ui_tab_list);
  assets_manager_add(am, ui_animator);
  assets_manager_add(am, ui_slide_down);
  assets_manager_add(am, ui_value_anim);
  assets_manager_add(am, ui_gauge);
  assets_manager_add(am, ui_file_chooser_for_open);
  assets_manager_add(am, ui_scroll_view_h);
  assets_manager_add(am, ui_list_view_dynamic);
  assets_manager_add(am, ui_menu_up_center);
  assets_manager_add(am, ui_slide_right);
  assets_manager_add(am, ui_scroll_bar_h);
  assets_manager_add(am, ui_spinbox);
  assets_manager_add(am, ui_label);
  assets_manager_add(am, ui_list_view_auto_resize);
  assets_manager_add(am, ui_gif_image);
  assets_manager_add(am, ui_menu_down_center);
  assets_manager_add(am, ui_test_view);
  assets_manager_add(am, ui_auto_play_portrait);
  assets_manager_add(am, ui_system_bar_bottom);
  assets_manager_add(am, ui_text_selector);
  assets_manager_add(am, ui_rich_text_view);
  assets_manager_add(am, ui_big_font);
  assets_manager_add(am, ui_tab_top_compact);
  assets_manager_add(am, ui_slide_view_h);
  assets_manager_add(am, ui_dialogs);
  assets_manager_add(am, ui_selectm);
  assets_manager_add(am, ui_list_view);
  assets_manager_add(am, ui_stroke_gradient);
  assets_manager_add(am, ui_menu_left_middle);
  assets_manager_add(am, ui_slide_view_h1);
  assets_manager_add(am, ui_basic);
  assets_manager_add(am, ui_auto_play);
  assets_manager_add(am, ui_desktop);
  assets_manager_add(am, ui_file_browser);
  assets_manager_add(am, ui_color_picker_full);
  assets_manager_add(am, ui_dialog1);
  assets_manager_add(am, ui_fade);
  assets_manager_add(am, ui_menu_right_top);
  assets_manager_add(am, ui_list_view_d_f);
  assets_manager_add(am, ui_image_list);
  assets_manager_add(am, ui_slide_view_v1);
  assets_manager_add(am, ui_test_fscript);
  assets_manager_add(am, ui_mono_test);
  assets_manager_add(am, ui_select1);
  assets_manager_add(am, ui_hscroll_label);
  assets_manager_add(am, ui_slide_left);
  assets_manager_add(am, ui_list_view_h1);
  assets_manager_add(am, ui_image_animation);
  assets_manager_add(am, ui_image_value);
  assets_manager_add(am, ui_slide_view);
  assets_manager_add(am, ui_time_clock);
  assets_manager_add(am, ui_tab_top);
  assets_manager_add(am, ui_slide_view_v);
  assets_manager_add(am, ui_top);
  assets_manager_add(am, ui_button);
  assets_manager_add(am, ui_menu);
  assets_manager_add(am, ui_text_selector_fscript);
  assets_manager_add(am, ui_button_fscript);
  assets_manager_add(am, ui_file_chooser_for_save);
  assets_manager_add(am, ui_system_bar);
  assets_manager_add(am, ui_menu_right_bottom);
  assets_manager_add(am, ui_list_view_d);
  assets_manager_add(am, ui_vtranslate);
  assets_manager_add(am, ui_menu_left_top);
  assets_manager_add(am, ui_kb_ufloat);
  assets_manager_add(am, ui_kb_default);
  assets_manager_add(am, ui_tab_button_view_page2);
  assets_manager_add(am, ui_slide_view_remove_loop);
  assets_manager_add(am, ui_slide_up);
  assets_manager_add(am, ui_rich_text2);
  assets_manager_add(am, ui_list_view_h);
  assets_manager_add(am, ui_floating_keyboard);
  assets_manager_add(am, ui_color_picker_rgb);
  assets_manager_add(am, ui_tab_bottom_compact);
  assets_manager_add(am, ui_popup);
  assets_manager_add(am, ui_image_packed);
  assets_manager_add(am, ui_kb_uint);
  assets_manager_add(am, ui_scroll_view_v);
  assets_manager_add(am, ui_uiex_kb_ascii);
  assets_manager_add(am, ui_uiex_page_list_view);
  assets_manager_add(am, ui_uiex_kb_foo);
  assets_manager_add(am, ui_uiex_page_slide_view);
  assets_manager_add(am, ui_uiex_preload);
  assets_manager_add(am, ui_uiex_page_window);
  assets_manager_add(am, ui_uiex_cbp_fast_food);
  assets_manager_add(am, ui_uiex_page_color);
  assets_manager_add(am, ui_uiex_page_edit);
  assets_manager_add(am, ui_uiex_kb_int);
  assets_manager_add(am, ui_uiex_kb_hex);
  assets_manager_add(am, ui_uiex_page_date_time);
  assets_manager_add(am, ui_uiex_cbp_week);
  assets_manager_add(am, ui_uiex_sysbar_portrait);
  assets_manager_add(am, ui_uiex_page_animate);
  assets_manager_add(am, ui_uiex_page_layout);
  assets_manager_add(am, ui_uiex_page_chart);
  assets_manager_add(am, ui_uiex_page_checkbox);
  assets_manager_add(am, ui_uiex_sysbar_landscape);
  assets_manager_add(am, ui_uiex_page_button);
  assets_manager_add(am, ui_uiex_page_progress);
  assets_manager_add(am, ui_uiex_dlg_center_scale);
  assets_manager_add(am, ui_uiex_cbp_font);
  assets_manager_add(am, ui_uiex_htranslate);
  assets_manager_add(am, ui_uiex_overlay);
  assets_manager_add(am, ui_uiex_slide_down);
  assets_manager_add(am, ui_uiex_slide_right);
  assets_manager_add(am, ui_uiex_page_selector);
  assets_manager_add(am, ui_uiex_dlg_popup);
  assets_manager_add(am, ui_uiex_dlg_fade);
  assets_manager_add(am, ui_uiex_dlg_popdown);
  assets_manager_add(am, ui_uiex_cbp_font_size);
  assets_manager_add(am, ui_uiex_page_slider);
  assets_manager_add(am, ui_uiex_menu_portrait);
  assets_manager_add(am, ui_uiex_page_mledit);
  assets_manager_add(am, ui_uiex_page_image);
  assets_manager_add(am, ui_uiex_slide_left);
  assets_manager_add(am, ui_uiex_menu_landscape);
  assets_manager_add(am, ui_uiex_page_tab_ctrl);
  assets_manager_add(am, ui_uiex_vtranslate);
  assets_manager_add(am, ui_uiex_kb_default);
  assets_manager_add(am, ui_uiex_slide_up);
  assets_manager_add(am, ui_uiex_menu_bar);
  assets_manager_add(am, ui_uiex_popup);
  assets_manager_add(am, ui_uiex_page_label);
  assets_manager_add(am, ui_uiex_kb_uint);
  assets_manager_add(am, xml_test);
  assets_manager_add(am, data_com_zlg_app_json);
  assets_manager_add(am, data_gpinyin_dat);
  assets_manager_add(am, data_test_dat);
  assets_manager_add(am, data_conf_test_json);
  assets_manager_add(am, data_suggest_words_zh_cn_dat);
  assets_manager_add(am, data_test_json);
  assets_manager_add(am, data_a_b_c_any);
  assets_manager_add(am, image_uiex_big_logo_dark);
  assets_manager_add(am, image_uiex_num6_dark);
  assets_manager_add(am, image_uiex_shifton_dark);
  assets_manager_add(am, image_uiex_radio_d_dark);
  assets_manager_add(am, image_uiex_num7_dark);
  assets_manager_add(am, image_uiex_battery_5_dark);
  assets_manager_add(am, image_uiex_num5_dark);
  assets_manager_add(am, image_uiex_radio_p_dark);
  assets_manager_add(am, image_uiex_num4_dark);
  assets_manager_add(am, image_uiex_battery_4_dark);
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
  assets_manager_add(am, image_uiex_editor_TextItalic_dark);
  assets_manager_add(am, image_uiex_editor_TextCenteralignment_dark);
  assets_manager_add(am, image_uiex_button_time_dark);
  assets_manager_add(am, image_uiex_radio_n_dark);
  assets_manager_add(am, image_uiex_button_Empty_dark);
  assets_manager_add(am, image_uiex_editor_TextBold_dark);
  assets_manager_add(am, image_uiex_num9_dark);
  assets_manager_add(am, image_uiex_button_Import_dark);
  assets_manager_add(am, image_uiex_num8_dark);
  assets_manager_add(am, image_uiex_clock_Hourhand_dark);
  assets_manager_add(am, image_uiex_List_Nextpage_p_dark);
  assets_manager_add(am, image_uiex_shift_dark);
  assets_manager_add(am, image_ani6);
  assets_manager_add(am, image_checked);
  assets_manager_add(am, image_num_7);
  assets_manager_add(am, image_close_p);
  assets_manager_add(am, image_num_6);
  assets_manager_add(am, image_bg_landscape_1);
  assets_manager_add(am, image_shift);
  assets_manager_add(am, image_ani7);
  assets_manager_add(am, image_ania);
  assets_manager_add(am, image_anic);
  assets_manager_add(am, image_earth);
  assets_manager_add(am, image_ani5);
  assets_manager_add(am, image_flag_none);
  assets_manager_add(am, image_bee);
  assets_manager_add(am, image_debug_step_over_d);
  assets_manager_add(am, image_contact);
  assets_manager_add(am, image_bg_landscape_3);
  assets_manager_add(am, image_battery_4);
  assets_manager_add(am, image_num_4);
  assets_manager_add(am, image_slider_fg);
  assets_manager_add(am, image_contact_active);
  assets_manager_add(am, image_uiex_Configuration_nor);
  assets_manager_add(am, image_close_d);
  assets_manager_add(am, image_dialog_title);
  assets_manager_add(am, image_uiex_Boot_loading);
  assets_manager_add(am, image_battery_5);
  assets_manager_add(am, image_num_5);
  assets_manager_add(am, image_bg_landscape_2);
  assets_manager_add(am, image_ani4);
  assets_manager_add(am, image_anib);
  assets_manager_add(am, image_me_active);
  assets_manager_add(am, image_file);
  assets_manager_add(am, image_arrow_up_n);
  assets_manager_add(am, image_uiex_Universal_sel);
  assets_manager_add(am, image_cursor_hand);
  assets_manager_add(am, image_left_on);
  assets_manager_add(am, image_num_1);
  assets_manager_add(am, image_battery_1);
  assets_manager_add(am, image_arrow_right_p);
  assets_manager_add(am, image_slider_v_fg);
  assets_manager_add(am, image_empty);
  assets_manager_add(am, image_rgba);
  assets_manager_add(am, image_num_0);
  assets_manager_add(am, image_battery_0);
  assets_manager_add(am, image_edit_clear_p);
  assets_manager_add(am, image_arrow_up_o);
  assets_manager_add(am, image_visible);
  assets_manager_add(am, image_ani1);
  assets_manager_add(am, image_uiex_List_Mydevice);
  assets_manager_add(am, image_arrow_left_o);
  assets_manager_add(am, image_ani3);
  assets_manager_add(am, image_debug_pause_o);
  assets_manager_add(am, image_debug_stop_d);
  assets_manager_add(am, image_clock_second);
  assets_manager_add(am, image_debug_resume_n);
  assets_manager_add(am, image_debug_step_return_d);
  assets_manager_add(am, image_uiex_load2_loading);
  assets_manager_add(am, image_battery_2);
  assets_manager_add(am, image_num_2);
  assets_manager_add(am, image_uiex_Sliding_dot);
  assets_manager_add(am, image_uiex_List_password);
  assets_manager_add(am, image_en);
  assets_manager_add(am, image_slider_drag_o);
  assets_manager_add(am, image_battery_3);
  assets_manager_add(am, image_num_3);
  assets_manager_add(am, image_debug_resume_o);
  assets_manager_add(am, image_uiex_Coverwindow);
  assets_manager_add(am, image_debug_pause_n);
  assets_manager_add(am, image_ani2);
  assets_manager_add(am, image_arrow_left_n);
  assets_manager_add(am, image_middle_on);
  assets_manager_add(am, image_app3);
  assets_manager_add(am, image_bricks);
  assets_manager_add(am, image_image_packed_bg);
  assets_manager_add(am, image_msg);
  assets_manager_add(am, image_uiex_Windowclosed_n);
  assets_manager_add(am, image_play_o);
  assets_manager_add(am, image_play_n);
  assets_manager_add(am, image_uiex_load1_loading);
  assets_manager_add(am, image_msg_active);
  assets_manager_add(am, image_img_slider_bg);
  assets_manager_add(am, image_app2);
  assets_manager_add(am, image_radio_checked);
  assets_manager_add(am, image_active_dot);
  assets_manager_add(am, image_zh);
  assets_manager_add(am, image_question);
  assets_manager_add(am, image_discovery_active);
  assets_manager_add(am, image_dot);
  assets_manager_add(am, image_gauge_bg);
  assets_manager_add(am, image_shifton);
  assets_manager_add(am, image_app1);
  assets_manager_add(am, image_app5);
  assets_manager_add(am, image_check);
  assets_manager_add(am, image_uiex_Information_chat_bg);
  assets_manager_add(am, image_uiex_Sliding_Square);
  assets_manager_add(am, image_uiex_takeover_sel);
  assets_manager_add(am, image_cursor);
  assets_manager_add(am, image_uiex_send_nor);
  assets_manager_add(am, image_me);
  assets_manager_add(am, image_uiex_loading_repeat_bg);
  assets_manager_add(am, image_info);
  assets_manager_add(am, image_red_btn_p);
  assets_manager_add(am, image_app4);
  assets_manager_add(am, image_middle_off);
  assets_manager_add(am, image_debug_restart_d);
  assets_manager_add(am, image_img_slider_icon);
  assets_manager_add(am, image_arrow_down_n);
  assets_manager_add(am, image_debug_step_into_d);
  assets_manager_add(am, image_num_plus);
  assets_manager_add(am, image_return_up);
  assets_manager_add(am, image_green_btn_o);
  assets_manager_add(am, image_green_btn_n);
  assets_manager_add(am, image_debug_start_d);
  assets_manager_add(am, image_muted);
  assets_manager_add(am, image_uiex_List_Connectandshare);
  assets_manager_add(am, image_arrow_down_o);
  assets_manager_add(am, image_rgb);
  assets_manager_add(am, image_num_dot);
  assets_manager_add(am, image_unmuted);
  assets_manager_add(am, image_right_off);
  assets_manager_add(am, image_message);
  assets_manager_add(am, image_uiex_Windowclosed_p);
  assets_manager_add(am, image_uiex_Universal_nor);
  assets_manager_add(am, image_3);
  assets_manager_add(am, image_play_p);
  assets_manager_add(am, image_2);
  assets_manager_add(am, image_slider_bg);
  assets_manager_add(am, image_uiex_List_WLAN);
  assets_manager_add(am, image_right_on);
  assets_manager_add(am, image_gauge_pointer);
  assets_manager_add(am, image_bg2_s);
  assets_manager_add(am, image_flag_US);
  assets_manager_add(am, image_uiex_check);
  assets_manager_add(am, image_0);
  assets_manager_add(am, image_left_off);
  assets_manager_add(am, image_1);
  assets_manager_add(am, image_progress_circle);
  assets_manager_add(am, image_logo_dynamic);
  assets_manager_add(am, image_debug_restart_n);
  assets_manager_add(am, image_red_btn_o);
  assets_manager_add(am, image_debug_step_into_n);
  assets_manager_add(am, image_uiex_clock_Dots);
  assets_manager_add(am, image_slider_drag);
  assets_manager_add(am, image_num_minus);
  assets_manager_add(am, image_debug_start_o);
  assets_manager_add(am, image_bg_portrait_1);
  assets_manager_add(am, image_debug_start_n);
  assets_manager_add(am, image_bg4_s);
  assets_manager_add(am, image_cursor_text);
  assets_manager_add(am, image_unchecked);
  assets_manager_add(am, image_uiex_List_privacyprotection);
  assets_manager_add(am, image_debug_step_into_o);
  assets_manager_add(am, image_red_btn_n);
  assets_manager_add(am, image_debug_restart_o);
  assets_manager_add(am, image_uiex_Configuration_sel);
  assets_manager_add(am, image_arrow_down_p);
  assets_manager_add(am, image_slider_v_bg);
  assets_manager_add(am, image_warn);
  assets_manager_add(am, image_bg_portrait_3);
  assets_manager_add(am, image_green_btn_p);
  assets_manager_add(am, image_bg_portrait_2);
  assets_manager_add(am, image_flag_CN);
  assets_manager_add(am, image_repeat3);
  assets_manager_add(am, image_switch);
  assets_manager_add(am, image_radio_unchecked);
  assets_manager_add(am, image_bg1);
  assets_manager_add(am, image_uiex_List_sound);
  assets_manager_add(am, image_uiex_takeover_nor);
  assets_manager_add(am, image_debug_step_over_n);
  assets_manager_add(am, image_bg1_s);
  assets_manager_add(am, image_close_n);
  assets_manager_add(am, image_close_o);
  assets_manager_add(am, image_uiex_list_Battery);
  assets_manager_add(am, image_clock_bg);
  assets_manager_add(am, image_uiex_send_sel);
  assets_manager_add(am, image_clock_minute);
  assets_manager_add(am, image_debug_step_over_o);
  assets_manager_add(am, image_cross);
  assets_manager_add(am, image_repeat2);
  assets_manager_add(am, image_bg2);
  assets_manager_add(am, image_img_slider_fg);
  assets_manager_add(am, image_uiex_Information_avatar);
  assets_manager_add(am, image_clock);
  assets_manager_add(am, image_uiex_clock_Secondhand);
  assets_manager_add(am, image_bg3_s);
  assets_manager_add(am, image_image_packed_fg);
  assets_manager_add(am, image_bg3);
  assets_manager_add(am, image_repeat1);
  assets_manager_add(am, image_find);
  assets_manager_add(am, image_ani9);
  assets_manager_add(am, image_debug_stop_n);
  assets_manager_add(am, image_arrow_up_p);
  assets_manager_add(am, image_debug_step_return_n);
  assets_manager_add(am, image_edit_clear_o);
  assets_manager_add(am, image_folder);
  assets_manager_add(am, image_debug_resume_d);
  assets_manager_add(am, image_num_8);
  assets_manager_add(am, image_arrow_right_n);
  assets_manager_add(am, image_discovery);
  assets_manager_add(am, image_arrow_right_o);
  assets_manager_add(am, image_uiex_List_Moresettings);
  assets_manager_add(am, image_num_9);
  assets_manager_add(am, image_edit_clear_n);
  assets_manager_add(am, image_debug_step_return_o);
  assets_manager_add(am, image_uiex_List_show);
  assets_manager_add(am, image_debug_stop_o);
  assets_manager_add(am, image_ani8);
  assets_manager_add(am, image_debug_pause_d);
  assets_manager_add(am, image_repeat4);
  assets_manager_add(am, image_uiex_switch);
  assets_manager_add(am, image_bg4);
  assets_manager_add(am, image_logo);
  assets_manager_add(am, image_invisible);
  assets_manager_add(am, image_slider_drag_p);
  assets_manager_add(am, image_uiex_List_Bluetooth);
  assets_manager_add(am, image_backspace);
  assets_manager_add(am, image_uiex_List_desktop);
  assets_manager_add(am, image_bg5);
  assets_manager_add(am, image_arrow_left_p);
  assets_manager_add(am, image_clock_hour);
  assets_manager_add(am, image_bg5_s);
#ifdef WITH_VGCANVAS
  assets_manager_add(am, image_computer_dark);
  assets_manager_add(am, image_pointer_4);
  assets_manager_add(am, image_htest);
  assets_manager_add(am, image_ball);
  assets_manager_add(am, image_china);
  assets_manager_add(am, image_meter_pointer);
  assets_manager_add(am, image_pointer_1);
  assets_manager_add(am, image_pointer);
  assets_manager_add(am, image_girl);
  assets_manager_add(am, image_windmill);
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_trado);
  assets_manager_add(am, font_ap);
  assets_manager_add(am, font_default_full);
  assets_manager_add(am, font_default);
#else /*WITH_TRUETYPE_FONT*/
  assets_manager_add(am, font_default_20);
  assets_manager_add(am, font_default_16);
  assets_manager_add(am, font_default_18);
  assets_manager_add(am, font_default_96);
  assets_manager_add(am, font_default_32);
  assets_manager_add(am, font_default_24);
#endif /*WITH_TRUETYPE_FONT*/
  assets_manager_add(am, script_dummy);
#endif

  tk_init_assets();
  return RET_OK;
}
