#include "awtk.h"
#include "base/dialog.h"
#include "base/idle.h"
#include "base/window.h"
#include "base/window_manager.h"
#include "gtest/gtest.h"
#include "widgets/combo_box.h"
#include "widgets/overlay.h"
#include "widgets/popup.h"

typedef struct _fg_bg_counter_t {
  uint32_t to_foreground;
  uint32_t to_background;
} fg_bg_counter_t;

static ret_t on_to_foreground(void* ctx, event_t* e) {
  fg_bg_counter_t* counter = (fg_bg_counter_t*)ctx;
  (void)e;
  counter->to_foreground++;
  return RET_OK;
}

static ret_t on_to_background(void* ctx, event_t* e) {
  fg_bg_counter_t* counter = (fg_bg_counter_t*)ctx;
  (void)e;
  counter->to_background++;
  return RET_OK;
}

static void fg_bg_counter_attach(widget_t* win, fg_bg_counter_t* counter) {
  widget_on(win, EVT_WINDOW_TO_FOREGROUND, on_to_foreground, counter);
  widget_on(win, EVT_WINDOW_TO_BACKGROUND, on_to_background, counter);
}

static void fg_bg_counter_reset(fg_bg_counter_t* counter) {
  counter->to_foreground = 0;
  counter->to_background = 0;
}

static void drain_idle(void) {
  idle_dispatch();
}

static void close_all_windows(void) {
  window_manager_close_all(window_manager());
  drain_idle();
}

TEST(WindowForegroundEvent, should_dispatch) {
  widget_t* normal_win = window_create(NULL, 0, 0, 100, 100);
  widget_t* dialog_win = dialog_create(NULL, 0, 0, 100, 100);
  widget_t* toast_win = dialog_create(NULL, 0, 0, 100, 100);
  widget_t* popup_win = popup_create(NULL, 0, 0, 100, 100);
  widget_t* overlay_win = overlay_create(NULL, 0, 0, 100, 100);

  widget_set_name(toast_win, DIALOG_TOAST_WIDGET_NAME);
  widget_set_prop_str(toast_win, WIDGET_PROP_THEME, DIALOG_TOAST_THEME_NAME);

  ASSERT_EQ(widget_is_toast_dialog(toast_win), TRUE);
  ASSERT_EQ(widget_should_dispatch_window_foreground_event(normal_win), TRUE);
  ASSERT_EQ(widget_should_dispatch_window_foreground_event(dialog_win), TRUE);
  ASSERT_EQ(widget_should_dispatch_window_foreground_event(toast_win), FALSE);
  ASSERT_EQ(widget_should_dispatch_window_foreground_event(popup_win), FALSE);
  ASSERT_EQ(widget_should_dispatch_window_foreground_event(overlay_win), FALSE);
  ASSERT_EQ(widget_can_receive_window_foreground_event(normal_win), TRUE);
  ASSERT_EQ(widget_can_receive_window_foreground_event(dialog_win), TRUE);
  ASSERT_EQ(widget_can_receive_window_foreground_event(toast_win), FALSE);
  ASSERT_EQ(widget_can_receive_window_foreground_event(popup_win), FALSE);
  ASSERT_EQ(widget_can_receive_window_foreground_event(overlay_win), FALSE);
  ASSERT_EQ(widget_should_dispatch_window_foreground_event(NULL), FALSE);
  ASSERT_EQ(widget_can_receive_window_foreground_event(NULL), FALSE);

  window_close_force(normal_win);
  window_close_force(dialog_win);
  window_close_force(toast_win);
  window_close_force(popup_win);
  window_close_force(overlay_win);
  drain_idle();
}

