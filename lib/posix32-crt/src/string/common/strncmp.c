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

static int p32_strncmp_sbcs (const char *str1, const char *str2, size_t count, locale_t locale) {
  return strncmp (str1, str2, count);
  UNREFERENCED_PARAMETER (locale);
}

int p32_private_strncmp_l (const char *str1, const char *str2, size_t count, locale_t locale) {
  /**
   * Conversion state for `str1`.
   */
  mbstate_t str1state = {0};

  /**
   * Conversion state for `str2`.
   */
  mbstate_t str2state = {0};

  while (1) {
    const size_t length1 = p32_private_mbrlen_l (str1, count, &str1state, locale);

    if (length1 == (size_t) -1) {
      return _NLSCMPERROR;
    }

    const size_t length2 = p32_private_mbrlen_l (str2, count, &str2state, locale);

    if (length2 == (size_t) -1) {
      return _NLSCMPERROR;
    }

    /**
     * Reached the end of `str1` or `str2`.
     */
    if (length1 == 0 || length2 == 0) {
      return *str1 - *str2;
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

    /**
     * Two characters of different length cannot be equal.
     */
    int diff = memcmp (str1, str2, __min (length1, length2));

    if (diff) {
      return diff;
    }

    assert (length1 == length2);

    str1 += length1;
    str2 += length2;

    count -= length1;
  }
}

static void P32LocaleFunction_strncmp (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  functions->F_strncmp = p32_strncmp_sbcs;
  return;
  UNREFERENCED_PARAMETER (charset);
  UNREFERENCED_PARAMETER (locale);
}

int p32_strncmp (const char *str1, const char *str2, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return activeLocale->Functions.F_strncmp (str1, str2, count, activeLocale);
}
