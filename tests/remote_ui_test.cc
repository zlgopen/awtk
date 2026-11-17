#include "base/locale_info.h"
#include "base/window.h"
#include "base/window_manager.h"
#include "gtest/gtest.h"
#include "remote_ui/client/remote_ui.h"
#include "remote_ui/service/remote_ui_service.h"
#include "service/msg_header.h"
#include "tkc/crc.h"
#include "widgets/button.h"
#include <errno.h>
#include <string.h>

#define LOOP_PIPE_INIT_SIZE 4096

typedef struct _byte_pipe_t {
  uint8_t* data;
  uint32_t capacity;
  uint32_t rpos;
  uint32_t wpos;
} byte_pipe_t;

typedef struct _loopback_t {
  byte_pipe_t c2s;
  byte_pipe_t s2c;
  tk_service_t* service;
  uint32_t dispatch_count;
  bool_t pumping;
  bool_t fail_client_write;
} loopback_t;

typedef struct _loop_istream_t {
  tk_istream_t istream;
  loopback_t* lb;
  bool_t is_client;
} loop_istream_t;

typedef struct _loop_ostream_t {
  tk_ostream_t ostream;
  loopback_t* lb;
  bool_t is_client;
} loop_ostream_t;

typedef struct _loop_iostream_t {
  tk_iostream_t iostream;
  loopback_t* lb;
  bool_t is_client;
  tk_istream_t* istream;
  tk_ostream_t* ostream;
} loop_iostream_t;

static ret_t byte_pipe_init(byte_pipe_t* pipe) {
  return_value_if_fail(pipe != NULL, RET_BAD_PARAMS);

  memset(pipe, 0x00, sizeof(*pipe));
  pipe->data = (uint8_t*)TKMEM_ALLOC(LOOP_PIPE_INIT_SIZE);
  return_value_if_fail(pipe->data != NULL, RET_OOM);
  pipe->capacity = LOOP_PIPE_INIT_SIZE;

  return RET_OK;
}

static void byte_pipe_deinit(byte_pipe_t* pipe) {
  if (pipe == NULL) {
    return;
  }

  TKMEM_FREE(pipe->data);
  memset(pipe, 0x00, sizeof(*pipe));
}

static uint32_t byte_pipe_available(const byte_pipe_t* pipe) {
  return pipe->wpos - pipe->rpos;
}

static ret_t byte_pipe_ensure(byte_pipe_t* pipe, uint32_t extra) {
  uint32_t need = 0;
  uint32_t capacity = 0;
  uint8_t* data = NULL;

  if (pipe->rpos > 0 && pipe->wpos + extra > pipe->capacity) {
    uint32_t size = byte_pipe_available(pipe);
    memmove(pipe->data, pipe->data + pipe->rpos, size);
    pipe->rpos = 0;
    pipe->wpos = size;
  }

  need = pipe->wpos + extra;
  if (need <= pipe->capacity) {
    return RET_OK;
  }

  capacity = pipe->capacity;
  while (capacity < need) {
    capacity *= 2;
  }

  data = (uint8_t*)TKMEM_REALLOC(pipe->data, capacity);
  return_value_if_fail(data != NULL, RET_OOM);
  pipe->data = data;
  pipe->capacity = capacity;

  return RET_OK;
}

static int32_t byte_pipe_write(byte_pipe_t* pipe, const uint8_t* buff, uint32_t size) {
  return_value_if_fail(pipe != NULL && buff != NULL, -1);
  if (size == 0) {
    return 0;
  }
  if (byte_pipe_ensure(pipe, size) != RET_OK) {
    errno = ENOMEM;
    return -1;
  }

  memcpy(pipe->data + pipe->wpos, buff, size);
  pipe->wpos += size;

  return (int32_t)size;
}

static int32_t byte_pipe_read(byte_pipe_t* pipe, uint8_t* buff, uint32_t size) {
  uint32_t avail = 0;
  return_value_if_fail(pipe != NULL && buff != NULL, -1);

  avail = byte_pipe_available(pipe);
  if (avail == 0 || size == 0) {
    return 0;
  }
  if (size > avail) {
    size = avail;
  }

  memcpy(buff, pipe->data + pipe->rpos, size);
  pipe->rpos += size;

  return (int32_t)size;
}

