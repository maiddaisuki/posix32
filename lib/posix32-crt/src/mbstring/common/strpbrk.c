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

static char *p32_strpbrk_posix (const char *str, const char *set, locale_t locale) {
  return strpbrk (str, set);
  UNREFERENCED_PARAMETER (locale);
}

static char *p32_strpbrk_common (const char *str, const char *set, locale_t locale) {
  /**
   * Empty string is not a valid set.
   */
  if (set[0] == '\0') {
    return NULL;
  }

  /**
   * Conversion state for `str`.
   */
  mbstate_t strState = {0};

  while (1) {
    const size_t length = p32_private_mbrlen_l (str, locale->Charset.MaxLength, &strState, locale);
    assert (length != (size_t) -2);

    /**
     * `str` points to terminating '\0' or invalid character.
     */
    if (length == 0 || length == (size_t) -1 || length == (size_t) -2) {
      return NULL;
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
       * `ptr` points to invalid character.
       */
      if (setLength == (size_t) -1 || setLength == (size_t) -2) {
        return NULL;
      }

      /**
       * `ptr` points to terminating NUL.
       */
      if (setLength == 0) {
        break;
      }

      if (length == setLength && !memcmp (str, ptr, setLength)) {
        return (char *) str;
      }

      ptr += setLength;
    }

    str += length;
  }
}

static void P32LocaleFunction_strpbrk (LocaleFunctions *functions, Charset *charset) {
  if (P32_IS_SBCS (charset)) {
    functions->F_strpbrk = p32_strpbrk_posix;
  } else {
    functions->F_strpbrk = p32_strpbrk_common;
  }
}

char *p32_private_strpbrk_l (const char *str, const char *set, locale_t locale) {
  return locale->Functions.F_strpbrk (str, set, locale);
}
