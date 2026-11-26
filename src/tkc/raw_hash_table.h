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
 * @annotation ["private"]
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
 * @enum tk_raw_hash_table_capacity_policy_t
 * @prefix TK_RAW_HASH_TABLE_CAPACITY_POLICY_
 * @annotation ["private"]
 * 元素个数将要超过capacity时的处理策略。
 */
typedef enum _tk_raw_hash_table_capacity_policy_t {
  /**
   * @const TK_RAW_HASH_TABLE_CAPACITY_POLICY_IGNORE
   * 忽略：照样加入，桶内会多挂几个元素(默认)。
   */
  TK_RAW_HASH_TABLE_CAPACITY_POLICY_IGNORE = 0,
  /**
   * @const TK_RAW_HASH_TABLE_CAPACITY_POLICY_FAIL
   * 返回失败：加不进去，表不变。
   */
  TK_RAW_HASH_TABLE_CAPACITY_POLICY_FAIL,
  /**
   * @const TK_RAW_HASH_TABLE_CAPACITY_POLICY_GROW
   * 自动扩容：先用更大的容量reserve(必要时rehash)，再加入。
   */
  TK_RAW_HASH_TABLE_CAPACITY_POLICY_GROW,
} tk_raw_hash_table_capacity_policy_t;

/**
 * @class tk_raw_hash_table_t
 * @annotation ["private"]
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
   * @property {size_t} size
   * @annotation ["readable"]
   * 当前元素个数，加入/删除/清空时维护。
   */
  size_t size;

  /**
   * @property {size_t} capacity
   * @annotation ["readable"]
   * 期望容量(元素个数)。
   * 桶数按它换算；它只是记录调用方的期望值，不是硬上限。
   */
  size_t capacity;

  /**
   * @property {tk_raw_hash_table_value_destroy_t} value_destroy
   * @annotation ["readable"]
   * 值的销毁函数，可以为NULL。
   */
  tk_raw_hash_table_value_destroy_t value_destroy;

  /**
   * @property {tk_raw_hash_table_capacity_policy_t} capacity_policy
   * @annotation ["readable"]
   * 元素个数将要超过capacity时的处理策略。
   */
  tk_raw_hash_table_capacity_policy_t capacity_policy;
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
 * @annotation ["private", "static"]
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
 * @annotation ["private", "static"]
 * @export none
 * 由期望容量计算桶数(负载因子2/3，可以是任意值)。
 * 因为桶下标用乘法算(见tk_raw_hash_table_bucket_index)，不需要凑2的幂或质数。
 * @param {size_t} capacity 期望容量(元素个数)。
 *
 * @return {size_t} 返回桶数。
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
 * @annotation ["private"]
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

inline static size_t tk_raw_hash_table_bucket_index(tk_raw_hash_table_t* table,
                                                    TK_RAW_HASH_TABLE_ENTRY_KEY_T key);
inline static ret_t tk_raw_hash_table_bucket_add(tk_raw_hash_table_t* table, size_t index,
                                                 TK_RAW_HASH_TABLE_ENTRY_KEY_T key,
                                                 TK_RAW_HASH_TABLE_ENTRY_VALUE_T value);
inline static ret_t tk_raw_hash_table_reserve(tk_raw_hash_table_t* table, size_t capacity);

/**
 * @method tk_raw_hash_table_rehash
 * @annotation ["private"]
 * @export none
 * 按count个桶重建桶数组并重新计算哈希值。
 * 只改桶数，不动capacity字段。重新计算哈希值后元素所在的桶会变，之前查到的元素地址都失效。
 *
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} count 新的桶数，必须大于0。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
TK_MAYBE_UNUSED static ret_t tk_raw_hash_table_rehash(tk_raw_hash_table_t* table, size_t count) {
  ret_t ret = RET_OK;
  size_t i = 0;
  size_t tmp_size = 0;
  tk_raw_hash_table_bucket_t* tmp_buckets = NULL;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);

  tmp_buckets = TK_RAW_DARRAY_CREATE(tk_raw_hash_table_bucket_t, count);
  return_value_if_fail(tmp_buckets != NULL, RET_OOM);
  if (RET_OK != TK_RAW_DARRAY_RESIZE(tmp_buckets, count)) {
    log_warn("%s:%d condition(RET_OK == TK_RAW_DARRAY_RESIZE(tmp_buckets, count)) failed!\n",
             __FUNCTION__, __LINE__);
    TK_RAW_DARRAY_DESTROY(tmp_buckets);
    return RET_OOM;
  }

  for (i = 0; i < TK_RAW_DARRAY_SIZE(tmp_buckets); i++) {
    tmp_buckets[i] = NULL;
  }

  tk_swap(table->buckets, tmp_buckets, tk_raw_hash_table_bucket_t*);
  tk_swap(table->size, tmp_size, size_t);

  /* 只拷贝键和值，不碰任何销毁函数；桶下标按新桶数算。 */
  for (i = 0; i < TK_RAW_DARRAY_SIZE(tmp_buckets); i++) {
    size_t j = 0;
    for (j = 0; j < TK_RAW_DARRAY_SIZE(tmp_buckets[i]); j++) {
      size_t index = tk_raw_hash_table_bucket_index(table, tmp_buckets[i][j].key);
      ret = tk_raw_hash_table_bucket_add(table, index, tmp_buckets[i][j].key,
                                         tmp_buckets[i][j].value);
      goto_error_if_fail(RET_OK == ret);
    }
  }

  ENSURE(tmp_size == table->size);
