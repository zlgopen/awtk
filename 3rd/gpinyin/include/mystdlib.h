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

#ifndef PINYINIME_INCLUDE_MYSTDLIB_H__
#define PINYINIME_INCLUDE_MYSTDLIB_H__

#include <stdlib.h>

namespace ime_pinyin {

void myqsort(void *p, unsigned n, unsigned es,
             int (*cmp)(const void *, const void *));

void *mybsearch(const void *key, const void *base,
                unsigned nmemb, unsigned size,
                int (*compar)(const void *, const void *));
}

#endif  // PINYINIME_INCLUDE_MYSTDLIB_H__
