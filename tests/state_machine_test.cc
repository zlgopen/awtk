#include "gtest/gtest.h"

extern "C" void state_machine_utest(void);

TEST(StateMachine, utest) {
  state_machine_utest();
}
