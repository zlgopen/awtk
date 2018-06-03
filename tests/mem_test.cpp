#include "base/mem.h"
#include <assert.h>

#ifndef HAS_STD_MALLOC
#define STACK_SIZE 1000
uint32_t s_heap_mem[1024 * 100];

typedef struct _mem_stack_t {
  void* ptr[1000];
  uint32_t top;
  uint32_t max;
} mem_stack_t;

void mem_stack_init(mem_stack_t* s) {
  memset(s, 0x00, sizeof(mem_stack_t));
  s->max = ARRAY_SIZE(s->ptr);
}

void mem_stack_push(mem_stack_t* s, void* ptr) {
  assert(s->top < s->max);
  s->ptr[s->top++] = ptr;
}

bool_t mem_stack_has_space(mem_stack_t* s) {
  return s->top < s->max;
}

void* mem_stack_pop(mem_stack_t* s) {
  assert(s->top > 0);
  s->top--;
  return s->ptr[s->top];
}

void mem_stack_free_n(mem_stack_t* s, uint32_t nr) {
  for (uint32_t i = 0; i < nr && s->top > 0; i++) {
    tk_free(mem_stack_pop(s));
  }
}

int main() {
  mem_stack_t s;
  uint32_t i = 0;
  uint32_t nr = 10000 * 10000;

  srand(time(0));
  mem_stack_init(&s);
  tk_mem_init(s_heap_mem, sizeof(s_heap_mem));

  tk_free(tk_alloc(100));

  for (i = 0; i < nr; i++) {
    uint32_t size = rand() % 100;
    void* ptr = tk_calloc(size, 1);

    if (ptr != NULL) {
      if (mem_stack_has_space(&s)) {
        mem_stack_push(&s, ptr);
      } else {
        tk_free(ptr);
      }
    } else {
      mem_stack_free_n(&s, s.top >> 1);
    }
  }

  mem_stack_free_n(&s, s.top);
  tk_mem_info_dump();

  return 0;
}
#else
int main() {
  printf("HAS_STD_MALLOC defined, no test.\n");
  return 0;
}
#endif
