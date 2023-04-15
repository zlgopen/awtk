#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/darray.h"
#include "tkc/cmd_args.h"
#include "gtest/gtest.h"

const cmd_arg_desc_t args_desc[] = {
    {"auto_type_promotion", "enable auto type promotion", "atp", VALUE_TYPE_INVALID},
    {"locale", "locale. eg: zh_CN", "l", VALUE_TYPE_STRING},
    {"strings_file", "strings file", "s", VALUE_TYPE_STRING},
    {"output", "output file name", "o", VALUE_TYPE_STRING}};

typedef struct _st_args_t {
  char* output;
  char* locale;
  char* strings_file;
  darray_t sources;
  bool_t auto_type_promotion;
} st_args_t;

static st_args_t* st_args_init(st_args_t* args) {
  return_value_if_fail(args != NULL, NULL);

  memset(args, 0x00, sizeof(st_args_t));
  darray_init(&(args->sources), 5, default_destroy, NULL);

  return args;
}

static ret_t st_args_reset(st_args_t* args) {
  return_value_if_fail(args != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(args->locale);
  TKMEM_FREE(args->output);
  TKMEM_FREE(args->strings_file);
  darray_deinit(&(args->sources));

  return RET_OK;
}

static ret_t st_args_set_locale(st_args_t* args, const char* locale) {
  return_value_if_fail(args != NULL && locale != NULL, RET_BAD_PARAMS);
  args->locale = tk_str_copy(args->locale, locale);
  return RET_OK;
}

static ret_t st_args_set_output(st_args_t* args, const char* output) {
  return_value_if_fail(args != NULL && output != NULL, RET_BAD_PARAMS);
  args->output = tk_str_copy(args->output, output);
  return RET_OK;
}

static ret_t st_args_set_strings_file(st_args_t* args, const char* strings_file) {
  return_value_if_fail(args != NULL && strings_file != NULL, RET_BAD_PARAMS);
  args->strings_file = tk_str_copy(args->strings_file, strings_file);
  return RET_OK;
}

static ret_t st_args_add_source(st_args_t* args, const char* filename) {
  return_value_if_fail(args != NULL && filename != NULL, RET_BAD_PARAMS);
  darray_push(&(args->sources), tk_strdup(filename));

  return RET_OK;
}

static ret_t st_args_on_arg(void* ctx, const char* name, const value_t* v) {
  st_args_t* args = (st_args_t*)ctx;
  if (tk_str_eq(name, "auto_type_promotion")) {
    args->auto_type_promotion = TRUE;
  } else if (tk_str_eq(name, "locale")) {
    st_args_set_locale(args, value_str(v));
  } else if (tk_str_eq(name, "output")) {
    st_args_set_output(args, value_str(v));
  } else if (tk_str_eq(name, "strings_file")) {
    st_args_set_strings_file(args, value_str(v));
  } else {
    if (*name != '-') {
      st_args_add_source(args, name);
    }
  }
  return RET_OK;
}

TEST(CmdArgs, basic1) {
  cmd_args_t parser;
  st_args_t args;
  const char* argv[] = {"st",
                        "--auto_type_promotion",
                        "--locale=zh_CN",
                        "--strings_file",
                        "strings.xml",
                        "--output",
                        "test.st",
                        "a.st",
                        "b.st"};
  const char* usage = "parse st text";
  st_args_init(&args);
  cmd_args_init(&parser, usage, args_desc, ARRAY_SIZE(args_desc), st_args_on_arg, &args);
  cmd_args_process(&parser, ARRAY_SIZE(argv), (char**)argv);
  ASSERT_STREQ(args.locale, "zh_CN");
  ASSERT_STREQ(args.output, "test.st");
  ASSERT_STREQ(args.strings_file, "strings.xml");
  ASSERT_EQ(args.sources.size, 2);

  ASSERT_STREQ((char*)darray_get(&(args.sources), 0), "a.st");
  ASSERT_STREQ((char*)darray_get(&(args.sources), 1), "b.st");

  st_args_reset(&args);
}

TEST(CmdArgs, basic2) {
  cmd_args_t parser;
  st_args_t args;
  const char* cmd_line =
      "--auto_type_promotion --locale=zh_CN --strings_file strings.xml a.st b.st";
  const char* usage = "parse st text";
  st_args_init(&args);
  cmd_args_init(&parser, usage, args_desc, ARRAY_SIZE(args_desc), st_args_on_arg, &args);
  cmd_args_process_str(&parser, cmd_line);
  ASSERT_STREQ(args.locale, "zh_CN");
  ASSERT_STREQ(args.strings_file, "strings.xml");
  ASSERT_EQ(args.sources.size, 2);

  ASSERT_STREQ((char*)darray_get(&(args.sources), 0), "a.st");
  ASSERT_STREQ((char*)darray_get(&(args.sources), 1), "b.st");

  st_args_reset(&args);
}
