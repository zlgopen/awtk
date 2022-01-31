/**
 * File:   plugin_manager.c
 * Author: AWTK Develop Team
 * Brief:  plugin manager
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-10-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/darray.h"
#include "tkc/plugin_manager.h"

typedef ret_t (*plugin_init_func_t)(void);
typedef ret_t (*plugin_deinit_func_t)(void);

typedef struct _plugin_t {
  char* lib_name;
  tk_dl_t* handle;
  plugin_manager_t* plugin_manager;
} plugin_t;

static const char* name_from_lib_name(char name[MAX_PATH + 1], const char* lib_name) {
  const char* start = lib_name + strlen(TK_DLL_PRE_NAME);
  const char* end = strstr(lib_name, TK_DLL_EXT_NAME);
  return_value_if_fail(end != NULL, NULL);

  tk_strncpy(name, start, tk_min(MAX_PATH, end - start));

  return name;
}

static ret_t plugin_destroy(plugin_t* plugin) {
  return_value_if_fail(plugin != NULL, RET_BAD_PARAMS);

  if (plugin->plugin_manager != NULL && plugin->handle != NULL) {
    char name[MAX_PATH + 1];
    char func[TK_FUNC_NAME_LEN + 1];
    tk_dl_t* handle = plugin->handle;
    const char* lib_name = plugin->lib_name;
    plugin_manager_t* plugin_manager = plugin->plugin_manager;

    memset(func, 0x00, sizeof(func));
    memset(name, 0x00, sizeof(name));
    if (plugin_manager->get_deinit != NULL &&
        plugin_manager->get_deinit(func, name_from_lib_name(name, lib_name)) == RET_OK) {
      log_debug("deinit func:%s\n", func);
    } else {
      tk_strncpy(func, TK_PLUGIN_DEINIT, TK_FUNC_NAME_LEN);
    }
    plugin_deinit_func_t deinit = (plugin_deinit_func_t)tk_dl_sym(handle, func);
    if (deinit != NULL) {
      ret_t ret = deinit();
      log_debug("%s: deinit %d\n", lib_name, ret);
    } else {
      log_debug("%s: no deinit func\n", lib_name);
    }
  }

  TKMEM_FREE(plugin->lib_name);
  if (plugin->handle != NULL) {
    tk_dl_close(plugin->handle);
    plugin->handle = NULL;
  }
  TKMEM_FREE(plugin);

  return RET_OK;
}

static plugin_t* plugin_create(const char* path, const char* lib_name,
                               plugin_manager_t* plugin_manager) {
  tk_dl_t* handle = NULL;
  plugin_t* plugin = NULL;
  char filename[MAX_PATH + 1];

  memset(filename, 0x00, sizeof(filename));
  return_value_if_fail(lib_name != NULL && path != NULL, NULL);

  path_build(filename, MAX_PATH, path, lib_name, NULL);
  handle = tk_dl_open(filename);

  if (handle == NULL) {
    log_debug("dlopen %s failed: %s\n", filename, tk_dl_error());
  }
  return_value_if_fail(handle != NULL, NULL);

  plugin = TKMEM_ZALLOC(plugin_t);
  if (plugin != NULL) {
    plugin->handle = handle;
    plugin->plugin_manager = plugin_manager;
    plugin->lib_name = tk_strdup(lib_name);
    if (plugin->lib_name == NULL) {
      plugin_destroy(plugin);
      plugin = NULL;
    }
  } else {
    tk_dl_close(handle);
  }

  if (plugin != NULL) {
    char name[MAX_PATH + 1];
    char func[TK_FUNC_NAME_LEN + 1];

    memset(name, 0x00, sizeof(name));
    memset(func, 0x00, sizeof(func));
    if (plugin_manager->get_init != NULL &&
        plugin_manager->get_init(func, name_from_lib_name(name, lib_name)) == RET_OK) {
      log_debug("init func:%s\n", func);
    } else {
      tk_strncpy(func, TK_PLUGIN_INIT, TK_FUNC_NAME_LEN);
    }
    plugin_init_func_t init = (plugin_init_func_t)tk_dl_sym(handle, func);
    if (init != NULL) {
      ret_t ret = init();
      log_debug("%s: init %d\n", lib_name, ret);
    } else {
      log_debug("%s: no init func\n", lib_name);
    }
  }

  return plugin;
}

static int32_t plugin_compare(plugin_t* plugin, const char* lib_name) {
  return strcmp(plugin->lib_name, lib_name);
}

plugin_manager_t* plugin_manager_create(const char* path, plugin_get_init_func_name_t get_init,
                                        plugin_get_deinit_func_name_t get_deinit) {
  plugin_manager_t* plugin_manager = NULL;
  return_value_if_fail(path != NULL && path_exist(path), NULL);
  plugin_manager = TKMEM_ZALLOC(plugin_manager_t);
  return_value_if_fail(plugin_manager != NULL, NULL);

  plugin_manager->path = tk_strdup(path);
  plugin_manager->get_init = get_init;
  plugin_manager->get_deinit = get_deinit;
  ENSURE(plugin_manager->path != NULL);

  darray_init(&(plugin_manager->plugins), 5, (tk_destroy_t)plugin_destroy,
              (tk_compare_t)plugin_compare);
  plugin_manager_reload(plugin_manager);

  return plugin_manager;
}

static ret_t plugin_manager_unload(plugin_manager_t* plugin_manager) {
  return_value_if_fail(plugin_manager != NULL, RET_BAD_PARAMS);

  darray_clear(&(plugin_manager->plugins));

  return RET_OK;
}

bool_t plugin_manager_exist(plugin_manager_t* plugin_manager, const char* lib_name) {
  return darray_find(&(plugin_manager->plugins), (void*)lib_name) != NULL;
}

ret_t plugin_manager_refresh(plugin_manager_t* plugin_manager) {
  fs_item_t item;
  fs_dir_t* dir = NULL;
  return_value_if_fail(plugin_manager != NULL, RET_BAD_PARAMS);

  dir = fs_open_dir(os_fs(), plugin_manager->path);
  return_value_if_fail(dir != NULL, RET_BAD_PARAMS);

  memset(&item, 0x00, sizeof(item));
  while (fs_dir_read(dir, &item) == RET_OK) {
    if (!item.is_reg_file) {
      continue;
    }

    if (strstr(item.name, TK_DLL_EXT_NAME) != NULL) {
      if (!plugin_manager_exist(plugin_manager, item.name)) {
        plugin_t* plugin = plugin_create(plugin_manager->path, item.name, plugin_manager);
        if (plugin != NULL) {
          if (darray_push(&(plugin_manager->plugins), plugin) != RET_OK) {
            plugin_destroy(plugin);
            return RET_OOM;
          }
        }
      } else {
        log_debug("%s exist\n", item.name);
      }
    } else {
      log_debug("skip %s\n", item.name);
    }
  }
  fs_dir_close(dir);

  return RET_OK;
}

ret_t plugin_manager_reload(plugin_manager_t* plugin_manager) {
  return_value_if_fail(plugin_manager != NULL, RET_BAD_PARAMS);
  plugin_manager_unload(plugin_manager);

  return plugin_manager_refresh(plugin_manager);
}

ret_t plugin_manager_dump(plugin_manager_t* plugin_manager) {
  uint32_t i = 0;
  return_value_if_fail(plugin_manager != NULL, RET_BAD_PARAMS);

  for (i = 0; i < plugin_manager->plugins.size; i++) {
    plugin_t* iter = (plugin_t*)(plugin_manager->plugins.elms[i]);
    log_debug("%u: %s %p\n", i, iter->lib_name, iter->handle);
  }

  return RET_OK;
}

ret_t plugin_manager_destroy(plugin_manager_t* plugin_manager) {
  return_value_if_fail(plugin_manager != NULL, RET_BAD_PARAMS);
  plugin_manager_unload(plugin_manager);

  TKMEM_FREE(plugin_manager->path);
  darray_deinit(&(plugin_manager->plugins));
  TKMEM_FREE(plugin_manager);

  return RET_OK;
}
