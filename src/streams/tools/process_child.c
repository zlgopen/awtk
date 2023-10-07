#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef WIN32
#include <unistd.h>
#endif

int main(int argc, char* argv[]) {
  char ret;
  int i = 0;

  srand((unsigned int)time(NULL));
  setbuf(stdin, NULL);
  for (i = 0; i < argc; i++) {
    printf("argv[%d]:%s \r\n", i, argv[i]);
  }

  printf("Do you want to continue ? [y/n] \r\n");
  fflush(stdin);
  fflush(stdout);
#ifndef WIN32
  usleep(1000 * 100);
#endif
  ret = getchar();
  if (!(ret == 'y' || ret == 'Y')) {
    printf("exit:%c \r\n", ret);
  } else {
    for (i = 0; i < 100; i++) {
      printf("random[%d]:%d \r\n", i, rand() % 100);
    }
  }
  return 0;
}
