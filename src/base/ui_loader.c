/**
 * File:   ui_loader.h
 * Author: AWTK Develop Team
 * Brief:  ui_loader interface
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/ui_loader.h"
#include "base/assets_manager.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_default.h"

ret_t ui_loader_load(ui_loader_t* loader, const uint8_t* data, uint32_t size, ui_builder_t* b) {
  return_value_if_fail(loader != NULL && loader->load != NULL, RET_BAD_PARAMS);
  return_value_if_fail(data != NULL && b != NULL, RET_BAD_PARAMS);

  return loader->load(loader, data, size, b);
}

widget_t* ui_loader_load_widget(const char* name) {
  ui_loader_t* loader = default_ui_loader();
  ui_builder_t* builder = ui_builder_default(name);
  const asset_info_t* ui = assets_manager_ref(assets_manager(), ASSET_TYPE_UI, name);
  return_value_if_fail(ui != NULL, NULL);

  ui_loader_load(loader, ui->data, ui->size, builder);
  assets_manager_unref(assets_manager(), ui);

  return builder->root;
}
