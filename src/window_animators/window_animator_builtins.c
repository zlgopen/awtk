#include "base/window_animator_factory.h"

#include "window_animators/window_animator_fade.h"
#include "window_animators/window_animator_htranslate.h"
#include "window_animators/window_animator_vtranslate.h"
#include "window_animators/window_animator_center_scale.h"
#include "window_animators/window_animator_top_to_bottom.h"
#include "window_animators/window_animator_bottom_to_top.h"

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
                                   window_animator_top_to_bottom_create);
  window_animator_factory_register(factory, WINDOW_ANIMATOR_BOTTOM_TO_TOP,
                                   window_animator_bottom_to_top_create);
#endif /*WITH_WINDOW_ANIMATORS*/

  return RET_OK;
}
