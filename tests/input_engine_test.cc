#include "gtest/gtest.h"
#include "base/input_method.h"
#include <string>

using namespace std;

#if defined(WITH_IME_T9)
#include "input_engines/t9_zh_cn.inc"
#include "input_engines/t9_en_us.inc"
#elif defined(WITH_IME_T9EXT)
#include "input_engines/t9ext_zh_cn.inc"
#endif

#if defined(WITH_IME_T9) || defined(WITH_IME_T9EXT) || defined(WITH_IME_SPINYIN)
#include "input_engines/pinyin_table.inc"
static const wchar_t* s_table_num_chars[] = {
    L"", L"", L"ABC", L"DEF", L"GHI", L"JKL", L"MNO", L"PQRS", L"TUV", L"WXYZ", L"",
};
#elif defined(WITH_IME_PINYIN)
static const char s_test_gpinyin[] = {
    106,  105,  0, -27, -113, -118, 0, -27, -115, -77,  0, -25, -70,  -89,  0, -27,
    -121, -96,  0, -26, -100, -70,  0, -26, -105, -94,  0, -26, -128, -91,  0, -23,
    -101, -122, 0, -24, -82,  -80,  0, -26, -98,  -127, 0, -27, -81,  -124, 0, -23,
    -72,  -95,  0, -27, -97,  -70,  0, -25, -69,  -89,  0, -27, -112, -119, 0, -24,
    -82,  -95,  0, -26, -116, -92,  0, -27, -83,  -93,  0, -27, -119, -126, 0, -26,
    -102, -88,  0, -25, -70,  -86,  0, -27, -73,  -79,  0, -25, -89,  -81,  0, -25,
    -79,  -115, 0, -26, -118, -128, 0, -27, -121, -69,  0, -27, -89,  -84,  0, -24,
    -126, -116, 0, -26, -75,  -114, 0, -27, -65,  -116, 0, -25, -91,  -83,  0, -25,
    -106, -66,  0, -26, -65,  -128, 0, -24, -65,  -71,  0, -24, -66,  -111, 0, -23,
    -103, -123, 0, -27, -122, -128, 0, -27, -113, -67,  0, -27, -81,  -126, 0, -27,
    -90,  -109, 0, -24, -124, -118, 0, -23, -91,  -91,  0, -23, -100, -127, 0, -26,
    -120, -97,  0, -23, -86,  -91,  0, -24, -105, -119, 0, -25, -69,  -87,  0, -26,
    -79,  -78,  0, -26, -93,  -104, 0, -26, -105, -95,  0, -23, -85,  -69,  0, -25,
    -97,  -74,  0, -27, -67,  -111, 0, -24, -109, -97,  0, -28, -70,  -68,  0, -25,
    -107, -72,  0, -28, -68,  -114, 0, -25, -82,  -107, 0, -27, -108, -89,  0, -28,
    -72,  -116, 0, -24, -82,  -91,  0, -25, -88,  -67,  0, -25, -107, -65,  0, -25,
    -77,  -69,  0, -28, -72,  -82,  0, -27, -127, -120, 0, -25, -66,  -127, 0, -24,
    -88,  -104, 0, -26, -87,  -97,  0, -27, -85,  -119, 0, -26, -91,  -75,  0, -25,
    -68,  -119, 0, -25, -88,  -73,  0, -27, -75,  -121, 0, -27, -71,  -66,  0, -23,
    -78,  -85,  0, -28, -70,  -97,  0, -24, -88,  -120, 0, -26, -126, -72,  0, -25,
    -114, -111, 0, -25, -76,  -102, 0, -27, -79,  -112, 0, -24, -75,  -115, 0, -23,
    -118, -120, 0, -25, -76,  -128, 0, -26, -100, -97,  0, -26, -82,  -101, 0, -25,
    -84,  -120, 0, -24, -89,  -118, 0, -27, -123, -74,  0, -27, -110, -83,  0, -23,
    -70,  -126, 0, -27, -91,  -121, 0, -26, -120, -94,  0, -27, -75,  -76,  0, -26,
    -91,  -85,  0, -27, -119, -92,  0, -23, -101, -98,  0, -24, -73,  -69,  0, -25,
    -71,  -68,  0, -28, -67,  -74,  0, -25, -69,  -103, 0, -23, -112, -106, 0, -27,
    -100, -66,  0, -25, -104, -96,  0, -23, -86,  -111, 0, -25, -84,  -124, 0, -27,
    -106, -98,  0, -24, -118, -88,  0, -27, -119, -98,  0, -26, -76,  -114, 0, -26,
    -109, -96,  0, -27, -118, -111, 0, -26, -68,  -120, 0, -27, -117, -93,  0, -25,
    -87,  -115, 0, -26, -110, -125, 0, -25, -72,  -66,  0, -24, -118, -80,  0, -24,
    -68,  -81,  0, -26, -85,  -123, 0, -28, -71,  -87,  0, -26, -114, -114, 0, -25,
    -118, -79,  0, -26, -109, -118, 0, -26, -65,  -97,  0, -23, -67,  -111, 0, -23,
    -99,  -87,  0, -28, -68,  -117, 0, -24, -103, -82,  0, -27, -78,  -116, 0, -25,
    -67,  -67,  0, -24, -115, -96,  0, -27, -89,  -98,  0, -27, -112, -121, 0, -27,
    -94,  -68,  0, -23, -74,  -113, 0, -24, -107, -70,  0, -27, -78,  -117, 0, -24,
    -73,  -95,  0, -27, -65,  -93,  0, -23, -102, -101, 0, -27, -127, -82,  0, -24,
    -116, -115, 0, -24, -96,  -128, 0, -27, -67,  -74,  0, -26, -67,  -105, 0, -25,
    -73,  -99,  0, -24, -116, -92,  0, -25, -93,  -81,  0, -23, -78,  -102, 0, -24,
    -83,  -113, 0, -24, -81,  -104, 0, -24, -73,  -67,  0, -27, -115, -67,  0, -27,
    -104, -80,  0, -25, -118, -124, 0, -23, -91,  -111, 0, -24, -110, -70,  0, -25,
    -74,  -103, 0, -27, -90,  -128, 0, -26, -105, -93,  0, -24, -86,  -117, 0, -26,
    -71,  -110, 0, -26, -102, -87,  0, -26, -125, -114, 0, -24, -106, -70,  0, -24,
    -106, -118, 0, -25, -71,  -117, 0, -25, -66,  -120, 0, -23, -93,  -94,  0, -26,
    -68,  -125, 0, -23, -87,  -91,  0, -27, -66,  -101, 0, -24, -107, -128, 0, -24,
    -85,  -123, 0, -23, -73,  -124, 0, -26, -77,  -78,  0, -25, -87,  -124, 0, -24,
    -90,  -65,  0, -23, -74,  -70,  0, -26, -98,  -123, 0, -25, -66,  -121, 0, -23,
    -100, -67,  0, -27, -95,  -120, 0, -24, -103, -128, 0, -24, -85,  -108, 0, -25,
    -110, -93,  0, -23, -67,  -114, 0, -25, -102, -128, 0, -24, -71,  -97,  0, -24,
    -71,  -112, 0, -27, -113, -99,  0, -23, -98,  -65,  0, -24, -90,  -118, 0, -24,
    -70,  -117, 0, -24, -72,  -106, 0, -24, -89,  -103, 0, -23, -82,  -122, 0, -24,
    -90,  -84,  0, -24, -122, -116, 0, -25, -80,  -118, 0, -24, -121, -82,  0, -24,
    -90,  -104, 0, -26, -110, -96,  0, -26, -113, -92,  0, -25, -80,  -114, 0, -26,
    -89,  -119, 0, -23, -113, -74,  0, -25, -128, -79,  0, -23, -79,  -83,  0, -23,
    -67,  -113, 0, -25, -87,  -118, 0, -23, -102, -82,  0, -23, -72,  -124, 0, -27,
    -96,  -78,  0, -23, -80,  -74,  0, -25, -90,  -99,  0, -24, -93,  -102, 0, -25,
    -105, -75,  0, -24, -80,  -69,  0, -23, -67,  -116, 0, -24, -104, -69,  0, -27,
    -74,  -81,  0, -26, -121, -69,  0, -24, -128, -83,  0, -24, -104, -82,  0, -23,
    -118, -95,  0, -27, -120, -119, 0, -27, -70,  -76,  0, -23, -100, -75,  0, -24,
    -67,  -102, 0, -25, -107, -97,  0, -23, -79,  -128, 0, -23, -111, -121, 0, -23,
    -116, -92,  0, -26, -86,  -107, 0, -25, -103, -96,  0, -25, -103, -86,  0, -24,
    -91,  -117, 0, -26, -87,  -74,  0, -23, -73,  -111, 0, -23, -80,  -65,  0, -23,
    -81,  -102, 0, -24, -98,  -113, 0, -23, -97,  -78,  0, -25, -87,  -106, 0, -27,
    -95,  -119, 0, -23, -75,  -117, 0};
