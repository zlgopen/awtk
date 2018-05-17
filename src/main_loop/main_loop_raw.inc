
typedef struct _main_loop_raw_t {
  main_loop_t base;

  int w;
  int h;
  widget_t* wm;
  canvas_t canvas;

  xy_t touch_x;
  xy_t touch_y;
  bool_t pressed;

  uint8_t key;
  bool_t key_pressed;
  event_queue_t* queue;
} main_loop_raw_t;

static main_loop_raw_t s_main_loop;

static ret_t post_touch_events(main_loop_t* l, bool_t pressed, xy_t x, xy_t y) {
  event_all_t any;
  pointer_event_t event;
  main_loop_raw_t* loop = (main_loop_raw_t*)l;

  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);

  loop->touch_x = x;
  loop->touch_y = y;

  event.button = 0;
  event.x = loop->touch_x;
  event.y = loop->touch_y;

  if (pressed) {
    if (loop->pressed) {
      event.e.type = EVT_POINTER_MOVE;
    } else {
      event.e.type = EVT_POINTER_DOWN;
    }
    loop->pressed = TRUE;
    event.pressed = loop->pressed;

    any.e.pointer_event = event;
    event_queue_send(loop->queue, &any);
  } else {
    if (loop->pressed) {
      loop->pressed = FALSE;
      event.e.type = EVT_POINTER_UP;
      event.pressed = loop->pressed;

      any.e.pointer_event = event;
      event_queue_send(loop->queue, &any);
    }
  }

  return RET_OK;
}

static void post_key_events(main_loop_t* l, bool_t pressed, uint8_t key) {
  event_all_t any;
  key_event_t event;
  main_loop_raw_t* loop = (main_loop_raw_t*)l;

  loop->key = key;
  event.key = key;

  if (pressed) {
    loop->key_pressed = TRUE;
    event.e.type = EVT_KEY_DOWN;

    any.e.key_event = event;
    event_queue_send(loop->queue, &any);
  } else {
    if (loop->key_pressed) {
      loop->key_pressed = FALSE;
      event.e.type = EVT_KEY_UP;

      any.e.key_event = event;
      event_queue_send(loop->queue, &any);
    }
  }
}

static ret_t main_loop_raw_dispatch(main_loop_raw_t* loop) {
  event_all_t event;
  widget_t* widget = loop->wm;
  platform_disaptch_input((main_loop_t*)loop);

  while (event_queue_recv(loop->queue, &event) == RET_OK) {
    switch (event.e.event.type) {
      case EVT_POINTER_DOWN:
        window_manager_dispatch_input_event(widget, (event_t*)&(event.e.pointer_event));
        break;
      case EVT_POINTER_MOVE:
        window_manager_dispatch_input_event(widget, (event_t*)&(event.e.pointer_event));
        break;
      case EVT_POINTER_UP:
        window_manager_dispatch_input_event(widget, (event_t*)&(event.e.pointer_event));
        break;
      default:
        break;
    }
  }

  return RET_OK;
}

static ret_t main_loop_raw_paint(main_loop_raw_t* loop) {
  canvas_t* c = &(loop->canvas);

  return window_manager_paint(loop->wm, c);
}

static ret_t main_loop_raw_run(main_loop_t* l) {
  main_loop_raw_t* loop = (main_loop_raw_t*)l;

  while (l->running) {
    timer_dispatch();
    main_loop_raw_dispatch(loop);
    idle_dispatch();

    main_loop_raw_paint(loop);
    if (!WINDOW_MANAGER(loop->wm)->animating) {
      sleep_ms(30);
    }
  }

  return RET_OK;
}

static ret_t main_loop_raw_quit(main_loop_t* l) { return RET_OK; }

static ret_t main_loop_raw_destroy(main_loop_t* l) {
  main_loop_raw_t* loop = (main_loop_raw_t*)l;
  event_queue_destroy(loop->queue);

  return RET_OK;
}

main_loop_t* main_loop_init(int w, int h) {
  lcd_t* lcd = NULL;
  event_queue_t* queue = NULL;
  widget_t* wm = window_manager();
  main_loop_raw_t* loop = &s_main_loop;
  main_loop_t* base = &(loop->base);
  return_value_if_fail(wm != NULL, NULL);

  queue = event_queue_create(20);
  return_value_if_fail(queue != NULL, NULL);

  lcd = platform_create_lcd(w, h);
  return_value_if_fail(lcd != NULL, NULL);

  memset(loop, 0x00, sizeof(main_loop_raw_t));

  base->run = main_loop_raw_run;
  base->quit = main_loop_raw_quit;
  base->destroy = main_loop_raw_destroy;

  loop->wm = wm;
  loop->queue = queue;
  window_manager_resize(wm, w, h);

  main_loop_set(base);
  canvas_init(&(loop->canvas), lcd, font_manager());

  return base;
}
