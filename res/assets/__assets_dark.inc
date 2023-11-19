#include "awtk.h"
#include "base/assets_manager.h"
#if !defined(WITH_FS_RES) || defined(AWTK_WEB)
extern TK_CONST_DATA_ALIGN(const unsigned char strings_en_US_default[]);
#include "dark/inc/styles/uiex.data"
#include "dark/inc/styles/uiex_dialog_confirm.data"
#include "dark/inc/styles/uiex_dialog_info.data"
#include "dark/inc/styles/uiex_dialog_warn.data"
#include "dark/inc/styles/uiex_keyboard.data"
extern TK_CONST_DATA_ALIGN(const unsigned char style_default_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_toast_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dlg_center_scale_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dlg_fade_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dlg_popdown_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_dlg_popup_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_htranslate_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_ascii_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_default_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_foo_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_hex_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_int_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_kb_uint_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_bar_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_landscape_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_menu_portrait_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_overlay_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_animate_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_button_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_chart_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_checkbox_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_color_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_date_time_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_edit_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_image_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_label_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_layout_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_list_view_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_mledit_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_progress_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_selector_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_slider_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_slide_view_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_tab_ctrl_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_page_window_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_popup_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_preload_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_down_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_left_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_right_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_slide_up_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_sysbar_landscape_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_sysbar_portrait_default_uiex[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_vtranslate_default_uiex[]);
#ifndef AWTK_WEB
#ifdef WITH_STB_IMAGE
#else /*WITH_STB_IMAGE*/
#include "dark/inc/images/uiex_arrow_down_n.data"
#include "dark/inc/images/uiex_arrow_down_p.data"
#include "dark/inc/images/uiex_arrow_left_n.data"
#include "dark/inc/images/uiex_arrow_left_p.data"
#include "dark/inc/images/uiex_arrow_right_n.data"
#include "dark/inc/images/uiex_arrow_right_p.data"
#include "dark/inc/images/uiex_arrow_up_n.data"
#include "dark/inc/images/uiex_arrow_up_p.data"
#include "dark/inc/images/uiex_battery_0.data"
#include "dark/inc/images/uiex_battery_1.data"
#include "dark/inc/images/uiex_battery_2.data"
#include "dark/inc/images/uiex_battery_3.data"
#include "dark/inc/images/uiex_battery_4.data"
#include "dark/inc/images/uiex_battery_5.data"
#include "dark/inc/images/uiex_big_logo.data"
#include "dark/inc/images/uiex_button_Empty.data"
#include "dark/inc/images/uiex_button_Export.data"
#include "dark/inc/images/uiex_button_Import.data"
#include "dark/inc/images/uiex_button_search.data"
#include "dark/inc/images/uiex_button_time.data"
#include "dark/inc/images/uiex_check_checked_d.data"
#include "dark/inc/images/uiex_check_checked_n.data"
#include "dark/inc/images/uiex_check_checked_p.data"
#include "dark/inc/images/uiex_check_d.data"
#include "dark/inc/images/uiex_check_n.data"
#include "dark/inc/images/uiex_check_p.data"
#include "dark/inc/images/uiex_clock_bg.data"
#include "dark/inc/images/uiex_clock_Hourhand.data"
#include "dark/inc/images/uiex_clock_Minutehand.data"
#include "dark/inc/images/uiex_close_n.data"
#include "dark/inc/images/uiex_close_p.data"
#include "dark/inc/images/uiex_combobox_btn_d.data"
#include "dark/inc/images/uiex_combobox_btn_n.data"
#include "dark/inc/images/uiex_combobox_btn_p.data"
#include "dark/inc/images/uiex_editor_bgcolor.data"
#include "dark/inc/images/uiex_editor_Importpictures.data"
#include "dark/inc/images/uiex_editor_TextAlignBoth.data"
#include "dark/inc/images/uiex_editor_TextAlignleft.data"
#include "dark/inc/images/uiex_editor_TextAlignright.data"
#include "dark/inc/images/uiex_editor_TextBold.data"
#include "dark/inc/images/uiex_editor_TextCenteralignment.data"
#include "dark/inc/images/uiex_editor_Textcolor.data"
#include "dark/inc/images/uiex_editor_TextItalic.data"
#include "dark/inc/images/uiex_editor_TextLinespacing.data"
#include "dark/inc/images/uiex_editor_TextUnderscore.data"
#include "dark/inc/images/uiex_keyboard_backspace_n.data"
#include "dark/inc/images/uiex_keyboard_backspace_p.data"
#include "dark/inc/images/uiex_keyboard_CN.data"
#include "dark/inc/images/uiex_keyboard_EN.data"
#include "dark/inc/images/uiex_List_Nextpage_n.data"
#include "dark/inc/images/uiex_List_Nextpage_p.data"
#include "dark/inc/images/uiex_load1_bg.data"
#include "dark/inc/images/uiex_load2_bg.data"
#include "dark/inc/images/uiex_logo.data"
#include "dark/inc/images/uiex_num0.data"
#include "dark/inc/images/uiex_num1.data"
#include "dark/inc/images/uiex_num2.data"
#include "dark/inc/images/uiex_num3.data"
#include "dark/inc/images/uiex_num4.data"
#include "dark/inc/images/uiex_num5.data"
#include "dark/inc/images/uiex_num6.data"
#include "dark/inc/images/uiex_num7.data"
#include "dark/inc/images/uiex_num8.data"
#include "dark/inc/images/uiex_num9.data"
#include "dark/inc/images/uiex_numdot.data"
#include "dark/inc/images/uiex_Portraitmenu_n.data"
#include "dark/inc/images/uiex_Portraitmenu_p.data"
#include "dark/inc/images/uiex_radio_checked_d.data"
#include "dark/inc/images/uiex_radio_checked_n.data"
#include "dark/inc/images/uiex_radio_checked_p.data"
#include "dark/inc/images/uiex_radio_d.data"
#include "dark/inc/images/uiex_radio_n.data"
#include "dark/inc/images/uiex_radio_p.data"
#include "dark/inc/images/uiex_RecordForm_bg.data"
#include "dark/inc/images/uiex_shift.data"
#include "dark/inc/images/uiex_shifton.data"
#include "dark/inc/images/uiex_texteditorhide.data"
#include "dark/inc/images/uiex_texteditor_show.data"
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani6_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani7_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani8_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani9_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_pointer_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_dynamic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Dots_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Secondhand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_avatar_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_chat_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_list_Battery_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Bluetooth_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Connectandshare_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_desktop_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Moresettings_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Mydevice_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_password_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_privacyprotection_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_show_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_sound_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_WLAN_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load1_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load2_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p_default[]);
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_TRUETYPE_FONT
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*AWTK_WEB*/
#ifdef WITH_VGCANVAS
#include "dark/inc/images/uiex_arrow_down_n.data"
#include "dark/inc/images/uiex_arrow_down_p.data"
#include "dark/inc/images/uiex_arrow_left_n.data"
#include "dark/inc/images/uiex_arrow_left_p.data"
#include "dark/inc/images/uiex_arrow_right_n.data"
#include "dark/inc/images/uiex_arrow_right_p.data"
#include "dark/inc/images/uiex_arrow_up_n.data"
#include "dark/inc/images/uiex_arrow_up_p.data"
#include "dark/inc/images/uiex_battery_0.data"
#include "dark/inc/images/uiex_battery_1.data"
#include "dark/inc/images/uiex_battery_2.data"
#include "dark/inc/images/uiex_battery_3.data"
#include "dark/inc/images/uiex_battery_4.data"
#include "dark/inc/images/uiex_battery_5.data"
#include "dark/inc/images/uiex_big_logo.data"
#include "dark/inc/images/uiex_button_Empty.data"
#include "dark/inc/images/uiex_button_Export.data"
#include "dark/inc/images/uiex_button_Import.data"
#include "dark/inc/images/uiex_button_search.data"
#include "dark/inc/images/uiex_button_time.data"
#include "dark/inc/images/uiex_check_checked_d.data"
#include "dark/inc/images/uiex_check_checked_n.data"
#include "dark/inc/images/uiex_check_checked_p.data"
#include "dark/inc/images/uiex_check_d.data"
#include "dark/inc/images/uiex_check_n.data"
#include "dark/inc/images/uiex_check_p.data"
#include "dark/inc/images/uiex_clock_bg.data"
#include "dark/inc/images/uiex_clock_Hourhand.data"
#include "dark/inc/images/uiex_clock_Minutehand.data"
#include "dark/inc/images/uiex_close_n.data"
#include "dark/inc/images/uiex_close_p.data"
#include "dark/inc/images/uiex_combobox_btn_d.data"
#include "dark/inc/images/uiex_combobox_btn_n.data"
#include "dark/inc/images/uiex_combobox_btn_p.data"
#include "dark/inc/images/uiex_editor_bgcolor.data"
#include "dark/inc/images/uiex_editor_Importpictures.data"
#include "dark/inc/images/uiex_editor_TextAlignBoth.data"
#include "dark/inc/images/uiex_editor_TextAlignleft.data"
#include "dark/inc/images/uiex_editor_TextAlignright.data"
#include "dark/inc/images/uiex_editor_TextBold.data"
#include "dark/inc/images/uiex_editor_TextCenteralignment.data"
#include "dark/inc/images/uiex_editor_Textcolor.data"
#include "dark/inc/images/uiex_editor_TextItalic.data"
#include "dark/inc/images/uiex_editor_TextLinespacing.data"
#include "dark/inc/images/uiex_editor_TextUnderscore.data"
#include "dark/inc/images/uiex_keyboard_backspace_n.data"
#include "dark/inc/images/uiex_keyboard_backspace_p.data"
#include "dark/inc/images/uiex_keyboard_CN.data"
#include "dark/inc/images/uiex_keyboard_EN.data"
#include "dark/inc/images/uiex_List_Nextpage_n.data"
#include "dark/inc/images/uiex_List_Nextpage_p.data"
#include "dark/inc/images/uiex_load1_bg.data"
#include "dark/inc/images/uiex_load2_bg.data"
#include "dark/inc/images/uiex_logo.data"
#include "dark/inc/images/uiex_num0.data"
#include "dark/inc/images/uiex_num1.data"
#include "dark/inc/images/uiex_num2.data"
#include "dark/inc/images/uiex_num3.data"
#include "dark/inc/images/uiex_num4.data"
#include "dark/inc/images/uiex_num5.data"
#include "dark/inc/images/uiex_num6.data"
#include "dark/inc/images/uiex_num7.data"
#include "dark/inc/images/uiex_num8.data"
#include "dark/inc/images/uiex_num9.data"
#include "dark/inc/images/uiex_numdot.data"
#include "dark/inc/images/uiex_Portraitmenu_n.data"
#include "dark/inc/images/uiex_Portraitmenu_p.data"
#include "dark/inc/images/uiex_radio_checked_d.data"
#include "dark/inc/images/uiex_radio_checked_n.data"
#include "dark/inc/images/uiex_radio_checked_p.data"
#include "dark/inc/images/uiex_radio_d.data"
#include "dark/inc/images/uiex_radio_n.data"
#include "dark/inc/images/uiex_radio_p.data"
#include "dark/inc/images/uiex_RecordForm_bg.data"
#include "dark/inc/images/uiex_shift.data"
#include "dark/inc/images/uiex_shifton.data"
#include "dark/inc/images/uiex_texteditorhide.data"
#include "dark/inc/images/uiex_texteditor_show.data"
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani6_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani7_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani8_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani9_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_s_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_d_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_pointer_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_dynamic_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat3_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat4_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Dots_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Secondhand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_avatar_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_chat_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_list_Battery_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Bluetooth_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Connectandshare_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_desktop_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Moresettings_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Mydevice_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_password_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_privacyprotection_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_show_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_sound_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_WLAN_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load1_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load2_loading_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p_default[]);
#endif /*WITH_VGCANVAS*/
#endif /*!defined(WITH_FS_RES) || defined(AWTK_WEB)*/

