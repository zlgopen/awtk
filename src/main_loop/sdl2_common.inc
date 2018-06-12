
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
      case SDL_QUIT: {
        ret = main_loop_quit(&(loop->base));
        break;
      }
    }
  }

  return ret;
}
