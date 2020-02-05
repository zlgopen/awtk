/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include "../include/lpicache.h"

namespace ime_pinyin {

LpiCache* LpiCache::instance_ = NULL;

LpiCache::LpiCache() {
  lpi_cache_ = new LmaPsbItem[kFullSplIdStart * kMaxLpiCachePerId];
  lpi_cache_len_ = new uint16[kFullSplIdStart];
  assert(NULL != lpi_cache_);
  assert(NULL != lpi_cache_len_);
  for (uint16 id = 0; id < kFullSplIdStart; id++)
    lpi_cache_len_[id] = 0;
}

LpiCache::~LpiCache() {
  if (NULL != lpi_cache_)
    delete [] lpi_cache_;

  if (NULL != lpi_cache_len_)
    delete [] lpi_cache_len_;
}

LpiCache& LpiCache::get_instance() {
  if (NULL == instance_) {
    instance_ = new LpiCache();
    assert(NULL != instance_);
  }
  return *instance_;
}

bool LpiCache::is_cached(uint16 splid) {
  if (splid >= kFullSplIdStart)
    return false;
  return lpi_cache_len_[splid] != 0;
}

unsigned LpiCache::put_cache(uint16 splid, LmaPsbItem lpi_items[],
                           unsigned lpi_num) {
  uint16 num = kMaxLpiCachePerId;
  if (num > lpi_num)
    num = static_cast<uint16>(lpi_num);

  LmaPsbItem *lpi_cache_this = lpi_cache_ + splid * kMaxLpiCachePerId;
  for (uint16 pos = 0; pos < num; pos++)
    lpi_cache_this[pos] = lpi_items[pos];

  lpi_cache_len_[splid] = num;
  return num;
}

unsigned LpiCache::get_cache(uint16 splid, LmaPsbItem lpi_items[],
                           unsigned lpi_max) {
  if (lpi_max > lpi_cache_len_[splid])
    lpi_max = lpi_cache_len_[splid];

  LmaPsbItem *lpi_cache_this = lpi_cache_ + splid * kMaxLpiCachePerId;
  for (uint16 pos = 0; pos < lpi_max; pos++) {
    lpi_items[pos] = lpi_cache_this[pos];
  }
  return lpi_max;
}

}  // namespace ime_pinyin
