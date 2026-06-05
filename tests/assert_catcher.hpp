#include "gtest/gtest.h"
#include "tkc/types_def.h"

#ifndef ASSERT_CATCHER_HPP
#define ASSERT_CATCHER_HPP

#if defined(NDEBUG)
#define ASSERT_CATCH(sentence) sentence
#elif defined(_MSC_VER)
#include <csignal>
#include <functional>

// On Windows MSVC, longjmp from abort() signal handler causes STATUS_BAD_STACK (0xc0000028),
// and even after __except catches it, the stack remains corrupted causing segfault.
// Instead, the signal handler uses RaiseException to trigger a SEH exception directly,
// which __except catches cleanly without longjmp's stack restoration issues.

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

  static bool failed_;

 private:
  static void AssertHandler(int sig) {
    (void)sig;
    failed_ = true;
    // Raise a SEH exception instead of longjmp.
    // longjmp would cause STATUS_BAD_STACK which corrupts the stack,
    // but RaiseException triggers the exception cleanly for __except to catch.
    RaiseException(0xc0000028, 0, 0, NULL);
  }

  void (*old_handler_)(int);
};

bool AssertCatcher::failed_ = false;

static std::function<void()> g_assert_catch_sentence;

// SEH helper: __try/__except requires no local C++ objects with destructors.
static bool RunWithSEHCatch() {
  bool caught = false;
  __try {
    g_assert_catch_sentence();
  } __except(AssertCatcher::failed_ ?
             EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
    caught = true;
  }
  return caught;
}

#define ASSERT_CATCH(sentence)                                          \
  do {                                                                  \
    AssertCatcher TK_CONCAT(_catcher, __LINE__);                        \
    g_assert_catch_sentence = [&]() { sentence; };                      \
    RunWithSEHCatch();                                                  \
    g_assert_catch_sentence = nullptr;                                  \
    EXPECT_TRUE(TK_CONCAT(_catcher, __LINE__).failed());               \
  } while (0)
#else
#include <csignal>
#include <csetjmp>

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
    (void)sig;
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
#endif

#endif /* ASSERT_CATCHER_HPP */
