#include "tkc/utils.h"
#include "gtest/gtest.h"
#include "tkc/plugin_manager.h"
#include <string>

using std::string;

static string name_to_libname(const char* name) {
  return string(TK_DLL_PRE_NAME) + string(name) + string(TK_DLL_EXT_NAME);
}

static ret_t get_init(char func_name[TK_NAME_LEN + 1], const char* name) {
  tk_snprintf(func_name, TK_NAME_LEN, "%s_register", name);
  return RET_OK;
}

static ret_t get_deinit(char func_name[TK_NAME_LEN + 1], const char* name) {
  tk_snprintf(func_name, TK_NAME_LEN, "%s_unregister", name);
  return RET_OK;
}

TEST(PluginManager, basic) {
  plugin_manager_t* plugin_manager =
      plugin_manager_create("bin/plugins_for_test", get_init, get_deinit);

  ASSERT_EQ(plugin_manager_exist(plugin_manager, name_to_libname("a").c_str()), TRUE);
  ASSERT_EQ(plugin_manager_exist(plugin_manager, name_to_libname("b").c_str()), TRUE);
  ASSERT_EQ(plugin_manager_exist(plugin_manager, name_to_libname("c").c_str()), FALSE);
  ASSERT_EQ(plugin_manager_reload(plugin_manager), RET_OK);
  ASSERT_EQ(plugin_manager_refresh(plugin_manager), RET_OK);
  ASSERT_EQ(plugin_manager_exist(plugin_manager, name_to_libname("a").c_str()), TRUE);
  ASSERT_EQ(plugin_manager_exist(plugin_manager, name_to_libname("b").c_str()), TRUE);
  ASSERT_EQ(plugin_manager_exist(plugin_manager, name_to_libname("c").c_str()), FALSE);

  plugin_manager_destroy(plugin_manager);
}
