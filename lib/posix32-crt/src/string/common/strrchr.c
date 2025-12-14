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

static char *p32_strrchr_sbcs (const char *str, int c, locale_t locale) {
  return strrchr (str, c);
  UNREFERENCED_PARAMETER (locale);
}

char *p32_private_strrchr_l (const char *str, int c, locale_t locale) {
  /**
   * Check if `c` is a valid single-byte character.
   */
  if (p32_btowc_l ((unsigned char) c, locale) == WEOF) {
    return NULL;
  }

  char *ret = NULL;

  /**
   * Conversion state for `str`.
   */
  mbstate_t state = {0};

  while (1) {
    const size_t length = p32_private_mbrlen_l (str, locale->Charset.MaxLength, &state, locale);
    assert (length != (size_t) -2);

    /**
     * `str` points to invalid character.
     */
    if (length == (size_t) -1 || length == (size_t) -2) {
      break;
    }

    /**
     * Reached the end of `str`.
     */
    if (length == 0) {
      /**
       * Check if we look for terminating NUL.
       */
      if ((unsigned char) c == '\0') {
        ret = (char *) str;
      }

      break;
    }

    /**
     * If `str` points to a single-byte character check if that's the one we
     * look for.
     */
    if (length == 1 && *str == (char) c) {
      ret = (char *) str;
    }

    str += length;
  }

  return ret;
}

static void P32LocaleFunction_strrchr (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (P32_IS_POSIX (locale) || P32_IS_SBCS (charset)) {
    functions->F_strrchr = p32_strrchr_sbcs;
  } else {
    functions->F_strrchr = p32_private_strrchr_l;
  }
}

char *p32_strrchr (const char *str, int c) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return activeLocale->Functions.F_strrchr (str, c, activeLocale);
}
