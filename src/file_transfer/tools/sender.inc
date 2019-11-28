static ret_t on_progress(void* ctx, event_t* e) {
  progress_event_t* evt = (progress_event_t*)e;

  log_debug("progress: %u\n", evt->percent);

  return RET_OK;
}

static ret_t do_send(tk_iostream_t* io, const char* filename) {
  file_sender_t* sender = file_sender_create();
  return_value_if_fail(sender != NULL, RET_FAIL);

  emitter_on(EMITTER(sender), EVT_PROGRESS, on_progress, NULL);
  file_sender_run(sender, io, filename);
  file_sender_destroy(sender);

  return RET_OK;
}
