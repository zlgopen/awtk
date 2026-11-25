/**
 * File:   raw_hash_table.h
 * Author: AWTK Develop Team
 * Brief:  原生哈希表。
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
 * 2026-11-23 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_RAW_HASH_TABLE_H
#define TK_RAW_HASH_TABLE_H

#include "tkc/raw_darray.h"

BEGIN_C_DECLS

/********************************************************************************************/
/* 可以配置的宏和类型，都要在包含本头文件之前定义，整个程序只能有一种配置 */

/* 键的类型。 */
#ifndef TK_RAW_HASH_TABLE_ENTRY_KEY_T
#define TK_RAW_HASH_TABLE_ENTRY_KEY_T size_t
#endif /*TK_RAW_HASH_TABLE_ENTRY_KEY_T*/

/* 值的类型。 */
#ifndef TK_RAW_HASH_TABLE_ENTRY_VALUE_T
#define TK_RAW_HASH_TABLE_ENTRY_VALUE_T void*
#endif /*TK_RAW_HASH_TABLE_ENTRY_VALUE_T*/

/**
 * @class tk_raw_hash_table_entry_t
 * @export none
 * 哈希表的一个元素。
 */
typedef struct _tk_raw_hash_table_entry_t {
  /**
   * @property {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key
   * @annotation ["readable"]
   * 键，用来计算桶下标，并在桶内匹配元素。
   */
  TK_RAW_HASH_TABLE_ENTRY_KEY_T key;

  /**
   * @property {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value
   * @annotation ["readable"]
   * 值。
   */
  TK_RAW_HASH_TABLE_ENTRY_VALUE_T value;
} tk_raw_hash_table_entry_t;

/**
 * 值的比较函数，返回0表示相等(实现里只判断是否为0)。
 * 默认按大小比较，值不支持>和<时(比如结构体)在这里定义，比如用memcmp。
 */
#ifndef TK_RAW_HASH_TABLE_VALUE_COMPARE
#define TK_RAW_HASH_TABLE_VALUE_COMPARE(entry_value, value) \
  ((entry_value) > (value) ? 1 : ((entry_value) < (value) ? -1 : 0))
#endif /*TK_RAW_HASH_TABLE_VALUE_COMPARE*/

/**
 * 键的比较函数，返回0表示相等(实现里只判断是否为0)。
 * 默认按大小比较，键不支持>和<时在这里定义，比如用strcmp。
 */
#ifndef TK_RAW_HASH_TABLE_KEY_COMPARE
#define TK_RAW_HASH_TABLE_KEY_COMPARE(entry_key, key) \
  ((entry_key) > (key) ? 1 : ((entry_key) < (key) ? -1 : 0))
#endif /*TK_RAW_HASH_TABLE_KEY_COMPARE*/

/**
 * 键的hash函数，默认用键的值本身。
 *
 * 它只负责把键映射成一个数，容器会用tk_raw_hash_table_mix再混合一次，
 * 所以自定义时不用自己混合，只要能区分不同的键就行：
 * 字符串键可以用named_value_hash_get_hash_from_str。
 */
#ifndef TK_RAW_HASH_TABLE_KEY_HASH
#define TK_RAW_HASH_TABLE_KEY_HASH(key) ((uint64_t)(size_t)(key))
#endif /*TK_RAW_HASH_TABLE_KEY_HASH*/

/**
 * 键的销毁函数，默认什么也不做。
 * 元素被删除(remove/clear/deinit/foreach返回RET_REMOVE)时调用。
 */
#ifndef TK_RAW_HASH_TABLE_KEY_DESTROY
#define TK_RAW_HASH_TABLE_KEY_DESTROY(key) \
  do {                                     \
    (void)(key);                           \
  } while (0)
#endif /*TK_RAW_HASH_TABLE_KEY_DESTROY*/

typedef ret_t (*tk_raw_hash_table_value_destroy_t)(TK_RAW_HASH_TABLE_ENTRY_VALUE_T value);

/**
 * 桶：存放元素的raw_darray。
 * NULL表示还没用过，加入第一个元素时才创建，clear/deinit时才回收。
 */
typedef TK_RAW_DARRAY_T(tk_raw_hash_table_entry_t) tk_raw_hash_table_bucket_t;

/**
 * @class tk_raw_hash_table_t
 * @export none
 * 原生哈希表。
 *
 * 示例：
 *
 * ```c
 * tk_raw_hash_table_t table;
 *
 * tk_raw_hash_table_init(&table, 100, NULL);
 * tk_raw_hash_table_add(&table, 1, (void*)100);
 * ENSURE(tk_raw_hash_table_find(&table, 1) != NULL);
 * tk_raw_hash_table_deinit(&table);
 * ```
 */
