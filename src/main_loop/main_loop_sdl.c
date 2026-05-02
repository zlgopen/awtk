/**
 * File:   main_loop_sdl.c
 * Author: AWTK Develop Team
 * Brief:  sdl2 implemented main_loop interface
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-01-13 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "native_window/native_window_sdl.h"
#include "main_loop/main_loop_simple.h"
#include "main_loop/main_loop_sdl.h"
#include "base/window_manager.h"
#include "base/font_manager.h"
#include "lcd/lcd_sdl2.h"
#include "base/idle.h"
#include "base/events.h"
#include "base/timer.h"
#include "base/system_info.h"
#include "base/awtk_sdl_api.h"

#ifdef AWTK_SDL3
#define AWTK_TF_TOUCH_ID(ev) ((ev)->touchID)
#define AWTK_TF_FINGER_ID(ev) ((ev)->fingerID)
#else
#define AWTK_TF_TOUCH_ID(ev) ((ev)->touchId)
#define AWTK_TF_FINGER_ID(ev) ((ev)->fingerId)
#endif

#ifdef AWTK_SDL3
#define AWTK_MOUSE_BTN_X(ev) ((int32_t)(ev)->button.x)
#define AWTK_MOUSE_BTN_Y(ev) ((int32_t)(ev)->button.y)
#define AWTK_MOUSE_MOTION_X(ev) ((int32_t)(ev)->motion.x)
#define AWTK_MOUSE_MOTION_Y(ev) ((int32_t)(ev)->motion.y)
#else
#define AWTK_MOUSE_BTN_X(ev) ((ev)->button.x)
#define AWTK_MOUSE_BTN_Y(ev) ((ev)->button.y)
#define AWTK_MOUSE_MOTION_X(ev) ((ev)->motion.x)
#define AWTK_MOUSE_MOTION_Y(ev) ((ev)->motion.y)
#endif

#ifdef AWTK_SDL3
#define AWTK_WIN_EVT_SWITCH switch (event->type)
#define CWIN_SHOWN SDL_EVENT_WINDOW_SHOWN
#define CWIN_HIDDEN SDL_EVENT_WINDOW_HIDDEN
#define CWIN_EXPOSED SDL_EVENT_WINDOW_EXPOSED
#define CWIN_MOVED SDL_EVENT_WINDOW_MOVED
#define CWIN_RESIZED SDL_EVENT_WINDOW_RESIZED
#define CWIN_SIZE_CHANGED SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED
#define CWIN_MIN SDL_EVENT_WINDOW_MINIMIZED
#define CWIN_MAX SDL_EVENT_WINDOW_MAXIMIZED
#define CWIN_RESTORED SDL_EVENT_WINDOW_RESTORED
#define CWIN_ENTER SDL_EVENT_WINDOW_MOUSE_ENTER
#define CWIN_LEAVE SDL_EVENT_WINDOW_MOUSE_LEAVE
#define CWIN_FOCUS_IN SDL_EVENT_WINDOW_FOCUS_GAINED
#define CWIN_FOCUS_OUT SDL_EVENT_WINDOW_FOCUS_LOST
#define CWIN_HIT_TEST SDL_EVENT_WINDOW_HIT_TEST
#else
#define AWTK_WIN_EVT_SWITCH switch (event->window.event)
#define CWIN_SHOWN SDL_WINDOWEVENT_SHOWN
#define CWIN_HIDDEN SDL_WINDOWEVENT_HIDDEN
#define CWIN_EXPOSED SDL_WINDOWEVENT_EXPOSED
#define CWIN_MOVED SDL_WINDOWEVENT_MOVED
#define CWIN_RESIZED SDL_WINDOWEVENT_RESIZED
#define CWIN_SIZE_CHANGED SDL_WINDOWEVENT_SIZE_CHANGED
#define CWIN_MIN SDL_WINDOWEVENT_MINIMIZED
#define CWIN_MAX SDL_WINDOWEVENT_MAXIMIZED
#define CWIN_RESTORED SDL_WINDOWEVENT_RESTORED
#define CWIN_ENTER SDL_WINDOWEVENT_ENTER
#define CWIN_LEAVE SDL_WINDOWEVENT_LEAVE
#define CWIN_FOCUS_IN SDL_WINDOWEVENT_FOCUS_GAINED
#define CWIN_FOCUS_OUT SDL_WINDOWEVENT_FOCUS_LOST
#define CWIN_HIT_TEST SDL_WINDOWEVENT_HIT_TEST
#endif

#ifdef AWTK_SDL3
#define EVT_DROPFILE SDL_EVENT_DROP_FILE
#define EVT_KEY_DOWN SDL_EVENT_KEY_DOWN
#define EVT_KEY_UP SDL_EVENT_KEY_UP
#define EVT_MOUSE_MOTION SDL_EVENT_MOUSE_MOTION
#define EVT_MOUSE_BTN_DOWN SDL_EVENT_MOUSE_BUTTON_DOWN
#define EVT_MOUSE_BTN_UP SDL_EVENT_MOUSE_BUTTON_UP
#define EVT_FINGER_DOWN SDL_EVENT_FINGER_DOWN
#define EVT_FINGER_UP SDL_EVENT_FINGER_UP
#define EVT_FINGER_MOTION SDL_EVENT_FINGER_MOTION
#define EVT_TEXT_INPUT SDL_EVENT_TEXT_INPUT
#define EVT_TEXT_EDIT SDL_EVENT_TEXT_EDITING
#define EVT_MOUSE_WHEEL SDL_EVENT_MOUSE_WHEEL
#define EVT_QUIT SDL_EVENT_QUIT
#define EVT_RENDER_DEV SDL_EVENT_RENDER_DEVICE_RESET
#define EVT_RENDER_TGT SDL_EVENT_RENDER_TARGETS_RESET
#else
#define EVT_DROPFILE SDL_DROPFILE
#define EVT_KEY_DOWN SDL_KEYDOWN
#define EVT_KEY_UP SDL_KEYUP
#define EVT_MOUSE_MOTION SDL_MOUSEMOTION
#define EVT_MOUSE_BTN_DOWN SDL_MOUSEBUTTONDOWN
#define EVT_MOUSE_BTN_UP SDL_MOUSEBUTTONUP
#define EVT_FINGER_DOWN SDL_FINGERDOWN
#define EVT_FINGER_UP SDL_FINGERUP
#define EVT_FINGER_MOTION SDL_FINGERMOTION
#define EVT_TEXT_INPUT SDL_TEXTINPUT
#define EVT_TEXT_EDIT SDL_TEXTEDITING
#define EVT_MOUSE_WHEEL SDL_MOUSEWHEEL
#define EVT_MULTIGESTURE SDL_MULTIGESTURE
#define EVT_QUIT SDL_QUIT
#define EVT_RENDER_DEV SDL_RENDER_DEVICE_RESET
#define EVT_RENDER_TGT SDL_RENDER_TARGETS_RESET
#endif

#include <stdio.h>
#include "awtk_global.h"
#include "tkc/time_now.h"
#include "base/input_method.h"

static ret_t main_loop_sdl2_dispatch_text_input(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  im_commit_event_t event;
  SDL_TextInputEvent* text_input_event = (SDL_TextInputEvent*)sdl_event;

  memset(&event, 0x00, sizeof(event));
  event.e = event_init(EVT_IM_COMMIT, NULL);
  event.text = text_input_event->text;

  return input_method_dispatch_to_widget(input_method(), &(event.e));
}

static ret_t main_loop_sdl2_dispatch_text_editing(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  return RET_OK;
}

static ret_t main_loop_sdl2_set_key_event_mod(key_event_t* event, uint16_t mod) {
#ifdef AWTK_SDL3
  event->capslock = (mod & SDL_KMOD_CAPS) != 0;
  event->numlock = (mod & SDL_KMOD_NUM) != 0;
#else
  event->capslock = (mod & KMOD_CAPS) != 0;
  event->numlock = (mod & KMOD_NUM) != 0;
#endif
  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch_key_event(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  key_event_t event;
  int type = sdl_event->type;
  widget_t* widget = loop->base.wm;
#ifdef AWTK_SDL3
  SDL_Keycode sym = sdl_event->key.key;
  uint16_t mod = (uint16_t)sdl_event->key.mod;
  Uint32 wid = (Uint32)sdl_event->key.windowID;
#else
  SDL_Keycode sym = sdl_event->key.keysym.sym;
  uint16_t mod = (uint16_t)sdl_event->key.keysym.mod;
  Uint32 wid = sdl_event->key.windowID;
#endif

  switch (type) {
#ifdef AWTK_SDL3
    case SDL_EVENT_KEY_DOWN: {
#else
    case SDL_KEYDOWN: {
#endif
      key_event_init(&event, EVT_KEY_DOWN, widget, sym);
      main_loop_sdl2_set_key_event_mod(&event, mod);
      event.e.native_window_handle = SDL_GetWindowFromID(wid);
      window_manager_dispatch_input_event(widget, (event_t*)&event);
      loop->key_pressed = TRUE;
      break;
    }
#ifdef AWTK_SDL3
    case SDL_EVENT_KEY_UP: {
#else
    case SDL_KEYUP: {
#endif
      key_event_init(&event, EVT_KEY_UP, widget, sym);
      main_loop_sdl2_set_key_event_mod(&event, mod);
      event.e.native_window_handle = SDL_GetWindowFromID(wid);
      window_manager_dispatch_input_event(widget, (event_t*)&event);
      loop->key_pressed = FALSE;
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

#define MIN_WHEEL_DELTA 12

static ret_t main_loop_sdl2_dispatch_wheel_event(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  wheel_event_t event;
  widget_t* widget = loop->base.wm;
#ifdef AWTK_SDL3
  int32_t dy = (int32_t)sdl_event->wheel.y;
#else
  int32_t dy = sdl_event->wheel.y;
#endif
  event_t* e = wheel_event_init(&event, EVT_WHEEL, widget, dy);

  event.e.native_window_handle = SDL_GetWindowFromID(sdl_event->wheel.windowID);
  if (event.dy > 0) {
    event.dy = tk_max(MIN_WHEEL_DELTA, event.dy);
  } else if (event.dy < 0) {
    event.dy = tk_min(-MIN_WHEEL_DELTA, event.dy);
  }
  window_manager_dispatch_input_event(widget, e);

  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch_multi_gesture_event(main_loop_simple_t* loop,
                                                         SDL_Event* sdl_event) {
#ifndef AWTK_SDL3
  multi_gesture_event_t event;
  widget_t* widget = loop->base.wm;
  int32_t x = sdl_event->mgesture.x * widget->w;
  int32_t y = sdl_event->mgesture.y * widget->h;
  float rotation = sdl_event->mgesture.dTheta;
  float distance = sdl_event->mgesture.dDist;
  event_t* e = multi_gesture_event_init(&event, widget, x, y, rotation, distance);

  event.e.native_window_handle = NULL;
  window_manager_dispatch_input_event(widget, e);
#endif
  (void)loop;
  (void)sdl_event;
  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch_touch_event(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  event_t* e = NULL;
  touch_event_t event;
  int type = EVT_TOUCH_DOWN;
  widget_t* widget = loop->base.wm;
  SDL_TouchFingerEvent* finger_event = (SDL_TouchFingerEvent*)sdl_event;

  memset(&event, 0x00, sizeof(event));
  switch (sdl_event->type) {
#ifdef AWTK_SDL3
    case SDL_EVENT_FINGER_DOWN: {
#else
    case SDL_FINGERDOWN: {
#endif
      type = EVT_TOUCH_DOWN;
      break;
    }
#ifdef AWTK_SDL3
    case SDL_EVENT_FINGER_UP: {
#else
    case SDL_FINGERUP: {
#endif
      type = EVT_TOUCH_UP;
      break;
    }
#ifdef AWTK_SDL3
    case SDL_EVENT_FINGER_MOTION: {
#else
    case SDL_FINGERMOTION: {
#endif
      type = EVT_TOUCH_MOVE;
      break;
    }
    default:
      break;
  }

  e = touch_event_init(&event, type, widget, AWTK_TF_TOUCH_ID(finger_event),
                       AWTK_TF_FINGER_ID(finger_event), finger_event->x, finger_event->y,
                       finger_event->pressure);

  if (e != NULL) {
    widget_t* win = window_manager_get_top_window(widget);
    widget_dispatch(win, e);
  }

  log_debug("touch event: type=%d touch_id=%" PRId64 " finger_id=%" PRId64 "  x=%f y=%f\n", type,
            event.touch_id, event.finger_id, event.x, event.y);

  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch_mouse_event(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  key_event_t key_event;
  pointer_event_t event;
  widget_t* widget = loop->base.wm;

  switch (sdl_event->type) {
#ifdef AWTK_SDL3
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
#else
    case SDL_MOUSEBUTTONDOWN: {
#endif
      if (sdl_event->button.button == SDL_BUTTON_LEFT) {
        loop->pressed = TRUE;
        pointer_event_init(&event, EVT_POINTER_DOWN, widget, AWTK_MOUSE_BTN_X(sdl_event),
                           AWTK_MOUSE_BTN_Y(sdl_event));
        event.button = sdl_event->button.button;
        event.pressed = loop->pressed;
        event.e.native_window_handle = SDL_GetWindowFromID(sdl_event->button.windowID);

#ifdef AWTK_SDL3
        SDL_CaptureMouse(true);
#else
        SDL_CaptureMouse(TRUE);
#endif
        window_manager_dispatch_input_event(widget, (event_t*)&event);
      } else if (sdl_event->button.button == SDL_BUTTON_RIGHT) {
      } else if (sdl_event->button.button == SDL_BUTTON_MIDDLE) {
        key_event_init(&key_event, EVT_KEY_DOWN, widget, TK_KEY_WHEEL);
        window_manager_dispatch_input_event(widget, (event_t*)&key_event);
      } else {
        pointer_event_init(&event, EVT_MOUSE_EXTRA_BUTTON_DOWN, widget, AWTK_MOUSE_BTN_X(sdl_event),
                           AWTK_MOUSE_BTN_Y(sdl_event));
        event.button = sdl_event->button.button;
        event.e.native_window_handle = SDL_GetWindowFromID(sdl_event->button.windowID);
        window_manager_dispatch_input_event(widget, (event_t*)&event);
      }
      break;
    }
#ifdef AWTK_SDL3
    case SDL_EVENT_MOUSE_BUTTON_UP: {
#else
    case SDL_MOUSEBUTTONUP: {
#endif
      if (sdl_event->button.button == SDL_BUTTON_LEFT) {
#ifdef AWTK_SDL3
        SDL_CaptureMouse(false);
#else
        SDL_CaptureMouse(FALSE);
#endif
        pointer_event_init(&event, EVT_POINTER_UP, widget, AWTK_MOUSE_BTN_X(sdl_event),
                           AWTK_MOUSE_BTN_Y(sdl_event));
        event.button = sdl_event->button.button;
        event.pressed = loop->pressed;
        event.e.native_window_handle = SDL_GetWindowFromID(sdl_event->button.windowID);

        window_manager_dispatch_input_event(widget, (event_t*)&event);
        loop->pressed = FALSE;
      } else if (sdl_event->button.button == SDL_BUTTON_RIGHT) {
        pointer_event_init(&event, EVT_CONTEXT_MENU, widget, AWTK_MOUSE_BTN_X(sdl_event),
                           AWTK_MOUSE_BTN_Y(sdl_event));
        event.button = sdl_event->button.button;
        event.e.native_window_handle = SDL_GetWindowFromID(sdl_event->button.windowID);
        window_manager_dispatch_input_event(widget, (event_t*)&event);
      } else if (sdl_event->button.button == SDL_BUTTON_MIDDLE) {
        key_event_init(&key_event, EVT_KEY_UP, widget, TK_KEY_WHEEL);
        window_manager_dispatch_input_event(widget, (event_t*)&key_event);
      } else {
        pointer_event_init(&event, EVT_MOUSE_EXTRA_BUTTON_UP, widget, AWTK_MOUSE_BTN_X(sdl_event),
                           AWTK_MOUSE_BTN_Y(sdl_event));
        event.button = sdl_event->button.button;
        event.e.native_window_handle = SDL_GetWindowFromID(sdl_event->button.windowID);
        window_manager_dispatch_input_event(widget, (event_t*)&event);
      }
      break;
    }
#ifdef AWTK_SDL3
    case SDL_EVENT_MOUSE_MOTION: {
#else
    case SDL_MOUSEMOTION: {
#endif
      pointer_event_init(&event, EVT_POINTER_MOVE, widget, AWTK_MOUSE_MOTION_X(sdl_event),
                         AWTK_MOUSE_MOTION_Y(sdl_event));
      event.button = 0;
      event.pressed = loop->pressed;
      event.e.native_window_handle = SDL_GetWindowFromID(sdl_event->motion.windowID);

      window_manager_dispatch_input_event(widget, (event_t*)&event);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t on_resized_timer(const timer_info_t* info) {
  widget_t* wm = WIDGET(info->ctx);
  widget_set_need_relayout_children(wm);
  widget_invalidate_force(wm, NULL);
  window_manager_set_ignore_input_events(wm, FALSE);

  log_debug("on_resized_timer\n");
  return RET_REMOVE;
}

static ret_t main_loop_sdl2_dispatch_window_event(main_loop_simple_t* loop, SDL_Event* event) {
  main_loop_t* l = (main_loop_t*)(loop);

  AWTK_WIN_EVT_SWITCH {
    case CWIN_SHOWN:
      log_debug("Window %d shown\n", event->window.windowID);
      widget_invalidate_force(l->wm, NULL);
      break;
    case CWIN_HIDDEN:
      log_debug("Window %d hidden\n", event->window.windowID);
      break;
    case CWIN_EXPOSED:
      log_debug("Window %d exposed\n", event->window.windowID);
      widget_invalidate_force(l->wm, NULL);
      break;
    case CWIN_MOVED:
      log_debug("Window %d moved to %d,%d\n", event->window.windowID, event->window.data1,
                event->window.data2);
      break;
    case CWIN_RESIZED:
      window_manager_set_ignore_input_events(l->wm, TRUE);
      log_debug("Window %d resized to %dx%d\n", event->window.windowID, event->window.data1,
                event->window.data2);
      timer_add(on_resized_timer, l->wm, 100);
      break;
    case CWIN_SIZE_CHANGED: {
      native_window_info_t info;
      event_t e = event_init(EVT_NATIVE_WINDOW_RESIZED, NULL);
      SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
      native_window_t* native_window =
          (native_window_t*)widget_get_prop_pointer(window_manager(), WIDGET_PROP_NATIVE_WINDOW);
      window_manager_set_ignore_input_events(l->wm, TRUE);
      native_window_get_info(native_window, &info);
      system_info_set_lcd_w(system_info(), info.w);
      system_info_set_lcd_h(system_info(), info.h);
      window_manager_dispatch_native_window_event(l->wm, &e, win);
      timer_add(on_resized_timer, l->wm, 100);
      break;
    }
    case CWIN_MIN:
      log_debug("Window %d minimized\n", event->window.windowID);
      break;
    case CWIN_MAX:
      log_debug("Window %d maximized\n", event->window.windowID);
      widget_invalidate_force(l->wm, NULL);
      break;
    case CWIN_RESTORED:
      log_debug("Window %d restored\n", event->window.windowID);
      widget_invalidate_force(l->wm, NULL);
      break;
    case CWIN_ENTER: {
      int x = 0;
      int y = 0;
      pointer_event_t e;
      SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);

      log_debug("Mouse entered window %d\n", event->window.windowID);
#ifdef AWTK_SDL3
      {
        float fx = 0;
        float fy = 0;
        SDL_GetMouseState(&fx, &fy);
        x = (int)fx;
        y = (int)fy;
      }
#else
      SDL_GetMouseState(&x, &y);
#endif
      pointer_event_init(&e, EVT_NATIVE_WINDOW_ENTER, l->wm, x, y);
      window_manager_dispatch_native_window_event(l->wm, (event_t*)&e, win);
      break;
    }
    case CWIN_LEAVE: {
      event_t e = event_init(EVT_NATIVE_WINDOW_LEAVE, NULL);
      SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);

      log_debug("Mouse left window %d\n", event->window.windowID);
      window_manager_dispatch_native_window_event(l->wm, &e, win);
      break;
    }
    case CWIN_FOCUS_IN: {
      event_t e = event_init(EVT_NATIVE_WINDOW_FOCUS_GAINED, NULL);
      SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);

      log_debug("Window %d gained keyboard focus\n", event->window.windowID);
      window_manager_dispatch_native_window_event(l->wm, &e, win);
      break;
    }
    case CWIN_FOCUS_OUT: {
      event_t e = event_init(EVT_NATIVE_WINDOW_FOCUS_LOST, NULL);
      SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);

#ifdef AWTK_SDL3
      SDL_CaptureMouse(false);
#else
      SDL_CaptureMouse(FALSE);
#endif
      log_debug("Window %d lost keyboard focus\n", event->window.windowID);
      window_manager_dispatch_native_window_event(l->wm, &e, win);
      break;
    }
#ifdef AWTK_SDL3
    case CWIN_HIT_TEST:
      log_debug("Window %d has a special hit test\n", event->window.windowID);
      break;
#else
#if SDL_VERSION_ATLEAST(2, 0, 5)
    case SDL_WINDOWEVENT_TAKE_FOCUS:
      log_debug("Window %d is offered a focus\n", event->window.windowID);
      break;
    case CWIN_HIT_TEST:
      log_debug("Window %d has a special hit test\n", event->window.windowID);
      break;
#endif
#endif
    default:
#ifdef AWTK_SDL3
      log_debug("Window %d got unknown event %u\n", event->window.windowID, (unsigned)event->type);
#else
      log_debug("Window %d got unknown event %d\n", event->window.windowID, event->window.event);
#endif
      break;
  }

  return RET_OK;
}

ret_t main_loop_sdl2_dispatch(main_loop_simple_t* loop) {
  SDL_Event event;
  ret_t ret = RET_OK;
  widget_t* wm = loop->base.wm;
  while (SDL_PollEvent(&event) && loop->base.running) {
#ifdef AWTK_SDL3
    if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
      main_loop_sdl2_dispatch_window_event(loop, &event);
      continue;
    }
#endif
    switch (event.type) {
      case EVT_DROPFILE: {
        drop_file_event_t drop;
        widget_t* top = window_manager_get_top_window(wm);
#ifdef AWTK_SDL3
        event_t* e = drop_file_event_init(&drop, NULL, (char*)event.drop.data);
#else
        event_t* e = drop_file_event_init(&drop, NULL, event.drop.file);
#endif

        widget_dispatch(wm, e);
        if (top != NULL) {
          widget_dispatch(top, e);
        }

        break;
      }
      case EVT_KEY_DOWN:
      case EVT_KEY_UP: {
        ret = main_loop_sdl2_dispatch_key_event(loop, &event);
        break;
      }
      case EVT_MOUSE_MOTION:
      case EVT_MOUSE_BTN_DOWN:
      case EVT_MOUSE_BTN_UP: {
        ret = main_loop_sdl2_dispatch_mouse_event(loop, &event);
        break;
      }
      case EVT_FINGER_DOWN:
      case EVT_FINGER_UP:
      case EVT_FINGER_MOTION: {
        ret = main_loop_sdl2_dispatch_touch_event(loop, &event);
        break;
      }
      case EVT_TEXT_INPUT: {
        ret = main_loop_sdl2_dispatch_text_input(loop, &event);
        break;
      }
      case EVT_TEXT_EDIT: {
        ret = main_loop_sdl2_dispatch_text_editing(loop, &event);
        break;
      }
      case EVT_MOUSE_WHEEL: {
        ret = main_loop_sdl2_dispatch_wheel_event(loop, &event);
        break;
      }
#ifndef AWTK_SDL3
      case EVT_MULTIGESTURE: {
        ret = main_loop_sdl2_dispatch_multi_gesture_event(loop, &event);
        break;
      }
      case SDL_WINDOWEVENT: {
        main_loop_sdl2_dispatch_window_event(loop, &event);
        break;
      }
      case SDL_SYSWMEVENT: {
        system_event_t e;
        widget_dispatch(wm, system_event_init(&e, NULL, &event));
        break;
      }
#endif
      case EVT_RENDER_DEV: {
#ifdef WITH_NANOVG_SOFT
        native_window_t* native_window =
            (native_window_t*)widget_get_prop_pointer(window_manager(), WIDGET_PROP_NATIVE_WINDOW);
        canvas_t* canvas = native_window_get_canvas(native_window);
        if (canvas != NULL) {
          lcd_sdl2_texture_reset(canvas->lcd);
        }
#endif
        widget_invalidate_force(wm, NULL);
        log_debug("SDL_RENDER_DEVICE_RESET\n");
        break;
      }
      case EVT_RENDER_TGT: {
        widget_invalidate_force(wm, NULL);
        log_debug("SDL_RENDER_TARGETS_RESET\n");
        break;
      }
      case EVT_QUIT: {
        event_t e = event_init(EVT_REQUEST_QUIT_APP, NULL);
        if (widget_dispatch(wm, &e) == RET_OK) {
          main_loop_quit((main_loop_t*)loop);
        }
        break;
      }
    }
  }

  return ret;
}

static ret_t main_loop_sdl2_destroy(main_loop_t* l) {
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  main_loop_simple_reset(loop);
  native_window_sdl_deinit();

  return RET_OK;
}

main_loop_t* main_loop_init(int w, int h) {
  main_loop_simple_t* loop = NULL;
#ifdef MULTI_NATIVE_WINDOW
  native_window_sdl_init(FALSE, w, h);
#else
  native_window_sdl_init(TRUE, w, h);
#endif /*MULTI_NATIVE_WINDOW*/
  loop = main_loop_simple_init(w, h, NULL, NULL);
  loop->base.destroy = main_loop_sdl2_destroy;
  loop->dispatch_input = main_loop_sdl2_dispatch;
#ifdef AWTK_SDL3
  SDL_SetEventEnabled(SDL_EVENT_DROP_FILE, true);
#else
  SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
  SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
#endif

  return (main_loop_t*)loop;
}