static byte_pipe_t* loop_istream_pipe(loop_istream_t* stream) {
  return stream->is_client ? &(stream->lb->s2c) : &(stream->lb->c2s);
}

static byte_pipe_t* loop_ostream_pipe(loop_ostream_t* stream) {
  return stream->is_client ? &(stream->lb->c2s) : &(stream->lb->s2c);
}

static ret_t loop_istream_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  loop_istream_t* stream = (loop_istream_t*)obj;

  if (tk_str_eq(name, TK_STREAM_PROP_IS_EOS)) {
    value_set_bool(v, byte_pipe_available(loop_istream_pipe(stream)) == 0);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    value_set_bool(v, TRUE);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static int32_t loop_istream_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  loop_istream_t* is = (loop_istream_t*)stream;

  return byte_pipe_read(loop_istream_pipe(is), buff, max_size);
}

static ret_t loop_istream_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  loop_istream_t* is = (loop_istream_t*)stream;
  byte_pipe_t* pipe = loop_istream_pipe(is);
  (void)timeout_ms;

  if (byte_pipe_available(pipe) > 0) {
    return RET_OK;
  }

  if (!is->is_client || is->lb->pumping || is->lb->service == NULL) {
    return RET_FAIL;
  }
  if (byte_pipe_available(&(is->lb->c2s)) == 0) {
    return RET_FAIL;
  }

  is->lb->pumping = TRUE;
  is->lb->dispatch_count++;
  tk_service_dispatch(is->lb->service);
  is->lb->pumping = FALSE;

  return byte_pipe_available(pipe) > 0 ? RET_OK : RET_FAIL;
}

static bool_t loop_istream_eos(tk_istream_t* stream) {
  loop_istream_t* is = (loop_istream_t*)stream;

  return byte_pipe_available(loop_istream_pipe(is)) == 0;
}

static int32_t loop_ostream_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size) {
  loop_ostream_t* os = (loop_ostream_t*)stream;

  if (os->is_client && os->lb->fail_client_write) {
    errno = EIO;
    return -1;
  }

  return byte_pipe_write(loop_ostream_pipe(os), buff, max_size);
}

static tk_istream_t* loop_iostream_get_istream(tk_iostream_t* stream) {
  return ((loop_iostream_t*)stream)->istream;
}

static tk_ostream_t* loop_iostream_get_ostream(tk_iostream_t* stream) {
  return ((loop_iostream_t*)stream)->ostream;
}

static ret_t loop_iostream_on_destroy(tk_object_t* obj) {
  loop_iostream_t* io = (loop_iostream_t*)obj;

  TK_OBJECT_UNREF(io->istream);
  TK_OBJECT_UNREF(io->ostream);

  return RET_OK;
}

static object_vtable_t s_loop_istream_vt;
static object_vtable_t s_loop_ostream_vt;
static object_vtable_t s_loop_iostream_vt;
static bool_t s_loop_vt_ready = FALSE;

static void loop_vt_init(void) {
  if (s_loop_vt_ready) {
    return;
  }

  memset(&s_loop_istream_vt, 0x00, sizeof(s_loop_istream_vt));
  s_loop_istream_vt.type = "loop_istream";
  s_loop_istream_vt.desc = "loop_istream";
  s_loop_istream_vt.size = sizeof(loop_istream_t);
  s_loop_istream_vt.get_prop = loop_istream_get_prop;

  memset(&s_loop_ostream_vt, 0x00, sizeof(s_loop_ostream_vt));
  s_loop_ostream_vt.type = "loop_ostream";
  s_loop_ostream_vt.desc = "loop_ostream";
  s_loop_ostream_vt.size = sizeof(loop_ostream_t);

  memset(&s_loop_iostream_vt, 0x00, sizeof(s_loop_iostream_vt));
  s_loop_iostream_vt.type = "loop_iostream";
  s_loop_iostream_vt.desc = "loop_iostream";
  s_loop_iostream_vt.size = sizeof(loop_iostream_t);
  s_loop_iostream_vt.on_destroy = loop_iostream_on_destroy;

  s_loop_vt_ready = TRUE;
}