error:
  if (RET_OK != ret) {
    /* 新表里只是刚搬过去的副本，整块丢掉，换回没动过的旧桶数组，表就恢复成rehash之前的样子。 */
    tk_swap(table->buckets, tmp_buckets, tk_raw_hash_table_bucket_t*);
    tk_swap(table->size, tmp_size, size_t);
  }

  for (i = 0; i < TK_RAW_DARRAY_SIZE(tmp_buckets); i++) {
    if (tmp_buckets[i] != NULL) {
      TK_RAW_DARRAY_DESTROY(tmp_buckets[i]);
    }
  }
  TK_RAW_DARRAY_DESTROY(tmp_buckets);

  return ret;
}

/**
 * @method tk_raw_hash_table_check_capacity
 * @annotation ["private"]
 * @export none
 * 检查能否容纳size个元素：size > capacity时按table->capacity_policy忽略、失败或者扩容。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} size 需要容纳的元素个数(加入之后/当前的元素个数)。
 *
 * @return {ret_t} 返回RET_OK表示可以加入，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_check_capacity(tk_raw_hash_table_t* table, size_t size) {
  return_value_if_fail(table != NULL, RET_BAD_PARAMS);

  if (size <= table->capacity) {
    return RET_OK;
  }

  switch (table->capacity_policy) {
    case TK_RAW_HASH_TABLE_CAPACITY_POLICY_GROW: {
      /* 翻倍，免得每加一个元素就rehash一次；至少要放得下需要的个数。 */
      size_t capacity = table->capacity;
      if (capacity <= (SIZE_MAX / 2)) {
        capacity += capacity;
      }
      if (capacity <= size) {
        capacity = size + 1;
      }
      return tk_raw_hash_table_reserve(table, capacity);
    }
    case TK_RAW_HASH_TABLE_CAPACITY_POLICY_FAIL:
      return RET_EXCEED_RANGE;
    case TK_RAW_HASH_TABLE_CAPACITY_POLICY_IGNORE:
    default:
      return RET_OK;
  }
}

/********************************************************************************************/
/* 给用户使用的函数 */

/**
 * @method tk_raw_hash_table_entry_set_value
 * @export none
 * 替换元素的值：新值和旧值不同时才用value_destroy销毁旧值。
 *
 * 是否相同见TK_RAW_HASH_TABLE_VALUE_COMPARE：传回表里的同一个值不会销毁它，
 * 所以调用方把取出来的值再写回去是安全的。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {tk_raw_hash_table_entry_t*} entry 元素。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 新值。
 */
inline static void tk_raw_hash_table_entry_set_value(tk_raw_hash_table_t* table,
                                                     tk_raw_hash_table_entry_t* entry,
                                                     TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  if (0 != TK_RAW_HASH_TABLE_VALUE_COMPARE(entry->value, value)) {
    tk_raw_hash_table_destroy_value(table, entry->value);
  }
  entry->value = value;
}

/**
 * @method tk_raw_hash_table_bucket_index
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
 * @export none
 * 在第index个桶里查找第一个键匹配的元素。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} index 桶下标。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 *
 * @return {tk_raw_hash_table_entry_t*} 返回元素，没有找到返回NULL。
 */
inline static tk_raw_hash_table_entry_t* tk_raw_hash_table_bucket_find(
    tk_raw_hash_table_t* table, size_t index, TK_RAW_HASH_TABLE_ENTRY_KEY_T key) {
  size_t i = 0, n = 0;
  tk_raw_hash_table_bucket_t bucket = NULL;
  return_value_if_fail(table != NULL && table->buckets != NULL, NULL);
  return_value_if_fail(index < TK_RAW_HASH_TABLE_BUCKET_COUNT(table), NULL);

  bucket = table->buckets[index];

  for (i = 0, n = TK_RAW_DARRAY_SIZE(bucket); i < n; i++) {
    if (0 == TK_RAW_HASH_TABLE_KEY_COMPARE(bucket[i].key, key)) {
      return bucket + i;
    }
  }

  return NULL;
}