#endif

class candidate_info_t {
 public:
  char** candidates;
  uint32_t candidates_nr;
  uint32_t char_number;
  char* char_candidates;

  candidate_info_t() {
    candidates = NULL;
    candidates_nr = 0;
    char_number = 0;
    char_candidates = NULL;
  }

  void deint() {
    if (candidates != NULL) {
      for (size_t i = 0; i < candidates_nr; i++) {
        delete[] candidates[i];
      }
      delete[] candidates;
      candidates = NULL;
    }
    if (char_candidates != NULL) {
      delete[] char_candidates;
      char_candidates = NULL;
    }
    char_number = 0;
    candidates_nr = 0;
  }

  void init(im_candidates_event_t* evt) {
    const char* p_candidates = evt->candidates;

    deint();
    candidates = new char*[evt->candidates_nr];
    candidates_nr = evt->candidates_nr;
    for (uint32_t i = 0; i < candidates_nr; i++) {
      uint32_t len = strlen(p_candidates);
      candidates[i] = new char[len + 1];
      memcpy(candidates[i], p_candidates, len);
      candidates[i][len] = 0;
      p_candidates += (len + 1);
      char_number += len + 1;
    }

    char_candidates = new char[char_number];
    memcpy(char_candidates, evt->candidates, char_number);
  }
  ~candidate_info_t() {
    deint();
  }
};