static tk_iostream_t* loop_iostream_create(loopback_t* lb, bool_t is_client) {
  tk_object_t* obj = NULL;
  tk_object_t* in_obj = NULL;
  tk_object_t* out_obj = NULL;
  loop_istream_t* in = NULL;
  loop_ostream_t* out = NULL;
  loop_iostream_t* io = NULL;

  loop_vt_init();
  in_obj = tk_object_create(&s_loop_istream_vt);
  out_obj = tk_object_create(&s_loop_ostream_vt);
  obj = tk_object_create(&s_loop_iostream_vt);
  if (in_obj == NULL || out_obj == NULL || obj == NULL) {
    TK_OBJECT_UNREF(in_obj);
    TK_OBJECT_UNREF(out_obj);
    TK_OBJECT_UNREF(obj);
    return NULL;
  }

  in = (loop_istream_t*)in_obj;
  in->lb = lb;
  in->is_client = is_client;
  in->istream.read = loop_istream_read;
  in->istream.eos = loop_istream_eos;
  in->istream.wait_for_data = loop_istream_wait_for_data;

  out = (loop_ostream_t*)out_obj;
  out->lb = lb;
  out->is_client = is_client;
  out->ostream.write = loop_ostream_write;

  io = (loop_iostream_t*)obj;
  io->lb = lb;
  io->is_client = is_client;
  io->istream = TK_ISTREAM(in);
  io->ostream = TK_OSTREAM(out);
  io->iostream.get_istream = loop_iostream_get_istream;
  io->iostream.get_ostream = loop_iostream_get_ostream;

  return TK_IOSTREAM(io);
}

static ret_t write_req(byte_pipe_t* pipe, uint16_t type, const void* data, uint32_t size) {
  tk_msg_header_t header;
  uint16_t crc_value = PPPINITFCS16;

  memset(&header, 0x00, sizeof(header));
  header.type = type;
  header.size = size;
  header.data_type = MSG_DATA_TYPE_BINARY;
  crc_value = tk_crc16(crc_value, &header, sizeof(header));
  if (data != NULL && size > 0) {
    crc_value = tk_crc16(crc_value, data, size);
  }

  if (byte_pipe_write(pipe, (const uint8_t*)&header, sizeof(header)) != (int32_t)sizeof(header)) {
    return RET_FAIL;
  }
  if (size > 0 && byte_pipe_write(pipe, (const uint8_t*)data, size) != (int32_t)size) {
    return RET_FAIL;
  }
  if (byte_pipe_write(pipe, (const uint8_t*)&crc_value, sizeof(crc_value)) !=
      (int32_t)sizeof(crc_value)) {
    return RET_FAIL;
  }

  return RET_OK;
}

static ret_t read_resp_code(byte_pipe_t* pipe, uint32_t* code) {
  tk_msg_header_t header;
  uint16_t crc_value = 0;

  return_value_if_fail(code != NULL, RET_BAD_PARAMS);
  if (byte_pipe_read(pipe, (uint8_t*)&header, sizeof(header)) != (int32_t)sizeof(header)) {
    return RET_FAIL;
  }
  if (header.size > 0) {
    if (byte_pipe_available(pipe) < header.size) {
      return RET_FAIL;
    }
    pipe->rpos += header.size;
  }
  if (byte_pipe_read(pipe, (uint8_t*)&crc_value, sizeof(crc_value)) != (int32_t)sizeof(crc_value)) {
    return RET_FAIL;
  }

  *code = header.resp_code;
  return RET_OK;
}

class RemoteUITest : public ::testing::Test {
 protected:
  RemoteUITest()
      : win_(NULL),
        ok_(NULL),
        cancel_(NULL),
        client_io_(NULL),
        service_io_(NULL),
        ui_(NULL),
        service_(NULL) {
    memset(&lb_, 0x00, sizeof(lb_));
  }

