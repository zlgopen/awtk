#include "base/input_method.h"

static ret_t main_loop_sdl2_dispatch_text_input(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  im_commit_event_t e;
  SDL_TextInputEvent* text_input_event = (SDL_TextInputEvent*)sdl_event;

  e.e = event_init(EVT_IM_COMMIT, NULL);
  e.text = text_input_event->text;

  return input_method_dispatch_to_widget(input_method(), &(e.e));
}

static ret_t main_loop_sdl2_dispatch_text_editing(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch_key_event(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  key_event_t event;
  int type = sdl_event->type;
  widget_t* widget = loop->base.wm;

  event.e.time = time_now_ms();
  event.key = sdl_event->key.keysym.sym;
  switch (type) {
    case SDL_KEYDOWN: {
      event.e.type = EVT_KEY_DOWN;
      window_manager_dispatch_input_event(widget, (event_t*)&event);
      break;
    }
    case SDL_KEYUP: {
      event.e.type = EVT_KEY_UP;
      window_manager_dispatch_input_event(widget, (event_t*)&event);
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

  event.e = event_init(EVT_WHEEL, widget);
  event.dx = sdl_event->wheel.x;
  event.dy = sdl_event->wheel.y;

  if (event.dy > 0) {
    event.dy = tk_max(MIN_WHEEL_DELTA, event.dy);
  } else if (event.dy < 0) {
    event.dy = tk_min(-MIN_WHEEL_DELTA, event.dy);
  }
  window_manager_dispatch_input_event(widget, (event_t*)&event);

  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch_mouse_event(main_loop_simple_t* loop, SDL_Event* sdl_event) {
  pointer_event_t event;
  int type = sdl_event->type;
  widget_t* widget = loop->base.wm;

  event.e.time = time_now_ms();
  switch (type) {
    case SDL_MOUSEBUTTONDOWN: {
      loop->pressed = 1;
      event.e.type = EVT_POINTER_DOWN;
      event.x = sdl_event->button.x;
      event.y = sdl_event->button.y;
      event.button = sdl_event->button.button;
      event.pressed = loop->pressed;

      window_manager_dispatch_input_event(widget, (event_t*)&event);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      event.e.type = EVT_POINTER_UP;
      event.x = sdl_event->button.x;
      event.y = sdl_event->button.y;
      event.button = sdl_event->button.button;
      event.pressed = loop->pressed;

      window_manager_dispatch_input_event(widget, (event_t*)&event);
      loop->pressed = 0;
      break;
    }
    case SDL_MOUSEMOTION: {
      event.e.type = EVT_POINTER_MOVE;
      event.x = sdl_event->motion.x;
      event.y = sdl_event->motion.y;
      event.button = 0;
      event.pressed = loop->pressed;

      window_manager_dispatch_input_event(widget, (event_t*)&event);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t main_loop_sdl2_dispatch(main_loop_simple_t* loop) {
  SDL_Event event;
  ret_t ret = RET_OK;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP: {
        ret = main_loop_sdl2_dispatch_key_event(loop, &event);
        break;
      }
      case SDL_MOUSEMOTION:
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        ret = main_loop_sdl2_dispatch_mouse_event(loop, &event);
        break;
      }
      case SDL_TEXTINPUT: {
        ret = main_loop_sdl2_dispatch_text_input(loop, &event);
        break;
      }
      case SDL_TEXTEDITING: {
        ret = main_loop_sdl2_dispatch_text_editing(loop, &event);
        break;
      }
      case SDL_QUIT: {
        ret = main_loop_quit(&(loop->base));
        break;
      }
      case SDL_MOUSEWHEEL: {
        ret = main_loop_sdl2_dispatch_wheel_event(loop, &event);
        break;
      }
    }
  }

  return ret;
}
