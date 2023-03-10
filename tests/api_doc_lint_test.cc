#include "tools/api_doc_lint/api_doc.h"
#include "gtest/gtest.h"
#include "gtest/gtest_helper.h"

#include "awtk.h"
#include <string>
using std::string;

static char exe[MAX_PATH+1];

/*
./bin/runTest "--gtest_filter=api_doc.*"
*/

static void gen_path(const char* exe, const char* file, string& out) {
  char full[MAX_PATH+1] = {0};
  char normalized[MAX_PATH+1] = {0};
  tk_snprintf(full, sizeof(full), "%s%s", exe, file);
  path_normalize(full, normalized, MAX_PATH);
  out = normalized;
}

typedef struct _out_info_t {
  int32_t nwarn;
  int32_t nerr;
  str_t err;
  str_t warn;
} out_info_t;

void log_to_str(void* ctx, log_level_t level, const char* s) {
  out_info_t* info = (out_info_t*)ctx;
  if (s) {
    if (level == LOG_LEVEL_WARN) {
      ++info->nwarn;
      str_append(&info->warn, s);
    } else if (level == LOG_LEVEL_ERROR) {
      ++info->nerr;
      str_append(&info->err, s);
    }
    // printf("%s", s);
  }
}

static const char* errs[] = {
  "(60): error: @property's name invalid: lang2 -- lang\n",
  "(67): error: @property's type invalid: char* -- const char*\n",
  "(172): error: @method name invalid, expect(code_edit_create) -- actual(code_edit_createx)\n",
  "(186): error: @method name invalid, expect(code_edit_cast) -- actual(code_edit_cast,)\n",
  "(310): error: @param_1's name invalid: size -- fold\n",
  "(616): error: @parem_2 slop missing comment\n",
  "(651): error: not enough @param (1 / 2)\nsuggest:\n * @param {int64_t} pos \n",
  "(656): error: unknown annotation \"scriptable1\"\n",
  "(657): error: @param_0's type invalid: widget_t -- widget_t*\n",
  "(677): error: @return type invalid ret_t -- char*\n",
  "(685): error: @param_0's name invalid: widget -- s\n",
  "(686): error: too much @param\n",
  "(829): error: nothing can be written between @param\n",
  "(934): error: unknown annotation \"string1\"\n",
  "(940): error: @const name invalid CODE_EDIT_PROP_LANG1 -- CODE_EDIT_PROP_LANG\n",
  "(1096): error: @prefix name missing\n"
};

static const char* warns[] = {
  "(1076): warning: @prefix missing\n",
};

void dummy_log(void* ctx, log_level_t level, const char* s) {
}


void auto_fix_hook(const char* path, const char* actual, uint32_t size) {
  //file_write("f:/foo.h", actual, size);

  string file;
  gen_path(exe, "/../../tests/testdata/fixed.h", file);

  uint32_t len;
  char* expect_result = (char*)file_read(file.c_str(), &len);

  ASSERT_STREQ_UNIX(actual, expect_result);

  TKMEM_FREE(expect_result);
}

TEST(api_doc, basic) {
  code_assist_t* ca;
  string file1;

  out_info_t out = {0};
  str_init(&out.err, 1024);
  str_init(&out.warn, 1024);
  ca = code_assist_create();
  path_exe(exe);
  gen_path(exe, "/../../tests/testdata/apidoc_test.h", file1);
  
  check_api_doc(ca, file1.c_str(), log_to_str, &out, FALSE, NULL);

  uint32_t i = 0;
  string expErr;
  for (i = 0; i < sizeof(errs)/sizeof(*errs); ++i) {
    const char* err = errs[i];
    expErr += file1 + err;
  }
  ASSERT_EQ(expErr, out.err.str);

  string expWarn;
  for (i = 0; i < sizeof(warns)/sizeof(*warns); ++i) {
    const char* warn = warns[i];
    expWarn += file1 + warn;
  }
  ASSERT_EQ(expWarn, out.warn.str);

  str_reset(&out.err);
  str_reset(&out.warn);

  check_api_doc2(ca, file1.c_str(), dummy_log, NULL, TRUE, auto_fix_hook, NULL);
  code_assist_destroy(ca);
}