TEST(input_engine, create) {
  input_method_t* im = input_method_create();
  ASSERT_EQ(im != NULL, TRUE);
  input_method_destroy(im);
}

static bool_t check_candidates_from_wchar(const char* candidates1, const wchar_t* candidates2,
                                          uint32_t nr) {
  char str[8];
  uint32_t n = 0;
  bool_t same = TRUE;
  while (*candidates2) {
    if (nr == n) {
      same = FALSE;
      break;
    }
    const char* c1 = candidates1;
    memset(str, 0x00, sizeof(str));
    tk_utf8_from_utf16_ex(candidates2, 1, str, sizeof(str));
    uint32_t n1 = strlen(c1);
    uint32_t n2 = strlen(str);
    if (!(n1 == n2 && strncmp(str, c1, n1) == 0)) {
      same = FALSE;
      break;
    }
    n++;
    candidates2++;
    candidates1 += (n1 + 1);
  }
  return same;
}

static bool_t check_candidates_from_char(const char* candidates1, const char** candidates2,
                                         uint32_t nr) {
  bool_t same = TRUE;
  for (uint32_t i = 0; i < nr; i++) {
    const char* c1 = candidates1;
    const char* c2 = candidates2[i];
    uint32_t n1 = strlen(c1);
    uint32_t n2 = strlen(c2);
    if (!(n1 == n2 && strncmp(c1, c2, n1) == 0)) {
      same = FALSE;
      break;
    }
    candidates1 += (n1 + 1);
  }
  return same;
}

