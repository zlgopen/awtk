#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static char s_token[10240];
static char s_line[10240];

static const char* skip_to(const char* p, char c) {
  while(*p && *p != c) p++;
  return p;
}

static const char* skip_char(const char* p, char c) {
  while(*p && *p == c) p++;

  return p;
}

static const char* skip_to_next(const char* p, char c) {
  return skip_char(skip_to(skip_char(p, c), c), c);
}

static const char* next_token(const char* p, char c) {
  const char* start = skip_char(p, c);
  const char* end = skip_to(start, c);
  uint32_t size = end - start;
  strncpy(s_token, start, size);
  s_token[size] = '\0';

  return s_token;
}

static int start_with(const char* p, const char* str) {
  return strncmp(p, str, strlen(str)) == 0;
}

static void gen_xywh(FILE* fout, const char* p) {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;

  sscanf(p, "%d,%d,%d,%d", &x, &y, &w, &h);
  fprintf(fout, " x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" />\n", x, y, w, h);
}

void gen_button(FILE* fout, const char* p) {
  const char* token = NULL;
  token = next_token(p, '\"');
  fprintf(fout, "<button text=\"%s\"", token);
  p = skip_to_next(p, ','); 
  token = next_token(p, ',');
  fprintf(fout, " name=\"%s\"", token);
  gen_xywh(fout, skip_to_next(p, ','));   
}

void gen_widget(FILE* fout) {
  const char* p = skip_char(s_line, ' ');  
  if(start_with(p, "DEFPUSHBUTTON")) {
    gen_button(fout, skip_char(skip_to_next(s_line, ' '), ' '));
  } else if(start_with(p, "PUSHBUTTON")) {
    gen_button(fout, skip_char(skip_to_next(s_line, ' '), ' '));
  }
}

void gen_window(FILE* fin) {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  const char* text = NULL;
  FILE* fout = fopen("test.xml", "wb+");
  char* p = strstr(s_line, "DIALOGEX");
  const char* name = next_token(s_line, ' ');

  sscanf(p, "DIALOGEX %d, %d, %d, %d", &x, &y, &w, &h);
  fprintf(fout, "<dialog name=\"%s\" x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\"", name, x, y, w, h);

  fgets(s_line, sizeof(s_line), fin); /*STYLE*/
  fgets(s_line, sizeof(s_line), fin); /*CAPTION*/
  text = next_token(strchr(s_line, '\"'), '\"');
  fprintf(fout, "text=\"%s\">\n", text);
  fgets(s_line, sizeof(s_line), fin); /*FONT*/
  fgets(s_line, sizeof(s_line), fin); /*BEGIN*/

  while(fgets(s_line, sizeof(s_line), fin)) {
    if(strstr(s_line, "END") == NULL) {
      gen_widget(fout);
    } else {
      break;
    }
  }
  fprintf(fout, "</dialog>\n");
  fclose(fout);

  return;
}

void gen(const char* in) {
  FILE* fin = fopen(in, "r");
  while(fgets(s_line, sizeof(s_line), fin)) {
    if(strstr(s_line, "DIALOGEX") != NULL) {
      gen_window(fin);
    }
  }

  fclose(fin);
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("Usage: %s in out\n", argv[0]);
    exit(0);
  }

  gen(argv[1]);

  return 0;
}
