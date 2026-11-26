/**
 * File:   cache_clock.h
 * Author: AWTK Develop Team
 * Brief:  CLOCK(二次机会)缓存。
 *
 * Copyright (c) 2026 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2026-11-24 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_CACHE_CLOCK_H
#define TK_CACHE_CLOCK_H

#include "tkc/types_def.h"
#include "tkc/mem.h"
#include "tkc/bitset.h"

#ifdef TK_RAW_HASH_TABLE_H
#error "cache_clock.h configures raw_hash_table, it must be included before raw_hash_table.h"
#endif /*TK_RAW_HASH_TABLE_H*/

BEGIN_C_DECLS

/********************************************************************************************/
/* 可配置的宏和类型：在包含本头文件之前定义，不定义时继承raw_hash_table的配置 */

/* 定义了才覆盖raw_hash_table的配置。 */
#ifdef TK_CACHE_CLOCK_DATA_T
#undef TK_RAW_HASH_TABLE_ENTRY_VALUE_T
#define TK_RAW_HASH_TABLE_ENTRY_VALUE_T TK_CACHE_CLOCK_DATA_T
#endif /*TK_CACHE_CLOCK_DATA_T*/

#ifdef TK_CACHE_CLOCK_DATA_COMPARE
#undef TK_RAW_HASH_TABLE_VALUE_COMPARE
#define TK_RAW_HASH_TABLE_VALUE_COMPARE(entry_value, value) \
  TK_CACHE_CLOCK_DATA_COMPARE(entry_value, value)
#endif /*TK_CACHE_CLOCK_DATA_COMPARE*/

#ifdef TK_CACHE_CLOCK_KEY_T
#undef TK_RAW_HASH_TABLE_ENTRY_KEY_T
#define TK_RAW_HASH_TABLE_ENTRY_KEY_T TK_CACHE_CLOCK_KEY_T
#endif /*TK_CACHE_CLOCK_KEY_T*/

#ifdef TK_CACHE_CLOCK_KEY_HASH
#undef TK_RAW_HASH_TABLE_KEY_HASH
#define TK_RAW_HASH_TABLE_KEY_HASH(key) TK_CACHE_CLOCK_KEY_HASH(key)
#endif /*TK_CACHE_CLOCK_KEY_HASH*/

#ifdef TK_CACHE_CLOCK_KEY_COMPARE
#undef TK_RAW_HASH_TABLE_KEY_COMPARE
#define TK_RAW_HASH_TABLE_KEY_COMPARE(entry_key, key) TK_CACHE_CLOCK_KEY_COMPARE(entry_key, key)
#endif /*TK_CACHE_CLOCK_KEY_COMPARE*/

#include "tkc/raw_hash_table.h"

/* 没定义的，这里从raw_hash_table继承。 */
#ifndef TK_CACHE_CLOCK_DATA_T
#define TK_CACHE_CLOCK_DATA_T TK_RAW_HASH_TABLE_ENTRY_VALUE_T
#endif /*TK_CACHE_CLOCK_DATA_T*/

#ifndef TK_CACHE_CLOCK_DATA_COMPARE
#define TK_CACHE_CLOCK_DATA_COMPARE(entry_value, value) \
  TK_RAW_HASH_TABLE_VALUE_COMPARE(entry_value, value)
#endif /*TK_CACHE_CLOCK_DATA_COMPARE*/

#ifndef TK_CACHE_CLOCK_KEY_T
#define TK_CACHE_CLOCK_KEY_T TK_RAW_HASH_TABLE_ENTRY_KEY_T
#endif /*TK_CACHE_CLOCK_KEY_T*/

#ifndef TK_CACHE_CLOCK_KEY_HASH
#define TK_CACHE_CLOCK_KEY_HASH(key) TK_RAW_HASH_TABLE_KEY_HASH(key)
#endif /*TK_CACHE_CLOCK_KEY_HASH*/

#ifndef TK_CACHE_CLOCK_KEY_COMPARE
#define TK_CACHE_CLOCK_KEY_COMPARE(entry_key, key) TK_RAW_HASH_TABLE_KEY_COMPARE(entry_key, key)
#endif /*TK_CACHE_CLOCK_KEY_COMPARE*/

/**
 * @enum tk_cache_clock_bit_t
 * @prefix TK_CACHE_CLOCK_BIT_
 * @annotation ["private"]
 * 每个hash桶的标志位。
 */