  virtual void SetUp() {
    memset(&lb_, 0x00, sizeof(lb_));
    ASSERT_EQ(byte_pipe_init(&lb_.c2s), RET_OK);
    ASSERT_EQ(byte_pipe_init(&lb_.s2c), RET_OK);

    win_ = window_create(NULL, 0, 0, 400, 300);
    ASSERT_EQ(widget_set_name(win_, "main"), RET_OK);
    ok_ = button_create(win_, 10, 20, 80, 30);
    cancel_ = button_create(win_, 30, 40, 80, 30);
    ASSERT_EQ(widget_set_name(ok_, "ok"), RET_OK);
    ASSERT_EQ(widget_set_name(cancel_, "cancel"), RET_OK);
    ASSERT_EQ(widget_set_text_utf8(cancel_, "cancel"), RET_OK);

    client_io_ = loop_iostream_create(&lb_, TRUE);
    service_io_ = loop_iostream_create(&lb_, FALSE);
    ASSERT_TRUE(client_io_ != NULL);
    ASSERT_TRUE(service_io_ != NULL);

    ui_ = remote_ui_create(client_io_);
    service_ = remote_ui_service_create(service_io_, NULL);
    ASSERT_TRUE(ui_ != NULL);
    ASSERT_TRUE(service_ != NULL);
    lb_.service = service_;
  }

  virtual void TearDown() {
    lb_.service = NULL;
    lb_.fail_client_write = FALSE;
    if (ui_ != NULL) {
      ui_->pending_props.extendable = TRUE;
      ui_->client.wb.extendable = TRUE;
      remote_ui_destroy(ui_);
      ui_ = NULL;
    }
    if (service_ != NULL) {
      tk_service_destroy(service_);
      service_ = NULL;
    }
    byte_pipe_deinit(&lb_.c2s);
    byte_pipe_deinit(&lb_.s2c);
    if (win_ != NULL) {
      widget_destroy(win_);
      win_ = NULL;
    }
  }

  ret_t dispatch_raw(void) {
    lb_.dispatch_count++;
    return tk_service_dispatch(service_);
  }

  loopback_t lb_;
  widget_t* win_;
  widget_t* ok_;
  widget_t* cancel_;
  tk_iostream_t* client_io_;
  tk_iostream_t* service_io_;
  remote_ui_t* ui_;
  tk_service_t* service_;
};

TEST(RemoteUI, null_ui) {
  value_t v;
  value_set_int(&v, 1);

  ASSERT_EQ(remote_ui_push_set_props_list(NULL, "ok", WIDGET_PROP_X, &v), RET_BAD_PARAMS);
  ASSERT_EQ(remote_ui_clear_set_props_list(NULL), RET_BAD_PARAMS);
  ASSERT_EQ(remote_ui_flush_set_props_list(NULL), RET_BAD_PARAMS);
  ASSERT_EQ(remote_ui_set_prop(NULL, "ok", WIDGET_PROP_X, &v), RET_BAD_PARAMS);
  ASSERT_EQ(remote_ui_get_prop(NULL, "ok", WIDGET_PROP_X, &v), RET_BAD_PARAMS);
}

TEST_F(RemoteUITest, null_args) {
  value_t v;
  value_set_int(&v, 1);

  ASSERT_EQ(remote_ui_push_set_props_list(ui_, NULL, WIDGET_PROP_X, &v), RET_BAD_PARAMS);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", NULL, &v), RET_BAD_PARAMS);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, NULL), RET_BAD_PARAMS);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
  ASSERT_EQ(ok_->x, 10);
}

TEST_F(RemoteUITest, push_does_not_send) {
  value_t v;
  value_set_int(&v, 64);

  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &v), RET_OK);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)1);
  ASSERT_EQ(lb_.dispatch_count, (uint32_t)0);
  ASSERT_EQ(ok_->x, 10);
}

TEST_F(RemoteUITest, empty_flush) {
  lb_.fail_client_write = TRUE;

  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);
  ASSERT_EQ(lb_.dispatch_count, (uint32_t)0);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
  ASSERT_EQ(ok_->x, 10);
}

TEST_F(RemoteUITest, clear_then_flush) {
  value_t v;
  value_set_int(&v, 99);

  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &v), RET_OK);
  ASSERT_EQ(remote_ui_clear_set_props_list(ui_), RET_OK);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
  ASSERT_EQ(ui_->pending_props.cursor, (uint32_t)0);

  lb_.fail_client_write = TRUE;
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);
  ASSERT_EQ(lb_.dispatch_count, (uint32_t)0);
  ASSERT_EQ(ok_->x, 10);
}

