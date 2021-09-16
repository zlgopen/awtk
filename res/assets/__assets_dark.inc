#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
extern TK_CONST_DATA_ALIGN(const unsigned char strings_en_US[]);
#include "dark/inc/styles/uiex_dialog_warn.data"
#include "dark/inc/styles/uiex_keyboard.data"
#include "dark/inc/styles/uiex_dialog_info.data"
#include "dark/inc/styles/uiex.data"
#include "dark/inc/styles/uiex_dialog_confirm.data"
extern TK_CONST_DATA_ALIGN(const unsigned char style_dialog_toast[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_default[]);
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
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani7[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Mydevice[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load2_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_password[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load1_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_chat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Connectandshare[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_WLAN[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_pointer[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_dynamic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Dots[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_privacyprotection[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_sound[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_list_Battery[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_avatar[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Secondhand[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani9[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Moresettings[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_show[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani8[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Bluetooth[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_desktop[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5[]);
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
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_p[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_logo_dynamic[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_switch[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_WLAN[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_avatar[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load1_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Connectandshare[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_list_Battery[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_loading_repeat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app3[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Information_chat_bg[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani9[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg3_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_send_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_over_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_sound[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Secondhand[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Mydevice[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_restart_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani5[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_stop_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Windowclosed_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_into_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani8[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Configuration_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_desktop[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app2[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_takeover_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_show[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat4[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_app1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg4_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani7[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_clock_Dots[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_privacyprotection[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Coverwindow[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani6[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_nor[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Boot_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_ani1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_step_return_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_bg2_s[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_repeat1[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_o[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_password[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Bluetooth[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_load2_loading[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_resume_n[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Universal_sel[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_start_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_gauge_pointer[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_debug_pause_d[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_Square[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_List_Moresettings[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_Sliding_dot[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_uiex_check[]);
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_VGCANVAS
#include "dark/inc/images/computer.bsvg"
extern TK_CONST_DATA_ALIGN(const unsigned char image_windmill[]);
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
extern TK_CONST_DATA_ALIGN(const unsigned char font_default[]);
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*WITH_FS_RES*/

ret_t assets_init_dark(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "dark");

#ifdef WITH_FS_RES
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else
  assets_manager_add(am, strings_en_US);
  assets_manager_add(am, style_uiex_dialog_warn_dark);
  assets_manager_add(am, style_uiex_keyboard_dark);
  assets_manager_add(am, style_uiex_dialog_info_dark);
  assets_manager_add(am, style_uiex_dark);
  assets_manager_add(am, style_uiex_dialog_confirm_dark);
  assets_manager_add(am, style_dialog_toast);
  assets_manager_add(am, style_default);
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
  assets_manager_add(am, image_ani7);
  assets_manager_add(am, image_ani5);
  assets_manager_add(am, image_debug_step_over_d);
  assets_manager_add(am, image_uiex_Configuration_nor);
  assets_manager_add(am, image_uiex_Boot_loading);
  assets_manager_add(am, image_ani4);
  assets_manager_add(am, image_uiex_Universal_sel);
  assets_manager_add(am, image_ani1);
  assets_manager_add(am, image_uiex_List_Mydevice);
  assets_manager_add(am, image_ani3);
  assets_manager_add(am, image_debug_pause_o);
  assets_manager_add(am, image_debug_stop_d);
  assets_manager_add(am, image_debug_resume_n);
  assets_manager_add(am, image_debug_step_return_d);
  assets_manager_add(am, image_uiex_load2_loading);
  assets_manager_add(am, image_uiex_Sliding_dot);
  assets_manager_add(am, image_uiex_List_password);
  assets_manager_add(am, image_debug_resume_o);
  assets_manager_add(am, image_uiex_Coverwindow);
  assets_manager_add(am, image_debug_pause_n);
  assets_manager_add(am, image_ani2);
  assets_manager_add(am, image_app3);
  assets_manager_add(am, image_uiex_Windowclosed_n);
  assets_manager_add(am, image_uiex_load1_loading);
  assets_manager_add(am, image_app2);
  assets_manager_add(am, image_app1);
  assets_manager_add(am, image_app5);
  assets_manager_add(am, image_uiex_Information_chat_bg);
  assets_manager_add(am, image_uiex_Sliding_Square);
  assets_manager_add(am, image_uiex_takeover_sel);
  assets_manager_add(am, image_uiex_send_nor);
  assets_manager_add(am, image_uiex_loading_repeat_bg);
  assets_manager_add(am, image_app4);
  assets_manager_add(am, image_debug_restart_d);
  assets_manager_add(am, image_debug_step_into_d);
  assets_manager_add(am, image_debug_start_d);
  assets_manager_add(am, image_uiex_List_Connectandshare);
  assets_manager_add(am, image_uiex_Windowclosed_p);
  assets_manager_add(am, image_uiex_Universal_nor);
  assets_manager_add(am, image_uiex_List_WLAN);
  assets_manager_add(am, image_gauge_pointer);
  assets_manager_add(am, image_bg2_s);
  assets_manager_add(am, image_uiex_check);
  assets_manager_add(am, image_logo_dynamic);
  assets_manager_add(am, image_debug_restart_n);
  assets_manager_add(am, image_debug_step_into_n);
  assets_manager_add(am, image_uiex_clock_Dots);
  assets_manager_add(am, image_debug_start_o);
  assets_manager_add(am, image_debug_start_n);
  assets_manager_add(am, image_bg4_s);
  assets_manager_add(am, image_uiex_List_privacyprotection);
  assets_manager_add(am, image_debug_step_into_o);
  assets_manager_add(am, image_debug_restart_o);
  assets_manager_add(am, image_uiex_Configuration_sel);
  assets_manager_add(am, image_repeat3);
  assets_manager_add(am, image_bg1);
  assets_manager_add(am, image_uiex_List_sound);
  assets_manager_add(am, image_uiex_takeover_nor);
  assets_manager_add(am, image_debug_step_over_n);
  assets_manager_add(am, image_bg1_s);
  assets_manager_add(am, image_uiex_list_Battery);
  assets_manager_add(am, image_uiex_send_sel);
  assets_manager_add(am, image_debug_step_over_o);
  assets_manager_add(am, image_repeat2);
  assets_manager_add(am, image_bg2);
  assets_manager_add(am, image_uiex_Information_avatar);
  assets_manager_add(am, image_uiex_clock_Secondhand);
  assets_manager_add(am, image_bg3_s);
  assets_manager_add(am, image_bg3);
  assets_manager_add(am, image_repeat1);
  assets_manager_add(am, image_ani9);
  assets_manager_add(am, image_debug_stop_n);
  assets_manager_add(am, image_debug_step_return_n);
  assets_manager_add(am, image_debug_resume_d);
  assets_manager_add(am, image_uiex_List_Moresettings);
  assets_manager_add(am, image_debug_step_return_o);
  assets_manager_add(am, image_uiex_List_show);
  assets_manager_add(am, image_debug_stop_o);
  assets_manager_add(am, image_ani8);
  assets_manager_add(am, image_debug_pause_d);
  assets_manager_add(am, image_repeat4);
  assets_manager_add(am, image_uiex_switch);
  assets_manager_add(am, image_bg4);
  assets_manager_add(am, image_uiex_List_Bluetooth);
  assets_manager_add(am, image_uiex_List_desktop);
  assets_manager_add(am, image_bg5);
  assets_manager_add(am, image_bg5_s);
#ifdef WITH_VGCANVAS
  assets_manager_add(am, image_computer_dark);
  assets_manager_add(am, image_windmill);
#endif /*WITH_VGCANVAS*/
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_default);
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif

  tk_init_assets();
  return RET_OK;
}