typedef struct _tk_raw_hash_table_t {
  /**
   * @property {tk_raw_hash_table_bucket_t*} buckets
   * @annotation ["readable"]
   * 桶数组。
   */
  TK_RAW_DARRAY_T(tk_raw_hash_table_bucket_t) buckets;

  /**
   * @property {tk_raw_hash_table_value_destroy_t} value_destroy
   * @annotation ["readable"]
   * 值的销毁函数，可以为NULL。
   */
  tk_raw_hash_table_value_destroy_t value_destroy;
} tk_raw_hash_table_t;

/**
 * @method TK_RAW_HASH_TABLE_BUCKET_COUNT
 * @annotation ["static"]
 * @export none
 * 获取桶数。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 *
 * @return {size_t} 返回桶数。
 */
#define TK_RAW_HASH_TABLE_BUCKET_COUNT(table) \
  (NULL != (table) ? TK_RAW_DARRAY_SIZE((table)->buckets) : 0)

/********************************************************************************************/
/* 文件内部使用 */

/**
 * @method tk_raw_hash_table_mix
 * @annotation ["static"]
 * @export none
 * 把任意值打散成均匀分布的hash值(容器内部会用它混合用户的hash输出)。
 * @param {uint64_t} hash 输入值。
 *
 * @return {uint64_t} 返回混合后的值。
 */
inline static uint64_t tk_raw_hash_table_mix(uint64_t hash) {
  /**
   * murmur3的fmix64：高低位都打散。
   * 桶下标取哪一段都均匀，见tk_raw_hash_table_bucket_index。
   */
  hash ^= hash >> 33;
  hash *= 0xff51afd7ed558ccdULL;
  hash ^= hash >> 33;
  hash *= 0xc4ceb9fe1a85ec53ULL;
  hash ^= hash >> 33;

  return hash;
}

/**
 * @method tk_raw_hash_table_bucket_count_for
 * @annotation ["static"]
 * @export none
 * 由期望容量计算桶数(负载因子2/3，可以是任意值)。
 * 因为桶下标用乘法算(见tk_raw_hash_table_bucket_index)，不需要凑2的幂或质数。
 * @param {size_t} capacity 期望容量(元素个数)。
 *
 * @return {size_t} 返回桶数，capacity为0时返回0。
 */
inline static size_t tk_raw_hash_table_bucket_count_for(size_t capacity) {
  size_t half_cap = 0;

  if (capacity == 0) {
    return 0;
  }

  half_cap = capacity / 2;
  if (capacity > (SIZE_MAX - half_cap)) {
    return capacity;
  }

  return capacity + half_cap;
}

/**
 * @method tk_raw_hash_table_destroy_value
 * @annotation ["static"]
 * @export none
 * 按table的配置销毁一个值，没有配置销毁函数时什么也不做。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则返回销毁函数的结果。
 */
inline static ret_t tk_raw_hash_table_destroy_value(tk_raw_hash_table_t* table,
                                                    TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  if (table->value_destroy != NULL) {
    return table->value_destroy(value);
  }
  return RET_OK;
}

/**
 * @method tk_raw_hash_table_set_value
 * @annotation ["static"]
 * @export none
 * 替换元素的值：新值和旧值不同时才用value_destroy销毁旧值。
 *
 * 是否相同见TK_RAW_HASH_TABLE_VALUE_COMPARE：传回表里的同一个值不会销毁它，
 * 所以调用方把取出来的值再写回去是安全的。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {tk_raw_hash_table_entry_t*} entry 元素。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 新值。
 */
inline static void tk_raw_hash_table_set_value(tk_raw_hash_table_t* table,
                                               tk_raw_hash_table_entry_t* entry,
                                               TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  if (0 != TK_RAW_HASH_TABLE_VALUE_COMPARE(entry->value, value)) {
    tk_raw_hash_table_destroy_value(table, entry->value);
  }
  entry->value = value;
}

