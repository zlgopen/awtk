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

#ifndef PINYINIME_ANDPY_INCLUDE_LPICACHE_H__
#define PINYINIME_ANDPY_INCLUDE_LPICACHE_H__

#include <stdlib.h>
#include "./searchutility.h"
#include "./spellingtrie.h"

namespace ime_pinyin {

// Used to cache LmaPsbItem list for half spelling ids.
class LpiCache {
 private:
  static LpiCache *instance_;
  static const int kMaxLpiCachePerId = 15;

  LmaPsbItem *lpi_cache_;
  uint16 *lpi_cache_len_;

 public:
  LpiCache();
  ~LpiCache();

  static LpiCache& get_instance();

  // Test if the LPI list of the given splid  has been cached.
  // If splid is a full spelling id, it returns false, because we only cache
  // list for half ids.
  bool is_cached(uint16 splid);

  // Put LPI list to cahce. If the length of the list, lpi_num, is longer than
  // the cache buffer. the list will be truncated, and function returns the
  // maximum length of the cache buffer.
  // Note: splid must be a half id, and lpi_items must be not NULL. The
  // caller of this function should guarantee this.
  unsigned put_cache(uint16 splid, LmaPsbItem lpi_items[], unsigned lpi_num);

  // Get the cached list for the given half id.
  // Return the length of the cached buffer.
  // Note: splid must be a half id, and lpi_items must be not NULL. The
  // caller of this function should guarantee this.
  unsigned get_cache(uint16 splid, LmaPsbItem lpi_items[], unsigned lpi_max);
};

}  // namespace

#endif  // PINYINIME_ANDPY_INCLUDE_LPICACHE_H__
