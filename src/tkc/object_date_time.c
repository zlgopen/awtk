/**
 * File:   object_date_time.c
 * Author: AWTK Develop Team
 * Brief:  wrap typed array to an object.
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/named_value.h"
#include "object_date_time.h"

static ret_t object_date_time_on_destroy(tk_object_t* obj) {
  object_date_time_t* o = OBJECT_DATE_TIME(obj);
  return_value_if_fail(o != NULL && o->dt != NULL, RET_BAD_PARAMS);

  date_time_destroy(o->dt);
  o->dt = NULL;

  return RET_OK;
}

static ret_t object_date_time_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  object_date_time_t* o = OBJECT_DATE_TIME(obj);
  return_value_if_fail(o != NULL && o->dt != NULL, RET_BAD_PARAMS);

  switch (*name) {
    case 'y': {
      date_time_set_year(o->dt, value_uint32(v));
      break;
    }
    case 'm': {
      if (name[1] == 'i') {
        date_time_set_minute(o->dt, value_uint32(v));
      } else {
        date_time_set_month(o->dt, value_uint32(v));
      }
      break;
    }
    case 'd': {
      date_time_set_day(o->dt, value_uint32(v));
      break;
    }
    case 'h': {
      date_time_set_hour(o->dt, value_uint32(v));
      break;
    }
    case 's': {
      date_time_set_second(o->dt, value_uint32(v));
      break;
    }
    case 'w': {
      /*日期确定后，wday也确定了，支持设置wday方便测试检查结果*/
      o->dt->wday = value_uint32(v);
      break;
    }
    default: {
      ret = RET_NOT_FOUND;
    }
  }

  return ret;
}

static ret_t object_date_time_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  object_date_time_t* o = OBJECT_DATE_TIME(obj);
  return_value_if_fail(o != NULL && o->dt != NULL, RET_BAD_PARAMS);

  switch (*name) {
    case 'y': {
      value_set_uint32(v, o->dt->year);
      break;
    }
    case 'm': {
      if (name[1] == 'i') {
        value_set_uint32(v, o->dt->minute);
      } else {
        value_set_uint32(v, o->dt->month);
      }
      break;
    }
    case 'd': {
      value_set_uint32(v, o->dt->day);
      break;
    }
    case 'h': {
      value_set_uint32(v, o->dt->hour);
      break;
    }
    case 's': {
      value_set_uint32(v, o->dt->second);
      break;
    }
    case 'w': {
      value_set_uint32(v, o->dt->wday);
      break;
    }
    default: {
      ret = RET_NOT_FOUND;
    }
  }

  return ret;
}

static ret_t object_date_time_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  named_value_t nv;
  ret_t ret = RET_OK;
  object_date_time_t* o = OBJECT_DATE_TIME(obj);
  return_value_if_fail(on_prop != NULL, RET_BAD_PARAMS);
  return_value_if_fail(o != NULL && o->dt != NULL, RET_BAD_PARAMS);

  nv.name = OBJECT_DATE_TIME_PROP_YEAR;
  value_set_uint32(&(nv.value), o->dt->year);
  ret = on_prop(ctx, &nv);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  nv.name = OBJECT_DATE_TIME_PROP_MONTH;
  value_set_uint32(&(nv.value), o->dt->month);
  ret = on_prop(ctx, &nv);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  nv.name = OBJECT_DATE_TIME_PROP_MINUTE;
  value_set_uint32(&(nv.value), o->dt->minute);
  ret = on_prop(ctx, &nv);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  nv.name = OBJECT_DATE_TIME_PROP_DAY;
  value_set_uint32(&(nv.value), o->dt->day);
  ret = on_prop(ctx, &nv);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  nv.name = OBJECT_DATE_TIME_PROP_HOUR;
  value_set_uint32(&(nv.value), o->dt->hour);
  ret = on_prop(ctx, &nv);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  nv.name = OBJECT_DATE_TIME_PROP_SECOND;
  value_set_uint32(&(nv.value), o->dt->second);
  ret = on_prop(ctx, &nv);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  nv.name = OBJECT_DATE_TIME_PROP_WDAY;
  value_set_uint32(&(nv.value), o->dt->wday);
  ret = on_prop(ctx, &nv);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  return RET_OK;
}

static const object_vtable_t s_object_date_time_vtable = {
    .type = "object_date_time",
    .desc = "object_date_time",
    .size = sizeof(object_date_time_t),
    .is_collection = FALSE,
    .on_destroy = object_date_time_on_destroy,
    .foreach_prop = object_date_time_foreach_prop,
    .get_prop = object_date_time_get_prop,
    .set_prop = object_date_time_set_prop};

tk_object_t* object_date_time_create(void) {
  tk_object_t* o = NULL;
  object_date_time_t* wrapper = NULL;
  o = tk_object_create(&s_object_date_time_vtable);
  return_value_if_fail(o != NULL, NULL);

  wrapper = OBJECT_DATE_TIME(o);
  return_value_if_fail(wrapper != NULL, NULL);

  wrapper->dt = date_time_create();
  if (wrapper->dt == NULL) {
    TK_OBJECT_UNREF(o);
  }

  return o;
}

object_date_time_t* object_date_time_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_date_time_vtable, NULL);

  return (object_date_time_t*)(obj);
}