/**
 * @method tk_raw_hash_table_bucket_add
 * @export none
 * 往第index个桶里加入一个元素，桶还没创建时先创建它。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} index 桶下标。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_bucket_add(tk_raw_hash_table_t* table, size_t index,
                                                 TK_RAW_HASH_TABLE_ENTRY_KEY_T key,
                                                 TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  tk_raw_hash_table_entry_t entry;
  tk_raw_hash_table_bucket_t* p_bucket = NULL;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < TK_RAW_HASH_TABLE_BUCKET_COUNT(table), RET_BAD_PARAMS);

  entry.key = key;
  entry.value = value;
  p_bucket = table->buckets + index;

  if (*p_bucket == NULL) {
    *p_bucket = TK_RAW_DARRAY_CREATE(tk_raw_hash_table_entry_t, 1);
    return_value_if_fail(*p_bucket != NULL, RET_OOM);
  }

  return_value_if_fail(TK_RAW_DARRAY_PUSH(*p_bucket, entry) == RET_OK, RET_OOM);
  table->size++;

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_bucket_remove
 * @export none
 * 删除第index个桶里第pos个元素：先从桶里移除，再销毁键和值。
 * 销毁函数里可以安全地再访问表(此时元素已经不在桶里了)。
 *
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} index 桶下标。
 * @param {size_t} pos 桶内元素下标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败(失败时不销毁)。
 */
inline static ret_t tk_raw_hash_table_bucket_remove(tk_raw_hash_table_t* table, size_t index,
                                                    size_t pos) {
  tk_raw_hash_table_entry_t entry;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < TK_RAW_HASH_TABLE_BUCKET_COUNT(table), RET_BAD_PARAMS);
  return_value_if_fail(pos < TK_RAW_DARRAY_SIZE(table->buckets[index]), RET_BAD_PARAMS);

  entry = table->buckets[index][pos];
  return_value_if_fail(RET_OK == TK_RAW_DARRAY_REMOVE(table->buckets[index], pos), RET_FAIL);

  table->size--;
  TK_RAW_HASH_TABLE_KEY_DESTROY(entry.key);
  tk_raw_hash_table_destroy_value(table, entry.value);

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_bucket_clear
 * @export none
 * 销毁第index个桶里的全部元素，并回收桶。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} index 桶下标。
 */
inline static void tk_raw_hash_table_bucket_clear(tk_raw_hash_table_t* table, size_t index) {
  size_t i = 0, n = 0;
  return_if_fail(table != NULL && table->buckets != NULL);
  return_if_fail(index < TK_RAW_HASH_TABLE_BUCKET_COUNT(table));

  /* 空桶是正常情况。 */
  if (table->buckets[index] == NULL) {
    return;
  }

  for (i = 0, n = TK_RAW_DARRAY_SIZE(table->buckets[index]); i < n; i++) {
    table->size--;
    TK_RAW_HASH_TABLE_KEY_DESTROY(table->buckets[index][i].key);
    tk_raw_hash_table_destroy_value(table, table->buckets[index][i].value);
  }

  TK_RAW_DARRAY_DESTROY(table->buckets[index]);
}