/**
 * @method tk_raw_hash_table_bucket_add
 * @annotation ["static"]
 * @export none
 * 往桶里加入一个元素，桶还没创建时先创建它。
 * @param {tk_raw_hash_table_bucket_t*} p_bucket 桶的地址。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_bucket_add(tk_raw_hash_table_bucket_t* p_bucket,
                                                 TK_RAW_HASH_TABLE_ENTRY_KEY_T key,
                                                 TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  tk_raw_hash_table_entry_t entry;
  return_value_if_fail(p_bucket != NULL, RET_BAD_PARAMS);

  entry.key = key;
  entry.value = value;

  if (*p_bucket == NULL) {
    *p_bucket = TK_RAW_DARRAY_CREATE(tk_raw_hash_table_entry_t, 1);
    return_value_if_fail(*p_bucket != NULL, RET_OOM);
  }

  return_value_if_fail(TK_RAW_DARRAY_PUSH(*p_bucket, entry) == RET_OK, RET_OOM);

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_bucket_remove
 * @annotation ["static"]
 * @export none
 * 删除桶里第idx个元素：先从桶里移除，再销毁键和值。
 * 销毁函数里可以安全地再访问表(此时元素已经不在桶里了)。
 *
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {tk_raw_hash_table_bucket_t} bucket 桶。
 * @param {size_t} idx 元素下标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败(失败时不销毁)。
 */
