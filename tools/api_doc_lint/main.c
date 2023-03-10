/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  main
 *
 * Copyright (c) 2022 - 2023 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-03-07 Wang JunSheng <wangjunsheng@zlg.cn> created
 *
 */

#include "api_doc.h"
#include "awtk.h"

typedef struct _foreach_ctx_t {
  const char* extname;
  void* log_ctx;
  log_hook_t log_hook;
  bool_t auto_fix;
  code_assist_t* ca;
  int32_t nchecked;
} foreach_ctx_t;

static ret_t on_file(foreach_ctx_t* ctx, const char* filename) {
  const char* extname = ctx->extname;
  bool_t checked = FALSE;

  if (*extname != 0) {
    if (!tk_str_end_with(filename, extname)) {
      return RET_OK;
    }
  }
  
  check_api_doc(ctx->ca, filename, ctx->log_hook, ctx->log_ctx, ctx->auto_fix, &checked);
  if (checked) {
    ++ctx->nchecked;
  }
  return RET_OK;
}

static int usage(const char* exe) {
  char exename[MAX_PATH+1] = {0};
  path_basename_ex(exe, TRUE, exename, sizeof(exename));
  printf("e.g:\n");
  printf("%s src\n", exename);
  printf("%s f:/awtk/src\n", exename);
  printf("%s F:/awtk/src/base/widget.c\n", exename);
  printf("%s f:/awtk/src --fix\n\n", exename);
  printf("Usage: %s path \n", exename);
  printf("\n");
  printf("  path        both abs or relative path are ok\n");
  printf("  --fix       auto fix error(experimental)\n");
  printf("  -h, --help  show usage\n");
  return 0;
}

typedef struct _out_info_t {
  int32_t nwarn;
  int32_t nerr;
} out_info_t;

void log_to_str(void* ctx, log_level_t level, const char* s) {
  out_info_t* info = (out_info_t*)ctx;
  if (s) {
    if (level == LOG_LEVEL_WARN) {
      ++info->nwarn;
    } else if (level == LOG_LEVEL_ERROR) {
      ++info->nerr;
    }
    printf("%s", s);
  }
}

static void set_path(const char* path, str_t* dir, str_t* file) {
  if (!path_is_abs(path)) {
    char cwd[MAX_PATH + 1] = {0};
    path_cwd(cwd);
    str_set(dir, cwd);
    str_append_more(dir, "/", path, NULL);

    if (file_exist(dir->str)) {
      str_set(file, dir->str);
      str_clear(dir);
    }
  } else {
    if (path_exist(path)) {
      str_set(dir, path);
    } else if (file_exist(path)) {
      str_set(file, path);
    }
  }
}

int main(int argc, char* argv[]) {
  char exe[MAX_PATH+1] = {0};
  out_info_t out = {0};
  str_t dir_path;
  str_t file_path;
  foreach_ctx_t ctx = { .extname = ".h", .log_ctx = &out, .log_hook = log_to_str};
  ctx.ca = code_assist_create();

  path_exe(exe);
  str_init(&dir_path, 256);
  str_init(&file_path, 256);

  const char* path = "src";
  for (int i = 1; i < argc; ++i) {
    if (0 == strcmp(argv[i], "--fix")) {
      ctx.auto_fix = TRUE;
    } else if (0 == strcmp(argv[i], "--help") || 0 == strcmp(argv[i], "-h")) {
      return usage(exe);
    } else if (*argv[i] == '-') {
      printf("ignore unknown argv : %s\n", argv[i]);
      continue;
    } else {
      path = argv[i];
    }
  }
  set_path(path, &dir_path, &file_path);

  char path_nor[MAX_PATH+1] = {0};
  if (dir_path.size > 0) {
    path_normalize(dir_path.str, path_nor, MAX_PATH);
    printf("check dir : %s\n", path_nor);
    fs_foreach_file(path_nor, (tk_visit_t)on_file, &ctx);
  } else if (file_path.size > 0) {
    path_normalize(file_path.str, path_nor, MAX_PATH);
    printf("check file : %s\n", path_nor);
    ctx.extname = "";
    on_file(&ctx, path_nor);
  }
  
  str_reset(&dir_path);
  str_reset(&file_path);

  printf("checked number of files(%d)\n", ctx.nchecked);
  if (0 == out.nerr + out.nwarn) {
    printf("check ok! \n");
  } else {
    printf("errors(%d) warnings(%d) \n", out.nerr, out.nwarn);
  }

  code_assist_destroy(ctx.ca);
  return out.nerr + out.nwarn;
}
