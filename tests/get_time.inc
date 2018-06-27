static uint32_t s_now = 0;
static uint32_t timer_get_time() {
  return s_now;
}
static void timer_set_time(uint32_t now) {
  s_now = now;
}
