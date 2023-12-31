#include "tkc/str.h"
#include "tkc/log.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"

static ret_t debugger_log(void* ctx, tk_log_level_t level, const char* format, va_list ap) {
  str_t* str = (str_t*)ctx;
  char msg[1024] = {0};
  tk_vsnprintf(msg, sizeof(msg)-1, format, ap);

  str_append_more(str, msg, ";", NULL);

  return RET_OK;
}

TEST(Log, basic) {
  str_t str;
  str_init(&str, 100);
  log_set_hook(debugger_log, &str);
  log_set_hook(debugger_log, &str);

  log_debug("%d:%s\n", 1, "debug");
  log_info("%d:%s\n", 2, "info");
  log_warn("%d:%s\n", 3, "warn");
  log_error("%d:%s\n", 4, "error");

  ASSERT_STREQ(str.str, "1:debug\n;2:info\n;3:warn\n;4:error\n;");

  log_set_hook(NULL, NULL);
  log_set_hook(NULL, NULL);

  str_reset(&str);
}
