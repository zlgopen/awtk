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
  FILE* file = NULL;

  srand((unsigned int)time(NULL));

  file = fopen("awtk_main.inc", "r");
  if (file == NULL) {
    printf("set work dir fail \r\n");
    return -1;
  } else {
    fclose(file);
    printf("set work dir success \r\n");
  }

  for (i = 0; i < argc; i++) {
    printf("argv[%d]:%s \r\n", i, argv[i]);
  }

  printf("Do you want to continue ? [y/n] \r\n");
  fflush(stdout);

  ret = getchar();
  if (!(ret == 'y' || ret == 'Y')) {
    printf("exit_code:%d \r\n", ret);
  } else {
    for (i = 0; i < 100; i++) {
      printf("random[%d]:%d \r\n", i, rand() % 100);
    }
  }
  printf("++++++++++++ end \r\n");
  fflush(stdout);
  return ret;
}
