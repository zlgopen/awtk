/**
 * File:   window_animator_builtins.h
 * Author: AWTK Develop Team
 * Brief:  builtins window animator
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-03-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/window_animator_factory.h"
#include "window_animators/window_animator_fade.h"
#include "window_animators/window_animator_htranslate.h"
#include "window_animators/window_animator_vtranslate.h"
#include "window_animators/window_animator_center_scale.h"
#include "window_animators/window_animator_popdown.h"
#include "window_animators/window_animator_popup.h"

#include "window_animators/window_animator_slide_up.h"
#include "window_animators/window_animator_slide_down.h"
#include "window_animators/window_animator_slide_left.h"
#include "window_animators/window_animator_slide_right.h"

ret_t window_animator_register_builtins(void) {
#ifdef WITH_WINDOW_ANIMATORS
  window_animator_factory_t* factory = window_animator_factory();

  window_animator_factory_register(factory, WINDOW_ANIMATOR_HTRANSLATE,
                                   window_animator_htranslate_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_VTRANSLATE,
                                   window_animator_vtranslate_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_FADE, window_animator_fade_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_CENTER_SCALE,
                                   window_animator_center_scale_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_TOP_TO_BOTTOM,
                                   window_animator_popdown_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_BOTTOM_TO_TOP,
                                   window_animator_popup_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_POPDOWN,
                                   window_animator_popdown_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_POPUP, window_animator_popup_create);

  window_animator_factory_register(factory, WINDOW_ANIMATOR_SLIDE_UP,
                                   window_animator_slide_up_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_SLIDE_DOWN,
                                   window_animator_slide_down_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_SLIDE_LEFT,
                                   window_animator_slide_left_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_SLIDE_RIGHT,
                                   window_animator_slide_right_create);
#endif /*WITH_WINDOW_ANIMATORS*/

  return RET_OK;
}
