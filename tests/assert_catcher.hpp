#include <csignal>
#include <csetjmp>
#include "gtest/gtest.h"
#include "tkc/types_def.h"

#ifndef ASSERT_CATCHER_HPP
#define ASSERT_CATCHER_HPP

#if defined(NDEBUG)
#define ASSERT_CATCH(sentence) sentence
#else
class AssertCatcher {
 public:
  AssertCatcher() {
    failed_ = false;
    old_handler_ = signal(SIGABRT, AssertHandler);
  }

  ~AssertCatcher() {
    signal(SIGABRT, old_handler_);
  }

  bool failed() const {
    return failed_;
  }
  void reset() {
    failed_ = false;
  }

  bool begin() {
    return setjmp(jmp_env_) == 0;
  }

 private:
  static void AssertHandler(int sig) {
    failed_ = true;
    longjmp(jmp_env_, 1);
  }

  static jmp_buf jmp_env_;
  static bool failed_;
  void (*old_handler_)(int);
};

jmp_buf AssertCatcher::jmp_env_;
bool AssertCatcher::failed_ = false;

#define ASSERT_CATCH(sentence)                                      \
  do {                                                              \
    AssertCatcher TK_CONCAT(_catcher, __LINE__);                    \
    if (TK_CONCAT(_catcher, __LINE__).begin()) {                    \
      sentence;                                                     \
    }                                                               \
    EXPECT_TRUE(TK_CONCAT(_catcher, __LINE__).failed());           \
  } while (0)
#endif /* NDEBUG */

#endif /* ASSERT_CATCHER_HPP */
