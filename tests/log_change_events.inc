
#include <string>
using std::string;
static string s_log;

static ret_t on_change_events(void* ctx, event_t* e) {
  if (e->type == EVT_VALUE_WILL_CHANGE) {
    s_log += "will_change;";
  }

  if (e->type == EVT_VALUE_CHANGED) {
    s_log += "change;";
  }

  return RET_OK;
}

static ret_t on_value_will_changed_accept(void* ctx, event_t* e) {
  return RET_OK;
}

static ret_t on_value_will_changed_abort(void* ctx, event_t* e) {
  return RET_STOP;
}

static ret_t on_value_changed(void* ctx, event_t* e) {
  value_change_event_t* evt = value_change_event_cast(e);
  value_change_event_t* ret = (value_change_event_t*)ctx;

  *ret = *evt;

  return RET_OK;
}
