#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "common/utils.h"

static char s_name[1024];
static char s_text[1024];
static char s_type[1024];
static char s_flags[1024];
static char s_line[10240];

static void gen_xywh(FILE* fout, const char* p) {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;

  sscanf(p, "%d,%d,%d,%d", &x, &y, &w, &h);
  fprintf(fout, " x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" />\n", x, y, w, h);
}

void gen_button(FILE* fout, const char* p) {
  get_next_token(p, s_text, '\"');
  p = skip_to_next(p, ',');

  get_next_token(p, s_name, ',');
  p = skip_to_next(p, ',');

  fprintf(fout, "<button name=\"%s\" text=\"%s\"", to_lower(s_name), s_text);

  gen_xywh(fout, p);
}

void gen_label(FILE* fout, const char* p) {
  get_next_token(p, s_text, '\"');
  p = skip_to_next(p, ',');

  get_next_token(p, s_name, ',');
  p = skip_to_next(p, ',');

  fprintf(fout, "<label name=\"%s\" text=\"%s\"", to_lower(s_name), s_text);

  gen_xywh(fout, p);
}

void gen_control(FILE* fout, const char* p) {
  get_next_token(p, s_text, '\"');
  p = skip_to_next(p, ',');

  get_next_token(p, s_name, ',');
  p = skip_to_next(p, ',');

  get_next_token(p, s_type, ',');
  p = skip_to_next(p, ',');

  get_next_token(p, s_flags, ',');
  p = skip_to_next(p, ',');

  if (strstr(s_type, "msctls_progress32") != NULL) {
    fprintf(fout, "<progress_bar name=\"%s\" text=\"%s\"", to_lower(s_name), s_text);
  } else if (strstr(s_flags, "BS_AUTOCHECKBOX") != NULL) {
    fprintf(fout, "<check_button name=\"%s\" text=\"%s\"", to_lower(s_name), s_text);
  } else if (strstr(s_flags, "BS_AUTORADIOBUTTON") != NULL) {
    fprintf(fout, "<radio_button name=\"%s\" text=\"%s\"", to_lower(s_name), s_text);
  } else {
    fprintf(fout, "<unknow name=\"%s\" text=\"%s\"", to_lower(s_name), s_text);
  }

  gen_xywh(fout, p);
}

void gen_widget(FILE* fout) {
  const char* p = skip_char(s_line, ' ');
  if (start_with(p, "DEFPUSHBUTTON")) {
    gen_button(fout, skip_char(skip_to_next(s_line, ' '), ' '));
  } else if (start_with(p, "PUSHBUTTON")) {
    gen_button(fout, skip_char(skip_to_next(s_line, ' '), ' '));
  } else if (start_with(p, "CONTROL")) {
    gen_control(fout, skip_char(skip_to_next(s_line, ' '), ' '));
  } else if (start_with(p, "LTEXT")) {
    gen_label(fout, skip_char(skip_to_next(s_line, ' '), ' '));
  }
}

static const char* to_filename(char* filename, uint32_t size) {
  snprintf(filename, size, "%s.xml", s_name);
  return to_lower(filename);
}

void gen_window(FILE* fin) {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  FILE* fout = NULL;
  char filename[1024];
  char* p = strstr(s_line, "DIALOGEX");

  get_next_token(s_line, s_name, ' ');
  fout = fopen(to_filename(filename, sizeof(filename)), "wb+");
  sscanf(p, "DIALOGEX %d, %d, %d, %d", &x, &y, &w, &h);
  fprintf(fout, "<window name=\"%s\" x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" ", to_lower(s_name), x, y,
          w, h);

  fgets(s_line, sizeof(s_line), fin); /*STYLE*/
  fgets(s_line, sizeof(s_line), fin); /*CAPTION*/
  get_next_token(strchr(s_line, '\"'), s_text, '\"');
  fprintf(fout, "text=\"%s\">\n", s_text);
  fgets(s_line, sizeof(s_line), fin); /*FONT*/
  fgets(s_line, sizeof(s_line), fin); /*BEGIN*/

  while (fgets(s_line, sizeof(s_line), fin)) {
    if (strstr(s_line, "END") == NULL) {
      gen_widget(fout);
    } else {
      break;
    }
  }
  fprintf(fout, "</window>\n");
  fclose(fout);

  return;
}

void gen(const char* in) {
  FILE* fin = fopen(in, "r");
  while (fgets(s_line, sizeof(s_line), fin)) {
    if (strstr(s_line, "DIALOGEX") != NULL) {
      gen_window(fin);
    }
  }

  fclose(fin);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s rcfile\n", argv[0]);
    exit(0);
  }

  gen(argv[1]);

  return 0;
}