TEST_F(RemoteUITest, flush_multiple_props_in_one_request) {
  value_t x;
  value_t enable;
  value_t text;
  value_t got;
  char utf8[32] = {0};
  uint32_t dispatch_before = 0;

  value_set_int(&x, 70);
  value_set_bool(&enable, FALSE);
  value_set_str(&text, "hello");

  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &x), RET_OK);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_ENABLE, &enable), RET_OK);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "cancel", WIDGET_PROP_TEXT, &text), RET_OK);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)3);

  dispatch_before = lb_.dispatch_count;
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);
  ASSERT_EQ(lb_.dispatch_count, dispatch_before + 1);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
  ASSERT_EQ(ui_->pending_props.cursor, (uint32_t)0);

  ASSERT_EQ(ok_->x, 70);
  ASSERT_EQ(ok_->enable, FALSE);
  ASSERT_EQ(widget_get_text_utf8(cancel_, utf8, sizeof(utf8)), RET_OK);
  ASSERT_STREQ(utf8, "hello");

  value_set_int(&got, 0);
  ASSERT_EQ(remote_ui_get_prop(ui_, "ok", WIDGET_PROP_X, &got), RET_OK);
  ASSERT_EQ(value_int(&got), 70);
  value_reset(&got);

  ASSERT_EQ(remote_ui_get_prop(ui_, "ok", WIDGET_PROP_ENABLE, &got), RET_OK);
  ASSERT_EQ(value_bool(&got), FALSE);
  value_reset(&got);
}

TEST_F(RemoteUITest, set_prop_and_get_prop) {
  value_t v;
  value_t got;

  value_set_int(&v, 55);
  ASSERT_EQ(remote_ui_set_prop(ui_, "ok", WIDGET_PROP_X, &v), RET_OK);
  ASSERT_EQ(ok_->x, 55);

  value_set_int(&got, 0);
  ASSERT_EQ(remote_ui_get_prop(ui_, "ok", WIDGET_PROP_X, &got), RET_OK);
  ASSERT_EQ(value_int(&got), 55);
  value_reset(&got);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
}

TEST_F(RemoteUITest, missing_target_still_applies_later_items) {
  value_t missing;
  value_t x;

  value_set_int(&missing, 9);
  value_set_int(&x, 15);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "no_such", WIDGET_PROP_X, &missing), RET_OK);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &x), RET_OK);

  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_BAD_PARAMS);
  ASSERT_EQ(ok_->x, 15);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
}

TEST_F(RemoteUITest, failed_item_keeps_earlier_items) {
  value_t x;
  value_t missing;

  value_set_int(&x, 21);
  value_set_int(&missing, 9);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &x), RET_OK);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "no_such", WIDGET_PROP_X, &missing), RET_OK);

  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_BAD_PARAMS);
  ASSERT_EQ(ok_->x, 21);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
}

TEST_F(RemoteUITest, truncated_payload_applies_parsed_items) {
  wbuffer_t wb;
  value_t v;
  uint32_t code = 0;

  wbuffer_init_extendable(&wb);
  value_set_int(&v, 80);
  ASSERT_EQ(wbuffer_write_uint32(&wb, 2), RET_OK);
  ASSERT_EQ(wbuffer_write_string(&wb, "ok"), RET_OK);
  ASSERT_EQ(wbuffer_write_string(&wb, WIDGET_PROP_X), RET_OK);
  ASSERT_EQ(wbuffer_write_value(&wb, &v), RET_OK);

  ASSERT_EQ(write_req(&lb_.c2s, REMOTE_UI_SET_PROPS, wb.data, wb.cursor), RET_OK);
  ASSERT_EQ(dispatch_raw(), RET_OK);
  ASSERT_EQ(read_resp_code(&lb_.s2c, &code), RET_OK);
  ASSERT_EQ(code, (uint32_t)RET_BAD_PARAMS);
  ASSERT_EQ(ok_->x, 80);

  wbuffer_deinit(&wb);
}

