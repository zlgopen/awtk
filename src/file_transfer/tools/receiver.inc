
static ret_t on_progress(void* ctx, event_t* e) {
  progress_event_t* evt = (progress_event_t*)e;

  log_debug("progress: %u\n", evt->percent);

  return RET_OK;
}

ret_t do_receive(tk_iostream_t* io, tk_ostream_t* target, const char* filename) {
  file_receiver_t* receiver = file_receiver_create(FILE_TRANSFER_DEFAULT_BLOCK_SIZE, "myboard");
  return_value_if_fail(receiver != NULL, RET_FAIL);

  emitter_on(EMITTER(receiver), EVT_PROGRESS, on_progress, NULL);
  file_receiver_run(receiver, io, target, filename);
  file_receiver_destroy(receiver);

  return RET_OK;
}