static bool_t check_candidates_from_chars(const char** candidates1, const char** candidates2,
                                          uint32_t nr) {
  bool_t same = TRUE;
  for (uint32_t i = 0; i < nr; i++) {
    const char* c1 = candidates1[i];
    const char* c2 = candidates2[i];
    uint32_t n1 = strlen(c1);
    uint32_t n2 = strlen(c2);
    if (!(n1 == n2 && strncmp(c1, c2, n1) == 0)) {
      same = FALSE;
      break;
    }
  }
  return same;
}

TEST(input_engine, add_candidates) {
#define py_ji s_py_ji
  const char* key_str = "ji";

  const char key_char = '2';
  const char* test_str = "test";
  input_method_t* im = input_method_create();
  ASSERT_EQ(im != NULL, TRUE);
  ASSERT_EQ(im->engine != NULL, TRUE);

  input_engine_reset_candidates(im->engine);
  ASSERT_EQ(im->engine->candidates_nr, 0);

  input_engine_add_candidate(im->engine, test_str);
  ASSERT_EQ(im->engine->candidates_nr, 1);
  ASSERT_EQ(im->engine->candidates.data != NULL, TRUE);
  ASSERT_EQ(strncmp((const char*)(im->engine->candidates.data), test_str, strlen(test_str)), 0);

#if defined(WITH_IME_T9) || defined(WITH_IME_T9EXT) || defined(WITH_IME_SPINYIN)
  input_engine_reset_candidates(im->engine);
  input_engine_add_candidates_from_char(im->engine, s_table_num_chars, key_char);
  ASSERT_EQ(im->engine->candidates_nr, 3);
  ASSERT_EQ(im->engine->candidates.data != NULL, TRUE);
  ASSERT_EQ(check_candidates_from_wchar((const char*)(im->engine->candidates.data),
                                        s_table_num_chars[2], im->engine->candidates_nr),
            TRUE);

  input_engine_reset_candidates(im->engine);
  input_engine_add_candidates_from_string(im->engine, s_pinyin_chinese_items,
                                          ARRAY_SIZE(s_pinyin_chinese_items), key_str, FALSE);
  ASSERT_EQ(im->engine->candidates_nr, 63);
  ASSERT_EQ(im->engine->candidates.data != NULL, TRUE);
  ASSERT_EQ(check_candidates_from_char((const char*)(im->engine->candidates.data), py_ji, 63),
            TRUE);
#endif

  input_method_destroy(im);
}

static ret_t input_method_cc_on_im_candidates_event(void* ctx, event_t* e) {
  candidate_info_t* candidate_info = (candidate_info_t*)ctx;
  im_candidates_event_t* evt = (im_candidates_event_t*)e;
  candidate_info->init(evt);
  return RET_OK;
}

TEST(input_engine, dispatch_candidates) {
  const char* test_str = "test";
  candidate_info_t candidate_info;
  input_method_t* im = input_method_create();
  ASSERT_EQ(im != NULL, TRUE);
  ASSERT_EQ(im->engine != NULL, TRUE);

  uint32_t event_id = input_method_on(im, EVT_IM_SHOW_CANDIDATES,
                                      input_method_cc_on_im_candidates_event, &candidate_info);
  ASSERT_EQ(event_id != TK_INVALID_ID, TRUE);

  input_engine_reset_candidates(im->engine);
  ASSERT_EQ(im->engine->candidates_nr, 0);

  input_engine_add_candidate(im->engine, test_str);
  ASSERT_EQ(im->engine->candidates_nr, 1);
  ASSERT_EQ(im->engine->candidates.data != NULL, TRUE);
  ASSERT_EQ(strncmp((const char*)(im->engine->candidates.data), test_str, strlen(test_str)), 0);

  input_engine_dispatch_candidates(im->engine, 0);

  ASSERT_EQ(im->engine->candidates_nr, candidate_info.candidates_nr);
  ASSERT_EQ(check_candidates_from_char((const char*)(im->engine->candidates.data),
                                       (const char**)(candidate_info.candidates),
                                       candidate_info.candidates_nr),
            TRUE);

  input_method_destroy(im);
}