typedef enum _tk_cache_clock_bit_t {
  /**
   * @const TK_CACHE_CLOCK_BIT_ACCESSED
   * 是否被访问过(二次机会位)。
   */
  TK_CACHE_CLOCK_BIT_ACCESSED = 0,
  TK_CACHE_CLOCK_BIT_NR,
} tk_cache_clock_bit_t;

/**
 * @class tk_cache_clock_t
 * @annotation ["private"]
 * @export none
 * CLOCK(二次机会)缓存。
 *
 * 直接拿哈希表的桶当槽位(键和值都只在桶里存一份)，外加一个标志位图记录每个桶是否被访问过。
 * hand是时钟指针：淘汰时从hand扫桶，空桶跳过，"被访问过"的清0并跳过(给一次机会)，
 * 否则从那个桶里删掉最早插入的元素。
 *
 * 键只保存不拷贝，指针键的生命周期由调用方保证；数据在淘汰/删除/替换时由传入的
 * value_destroy销毁，销毁回调里不要再操作这个缓存。
 *
 * 示例：
 *
 * ```c
 * tk_cache_clock_t cache;
 * void* data = NULL;
 *
 * tk_cache_clock_init(&cache, 100, NULL);
 * tk_cache_clock_put(&cache, 1, data);
 * tk_cache_clock_get(&cache, 1, &data);
 * tk_cache_clock_deinit(&cache);
 * ```
 */
typedef struct _tk_cache_clock_t {
  /**
   * @property {tk_raw_hash_table_t} table
   * @annotation ["readable"]
   * hash表。
   */
  tk_raw_hash_table_t table;

  /**
   * @property {uint8_t*} flags
   * @annotation ["readable"]
   * 标志位图。
   */
  uint8_t* flags;

  /**
   * @property {size_t} hand
   * @annotation ["readable"]
   * 时钟指针，淘汰从这里开始扫桶。
   */
  size_t hand;
} tk_cache_clock_t;

/********************************************************************************************/
/* 文件内部使用 */

/**
 * @method tk_cache_clock_bit_pos
 * @annotation ["private", "static"]
 * @export none
 * 返回第index个桶的bit标志位在位图里的位序号。
 * @param {size_t} index 桶下标。
 * @param {tk_cache_clock_bit_t} bit 标志位。
 *
 * @return {size_t} 返回位序号。
 */
inline static size_t tk_cache_clock_bit_pos(size_t index, tk_cache_clock_bit_t bit) {
  return (size_t)TK_BITSET_POS(TK_CACHE_CLOCK_BIT_NR, index, bit);
}

/**
 * @method tk_cache_clock_touch_index
 * @annotation ["private"]
 * @export none
 * 把第index个桶标记为访问过(给二次机会)。
 * @param {tk_cache_clock_t*} cache 缓存对象。
 * @param {size_t} index 桶下标。
 */
inline static void tk_cache_clock_touch_index(tk_cache_clock_t* cache, size_t index) {
  TK_BITSET_OP(TK_SET_BIT, cache->flags,
               tk_cache_clock_bit_pos(index, TK_CACHE_CLOCK_BIT_ACCESSED));
}

/**
 * @method tk_cache_clock_next_index
 * @annotation ["private", "static"]
 * @export none
 * 桶下标前移一个，到末尾回绕到0(不用对桶数取模)。
 * @param {size_t} index 当前桶下标。
 * @param {size_t} n 桶数。
 *
 * @return {size_t} 返回下一个桶下标。
 */
inline static size_t tk_cache_clock_next_index(size_t index, size_t n) {
  index++;
  return index < n ? index : 0;
}

/**
 * @method tk_cache_clock_evict_bucket
 * @annotation ["private"]
 * @export none
 * 从桶里删掉最早插入的元素，hand前移。
 *
 * @param {tk_cache_clock_t*} cache 缓存对象。
 * @param {size_t} index 桶下标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_cache_clock_evict_bucket(tk_cache_clock_t* cache, size_t index) {
  size_t n = TK_RAW_HASH_TABLE_BUCKET_COUNT(&(cache->table));
  ret_t ret = RET_OK;

  /* 桶里是追加的，第0个才是最早插入的(更接近"最久未使用")。 */
  ret = tk_raw_hash_table_bucket_remove(&(cache->table), index, 0);
  return_value_if_fail(ret == RET_OK, ret);

  cache->hand = tk_cache_clock_next_index(index, n);

  if (TK_RAW_DARRAY_SIZE(cache->table.buckets[index]) == 0) {
    tk_raw_hash_table_bucket_shrink(&(cache->table), index);
  }

  return RET_OK;
}