inline static ret_t tk_raw_hash_table_bucket_remove(tk_raw_hash_table_t* table,
                                                    tk_raw_hash_table_bucket_t bucket, size_t idx) {
  tk_raw_hash_table_entry_t entry;
  return_value_if_fail(idx < TK_RAW_DARRAY_SIZE(bucket), RET_BAD_PARAMS);

  entry = bucket[idx];
  return_value_if_fail(RET_OK == TK_RAW_DARRAY_REMOVE(bucket, idx), RET_FAIL);

  TK_RAW_HASH_TABLE_KEY_DESTROY(entry.key);
  tk_raw_hash_table_destroy_value(table, entry.value);

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_bucket_clear
 * @annotation ["static"]
 * @export none
 * 销毁桶里的全部元素，并回收桶。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {tk_raw_hash_table_bucket_t*} p_bucket 桶的地址。
 */
inline static void tk_raw_hash_table_bucket_clear(tk_raw_hash_table_t* table,
                                                  tk_raw_hash_table_bucket_t* p_bucket) {
  size_t i = 0, n = 0;

  /* 空桶是正常情况。 */
  if (p_bucket == NULL || *p_bucket == NULL) {
    return;
  }

  n = TK_RAW_DARRAY_SIZE(*p_bucket);
  for (i = 0; i < n; i++) {
    TK_RAW_HASH_TABLE_KEY_DESTROY((*p_bucket)[i].key);
    tk_raw_hash_table_destroy_value(table, (*p_bucket)[i].value);
  }

  TK_RAW_DARRAY_DESTROY(*p_bucket);
}

/********************************************************************************************/
/* 给用户使用的函数 */

/**
 * @method tk_raw_hash_table_bucket_index
 * @annotation ["static"]
 * @export none
 * 计算键所在的桶下标：先用tk_raw_hash_table_mix把用户的hash混合成64位，取低32位，
 * 再乘桶数取乘积的高位(fastrange)映射到[0, 桶数)，比取余快几倍，桶数取任意值都行。
 *
 * @param {tk_raw_hash_table_t*} table 哈希表，不能为NULL。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 *
 * @return {size_t} 返回桶下标。
 */
inline static size_t tk_raw_hash_table_bucket_index(tk_raw_hash_table_t* table,
                                                    TK_RAW_HASH_TABLE_ENTRY_KEY_T key) {
  uint32_t h = (uint32_t)tk_raw_hash_table_mix(TK_RAW_HASH_TABLE_KEY_HASH(key));
  uint32_t n = (uint32_t)TK_RAW_HASH_TABLE_BUCKET_COUNT(table);

  return (size_t)(((uint64_t)h * n) >> 32);
}

/**
 * @method tk_raw_hash_table_bucket_find
 * @annotation ["static"]
 * @export none
 * 在桶里查找第一个键匹配的元素。
 * @param {tk_raw_hash_table_bucket_t} bucket 桶。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 *
 * @return {tk_raw_hash_table_entry_t*} 返回元素，没有找到返回NULL。
 */
inline static tk_raw_hash_table_entry_t* tk_raw_hash_table_bucket_find(
    tk_raw_hash_table_bucket_t bucket, TK_RAW_HASH_TABLE_ENTRY_KEY_T key) {
  size_t i = 0, n = TK_RAW_DARRAY_SIZE(bucket);

  for (i = 0; i < n; i++) {
    if (0 == TK_RAW_HASH_TABLE_KEY_COMPARE(bucket[i].key, key)) {
      return bucket + i;
    }
  }

  return NULL;
}

/**
 * @method tk_raw_hash_table_init
 * @annotation ["static"]
 * @export none
 * 初始化哈希表，桶数由capacity换算得到。
 * 会先把table清零，所以重复init之前要先deinit(否则原来的元素会漏掉)。
 *
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} capacity 期望容量(元素个数)，必须大于0。
 * @param {tk_raw_hash_table_value_destroy_t} value_destroy 值的销毁函数，可以为NULL。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_init(tk_raw_hash_table_t* table, size_t capacity,
                                           tk_raw_hash_table_value_destroy_t value_destroy) {
  size_t i = 0, count = 0;
  return_value_if_fail(table != NULL, RET_BAD_PARAMS);

  memset(table, 0x00, sizeof(tk_raw_hash_table_t));

  count = tk_raw_hash_table_bucket_count_for(capacity);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);

  table->buckets = TK_RAW_DARRAY_CREATE(tk_raw_hash_table_bucket_t, count);
  return_value_if_fail(table->buckets != NULL, RET_OOM);

  goto_error_if_fail(RET_OK == TK_RAW_DARRAY_RESIZE(table->buckets, count));

  for (i = 0; i < count; i++) {
    table->buckets[i] = NULL;
  }
  table->value_destroy = value_destroy;

  return RET_OK;
error:
  TK_RAW_DARRAY_DESTROY(table->buckets);
  return RET_OOM;
}

/**
 * @method tk_raw_hash_table_clear
 * @annotation ["static"]
 * @export none
 * 销毁全部元素，并回收所有桶，桶数不变。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_clear(tk_raw_hash_table_t* table) {
  size_t index = 0, n = 0;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  for (index = 0, n = TK_RAW_HASH_TABLE_BUCKET_COUNT(table); index < n; index++) {
    tk_raw_hash_table_bucket_clear(table, &(table->buckets[index]));
  }

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_shrink
 * @annotation ["static"]
 * @export none
 * 回收桶里闲置的槽位：桶内数组缩到实际用到的个数，空桶直接销毁。
 * 桶内数组是按需增长、从不收缩的，反复插入/删除(churn)后常驻内存会比实际需要的大；
 * 调用本函数把多余的内存还给分配器。桶的个数不变，元素及其键值都不动。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_shrink(tk_raw_hash_table_t* table) {
  size_t index = 0, n = 0;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  for (index = 0, n = TK_RAW_HASH_TABLE_BUCKET_COUNT(table); index < n; index++) {
    /* 没用过的桶 */
    if (table->buckets[index] == NULL) {
      continue;
    }

    /* 用过又空了的桶：整块回收，桶槽位恢复成"没用过" */
    if (TK_RAW_DARRAY_SIZE(table->buckets[index]) == 0) {
      TK_RAW_DARRAY_DESTROY(table->buckets[index]);
      continue;
    }

    return_value_if_fail(RET_OK == TK_RAW_DARRAY_SHRINK(table->buckets[index]), RET_FAIL);
  }

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_deinit
 * @annotation ["static"]
 * @export none
 * 释放全部元素和桶数组，并把table复位。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_deinit(tk_raw_hash_table_t* table) {
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  tk_raw_hash_table_clear(table);
  TK_RAW_DARRAY_DESTROY(table->buckets);

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_add
 * @annotation ["static"]
 * @export none
 * 加入一个元素，键已存在时返回失败(用modify修改已有的元素)。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，键已存在返回RET_FAIL(不接管value)，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_add(tk_raw_hash_table_t* table,
                                          TK_RAW_HASH_TABLE_ENTRY_KEY_T key,
                                          TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  size_t index = 0;
  tk_raw_hash_table_bucket_t bucket = NULL;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  index = tk_raw_hash_table_bucket_index(table, key);
  bucket = table->buckets[index];

  if (bucket != NULL && tk_raw_hash_table_bucket_find(bucket, key) != NULL) {
    return RET_FAIL;
  }

  return tk_raw_hash_table_bucket_add(&(table->buckets[index]), key, value);
}

/**
 * @method tk_raw_hash_table_find
 * @annotation ["static"]
 * @export none
 * 查找第一个键匹配的元素。
 * 返回的是桶内元素的地址，add可能让桶内数组扩容，之后不要再用它。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 *
 * @return {tk_raw_hash_table_entry_t*} 返回元素，没有找到返回NULL。
 */
inline static tk_raw_hash_table_entry_t* tk_raw_hash_table_find(tk_raw_hash_table_t* table,
                                                                TK_RAW_HASH_TABLE_ENTRY_KEY_T key) {
  size_t index = 0;
  return_value_if_fail(table != NULL && table->buckets != NULL, NULL);

  index = tk_raw_hash_table_bucket_index(table, key);

  return tk_raw_hash_table_bucket_find(table->buckets[index], key);
}

/**
 * @method tk_raw_hash_table_modify
 * @annotation ["static"]
 * @export none
 * 修改键对应的值(旧值和新值不同时由value_destroy销毁)，键不存在时返回失败(用add加入)。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，RET_NOT_FOUND表示没找到，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_modify(tk_raw_hash_table_t* table,
                                             TK_RAW_HASH_TABLE_ENTRY_KEY_T key,
                                             TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  tk_raw_hash_table_entry_t* entry = NULL;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  entry = tk_raw_hash_table_find(table, key);
  if (entry == NULL) {
    return RET_NOT_FOUND;
  }

  tk_raw_hash_table_set_value(table, entry, value);

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_replace
 * @annotation ["static"]
 * @export none
 * 键存在时替换值(旧值和新值不同时由value_destroy销毁)，不存在时加入。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_replace(tk_raw_hash_table_t* table,
                                              TK_RAW_HASH_TABLE_ENTRY_KEY_T key,
                                              TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  ret_t ret = RET_OK;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  ret = tk_raw_hash_table_modify(table, key, value);
  if (ret == RET_NOT_FOUND) {
    ret = tk_raw_hash_table_add(table, key, value);
  }

  return ret;
}

/**
 * @method tk_raw_hash_table_remove
 * @annotation ["static"]
 * @export none
 * 删除键对应的元素，键和值交给销毁函数处理，桶保留。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 *
 * @return {ret_t} 返回RET_OK表示成功，RET_NOT_FOUND表示没找到，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_remove(tk_raw_hash_table_t* table,
                                             TK_RAW_HASH_TABLE_ENTRY_KEY_T key) {
  size_t index = 0, i = 0;
  tk_raw_hash_table_entry_t* entry = NULL;
  tk_raw_hash_table_bucket_t bucket = NULL;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  index = tk_raw_hash_table_bucket_index(table, key);
  bucket = table->buckets[index];
  entry = tk_raw_hash_table_bucket_find(bucket, key);
  if (entry == NULL) {
    return RET_NOT_FOUND;
  }

  i = (size_t)(entry - bucket);

  return tk_raw_hash_table_bucket_remove(table, bucket, i);
}

/**
 * @method tk_raw_hash_table_size
 * @annotation ["static"]
 * @export none
 * 返回元素个数，需要遍历所有桶。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 *
 * @return {size_t} 返回元素个数。
 */
inline static size_t tk_raw_hash_table_size(tk_raw_hash_table_t* table) {
  size_t index = 0, count = 0, n = 0;
  return_value_if_fail(table != NULL && table->buckets != NULL, 0);

  for (index = 0, n = TK_RAW_HASH_TABLE_BUCKET_COUNT(table); index < n; index++) {
    count += TK_RAW_DARRAY_SIZE(table->buckets[index]);
  }

  return count;
}

/**
 * @method tk_raw_hash_table_foreach
 * @annotation ["static"]
 * @export none
 * 遍历全部元素，返回值按TK\_FOREACH\_VISIT\_RESULT\_PROCESSING处理(RET\_REMOVE会删除当前元素)。
 *
 * 要删除元素请返回RET\_REMOVE，不要在visit里直接调用remove(会漏掉元素)。
 * visit里可以add，但加入后它的data(元素地址)可能失效，不要在add之后再用。
 *
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {tk_visit_t} visit 遍历函数，data是元素的地址。
 * @param {void*} ctx 遍历函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_foreach(tk_raw_hash_table_t* table, tk_visit_t visit,
                                              void* ctx) {
  ret_t ret = RET_OK;
  size_t index = 0, n = 0;
  return_value_if_fail(table != NULL && table->buckets != NULL && visit != NULL, RET_BAD_PARAMS);

  for (index = 0, n = TK_RAW_HASH_TABLE_BUCKET_COUNT(table); index < n && ret == RET_OK; index++) {
    size_t i = 0;
    while (i < TK_RAW_DARRAY_SIZE(table->buckets[index])) {
      tk_raw_hash_table_bucket_t bucket = table->buckets[index];
      ret = visit(ctx, bucket + i);

      /* 遍历函数可能加了元素，桶内数组可能被realloc，重新取。 */
      bucket = table->buckets[index];

      TK_FOREACH_VISIT_RESULT_PROCESSING(
          ret,
          continue_if_fail_ex(RET_OK == tk_raw_hash_table_bucket_remove(table, bucket, i), i++));

      i++;
    }
  }

  return ret;
}

END_C_DECLS

#endif /*TK_RAW_HASH_TABLE_H*/
