#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/buffer.h"
#include "tkc/platform.h"
#include "streams/socket_helper.h"

#define PORT     8080

// Driver code
int main() {
    wbuffer_t wb;
    int sockfd = 0;
    struct sockaddr_in cliaddr;

    socket_init();
    wbuffer_init_extendable(&wb);
    sockfd = udp_listen(PORT);

    log_debug("udp listen %d %d\n", PORT, sockfd);
    return_value_if_fail(sockfd >= 0, 0);

    do {
      int32_t ret = 0;
      uint32_t size = 0;
      int32_t addr_size = 0;

      memset(&cliaddr, 0, sizeof(cliaddr));
      socket_wait_for_data(sockfd, 5000);
      ret = recvfrom(sockfd, (char *)&size, sizeof(size), MSG_WAITALL, ( struct sockaddr *) &cliaddr, &addr_size);
      log_debug("ret=%d\n", ret);
      if(ret < 0) {
        continue;
      }
      
      socket_wait_for_data(sockfd, 5000);
      return_value_if_fail(ret == sizeof(size), 0);
      return_value_if_fail(wbuffer_extend_capacity(&wb, size) == RET_OK, 0);
      ret = recvfrom(sockfd, (char *)wb.data, size, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &addr_size);
    }while(1);
    socket_close(sockfd);

    socket_deinit();
    wbuffer_deinit(&wb);

    return 0;
}