TEST_F(RemoteUITest, empty_payload_and_zero_count) {
  wbuffer_t wb;
  uint32_t code = 0;

  ASSERT_EQ(write_req(&lb_.c2s, REMOTE_UI_SET_PROPS, NULL, 0), RET_OK);
  ASSERT_EQ(dispatch_raw(), RET_OK);
  ASSERT_EQ(read_resp_code(&lb_.s2c, &code), RET_OK);
  ASSERT_EQ(code, (uint32_t)RET_BAD_PARAMS);
  ASSERT_EQ(ok_->x, 10);

  wbuffer_init_extendable(&wb);
  ASSERT_EQ(wbuffer_write_uint32(&wb, 0), RET_OK);
  ASSERT_EQ(write_req(&lb_.c2s, REMOTE_UI_SET_PROPS, wb.data, wb.cursor), RET_OK);
  ASSERT_EQ(dispatch_raw(), RET_OK);
  ASSERT_EQ(read_resp_code(&lb_.s2c, &code), RET_OK);
  ASSERT_EQ(code, (uint32_t)RET_OK);
  ASSERT_EQ(ok_->x, 10);
  wbuffer_deinit(&wb);
}

TEST_F(RemoteUITest, flush_clears_cache_when_send_fails) {
  value_t failed;
  value_t next;

  value_set_int(&failed, 77);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &failed), RET_OK);
  lb_.fail_client_write = TRUE;
  ASSERT_NE(remote_ui_flush_set_props_list(ui_), RET_OK);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
  ASSERT_EQ(ui_->pending_props.cursor, (uint32_t)0);
  ASSERT_EQ(ok_->x, 10);

  lb_.fail_client_write = FALSE;
  value_set_int(&next, 33);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &next), RET_OK);
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);
  ASSERT_EQ(ok_->x, 33);
}

TEST_F(RemoteUITest, push_rolls_back_when_buffer_cannot_grow) {
  value_t kept;
  value_t extra;
  uint32_t cursor = 0;

  value_set_int(&kept, 64);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &kept), RET_OK);
  cursor = ui_->pending_props.cursor;
  ui_->pending_props.extendable = FALSE;
  ui_->pending_props.capacity = cursor;

  value_set_int(&extra, 90);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "cancel", WIDGET_PROP_X, &extra), RET_BAD_PARAMS);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)1);
  ASSERT_EQ(ui_->pending_props.cursor, cursor);

  ui_->pending_props.extendable = TRUE;
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);
  ASSERT_EQ(ok_->x, 64);
  ASSERT_EQ(cancel_->x, 30);
}

TEST_F(RemoteUITest, flush_keeps_cache_when_pack_fails) {
  value_t v;
  value_set_int(&v, 88);

  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &v), RET_OK);
  ui_->client.wb.extendable = FALSE;
  ui_->client.wb.capacity = 0;

  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OOM);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)1);
  ASSERT_GT(ui_->pending_props.cursor, (uint32_t)0);
  ASSERT_EQ(ok_->x, 10);

  ui_->client.wb.extendable = TRUE;
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);
  ASSERT_EQ(ok_->x, 88);
  ASSERT_EQ(ui_->pending_props_nr, (uint32_t)0);
}

TEST_F(RemoteUITest, empty_target_sets_current_window) {
  value_t v;
  value_set_int(&v, 12);

  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "", WIDGET_PROP_X, &v), RET_OK);
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);
  ASSERT_EQ(win_->x, 12);
  ASSERT_EQ(ok_->x, 10);
}

TEST_F(RemoteUITest, get_prop_string) {
  value_t text;
  value_t got;

  value_set_str(&text, "hello");
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "cancel", WIDGET_PROP_TEXT, &text), RET_OK);
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);

  value_set_int(&got, 0);
  ASSERT_EQ(remote_ui_get_prop(ui_, "cancel", WIDGET_PROP_TEXT, &got), RET_OK);
  ASSERT_STREQ(value_str(&got), "hello");
  value_reset(&got);
}