ret_t assets_init_dark(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "dark");

#if defined(WITH_FS_RES) && !defined(AWTK_WEB)
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else /*defined(WITH_FS_RES) && !defined(AWTK_WEB)*/
  assets_manager_add(am, strings_en_US_default);
  assets_manager_add(am, style_uiex_dark_dark);
  assets_manager_add(am, style_uiex_dialog_confirm_dark_dark);
  assets_manager_add(am, style_uiex_dialog_info_dark_dark);
  assets_manager_add(am, style_uiex_dialog_warn_dark_dark);
  assets_manager_add(am, style_uiex_keyboard_dark_dark);
  assets_manager_add(am, style_default_default);
  assets_manager_add(am, style_dialog_toast_default);
  assets_manager_add(am, ui_dlg_center_scale_default_uiex);
  assets_manager_add(am, ui_dlg_fade_default_uiex);
  assets_manager_add(am, ui_dlg_popdown_default_uiex);
  assets_manager_add(am, ui_dlg_popup_default_uiex);
  assets_manager_add(am, ui_htranslate_default_uiex);
  assets_manager_add(am, ui_kb_ascii_default_uiex);
  assets_manager_add(am, ui_kb_default_default_uiex);
  assets_manager_add(am, ui_kb_foo_default_uiex);
  assets_manager_add(am, ui_kb_hex_default_uiex);
  assets_manager_add(am, ui_kb_int_default_uiex);
  assets_manager_add(am, ui_kb_uint_default_uiex);
  assets_manager_add(am, ui_menu_bar_default_uiex);
  assets_manager_add(am, ui_menu_landscape_default_uiex);
  assets_manager_add(am, ui_menu_portrait_default_uiex);
  assets_manager_add(am, ui_overlay_default_uiex);
  assets_manager_add(am, ui_page_animate_default_uiex);
  assets_manager_add(am, ui_page_button_default_uiex);
  assets_manager_add(am, ui_page_chart_default_uiex);
  assets_manager_add(am, ui_page_checkbox_default_uiex);
  assets_manager_add(am, ui_page_color_default_uiex);
  assets_manager_add(am, ui_page_date_time_default_uiex);
  assets_manager_add(am, ui_page_edit_default_uiex);
  assets_manager_add(am, ui_page_image_default_uiex);
  assets_manager_add(am, ui_page_label_default_uiex);
  assets_manager_add(am, ui_page_layout_default_uiex);
  assets_manager_add(am, ui_page_list_view_default_uiex);
  assets_manager_add(am, ui_page_mledit_default_uiex);
  assets_manager_add(am, ui_page_progress_default_uiex);
  assets_manager_add(am, ui_page_selector_default_uiex);
  assets_manager_add(am, ui_page_slider_default_uiex);
  assets_manager_add(am, ui_page_slide_view_default_uiex);
  assets_manager_add(am, ui_page_tab_ctrl_default_uiex);
  assets_manager_add(am, ui_page_window_default_uiex);
  assets_manager_add(am, ui_popup_default_uiex);
  assets_manager_add(am, ui_preload_default_uiex);
  assets_manager_add(am, ui_slide_down_default_uiex);
  assets_manager_add(am, ui_slide_left_default_uiex);
  assets_manager_add(am, ui_slide_right_default_uiex);
  assets_manager_add(am, ui_slide_up_default_uiex);
  assets_manager_add(am, ui_sysbar_landscape_default_uiex);
  assets_manager_add(am, ui_sysbar_portrait_default_uiex);
  assets_manager_add(am, ui_vtranslate_default_uiex);
