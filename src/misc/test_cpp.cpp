// Used to test if target platform support cpp

#include "tkc/mem.h"
#include "misc/new.hpp"

class Test {
 public:
  int count;
  Test() {
    this->count = 0;
  }
  ~Test() {
    this->count = 0;
  }

  void inc() {
    this->count++;
  }

  void dec() {
    this->count--;
  }
};

extern "C" void runCppTest() {
  Test* t = new Test();
  t->inc();
  t->dec();

  delete t;
}
