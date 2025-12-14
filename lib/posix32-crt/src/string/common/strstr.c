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

#include "string-internal.h"

static char *p32_strstr_sbcs (const char *str, const char *substr, locale_t locale) {
  return strstr (str, substr);
  UNREFERENCED_PARAMETER (locale);
}

char *p32_private_strstr_l (const char *str, const char *substr, locale_t locale) {
  /**
   * POSIX requires that `strstr` returns `str` if `substr` is a zero-length
   * string.
   */
  if (substr[0] == '\0') {
    return (char *) str;
  }

  /**
   * Conversion state for `str`.
   */
  mbstate_t strState = {0};

  while (1) {
    const size_t strLength = p32_private_mbrlen_l (str, locale->Charset.MaxLength, &strState, locale);
    assert (strLength != (size_t) -2);

    /**
     * `str` points to terminating NUL or invalid character.
     */
    if (strLength == 0 || strLength == (size_t) -1 || strLength == (size_t) -2) {
      return NULL;
    }

    /**
     * Conversion state for `substr`.
     */
    mbstate_t subState = {0};

    const size_t subLength = p32_private_mbrlen_l (substr, locale->Charset.MaxLength, &subState, locale);
    assert (subLength != (size_t) -2);

    /**
     * `substr` points to invalid character.
     */
    if (subLength == (size_t) -1 || subLength == (size_t) -2) {
      return NULL;
    }

    /**
     * `str` and `substr` point to the same character.
     * Try to compare whole subsitrng.
     */
    if (strLength == subLength && !memcmp (str, substr, subLength)) {
      mbstate_t saveState = strState;

      const char *strNext = str + strLength;
      const char *subNext = substr + subLength;

      while (1) {
        const size_t subNextLength = p32_private_mbrlen_l (subNext, locale->Charset.MaxLength, &subState, locale);
        assert (subNextLength != (size_t) -2);

        /**
         * `subNext` point to invalid character.
         */
        if (subNextLength == (size_t) -1 || subNextLength == (size_t) -2) {
          return NULL;
        }

        /**
         * `subNext` point to terminating NUL.
         */
        if (subNextLength == 0) {
          return (char *) str;
        }

        const size_t strNextLength = p32_private_mbrlen_l (strNext, locale->Charset.MaxLength, &strState, locale);
        assert (strNextLength != (size_t) -2);

        /**
         * `strNext` point to terminating NUL or invalid character.
         */
        if (strNextLength == 0 || strNextLength == (size_t) -1 || strNextLength == (size_t) -2) {
          return NULL;
        }

        if (strNextLength != subNextLength || memcmp (strNext, subNext, subNextLength)) {
          break;
        }

        strNext += strNextLength;
        subNext += subNextLength;
      }

      strState = saveState;
    }

    str += strLength;
  }
}

static void P32LocaleFunction_strstr (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (P32_IS_POSIX (locale) || P32_IS_SBCS (charset)) {
    functions->F_strstr = p32_strstr_sbcs;
  } else {
    functions->F_strstr = p32_private_strstr_l;
  }
}

char *p32_strstr (const char *str, const char *substr) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return activeLocale->Functions.F_strstr (str, substr, activeLocale);
}
