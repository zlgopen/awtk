#include "base/mem.h"
#include <assert.h>

uint32_t heap[1024];

int main() {
  char* str = NULL;
  char* p[32];
  uint32_t i = 0;
  mem_stat_t st;
  mem_init(heap, sizeof(heap));

  str = (char*)lftk_alloc(100);
  lftk_free(str);

  for (i = 0; i < ARRAY_SIZE(p); i++) {
    p[i] = (char*)MEM_ALLOC(i);
    log_debug("%p\n", p[i]);
    st = mem_stat();
    assert(st.used_block_nr == (i + 1));
    assert(st.free_block_nr == 1);
  }

  for (i = 0; i < ARRAY_SIZE(p); i++) {
    MEM_FREE(p[i]);
    st = mem_stat();
    assert(st.used_block_nr == (ARRAY_SIZE(p) - i - 1));
    if (st.used_block_nr) {
      assert(st.free_block_nr == 2);
    } else {
      assert(st.free_block_nr == 1);
    }
  }

  mem_info_dump();

  return 0;
}
