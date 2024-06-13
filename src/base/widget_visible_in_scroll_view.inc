
#define WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_DEFAULT                               "default"

#define WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_IN_CENTER                             "in_center"
 
#define WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_IN_CENTER_IF_OUTSIDE_VIEWPORT         "in_center_if_outside_viewport"

#define WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_AT_TOP                                "at_top"

#define WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_NO_OPERATION                          "no_operation"

typedef ret_t (*widget_visible_reveal_in_scroll_func_t)(const rect_t* r, xy_t ox, xy_t oy, wh_t scroll_w, wh_t scroll_h, xy_t* new_ox, xy_t* new_oy);

/**
 * 调整到最近的可视位置，即块已在可视区域内，如果显示区域比滚动区域还要大，则不做任何操作，否则选择最短的路径移到到可视区域。
 */
static ret_t widget_visible_reveal_in_scroll_by_default(const rect_t* r, xy_t ox, xy_t oy, wh_t scroll_w, wh_t scroll_h, xy_t* new_ox, xy_t* new_oy) {
  return_value_if_fail(r != NULL && new_ox != NULL && new_oy != NULL, RET_BAD_PARAMS);

  if (oy > r->y) {
    oy = r->y;
  }
  if ((r->y + r->h) > (oy + scroll_h)) {
    oy = r->y + r->h - scroll_h;
  }

  if (ox > r->x) {
    ox = r->x;
  }
  if ((r->x + r->w) > (ox + scroll_w)) {
    ox = r->x + r->w - scroll_w;
  }

  *new_ox = tk_max(ox, 0);
  *new_oy = tk_max(oy, 0);

  return RET_OK;
}

/**
 * 调整到可视区域的中心，比如块没有在可视区域的中心（即使已在可视区域内），则调整到中心。
 */
static ret_t widget_visible_reveal_in_scroll_by_in_center(const rect_t* r, xy_t ox, xy_t oy, wh_t scroll_w, wh_t scroll_h, xy_t* new_ox, xy_t* new_oy) {
  return_value_if_fail(r != NULL && new_ox != NULL && new_oy != NULL, RET_BAD_PARAMS);
  (void)ox;
  (void)oy;
  *new_ox = r->x + (r->w - scroll_w) / 2;
  *new_oy = r->y + (r->h - scroll_h) / 2;
  *new_ox = tk_max(*new_ox, 0);
  *new_oy = tk_max(*new_oy, 0);
  return RET_OK;
}

/**
 * 如果在可视区域之外，则调整到可视区域的中心；否则，不做任何操作。
 */
static ret_t widget_visible_reveal_in_scroll_by_in_center_if_outside_viewport(const rect_t* r, xy_t ox, xy_t oy, wh_t scroll_w, wh_t scroll_h, xy_t* new_ox, xy_t* new_oy) {
  rect_t rect1;
  return_value_if_fail(r != NULL && new_ox != NULL && new_oy != NULL, RET_BAD_PARAMS);
  rect1 = rect_init(ox, oy, scroll_w, scroll_h);
  if (rect_has_intersect(&rect1, r)) {
    *new_ox = ox;
    *new_oy = oy;
    return RET_OK;
  } else {
    return widget_visible_reveal_in_scroll_by_in_center(r, ox, oy, scroll_w, scroll_h, new_ox, new_oy);
  }
}

/**
 * 调整到可视区域的顶端，比如块没有在可视区域的顶端（即使已在可视区域内），则调整到顶端。
 */
static ret_t widget_visible_reveal_in_scroll_by_at_top(const rect_t* r, xy_t ox, xy_t oy, wh_t scroll_w, wh_t scroll_h, xy_t* new_ox, xy_t* new_oy) {
  return_value_if_fail(r != NULL && new_ox != NULL && new_oy != NULL, RET_BAD_PARAMS);
  (void)scroll_w;
  (void)scroll_h;

  *new_ox = r->x;
  *new_oy = r->y;
  return RET_OK;
}

/* 不做任何处理 */
static ret_t widget_visible_reveal_in_scroll_by_no_operation(const rect_t* r, xy_t ox, xy_t oy, wh_t scroll_w, wh_t scroll_h, xy_t* new_ox, xy_t* new_oy) {
  return_value_if_fail(r != NULL && new_ox != NULL && new_oy != NULL, RET_BAD_PARAMS);
  *new_ox = ox;
  *new_oy = oy;
  return RET_OK;
}

static widget_visible_reveal_in_scroll_func_t widget_get_visible_reveal_in_scroll_func(widget_t* scroll_view) {
  const char* str = widget_get_prop_str(scroll_view, WIDGET_PROP_VISIBLE_REVEAL_IN_SCROLL, NULL);
  if (str != NULL) {
    if (tk_stricmp(str, WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_DEFAULT) == 0) {
      return widget_visible_reveal_in_scroll_by_default;
    } else if (tk_stricmp(str, WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_IN_CENTER) == 0) {
      return widget_visible_reveal_in_scroll_by_in_center;
    } else if (tk_stricmp(str, WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_IN_CENTER_IF_OUTSIDE_VIEWPORT) == 0) {
      return widget_visible_reveal_in_scroll_by_in_center_if_outside_viewport;
    } else if (tk_stricmp(str, WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_AT_TOP) == 0) {
      return widget_visible_reveal_in_scroll_by_at_top;
    } else if (tk_stricmp(str, WIDGET_VISIBLE_REVEAL_IN_SCROLL_TYPE_NO_OPERATION) == 0) {
      return widget_visible_reveal_in_scroll_by_no_operation;
    }
  }
  return widget_visible_reveal_in_scroll_by_default;
}
