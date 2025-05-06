#include "tkc/socket_helper.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"

#if 0
TEST(socket_helper, get_ip_by_ifname) {
  darray_t ips;
  darray_init(&ips, 10, default_destroy, NULL);
  ret_t ret = tk_socket_get_ips_by_ifname(L"以太网 3", &ips);
  ASSERT_EQ(ret, RET_OK);
  ASSERT_EQ(ips.size, 4);
  for (size_t i = 0; i < ips.size; i++) {
    printf("%s \r\n", (char*)darray_get(&ips, i));
  }
  darray_deinit(&ips);
}
#endif
