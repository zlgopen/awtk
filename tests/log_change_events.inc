
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
