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

static int p32_strncmp_posix (const char *str1, const char *str2, size_t count, locale_t locale) {
  return strncmp (str1, str2, count);
  UNREFERENCED_PARAMETER (locale);
}

static int p32_strncmp_common (const char *str1, const char *str2, size_t count, locale_t locale) {
  /**
   * Number of bytes allowed to examine in `str1` .
   */
  size_t str1Count = count;

  /**
   * Number of bytes allowed to examine in `str2`.
   */
  size_t str2Count = count;

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

    const size_t length1 = p32_mbrtoc32_l (&str1Char, str1, str1Count, &str1state, locale);

    if (length1 == (size_t) -1) {
      return _NLSCMPERROR;
    }

    /**
     * Next Code Point in `str2`.
     */
    char32_t str2Char = 0xFFFFFFFF;

    const size_t length2 = p32_mbrtoc32_l (&str2Char, str2, str2Count, &str2state, locale);

    if (length2 == (size_t) -1) {
      return _NLSCMPERROR;
    }

    /**
     * Reached the end of `str1` or `str2`.
     */
    if (length1 == 0 || length2 == 0) {
      return !!str1Char - !!str2Char;
    }

    /**
     * Valid complete subsequnce in first `count` bytes of both `str1` and
     * `str2` is equal.
     */
    if (length1 == (size_t) -2 && length2 == (size_t) -2) {
      return 0;

      /**
       * Valid complete subsequence in `str2` is longer than in `str1`.
       */
    } else if (length1 == (size_t) -2) {
      return -1;

      /**
       * Valid complete subsequence in `str1` is longer than in `str2`.
       */
    } else if (length2 == (size_t) -2) {
      return 1;
    }

    int diff = str1Char - str2Char;

    if (diff) {
      return diff;
    }

    str1 += length1;
    str2 += length2;

    str1Count -= length1;
    str2Count -= length2;
  }
}

static void P32LocaleFunction_strncmp (LocaleFunctions *functions, Charset *charset) {
  /**
   * All ISO-8859-1 Code Points map 1:1 with Unicode.
   */
  if (charset->CodePage == P32_CODEPAGE_ISO_8859_1) {
    functions->F_strncmp = p32_strncmp_posix;
  } else {
    functions->F_strncmp = p32_strncmp_common;
  }
}

int p32_private_strncmp_l (const char *str1, const char *str2, size_t count, locale_t locale) {
  return locale->Functions.F_strncmp (str1, str2, count, locale);
}
