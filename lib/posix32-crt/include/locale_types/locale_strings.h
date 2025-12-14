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

#ifndef LIBPOSIX32_LOCALE_STRINGS_H_INCLUDED
#define LIBPOSIX32_LOCALE_STRINGS_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "locale_types/windows_locale.h"

/**
 * Locale string formats.
 */
typedef enum LocaleStringFormat {
  /**
   * Explicit request to store "C" string.
   */
  LOCALE_STRING_FORMAT_C = 0,
  /**
   * Format locale strings using the same format as Windows locale names.
   */
  LOCALE_STRING_FORMAT_WINDOWS,
  /**
   * Format locale strings for use with CRT's functions `[_w]setlocale` and
   * `_[w]create_locale`.
   */
  LOCALE_STRING_FORMAT_CRT,
  /**
   * Format locale strings using portable format `ll_CC.CHARSET@MODIFIER`.
   */
  LOCALE_STRING_FORMAT_ISO,
} LocaleStringFormat;

/**
 * Wide locale strings.
 */
typedef struct LocaleStringsW {
  /**
   * Code page to include in formatted string.
   */
  uint32_t CodePage;
  wchar_t *LcAll;
  wchar_t *LcCollate;
  wchar_t *LcCtype;
  wchar_t *LcMessages;
  wchar_t *LcMonetary;
  wchar_t *LcNumeric;
  wchar_t *LcTime;
} LocaleStringsW;

/**
 * Multibyte locale strings.
 */
typedef struct LocaleStringsA {
  /**
   * Code page to use when converting locale strings (from `LocaleStringsW`).
   */
  uint32_t CodePage;
  char    *LcAll;
  char    *LcCollate;
  char    *LcCtype;
  char    *LcMessages;
  char    *LcMonetary;
  char    *LcNumeric;
  char    *LcTime;
} LocaleStringsA;

/**
 * Locale strings.
 */
typedef struct LocaleStrings {
  /**
   * Input: How to format locale string stored in `W->LcAll`.
   *
   * If this fields is set to `LC_ALL`, then `W->LcAll` will contain single
   * locale string.
   *
   * If this fields is set to `LC_ALL_MASK`, then `W->LcAll` will contain
   * ;-separated list constructed from all other `W->Lc*` fields.
   */
  int Mask;
  /**
   * Input: Locale string format to use.
   */
  LocaleStringFormat Format;
  LocaleStringsA     A;
  LocaleStringsW     W;
} LocaleStrings;

/**
 * Split locale string `string`.
 *
 * If `string` is ;-separated list specifying multiple locale string, store
 * string for each locale category in corresponding member of `localeStrings`
 * and set corresponding `LC_*_MASK` bits in `localeStrings->Mask`.
 *
 * Otherwise, set `localeString->Mask` to 0 (`LC_ALL`) and store single
 * locale string in `localeStrings->LcAll`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_localestr_split (LocaleStrings *localeStrings, uintptr_t heap, const wchar_t *string);

/**
 * Obtain locale string corresponding to user's default locale.
 *
 * Single locale string is stored in `localeStrings->LcAll`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_localestr_user_default (LocaleStrings *localeStrings, uintptr_t heap);

/**
 * Obtain locale strings for each locale category specified in `mask`.
 *
 * If environment variable `LC_ALL` is set to non-empty string, this string
 * is stored in `localeStrings->LcAll` and `localeStrings->Mask` is set to 0.
 *
 * Otherwise, check for category-specific environment variables and try to
 * fallback to `LANG` for each one that is not set.
 *
 * For each locale category in `mask` which are not retrieved from environment,
 * use user's default locale.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_localestr_from_env (LocaleStrings *localeStrings, uintptr_t heap, int mask);

/**
 * Format locale string for each locale in `winLocale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_localestr_format (LocaleStrings *localeStrings, uintptr_t heap, WindowsLocale *winLocale);

/**
 * Copy locale strings from `src` to `dest`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_localestr_copy (LocaleStrings *dest, uintptr_t heap, LocaleStrings *src);

/**
 * Free string stored in `localeStrings`.
 */
P32_TEST_DECL void p32_localestr_free (LocaleStrings *localeStrings, uintptr_t heap);

#endif /* LIBPOSIX32_LOCALE_STRINGS_H_INCLUDED */
