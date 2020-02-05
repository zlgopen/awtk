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

#include <stdlib.h>

namespace ime_pinyin {

// For debug purpose. You can add a fixed version of qsort and bsearch functions
// here so that the output will be totally the same under different platforms.

void myqsort(void *p, unsigned n, unsigned es,
             int (*cmp)(const void *, const void *)) {
  qsort(p,n, es, cmp);
}

void *mybsearch(const void *k, const void *b,
                unsigned n, unsigned es,
                int (*cmp)(const void *, const void *)) {
  return bsearch(k, b, n, es, cmp);
}
}  // namespace ime_pinyin
