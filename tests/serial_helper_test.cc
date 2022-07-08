#include "streams/serial/serial_helper.h"
#include "gtest/gtest.h"

TEST(SerialHelper, stopbits) {
  ASSERT_EQ(serial_stopbits_from_str("1"), stopbits_one);
  ASSERT_EQ(serial_stopbits_from_str("2"), stopbits_two);
  ASSERT_EQ(serial_stopbits_from_str("1.5"), stopbits_one_point_five);
  ASSERT_EQ(serial_stopbits_from_str("one"), stopbits_one);
  ASSERT_EQ(serial_stopbits_from_str("two"), stopbits_two);
  ASSERT_EQ(serial_stopbits_from_str("one_point_five"), stopbits_one_point_five);
}

TEST(SerialHelper, parity) {
  ASSERT_EQ(serial_parity_from_str(NULL), parity_none);
  ASSERT_EQ(serial_parity_from_str("none"), parity_none);
  ASSERT_EQ(serial_parity_from_str("even"), parity_even);
  ASSERT_EQ(serial_parity_from_str("odd"), parity_odd);
  ASSERT_EQ(serial_parity_from_str("mark"), parity_mark);
  ASSERT_EQ(serial_parity_from_str("space"), parity_space);
}

TEST(SerialHelper, bytesize) {
  ASSERT_EQ(serial_bytesize_from_str(NULL), eightbits);
  ASSERT_EQ(serial_bytesize_from_str("8"), eightbits);
  ASSERT_EQ(serial_bytesize_from_str("eight"), eightbits);
  ASSERT_EQ(serial_bytesize_from_str("7"), sevenbits);
  ASSERT_EQ(serial_bytesize_from_str("seven"), sevenbits);
  ASSERT_EQ(serial_bytesize_from_str("6"), sixbits);
  ASSERT_EQ(serial_bytesize_from_str("six"), sixbits);
  ASSERT_EQ(serial_bytesize_from_str("5"), fivebits);
  ASSERT_EQ(serial_bytesize_from_str("five"), fivebits);
}

TEST(SerialHelper, flowcontrol) {
  ASSERT_EQ(serial_flowcontrol_from_str(NULL), flowcontrol_none);
  ASSERT_EQ(serial_flowcontrol_from_str("none"), flowcontrol_none);
  ASSERT_EQ(serial_flowcontrol_from_str("soft"), flowcontrol_software);
  ASSERT_EQ(serial_flowcontrol_from_str("hard"), flowcontrol_hardware);
}
