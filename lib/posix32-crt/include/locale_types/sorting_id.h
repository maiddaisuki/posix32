/**
 * Copyright 2025-2026 Kirill Makurin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBPOSIX32_SORTING_ID_H_INCLUDED
#define LIBPOSIX32_SORTING_ID_H_INCLUDED

typedef enum SortingIdIndex {
  Sorting_de_phoneb = 0,
  Sorting_hu_default,
  Sorting_hu_technl,
  Sorting_ja_default,
  Sorting_ja_radstr,
  Sorting_ja_unicode,
  Sorting_ka_default,
  Sorting_ka_modern,
  Sorting_ko_default,
  Sorting_ko_unicode,
  Sorting_zh_Hans_default,
  Sorting_zh_Hant_default,
  Sorting_zh_radstr,
  Sorting_zh_stroke,
  Sorting_zh_pronun,
  Sorting_zh_unicode,
} SortingIdIndex;

#endif /* LIBPOSIX32_SORTING_ID_H_INCLUDED */