TEST_F(RemoteUITest, global_language_and_unknown_prop) {
  value_t language;
  value_t got;
  char prev_language[TK_NAME_LEN + 1] = {0};
  char prev_country[TK_NAME_LEN + 1] = {0};
  char theme_name[TK_NAME_LEN + 1] = {0};
  const char* next = NULL;
  locale_info_t* info = locale_info();

  tk_strncpy(prev_language, info->language, TK_NAME_LEN);
  tk_strncpy(prev_country, info->country, TK_NAME_LEN);
  next = tk_str_eq(info->language, "zh") ? "en_US" : "zh_CN";

  value_set_str(&language, next);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, REMOTE_UI_TARGET_GLOBAL, REMOTE_UI_PROP_LANGUAGE,
                                          &language),
            RET_OK);
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_OK);

  value_set_int(&got, 0);
  ASSERT_EQ(remote_ui_get_prop(ui_, REMOTE_UI_TARGET_GLOBAL, REMOTE_UI_PROP_LANGUAGE, &got),
            RET_OK);
  ASSERT_STREQ(value_str(&got), next);
  value_reset(&got);
  ASSERT_EQ(locale_info_change(locale_info(), prev_language, prev_country), RET_OK);

  tk_strncpy(theme_name, widget_get_theme_name(window_manager()), TK_NAME_LEN);
  value_set_str(&language, theme_name);
  ASSERT_EQ(
      remote_ui_push_set_props_list(ui_, REMOTE_UI_TARGET_GLOBAL, REMOTE_UI_PROP_THEME, &language),
      RET_OK);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, REMOTE_UI_TARGET_GLOBAL, "no_such", &language),
            RET_OK);
  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_NOT_FOUND);
  ASSERT_STREQ(widget_get_theme_name(window_manager()), theme_name);
}

TEST_F(RemoteUITest, first_error_is_kept) {
  value_t unknown;
  value_t missing;
  value_t x;

  value_set_str(&unknown, "default");
  value_set_int(&missing, 9);
  value_set_int(&x, 44);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, REMOTE_UI_TARGET_GLOBAL, "no_such", &unknown),
            RET_OK);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "no_such", WIDGET_PROP_X, &missing), RET_OK);
  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_X, &x), RET_OK);

  ASSERT_EQ(remote_ui_flush_set_props_list(ui_), RET_NOT_FOUND);
  ASSERT_EQ(ok_->x, 44);
}

TEST_F(RemoteUITest, truncated_value_keeps_earlier_item) {
  wbuffer_t wb;
  value_t v;
  uint32_t code = 0;

  wbuffer_init_extendable(&wb);
  value_set_int(&v, 81);
  ASSERT_EQ(wbuffer_write_uint32(&wb, 2), RET_OK);
  ASSERT_EQ(wbuffer_write_string(&wb, "ok"), RET_OK);
  ASSERT_EQ(wbuffer_write_string(&wb, WIDGET_PROP_X), RET_OK);
  ASSERT_EQ(wbuffer_write_value(&wb, &v), RET_OK);
  ASSERT_EQ(wbuffer_write_string(&wb, "cancel"), RET_OK);
  ASSERT_EQ(wbuffer_write_string(&wb, WIDGET_PROP_X), RET_OK);
  ASSERT_EQ(wbuffer_write_uint8(&wb, VALUE_TYPE_INT32), RET_OK);

  ASSERT_EQ(write_req(&lb_.c2s, REMOTE_UI_SET_PROPS, wb.data, wb.cursor), RET_OK);
  ASSERT_EQ(dispatch_raw(), RET_OK);
  ASSERT_EQ(read_resp_code(&lb_.s2c, &code), RET_OK);
  ASSERT_EQ(code, (uint32_t)RET_BAD_PARAMS);
  ASSERT_EQ(ok_->x, 81);
  ASSERT_EQ(cancel_->x, 30);
  wbuffer_deinit(&wb);
}

TEST_F(RemoteUITest, destroy_with_pending_props) {
  value_t v;
  value_set_str(&v, "not-sent");

  ASSERT_EQ(remote_ui_push_set_props_list(ui_, "ok", WIDGET_PROP_TEXT, &v), RET_OK);
  ASSERT_GT(ui_->pending_props.cursor, (uint32_t)0);
  ASSERT_EQ(remote_ui_destroy(ui_), RET_OK);
  ui_ = NULL;
  ASSERT_EQ(ok_->x, 10);
}