TEST(input_engine, input_key) {
  candidate_info_t candidate_info;
  candidate_info_t pre_candidate_info;
  input_method_t* im = input_method_create();
  ASSERT_EQ(im != NULL, TRUE);
  input_method_set(im);

  uint32_t event_id = input_method_on(input_method(), EVT_IM_SHOW_CANDIDATES,
                                      input_method_cc_on_im_candidates_event, &candidate_info);
  ASSERT_EQ(event_id != TK_INVALID_ID, TRUE);

  uint32_t pre_event_id =
      input_method_on(input_method(), EVT_IM_SHOW_PRE_CANDIDATES,
                      input_method_cc_on_im_candidates_event, &pre_candidate_info);
  ASSERT_EQ(pre_event_id != TK_INVALID_ID, TRUE);

#if defined(WITH_IME_T9) || defined(WITH_IME_T9EXT)
  input_method_dispatch_key(input_method(), '5');
  input_method_dispatch_key(input_method(), '4');
#if defined(WITH_IME_T9)
  ASSERT_EQ(check_candidates_from_chars((const char**)s_en_us_54,
                                        (const char**)(candidate_info.candidates),
                                        candidate_info.candidates_nr - 1),
            TRUE);
#elif defined(WITH_IME_T9EXT)
  ASSERT_EQ(
      check_candidates_from_chars((const char**)s_54, (const char**)(pre_candidate_info.candidates),
                                  pre_candidate_info.candidates_nr),
      TRUE);
  ASSERT_EQ(
      check_candidates_from_chars((const char**)s_py_li, (const char**)(candidate_info.candidates),
                                  candidate_info.candidates_nr),
      TRUE);
#endif
#else
  input_method_dispatch_key(input_method(), 'j');
  input_method_dispatch_key(input_method(), 'i');
#if defined(WITH_IME_SPINYIN)
  ASSERT_EQ(candidate_info.candidates_nr > 0, TRUE);
  ASSERT_EQ(strcmp("ji", candidate_info.candidates[0]), 0);
  char** s_test_py_ji = new char*[candidate_info.candidates_nr];
  s_test_py_ji[0] = new char[3];
  s_test_py_ji[0][0] = 'j';
  s_test_py_ji[0][1] = 'i';
  s_test_py_ji[0][2] = '\0';
  for (int32_t i = 1; i < candidate_info.candidates_nr; i++) {
    int32_t n = strlen(s_py_ji[i - 1]);
    s_test_py_ji[i] = new char[n + 1];
    memset(s_test_py_ji[i], 0x0, n + 1);
    memcpy(s_test_py_ji[i], s_py_ji[i - 1], n);
  }
  ASSERT_EQ(check_candidates_from_chars((const char**)s_test_py_ji,
                                        (const char**)(candidate_info.candidates),
                                        candidate_info.candidates_nr - 1),
            TRUE);
  for (int32_t i = 0; i < candidate_info.candidates_nr; i++) {
    delete s_test_py_ji[i];
  }
  delete[] s_test_py_ji;
#elif defined(WITH_IME_PINYIN)
  ASSERT_EQ(strncmp(s_test_gpinyin, candidate_info.char_candidates, candidate_info.char_number), 0);
#elif defined(WITH_IME_NULL)
#endif
#endif
  input_method_set(NULL);
  input_method_destroy(im);
}