/**
 * @method tk_raw_hash_table_bucket_shrink
 * @export none
 * 回收第index个桶的闲置空间：空桶直接销毁，否则缩到实际用到的个数。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} index 桶下标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_bucket_shrink(tk_raw_hash_table_t* table, size_t index) {
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < TK_RAW_HASH_TABLE_BUCKET_COUNT(table), RET_BAD_PARAMS);

  if (table->buckets[index] == NULL) {
    return RET_OK;
  }

  if (0 == TK_RAW_DARRAY_SIZE(table->buckets[index])) {
    return TK_RAW_DARRAY_DESTROY(table->buckets[index]);
  }

  return TK_RAW_DARRAY_SHRINK(table->buckets[index]);
}

/**
 * @method tk_raw_hash_table_init
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
  table->capacity = capacity;
  table->value_destroy = value_destroy;

  return RET_OK;
error:
  TK_RAW_DARRAY_DESTROY(table->buckets);
  return RET_OOM;
}

/**
 * @method tk_raw_hash_table_clear
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
    tk_raw_hash_table_bucket_clear(table, index);
  }
  ENSURE(0 == table->size);

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_shrink
 * @export none
 * 回收所有桶的闲置空间：空桶直接销毁，非空桶缩到实际用到的个数。
 * 桶内数组只按需增长、从不收缩，反复增删(churn)后用本函数把多余的内存还给分配器。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_shrink(tk_raw_hash_table_t* table) {
  size_t index = 0, n = 0;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  for (index = 0, n = TK_RAW_HASH_TABLE_BUCKET_COUNT(table); index < n; index++) {
    return_value_if_fail(RET_OK == tk_raw_hash_table_bucket_shrink(table, index), RET_FAIL);
  }

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_deinit
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
  table->capacity = 0;

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_add
 * @export none
 * 加入一个元素，键已存在时返回失败(用modify修改已有的元素)。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {TK_RAW_HASH_TABLE_ENTRY_KEY_T} key 键。
 * @param {TK_RAW_HASH_TABLE_ENTRY_VALUE_T} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_add(tk_raw_hash_table_t* table,
                                          TK_RAW_HASH_TABLE_ENTRY_KEY_T key,
                                          TK_RAW_HASH_TABLE_ENTRY_VALUE_T value) {
  size_t index = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  index = tk_raw_hash_table_bucket_index(table, key);

  if (tk_raw_hash_table_bucket_find(table, index, key) != NULL) {
    /* 键已存在：不增长，按原语义返回失败(用modify改值)，也不该触发容量策略。 */
    return RET_FAIL;
  }

  /* 新键才可能超过capacity。 */
  ret = tk_raw_hash_table_check_capacity(table, table->size + 1);
  return_value_if_fail(RET_OK == ret, ret);

  /* 自动扩容可能rehash，桶数变了，下标要重算。 */
  index = tk_raw_hash_table_bucket_index(table, key);

  return tk_raw_hash_table_bucket_add(table, index, key, value);
}

/**
 * @method tk_raw_hash_table_find
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

  return tk_raw_hash_table_bucket_find(table, index, key);
}

/**
 * @method tk_raw_hash_table_modify
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

  tk_raw_hash_table_entry_set_value(table, entry, value);

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_replace
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
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);

  index = tk_raw_hash_table_bucket_index(table, key);
  entry = tk_raw_hash_table_bucket_find(table, index, key);
  if (entry == NULL) {
    return RET_NOT_FOUND;
  }

  i = (size_t)(entry - table->buckets[index]);

  return tk_raw_hash_table_bucket_remove(table, index, i);
}

/**
 * @method tk_raw_hash_table_foreach
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

      /* 遍历函数可能加了元素，桶内数组可能被realloc，移除时按桶下标重新取。 */
      TK_FOREACH_VISIT_RESULT_PROCESSING(
          ret,
          continue_if_fail_ex(RET_OK == tk_raw_hash_table_bucket_remove(table, index, i), i++));

      i++;
    }
  }

  return ret;
}

/**
 * @method tk_raw_hash_table_set_capacity_policy
 * @export none
 * 设置size > capacity 时的处理策略(策略变化时立即按新策略处理当前的size)。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {tk_raw_hash_table_capacity_policy_t} capacity_policy 处理策略。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_set_capacity_policy(
    tk_raw_hash_table_t* table, tk_raw_hash_table_capacity_policy_t capacity_policy) {
  return_value_if_fail(table != NULL, RET_BAD_PARAMS);

  if (capacity_policy != table->capacity_policy) {
    table->capacity_policy = capacity_policy;
    tk_raw_hash_table_check_capacity(table, table->size);
  }

  return RET_OK;
}

/**
 * @method tk_raw_hash_table_reserve
 * @export none
 * 预留容量(元素个数)：扩容到能装下capacity个元素。
 * @param {tk_raw_hash_table_t*} table 哈希表。
 * @param {size_t} capacity 期望容量(元素个数)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
inline static ret_t tk_raw_hash_table_reserve(tk_raw_hash_table_t* table, size_t capacity) {
  size_t count = 0;
  return_value_if_fail(table != NULL && table->buckets != NULL, RET_BAD_PARAMS);
  return_value_if_fail(capacity > 0, RET_BAD_PARAMS);
  return_value_if_fail(capacity >= table->size, RET_FAIL);

  count = tk_raw_hash_table_bucket_count_for(capacity);
  if (count > TK_RAW_HASH_TABLE_BUCKET_COUNT(table)) {
    ret_t ret = tk_raw_hash_table_rehash(table, count);
    return_value_if_fail(RET_OK == ret, ret);

    table->capacity = capacity;
  }

  return RET_OK;
}

END_C_DECLS

#endif /*TK_RAW_HASH_TABLE_H*/