/**
 * @method tk_cache_clock_evict
 * @annotation ["private"]
 * @export none
 * 淘汰一个元素：从hand开始扫桶，给被访问过的桶一次机会，然后从选中的桶里删掉一个元素。
 *
 * @param {tk_cache_clock_t*} cache 缓存对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，RET_NOT_FOUND表示没有可淘汰的元素。
 */
inline static ret_t tk_cache_clock_evict(tk_cache_clock_t* cache) {
  size_t i = 0, n = TK_RAW_HASH_TABLE_BUCKET_COUNT(&(cache->table));
  size_t index = cache->hand;

  /* 最多扫两圈：第一圈清掉"访问过"位(给第二次机会)，第二圈必然能选中一个。 */
  for (i = 0; i < n * 2; i++, index = tk_cache_clock_next_index(index, n)) {
    size_t pos = 0;

    /* 空桶跳过 */
    if (TK_RAW_DARRAY_SIZE(cache->table.buckets[index]) == 0) {
      continue;
    }
    /* 访问过的桶给一次机会 */
    pos = tk_cache_clock_bit_pos(index, TK_CACHE_CLOCK_BIT_ACCESSED);
    if (TK_BITSET_OP(TK_TEST_BIT, cache->flags, pos)) {
      TK_BITSET_OP(TK_CLEAR_BIT, cache->flags, pos);
      continue;
    }
    return tk_cache_clock_evict_bucket(cache, index);
  }

  return RET_NOT_FOUND;
}

/********************************************************************************************/
/* 给用户使用的函数 */

/**
 * @method tk_cache_clock_clear
 * @export none
 * 删除全部元素，保留桶数组和位图。
 * @param {tk_cache_clock_t*} cache 缓存对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_cache_clock_clear(tk_cache_clock_t* cache) {
  return_value_if_fail(cache != NULL && cache->table.buckets != NULL, RET_BAD_PARAMS);

  if (cache->flags != NULL) {
    size_t bits_size = TK_BITSET_BYTES_FOR_BITS(TK_RAW_HASH_TABLE_BUCKET_COUNT(&(cache->table)) *
                                                TK_CACHE_CLOCK_BIT_NR);
    memset(cache->flags, 0, bits_size);
  }
  cache->hand = 0;

  return tk_raw_hash_table_clear(&(cache->table));
}

/**
 * @method tk_cache_clock_deinit
 * @export none
 * 释放全部元素和位图，之后cache需要重新init才能用。
 * @param {tk_cache_clock_t*} cache 缓存对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_cache_clock_deinit(tk_cache_clock_t* cache) {
  return_value_if_fail(cache != NULL, RET_BAD_PARAMS);

  tk_cache_clock_clear(cache);
  if (cache->table.buckets != NULL) {
    tk_raw_hash_table_deinit(&cache->table);
  }
  TKMEM_FREE(cache->flags);

  return RET_OK;
}

/**
 * @method tk_cache_clock_init
 * @export none
 * 初始化缓存。
 * @param {tk_cache_clock_t*} cache 缓存对象。
 * @param {size_t} capacity 最多缓存多少个元素，必须大于0。
 * @param {tk_raw_hash_table_value_destroy_t} data_destroy 数据的销毁函数，可以为NULL。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_cache_clock_init(tk_cache_clock_t* cache, size_t capacity,
                                        tk_raw_hash_table_value_destroy_t data_destroy) {
  ret_t ret = RET_OK;
  size_t n = 0;
  return_value_if_fail(cache != NULL && capacity > 0, RET_BAD_PARAMS);

  memset(cache, 0x00, sizeof(tk_cache_clock_t));

  ret = tk_raw_hash_table_init(&(cache->table), capacity, data_destroy);
  return_value_if_fail(RET_OK == ret, ret);

  n = TK_RAW_HASH_TABLE_BUCKET_COUNT(&(cache->table));
  cache->flags = TKMEM_ZALLOCN(uint8_t, TK_BITSET_BYTES_FOR_BITS(n * TK_CACHE_CLOCK_BIT_NR));
  goto_error_if_fail_ex(cache->flags != NULL, ret = RET_OOM);

  return ret;
error:
  tk_cache_clock_deinit(cache);
  return ret;
}

/**
 * @method tk_cache_clock_get
 * @export none
 * 获取键对应的数据，命中时把它的桶标记为访问过(给二次机会)。
 * @param {tk_cache_clock_t*} cache 缓存对象。
 * @param {TK_CACHE_CLOCK_KEY_T} key 键。
 * @param {TK_CACHE_CLOCK_DATA_T*} data 返回的数据。
 *
 * @return {ret_t} 返回RET_OK表示命中，RET_NOT_FOUND表示没有。
 */
