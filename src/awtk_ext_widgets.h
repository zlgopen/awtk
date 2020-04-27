/**
 * File:   awtk.h
 * Author: AWTK Develop Team
 * Brief:  awtk widgets
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
 * 2018-12-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef AWTK_EXT_WIDGETS_H
#define AWTK_EXT_WIDGETS_H

#include "switch/switch.h"
#include "guage/guage.h"
#include "guage/guage_pointer.h"
#include "gif_image/gif_image.h"
#include "svg_image/svg_image.h"
#include "keyboard/keyboard.h"
#include "keyboard/candidates.h"
#include "keyboard/lang_indicator.h"
#include "base/widget_factory.h"
#include "rich_text/rich_text.h"
#include "rich_text/rich_text_view.h"
#include "slide_menu/slide_menu.h"
#include "image_value/image_value.h"
#include "time_clock/time_clock.h"
#include "scroll_view/list_item.h"
#include "scroll_view/list_view.h"
#include "slide_view/slide_view.h"
#include "slide_view/slide_indicator.h"
#include "scroll_view/scroll_bar.h"
#include "scroll_view/scroll_view.h"
#include "scroll_view/list_view_h.h"
#include "color_picker/color_picker.h"
#include "canvas_widget/canvas_widget.h"
#include "text_selector/text_selector.h"
#include "color_picker/color_component.h"
#include "progress_circle/progress_circle.h"
#include "image_animation/image_animation.h"
#include "mutable_image/mutable_image.h"
#include "combo_box_ex/combo_box_ex.h"
#include "scroll_label/hscroll_label.h"
#include "mledit/line_number.h"
#include "mledit/mledit.h"
#include "features/draggable.h"

#if defined(WITH_FS_RES) || defined(WITH_FS)
#include "file_browser/file_chooser.h"
#include "file_browser/file_browser_view.h"
#endif /*WITH_FS*/

#include "ext_widgets/ext_widgets.h"

#endif /*AWTK_EXT_WIDGETS_H*/