TEST(WindowForegroundEvent, dispatch_helper) {
  widget_t* normal_win = window_create(NULL, 0, 0, 100, 100);
  widget_t* dialog_win = dialog_create(NULL, 0, 0, 100, 100);
  widget_t* popup_win = popup_create(NULL, 0, 0, 100, 100);
  widget_t* overlay_win = overlay_create(NULL, 0, 0, 100, 100);
  fg_bg_counter_t normal_counter = {0, 0};
  fg_bg_counter_t dialog_counter = {0, 0};
  fg_bg_counter_t overlay_counter = {0, 0};

  fg_bg_counter_attach(normal_win, &normal_counter);
  fg_bg_counter_attach(dialog_win, &dialog_counter);
  fg_bg_counter_attach(overlay_win, &overlay_counter);

  ASSERT_EQ(window_manager_dispatch_window_foreground_events(popup_win, normal_win, dialog_win),
            RET_OK);
  ASSERT_EQ(normal_counter.to_background, 0u);
  ASSERT_EQ(normal_counter.to_foreground, 0u);
  ASSERT_EQ(dialog_counter.to_background, 0u);
  ASSERT_EQ(dialog_counter.to_foreground, 0u);

  ASSERT_EQ(window_manager_dispatch_window_foreground_events(normal_win, dialog_win, normal_win),
            RET_OK);
  ASSERT_EQ(dialog_counter.to_background, 1u);
  ASSERT_EQ(normal_counter.to_foreground, 1u);

  fg_bg_counter_reset(&overlay_counter);
  {
    uint32_t dialog_fg_before = dialog_counter.to_foreground;
    ASSERT_EQ(window_manager_dispatch_window_foreground_events(dialog_win, overlay_win, dialog_win),
              RET_OK);
    ASSERT_EQ(overlay_counter.to_background, 0u);
    ASSERT_EQ(overlay_counter.to_foreground, 0u);
    ASSERT_EQ(dialog_counter.to_foreground, dialog_fg_before + 1);
  }

  window_close_force(popup_win);
  window_close_force(overlay_win);
  window_close_force(dialog_win);
  window_close_force(normal_win);
  drain_idle();
}

TEST(WindowForegroundEvent, dispatch_helper_fallback_for_ineligible_receiver) {
  widget_t* normal_win = window_create(NULL, 0, 0, 100, 100);
  widget_t* dialog_win = dialog_create(NULL, 0, 0, 100, 100);
  widget_t* overlay_win = overlay_create(NULL, 0, 0, 100, 100);
  fg_bg_counter_t overlay_counter = {0, 0};

  widget_set_prop_int(overlay_win, WIDGET_PROP_STAGE, WINDOW_STAGE_OPENED);
  fg_bg_counter_attach(overlay_win, &overlay_counter);

  ASSERT_EQ(window_manager_dispatch_window_foreground_events(dialog_win, overlay_win, dialog_win),
            RET_OK);
  ASSERT_EQ(overlay_counter.to_background, 0u);
  ASSERT_EQ(overlay_counter.to_foreground, 0u);
  ASSERT_EQ(widget_get_prop_int(overlay_win, WIDGET_PROP_STAGE, WINDOW_STAGE_NONE),
            WINDOW_STAGE_SUSPEND);

  widget_set_prop_int(overlay_win, WIDGET_PROP_STAGE, WINDOW_STAGE_SUSPEND);
  fg_bg_counter_reset(&overlay_counter);
  ASSERT_EQ(window_manager_dispatch_window_foreground_events(normal_win, dialog_win, overlay_win),
            RET_OK);
  ASSERT_EQ(overlay_counter.to_foreground, 0u);
  ASSERT_EQ(widget_get_prop_int(overlay_win, WIDGET_PROP_STAGE, WINDOW_STAGE_NONE),
            WINDOW_STAGE_OPENED);

  window_close_force(overlay_win);
  window_close_force(dialog_win);
  window_close_force(normal_win);
  drain_idle();
}

TEST(WindowForegroundEvent, open_popup) {
  fg_bg_counter_t counter = {0, 0};
  widget_t* main_win = NULL;
  widget_t* popup_win = NULL;

  close_all_windows();
  main_win = window_create(NULL, 0, 0, 400, 300);
  drain_idle();
  fg_bg_counter_attach(main_win, &counter);

  popup_win = popup_create(NULL, 0, 0, 120, 160);
  drain_idle();

  ASSERT_EQ(counter.to_background, 0u);
  ASSERT_EQ(counter.to_foreground, 0u);

  window_close_force(popup_win);
  drain_idle();

  ASSERT_EQ(counter.to_background, 0u);
  ASSERT_EQ(counter.to_foreground, 0u);

  window_close_force(main_win);
  drain_idle();
}

