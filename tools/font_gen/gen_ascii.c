#include <stdio.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
  unsigned char c = 0;
  FILE* fp = fopen("ascii.txt", "wb+");
  if (fp != NULL) {
    for (c = 0; c < 128; c++) {
      if (isprint(c) && !isspace(c)) {
        printf("%c", c);
        fprintf(fp, "%c", c);
      }
    }
    fclose(fp);
  }

  return 0;
}
