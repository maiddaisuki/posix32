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

#include "string-internal.h"

static int p32_strcmp_posix (const char *str1, const char *str2, locale_t locale) {
  return strcmp (str1, str2);
  UNREFERENCED_PARAMETER (locale);
}

static int p32_strcmp_common (const char *str1, const char *str2, locale_t locale) {
  /**
   * Conversion state for `str1`.
   */
  mbstate_t str1state = {0};

  /**
   * Conversion state for `str2`.
   */
  mbstate_t str2state = {0};

  while (1) {
    /**
     * Next Code Point in `str1`.
     */
    char32_t str1Char = 0xFFFFFFFF;

    const size_t length1 = p32_private_mbrtoc32_l (&str1Char, str1, locale->Charset.MaxLength, &str1state, locale);
    assert (length1 != (size_t) -2);

    if (length1 == (size_t) -1 || length1 == (size_t) -2) {
      return _NLSCMPERROR;
    }

    /**
     * Next Code Point in `str2`.
     */
    char32_t str2Char = 0xFFFFFFFF;

    const size_t length2 = p32_private_mbrtoc32_l (&str2Char, str2, locale->Charset.MaxLength, &str2state, locale);
    assert (length2 != (size_t) -2);

    if (length2 == (size_t) -1 || length2 == (size_t) -2) {
      return _NLSCMPERROR;
    }

    /**
     * Reached the end of `str1` or `str2`.
     */
    if (length1 == 0 || length2 == 0) {
      return !!str1Char - !!str2Char;
    }

    int diff = str1Char - str2Char;

    if (diff) {
      return diff;
    }

    str1 += length1;
    str2 += length2;
  }
}

static void P32LocaleFunction_strcmp (LocaleFunctions *functions, Charset *charset) {
  /**
   * All ISO-8859-1 Code Points map 1:1 with Unicode.
   */
  if (charset->CodePage == P32_CODEPAGE_ISO_8859_1) {
    functions->F_strcmp = p32_strcmp_posix;
  } else {
    functions->F_strcmp = p32_strcmp_common;
  }
}

int p32_private_strcmp_l (const char *str1, const char *str2, locale_t locale) {
  return locale->Functions.F_strcmp (str1, str2, locale);
}
