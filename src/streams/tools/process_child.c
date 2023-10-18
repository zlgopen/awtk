#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[]) {
  char ret;
  int i = 0;

  srand((unsigned int)time(NULL));

  for (i = 0; i < argc; i++) {
    printf("argv[%d]:%s \r\n", i, argv[i]);
  }

  printf("Do you want to continue ? [y/n] \r\n");
  fflush(stdout);

  ret = getchar();
  if (!(ret == 'y' || ret == 'Y')) {
    printf("exit:%c \r\n", ret);
  } else {
    for (i = 0; i < 100; i++) {
      printf("random[%d]:%d \r\n", i, rand() % 100);
    }
  }
  fflush(stdout);
#ifdef WIN32
  Sleep(1000 * 5);
#else
  usleep(1000 * 1000 * 5);
#endif
  return 0;
}
