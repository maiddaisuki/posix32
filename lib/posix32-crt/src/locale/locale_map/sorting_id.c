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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#include <assert.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

#if !P32_LOCALE_NAMES
#define SORTING_DECL(id, ll, sorting) {id, ll, sorting}

typedef struct SortingId {
  uint16_t      SortingId;
  LanguageIndex Language;
  SortingIndex  Sorting;
} SortingId;

static const SortingId SortingIds[] = {
  /* clang-format off */
  SORTING_DECL (SORT_GERMAN_PHONE_BOOK,      de, phoneb),
  SORTING_DECL (SORT_HUNGARIAN_DEFAULT,      hu, SortingIndex_default),
  SORTING_DECL (SORT_HUNGARIAN_TECHNICAL,    hu, technl),
  SORTING_DECL (SORT_JAPANESE_XJIS,          ja, SortingIndex_default),
  SORTING_DECL (SORT_JAPANESE_RADICALSTROKE, ja, radstr),
  SORTING_DECL (SORT_JAPANESE_UNICODE,       ja, SortingIndex_invalid),
  SORTING_DECL (SORT_GEORGIAN_TRADITIONAL,   ka, SortingIndex_default),
  SORTING_DECL (SORT_GEORGIAN_MODERN,        ka, modern),
  SORTING_DECL (SORT_KOREAN_KSC,             ko, SortingIndex_default),
  SORTING_DECL (SORT_KOREAN_UNICODE,         ko, SortingIndex_invalid),
  SORTING_DECL (SORT_CHINESE_BIG5,           zh, SortingIndex_default),
  SORTING_DECL (SORT_CHINESE_PRCP,           zh, SortingIndex_default),
  SORTING_DECL (SORT_CHINESE_RADICALSTROKE,  zh, radstr),
  SORTING_DECL (SORT_CHINESE_PRC,            zh, stroke),
  SORTING_DECL (SORT_CHINESE_BOPOMOFO,       zh, pronun),
  SORTING_DECL (SORT_CHINESE_UNICODE,        zh, SortingIndex_invalid),
  /* clang-format on */
};

SortingIndex p32_sorting_from_id (uint16_t sortingId, LanguageIndex ll) {
  for (SortingIdIndex i = 0; i < (SortingIdIndex) _countof (SortingIds); ++i) {
    if (SortingIds[i].Language == ll && SortingIds[i].SortingId == sortingId) {
      return SortingIds[i].Sorting;
    }
  }

  return sortingId == 0 ? SortingIndex_default : SortingIndex_invalid;
}
#endif
