#include "common.h"
#include "gtest/gtest.h"
#include "base/locale_info.h"
#include "tools/str_gen/str_gen.h"
#include "tools/str_gen/xml_str_gen.h"

static void TestStrGen(const char* strs[]) {
  StrGen sg;
  size_t nr = 0;
  uint8_t* p = NULL;
  uint8_t buff[1024 * 4];
  vector<string> vstrs;
  for (size_t i = 0; strs[i]; i++) {
    string key = strs[i];
    string value = string("++") + key;

    sg.Add("en", Sentence(key, value));
    sg.Add("zh_CN", Sentence(key, value));
    sg.Add("zh_TW", Sentence(key, value));

    vstrs.push_back(key);
    nr++;
  }

  vector<string> languages = sg.GetLanguages();

  ASSERT_EQ(languages.size(), 3);
  for (size_t k = 0; k < languages.size(); k++) {
    string lang = languages[k];
    str_table_t* table = (str_table_t*)buff;
    p = sg.Output(lang, buff, sizeof(buff));

    ASSERT_EQ(table->nr, nr);
    std::sort(vstrs.begin(), vstrs.end());

    for (size_t i = 0; strs[i]; i++) {
      string key = vstrs[i];
      string value = string("++") + key;
      const char* k = (const char*)(buff + table->strs[i].key);
      const char* v = (const char*)(buff + table->strs[i].value);
      ASSERT_EQ(k, key);
      ASSERT_EQ(v, value);
      ASSERT_EQ(value, str_table_lookup(table, k));
    }
  }
}

TEST(StrGen, basic1) {
  const char* strs[] = {"Ok", NULL};
  TestStrGen(strs);
}

TEST(StrGen, basic2) {
  const char* strs[] = {"Ok", "Cancel", NULL};
  TestStrGen(strs);
}

TEST(StrGen, basic3) {
  const char* strs[] = {"Ok", "Cancel", "Test", NULL};
  TestStrGen(strs);
}

TEST(StrGen, basicN) {
  const char* strs[] = {"Ok", "Cancel", "Test", "Aa", "bB", "CC", NULL};
  TestStrGen(strs);
}

TEST(StrGen, xml1) {
  StrGen sg;
  uint8_t buff[1024 * 4];
  const char* xml =
      "<string name=\"ok\"> \
    <language name=\"en_US\">OK</language> \
    <language name=\"zh_CN\">确定</language> \
    </string> \
    <string name=\"abc\"> \
    <language name=\"en_US\">a&lt;b&gt;c</language> \
    <language name=\"zh_CN\">a&quota;b&amp;c</language> \
    </string> \
    <string name=\"cancel\"> \
    <language name=\"en_US\">Cancel</language> \
    <language name=\"zh_CN\">取消</language> \
    </string>";

  xml_buff_to_str_gen(xml, &sg);
  vector<string> languages = sg.GetLanguages();

  ASSERT_EQ(languages.size(), 2);
  str_table_t* table = (str_table_t*)buff;
  sg.Output("en_US", buff, sizeof(buff));
  ASSERT_EQ(string("OK"), str_table_lookup(table, "ok"));
  ASSERT_EQ(string("Cancel"), str_table_lookup(table, "cancel"));
  ASSERT_EQ(string("a<b>c"), str_table_lookup(table, "abc"));

  sg.Output("zh_CN", buff, sizeof(buff));
  assert_str_eq(L"确定", str_table_lookup(table, "ok"));
  assert_str_eq(L"取消", str_table_lookup(table, "cancel"));

  ASSERT_EQ(string("a\"b&c"), str_table_lookup(table, "abc"));
  ASSERT_EQ(NULL, str_table_lookup(table, "not exist"));
}
