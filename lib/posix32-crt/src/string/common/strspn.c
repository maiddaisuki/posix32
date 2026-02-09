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

static size_t p32_strspn_sbcs (const char *str, const char *set, locale_t locale) {
  return strspn (str, set);
  UNREFERENCED_PARAMETER (locale);
}

size_t p32_private_strspn_l (const char *str, const char *set, locale_t locale) {
  /**
   * Return value.
   */
  size_t ret = 0;

  /**
   * Conversion state for `str`.
   */
  mbstate_t strState = {0};

  while (1) {
    const size_t length = p32_private_mbrlen_l (str, locale->Charset.MaxLength, &strState, locale);
    assert (length != (size_t) -2);

    /**
     * `str` points to terminating NUL or invalid character.
     */
    if (length == 0 || length == (size_t) -1 || length == (size_t) -2) {
      return ret;
    }

    const char *ptr = set;

    /**
     * Conversion state for `set`.
     */
    mbstate_t setState = {0};

    while (1) {
      const size_t setLength = p32_private_mbrlen_l (ptr, locale->Charset.MaxLength, &setState, locale);
      assert (setLength != (size_t) -2);

      /**
       * `ptr` points to terminating NUL or invalid character.
       */
      if (setLength == 0 || setLength == (size_t) -1 || setLength == (size_t) -2) {
        return ret;
      }

      if (length == setLength && !memcmp (str, ptr, setLength)) {
        break;
      }

      ptr += setLength;
    }

    ret += length;
    str += length;
  }
}

static void P32LocaleFunction_strspn (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (P32_IS_POSIX (locale) || P32_IS_SBCS (charset)) {
    functions->F_strspn = p32_strspn_sbcs;
  } else {
    functions->F_strspn = p32_private_strspn_l;
  }
}

size_t p32_strspn (const char *str, const char *set) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_posix_locale ();
  }
#endif

  return activeLocale->Functions.F_strspn (str, set, activeLocale);
}