inline static ret_t tk_cache_clock_get(tk_cache_clock_t* cache, TK_CACHE_CLOCK_KEY_T key,
                                       TK_CACHE_CLOCK_DATA_T* data) {
  size_t index = 0;
  tk_raw_hash_table_entry_t* entry = NULL;
  return_value_if_fail(cache != NULL && cache->table.buckets != NULL && data != NULL,
                       RET_BAD_PARAMS);

  index = tk_raw_hash_table_bucket_index(&(cache->table), key);
  entry = tk_raw_hash_table_bucket_find(&(cache->table), index, key);
  if (entry == NULL) {
    return RET_NOT_FOUND;
  }

  tk_cache_clock_touch_index(cache, index);
  *data = entry->value;

  return RET_OK;
}

/**
 * @method tk_cache_clock_put
 * @export none
 * 写入元素并标记为访问过；超出capacity时先淘汰一个元素。
 *
 * @param {tk_cache_clock_t*} cache 缓存对象。
 * @param {TK_CACHE_CLOCK_KEY_T} key 键。
 * @param {TK_CACHE_CLOCK_DATA_T} data 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_cache_clock_put(tk_cache_clock_t* cache, TK_CACHE_CLOCK_KEY_T key,
                                       TK_CACHE_CLOCK_DATA_T data) {
  size_t index = 0;
  ret_t ret = RET_OK;
  tk_raw_hash_table_entry_t* entry = NULL;
  return_value_if_fail(cache != NULL && cache->flags != NULL && cache->table.buckets != NULL,
                       RET_BAD_PARAMS);

  index = tk_raw_hash_table_bucket_index(&(cache->table), key);
  entry = tk_raw_hash_table_bucket_find(&(cache->table), index, key);
  if (entry != NULL) {
    /* 键已存在：只换数据(传回同一个值不会销毁它)，再标记为访问过 */
    tk_raw_hash_table_entry_set_value(&(cache->table), entry, data);
    tk_cache_clock_touch_index(cache, index);
    return RET_OK;
  }

  if (cache->table.size >= cache->table.capacity) {
    /* 满了：先淘汰一个，腾出位置 */
    ret = tk_cache_clock_evict(cache);
    return_value_if_fail(ret == RET_OK, ret);
  }

  /* 桶下标在淘汰前后都不变，而且上面刚确认键不在桶里，直接加入，省掉add里的重复查找 */
  ret = tk_raw_hash_table_bucket_add(&(cache->table), index, key, data);
  return_value_if_fail(ret == RET_OK, ret);

  tk_cache_clock_touch_index(cache, index);

  return RET_OK;
}

/**
 * @method tk_cache_clock_remove
 * @export none
 * 删除键对应的元素。
 * @param {tk_cache_clock_t*} cache 缓存对象。
 * @param {TK_CACHE_CLOCK_KEY_T} key 键。
 *
 * @return {ret_t} 返回RET_OK表示成功，RET_NOT_FOUND表示没找到，否则表示失败。
 */
inline static ret_t tk_cache_clock_remove(tk_cache_clock_t* cache, TK_CACHE_CLOCK_KEY_T key) {
  ret_t ret = RET_OK;
  return_value_if_fail(cache != NULL && cache->flags != NULL, RET_BAD_PARAMS);

  ret = tk_raw_hash_table_remove(&(cache->table), key);

  if (RET_NOT_FOUND == ret) {
    return RET_NOT_FOUND;
  }
  return_value_if_fail(ret == RET_OK, ret);

  return RET_OK;
}

/**
 * @method tk_cache_clock_shrink
 * @export none
 * 按二次机会逐个淘汰，把元素个数缩到cache_size个；最后回收桶里闲置的槽位和空桶。
 * @param {tk_cache_clock_t*} cache 缓存对象。
 * @param {size_t} cache_size 保留的个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_cache_clock_shrink(tk_cache_clock_t* cache, size_t cache_size) {
  return_value_if_fail(cache != NULL && cache->flags != NULL, RET_BAD_PARAMS);

  while (cache->table.size > cache_size) {
    ret_t ret = tk_cache_clock_evict(cache);
    return_value_if_fail(ret == RET_OK, ret);
  }

  return tk_raw_hash_table_shrink(&(cache->table));
}

END_C_DECLS

#endif /*TK_CACHE_CLOCK_H*/