TEST(WindowForegroundEvent, open_dialog) {
  fg_bg_counter_t counter = {0, 0};
  widget_t* main_win = NULL;
  widget_t* dialog_win = NULL;

  close_all_windows();
  main_win = window_create(NULL, 0, 0, 400, 300);
  drain_idle();
  fg_bg_counter_attach(main_win, &counter);

  dialog_win = dialog_create(NULL, 0, 0, 200, 200);
  drain_idle();

  ASSERT_EQ(counter.to_background, 1u);
  ASSERT_EQ(counter.to_foreground, 0u);

  fg_bg_counter_reset(&counter);
  window_close_force(dialog_win);
  drain_idle();

  ASSERT_EQ(counter.to_background, 0u);
  ASSERT_EQ(counter.to_foreground, 1u);

  window_close_force(main_win);
  drain_idle();
}

TEST(WindowForegroundEvent, combo_box_popup) {
  fg_bg_counter_t counter = {0, 0};
  widget_t* main_win = NULL;
  widget_t* combo = NULL;

  close_all_windows();
  main_win = window_create(NULL, 0, 0, 400, 300);
  drain_idle();
  fg_bg_counter_attach(main_win, &counter);

  combo = combo_box_create(main_win, 10, 10, 120, 30);
  combo_box_set_options(combo, "1:red;2:green;3:blue");
  widget_set_prop_bool(combo, WIDGET_PROP_READONLY, TRUE);

  widget_dispatch_simple_event(combo, EVT_POINTER_DOWN);
  widget_dispatch_simple_event(combo, EVT_POINTER_UP);
  drain_idle();

  ASSERT_EQ(counter.to_background, 0u);
  ASSERT_EQ(counter.to_foreground, 0u);
  ASSERT_NE(COMBO_BOX(combo)->combobox_popup, (widget_t*)NULL);

  window_close_force(COMBO_BOX(combo)->combobox_popup);
  drain_idle();

  ASSERT_EQ(counter.to_background, 0u);
  ASSERT_EQ(counter.to_foreground, 0u);

  window_close_force(main_win);
  drain_idle();
}

TEST(WindowForegroundEvent, open_toast) {
  fg_bg_counter_t counter = {0, 0};
  widget_t* main_win = NULL;
  widget_t* toast_win = NULL;

  close_all_windows();
  main_win = window_create(NULL, 0, 0, 400, 300);
  drain_idle();
  fg_bg_counter_attach(main_win, &counter);

  toast_win = dialog_create(NULL, 0, 0, 200, 80);
  widget_set_name(toast_win, DIALOG_TOAST_WIDGET_NAME);
  widget_set_prop_str(toast_win, WIDGET_PROP_THEME, DIALOG_TOAST_THEME_NAME);
  drain_idle();

  ASSERT_EQ(counter.to_background, 0u);
  ASSERT_EQ(counter.to_foreground, 0u);

  window_close_force(toast_win);
  drain_idle();

  ASSERT_EQ(counter.to_background, 0u);
  ASSERT_EQ(counter.to_foreground, 0u);

  window_close_force(main_win);
  drain_idle();
}

TEST(WindowForegroundEvent, open_dialog_over_overlay) {
  fg_bg_counter_t main_counter = {0, 0};
  fg_bg_counter_t overlay_counter = {0, 0};
  widget_t* main_win = NULL;
  widget_t* overlay_win = NULL;
  widget_t* dialog_win = NULL;

  close_all_windows();
  main_win = window_create(NULL, 0, 0, 400, 300);
  drain_idle();
  overlay_win = overlay_create(NULL, 0, 0, 400, 300);
  drain_idle();
  fg_bg_counter_attach(main_win, &main_counter);
  fg_bg_counter_attach(overlay_win, &overlay_counter);

  dialog_win = dialog_create(NULL, 0, 0, 200, 200);
  drain_idle();

  ASSERT_EQ(overlay_counter.to_background, 0u);
  ASSERT_EQ(overlay_counter.to_foreground, 0u);
  ASSERT_EQ(main_counter.to_background, 0u);

  window_close_force(dialog_win);
  drain_idle();
  window_close_force(overlay_win);
  drain_idle();
  window_close_force(main_win);
  drain_idle();
}
