/**
 * Copyright 2025 Kirill Makurin
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

#ifndef LIBPOSIX32_LC_COLLATE_H_INCLUDED
#define LIBPOSIX32_LC_COLLATE_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

/**
 * Locale information dependant on `LC_COLLATE` locale category.
 */
typedef struct LcCollateInfo {
  /**
   * Used by `strcoll` and `wcscoll`.
   */
  uint32_t StringCompareFlags;
  /**
   * TODO: Used by `strcasecoll` and `wcscasecoll`.
   */
  uint32_t StringCaseCompareFlags;
  /**
   * Used by `strxfrm` and `wcsxfrm`.
   */
  uint32_t StringTransformationFlags;
} LcCollateInfo;

/**
 * Gather locale information dependant on `LC_COLLATE` locale category.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_collate (locale_t locale, uintptr_t heap);

/**
 * Copy locale information from `src` to `dest`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_collate_copy (LcCollateInfo *dest, uintptr_t heap, LcCollateInfo *src);

/**
 * Free locale information stored in `lcCollateInfo`.
 */
void p32_localeinfo_collate_free (LcCollateInfo *lcCollateInfo, uintptr_t heap);

#endif /* LIBPOSIX32_LC_COLLATE_H_INCLUDED */
