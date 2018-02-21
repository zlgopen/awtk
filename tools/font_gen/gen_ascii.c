#include <stdio.h>

int main(int argc, char* argv[]) {
  char c = 0;
  FILE* fp = fopen("ascii.txt", "wb+");
  if (fp != NULL) {
    for (c = '0'; c <= '9'; c++) {
      fprintf(fp, "%c", c);
    }
    for (c = 'a'; c <= 'z'; c++) {
      fprintf(fp, "%c", c);
    }
    for (c = 'A'; c <= 'Z'; c++) {
      fprintf(fp, "%c", c);
    }
    fprintf(fp, "%c", ',');
    fprintf(fp, "%c", '.');
    fprintf(fp, "%c", ';');
    fprintf(fp, "%c", '?');
    fclose(fp);
  }

  return 0;
}
