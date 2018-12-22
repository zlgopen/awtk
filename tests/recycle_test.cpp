#include "awtk.h"
#include "demos/assets.h"

static void create_destroy_test1(void) {
  value_t v1;
  uint32_t i = 0;
  widget_t* labels[100];
  for (i = 0; i < ARRAY_SIZE(labels); i++) {
    widget_t* l = label_create(NULL, 10, 20, 30, 40);

    widget_set_text(l, L"OK");
    widget_set_text(l, L"Cancel");

    value_set_wstr(&v1, L"label");
    widget_set_prop(l, WIDGET_PROP_TEXT, &v1);
    widget_set_tr_text(l, "name");
    widget_set_prop_str(l, WIDGET_PROP_NAME, "name");

    labels[i] = l;
  }

  for (i = 0; i < ARRAY_SIZE(labels); i++) {
    widget_t* l = labels[i];
    widget_destroy(l);
  }
}

static void create_destroy_test(uint32_t nr) {
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    create_destroy_test1();
  }
}

int main(void) {
  system_info_init(APP_SIMULATOR, NULL, "./demos");
  tk_init_internal();

  assets_init();
  tk_init_assets();
  create_destroy_test(10000);

  tk_deinit_internal();

  return 0;
}