#ifndef AWTK_WEB
#ifdef WITH_TRUETYPE_FONT
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*AWTK_WEB*/
#ifdef WITH_VGCANVAS
  assets_manager_add(am, image_uiex_arrow_down_n_dark_dark);
  assets_manager_add(am, image_uiex_arrow_down_p_dark_dark);
  assets_manager_add(am, image_uiex_arrow_left_n_dark_dark);
  assets_manager_add(am, image_uiex_arrow_left_p_dark_dark);
  assets_manager_add(am, image_uiex_arrow_right_n_dark_dark);
  assets_manager_add(am, image_uiex_arrow_right_p_dark_dark);
  assets_manager_add(am, image_uiex_arrow_up_n_dark_dark);
  assets_manager_add(am, image_uiex_arrow_up_p_dark_dark);
  assets_manager_add(am, image_uiex_battery_0_dark_dark);
  assets_manager_add(am, image_uiex_battery_1_dark_dark);
  assets_manager_add(am, image_uiex_battery_2_dark_dark);
  assets_manager_add(am, image_uiex_battery_3_dark_dark);
  assets_manager_add(am, image_uiex_battery_4_dark_dark);
  assets_manager_add(am, image_uiex_battery_5_dark_dark);
  assets_manager_add(am, image_uiex_big_logo_dark_dark);
  assets_manager_add(am, image_uiex_button_Empty_dark_dark);
  assets_manager_add(am, image_uiex_button_Export_dark_dark);
  assets_manager_add(am, image_uiex_button_Import_dark_dark);
  assets_manager_add(am, image_uiex_button_search_dark_dark);
  assets_manager_add(am, image_uiex_button_time_dark_dark);
  assets_manager_add(am, image_uiex_check_checked_d_dark_dark);
  assets_manager_add(am, image_uiex_check_checked_n_dark_dark);
  assets_manager_add(am, image_uiex_check_checked_p_dark_dark);
  assets_manager_add(am, image_uiex_check_d_dark_dark);
  assets_manager_add(am, image_uiex_check_n_dark_dark);
  assets_manager_add(am, image_uiex_check_p_dark_dark);
  assets_manager_add(am, image_uiex_clock_bg_dark_dark);
  assets_manager_add(am, image_uiex_clock_Hourhand_dark_dark);
  assets_manager_add(am, image_uiex_clock_Minutehand_dark_dark);
  assets_manager_add(am, image_uiex_close_n_dark_dark);
  assets_manager_add(am, image_uiex_close_p_dark_dark);
  assets_manager_add(am, image_uiex_combobox_btn_d_dark_dark);
  assets_manager_add(am, image_uiex_combobox_btn_n_dark_dark);
  assets_manager_add(am, image_uiex_combobox_btn_p_dark_dark);
  assets_manager_add(am, image_uiex_editor_bgcolor_dark_dark);
  assets_manager_add(am, image_uiex_editor_Importpictures_dark_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignBoth_dark_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignleft_dark_dark);
  assets_manager_add(am, image_uiex_editor_TextAlignright_dark_dark);
  assets_manager_add(am, image_uiex_editor_TextBold_dark_dark);
  assets_manager_add(am, image_uiex_editor_TextCenteralignment_dark_dark);
  assets_manager_add(am, image_uiex_editor_Textcolor_dark_dark);
  assets_manager_add(am, image_uiex_editor_TextItalic_dark_dark);
  assets_manager_add(am, image_uiex_editor_TextLinespacing_dark_dark);
  assets_manager_add(am, image_uiex_editor_TextUnderscore_dark_dark);
  assets_manager_add(am, image_uiex_keyboard_backspace_n_dark_dark);
  assets_manager_add(am, image_uiex_keyboard_backspace_p_dark_dark);
  assets_manager_add(am, image_uiex_keyboard_CN_dark_dark);
  assets_manager_add(am, image_uiex_keyboard_EN_dark_dark);
  assets_manager_add(am, image_uiex_List_Nextpage_n_dark_dark);
  assets_manager_add(am, image_uiex_List_Nextpage_p_dark_dark);
  assets_manager_add(am, image_uiex_load1_bg_dark_dark);
  assets_manager_add(am, image_uiex_load2_bg_dark_dark);
  assets_manager_add(am, image_uiex_logo_dark_dark);
  assets_manager_add(am, image_uiex_num0_dark_dark);
  assets_manager_add(am, image_uiex_num1_dark_dark);
  assets_manager_add(am, image_uiex_num2_dark_dark);
  assets_manager_add(am, image_uiex_num3_dark_dark);
  assets_manager_add(am, image_uiex_num4_dark_dark);
  assets_manager_add(am, image_uiex_num5_dark_dark);
  assets_manager_add(am, image_uiex_num6_dark_dark);
  assets_manager_add(am, image_uiex_num7_dark_dark);
  assets_manager_add(am, image_uiex_num8_dark_dark);
  assets_manager_add(am, image_uiex_num9_dark_dark);
  assets_manager_add(am, image_uiex_numdot_dark_dark);
  assets_manager_add(am, image_uiex_Portraitmenu_n_dark_dark);
  assets_manager_add(am, image_uiex_Portraitmenu_p_dark_dark);
  assets_manager_add(am, image_uiex_radio_checked_d_dark_dark);
  assets_manager_add(am, image_uiex_radio_checked_n_dark_dark);
  assets_manager_add(am, image_uiex_radio_checked_p_dark_dark);
  assets_manager_add(am, image_uiex_radio_d_dark_dark);
  assets_manager_add(am, image_uiex_radio_n_dark_dark);
  assets_manager_add(am, image_uiex_radio_p_dark_dark);
  assets_manager_add(am, image_uiex_RecordForm_bg_dark_dark);
  assets_manager_add(am, image_uiex_shift_dark_dark);
  assets_manager_add(am, image_uiex_shifton_dark_dark);
  assets_manager_add(am, image_uiex_texteditorhide_dark_dark);
  assets_manager_add(am, image_uiex_texteditor_show_dark_dark);
  assets_manager_add(am, image_ani1_default);
  assets_manager_add(am, image_ani2_default);
  assets_manager_add(am, image_ani3_default);
  assets_manager_add(am, image_ani4_default);
  assets_manager_add(am, image_ani5_default);
  assets_manager_add(am, image_ani6_default);
  assets_manager_add(am, image_ani7_default);
  assets_manager_add(am, image_ani8_default);
  assets_manager_add(am, image_ani9_default);
  assets_manager_add(am, image_app1_default);
  assets_manager_add(am, image_app2_default);
  assets_manager_add(am, image_app3_default);
  assets_manager_add(am, image_app4_default);
  assets_manager_add(am, image_app5_default);
  assets_manager_add(am, image_bg1_default);
  assets_manager_add(am, image_bg1_s_default);
  assets_manager_add(am, image_bg2_default);
  assets_manager_add(am, image_bg2_s_default);
  assets_manager_add(am, image_bg3_default);
  assets_manager_add(am, image_bg3_s_default);
  assets_manager_add(am, image_bg4_default);
  assets_manager_add(am, image_bg4_s_default);
  assets_manager_add(am, image_bg5_default);
  assets_manager_add(am, image_bg5_s_default);
  assets_manager_add(am, image_debug_pause_d_default);
  assets_manager_add(am, image_debug_pause_n_default);
  assets_manager_add(am, image_debug_pause_o_default);
  assets_manager_add(am, image_debug_restart_d_default);
  assets_manager_add(am, image_debug_restart_n_default);
  assets_manager_add(am, image_debug_restart_o_default);
  assets_manager_add(am, image_debug_resume_d_default);
  assets_manager_add(am, image_debug_resume_n_default);
  assets_manager_add(am, image_debug_resume_o_default);
  assets_manager_add(am, image_debug_start_d_default);
  assets_manager_add(am, image_debug_start_n_default);
  assets_manager_add(am, image_debug_start_o_default);
  assets_manager_add(am, image_debug_step_into_d_default);
  assets_manager_add(am, image_debug_step_into_n_default);
  assets_manager_add(am, image_debug_step_into_o_default);
  assets_manager_add(am, image_debug_step_over_d_default);
  assets_manager_add(am, image_debug_step_over_n_default);
  assets_manager_add(am, image_debug_step_over_o_default);
  assets_manager_add(am, image_debug_step_return_d_default);
  assets_manager_add(am, image_debug_step_return_n_default);
  assets_manager_add(am, image_debug_step_return_o_default);
  assets_manager_add(am, image_debug_stop_d_default);
  assets_manager_add(am, image_debug_stop_n_default);
  assets_manager_add(am, image_debug_stop_o_default);
  assets_manager_add(am, image_gauge_pointer_default);
  assets_manager_add(am, image_logo_dynamic_default);
  assets_manager_add(am, image_repeat1_default);
  assets_manager_add(am, image_repeat2_default);
  assets_manager_add(am, image_repeat3_default);
  assets_manager_add(am, image_repeat4_default);
  assets_manager_add(am, image_uiex_Boot_loading_default);
  assets_manager_add(am, image_uiex_check_default);
  assets_manager_add(am, image_uiex_clock_Dots_default);
  assets_manager_add(am, image_uiex_clock_Secondhand_default);
  assets_manager_add(am, image_uiex_Configuration_nor_default);
  assets_manager_add(am, image_uiex_Configuration_sel_default);
  assets_manager_add(am, image_uiex_Coverwindow_default);
  assets_manager_add(am, image_uiex_Information_avatar_default);
  assets_manager_add(am, image_uiex_Information_chat_bg_default);
  assets_manager_add(am, image_uiex_list_Battery_default);
  assets_manager_add(am, image_uiex_List_Bluetooth_default);
  assets_manager_add(am, image_uiex_List_Connectandshare_default);
  assets_manager_add(am, image_uiex_List_desktop_default);
  assets_manager_add(am, image_uiex_List_Moresettings_default);
  assets_manager_add(am, image_uiex_List_Mydevice_default);
  assets_manager_add(am, image_uiex_List_password_default);
  assets_manager_add(am, image_uiex_List_privacyprotection_default);
  assets_manager_add(am, image_uiex_List_show_default);
  assets_manager_add(am, image_uiex_List_sound_default);
  assets_manager_add(am, image_uiex_List_WLAN_default);
  assets_manager_add(am, image_uiex_load1_loading_default);
  assets_manager_add(am, image_uiex_load2_loading_default);
  assets_manager_add(am, image_uiex_loading_repeat_bg_default);
  assets_manager_add(am, image_uiex_send_nor_default);
  assets_manager_add(am, image_uiex_send_sel_default);
  assets_manager_add(am, image_uiex_Sliding_dot_default);
  assets_manager_add(am, image_uiex_Sliding_Square_default);
  assets_manager_add(am, image_uiex_switch_default);
  assets_manager_add(am, image_uiex_takeover_nor_default);
  assets_manager_add(am, image_uiex_takeover_sel_default);
  assets_manager_add(am, image_uiex_Universal_nor_default);
  assets_manager_add(am, image_uiex_Universal_sel_default);
  assets_manager_add(am, image_uiex_Windowclosed_n_default);
  assets_manager_add(am, image_uiex_Windowclosed_p_default);
#endif /*WITH_VGCANVAS*/
#endif /*defined(WITH_FS_RES) && !defined(AWTK_WEB)*/

  tk_init_assets();
  return RET_OK;
}
