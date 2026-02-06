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

static int p32_strcmp_sbcs (const char *str1, const char *str2, locale_t locale) {
  return strcmp (str1, str2);
  UNREFERENCED_PARAMETER (locale);
}

int p32_private_strcmp_l (const char *str1, const char *str2, locale_t locale) {
  /**
   * Conversion state for `str1`.
   */
  mbstate_t str1state = {0};

  /**
   * Conversion state for `str2`.
   */
  mbstate_t str2state = {0};

  while (1) {
    const size_t length1 = p32_private_mbrlen_l (str1, locale->Charset.MaxLength, &str1state, locale);
    assert (length1 != (size_t) -2);

    if (length1 == (size_t) -1 || length1 == (size_t) -2) {
      return _NLSCMPERROR;
    }

    const size_t length2 = p32_private_mbrlen_l (str2, locale->Charset.MaxLength, &str2state, locale);
    assert (length2 != (size_t) -2);

    if (length2 == (size_t) -1 || length2 == (size_t) -2) {
      return _NLSCMPERROR;
    }

    /**
     * Reached the end of `str1` or `str2`.
     */
    if (length1 == 0 || length2 == 0) {
      return *str1 - *str2;
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
  }
}

static void P32LocaleFunction_strcmp (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  functions->F_strcmp = p32_strcmp_sbcs;
  return;
  UNREFERENCED_PARAMETER (charset);
  UNREFERENCED_PARAMETER (locale);
}

int p32_strcmp (const char *str1, const char *str2) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return activeLocale->Functions.F_strcmp (str1, str2, activeLocale);
}
