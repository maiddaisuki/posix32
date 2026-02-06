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

#ifndef LIBPOSIX32_LC_NUMERIC_H_INCLUDED
#define LIBPOSIX32_LC_NUMERIC_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

/**
 * Locale information dependant on `LC_NUMERIC` locale category.
 */
typedef struct LcNumericInfo {
  struct {
    char    *A;
    wchar_t *W;
  } RadixChar;
  struct {
    char    *A;
    wchar_t *W;
  } ThousandsSeparator;
} LcNumericInfo;

/**
 * Gather locale information dependant on `LC_NUMERIC` locale category.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_numeric (locale_t locale, uintptr_t heap);

/**
 * Copy locale information from `src` to `dest`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_numeric_copy (LcNumericInfo *dest, uintptr_t heap, LcNumericInfo *src);

/**
 * Free locale information stored in `lcNumericInfo`.
 */
void p32_localeinfo_numeric_free (LcNumericInfo *lcNumericInfo, uintptr_t heap);

#endif /* LIBPOSIX32_LC_NUMERIC_H_INCLUDED */
