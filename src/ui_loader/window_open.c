#include "base/assets_manager.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_default.h"

static ret_t on_window_open(void* ctx, event_t* e) {
  widget_t* to_close = WIDGET(ctx);
  widget_t* open = WIDGET(e->target);

  if (open != NULL && open->name != NULL) {
    log_debug("window %s open\n", open->name);
  }

  if (to_close != NULL && to_close->name != NULL) {
    log_debug("window %s close\n", to_close->name);
  }

  return RET_REMOVE;
}

static widget_t* window_open_impl(const char* name, asset_type_t res_type, widget_t* to_close) {
  ui_loader_t* loader = default_ui_loader();
  ui_builder_t* builder = ui_builder_default(name);
  const asset_info_t* ui = assets_manager_ref(assets_manager(), res_type, name);
  return_value_if_fail(ui != NULL, NULL);

  ui_loader_load(loader, ui->data, ui->size, builder);
  assets_manager_unref(assets_manager(), ui);

  if (builder->root != NULL) {
    widget_on(builder->root, EVT_WINDOW_OPEN, on_window_open, to_close);
  }

  return builder->root;
}

widget_t* window_open_and_close(const char* name, widget_t* to_close) {
  return window_open_impl(name, ASSET_TYPE_UI, to_close);
}

widget_t* window_open(const char* name) {
  return window_open_impl(name, ASSET_TYPE_UI, NULL);
}

widget_t* dialog_open(const char* name) {
  return window_open(name);
}
