#undef HAS_STD_MALLOC
#include "tkc/mem.c"
#include <time.h>
#include <assert.h>

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

void check_zero(void* ptr, uint32_t size) {
  uint32_t i = 0;
  char* p = (char*)ptr;

  for (i = 0; i < size; i++) {
    assert(p[i] == 0);
  }
}

int main() {
  mem_stack_t s;
  uint32_t i = 0;
  uint32_t nr = 10000 * 10000;

  srand(time(0));
  mem_stack_init(&s);
  tk_mem_init(s_heap_mem, sizeof(s_heap_mem));

  tk_free(TKMEM_ALLOC(100));

  for (i = 0; i < nr; i++) {
    uint32_t size = rand() % 100;
    void* ptr = TKMEM_CALLOC(size, 1);

    check_zero(ptr, size);
    if (ptr != NULL) {
      if (mem_stack_has_space(&s)) {
        mem_stack_push(&s, ptr);
      } else {
        TKMEM_FREE(ptr);
      }
    } else {
      mem_stack_free_n(&s, s.top >> 1);
    }
  }

  mem_stack_free_n(&s, s.top);
  tk_mem_dump();

  return 0;
}
