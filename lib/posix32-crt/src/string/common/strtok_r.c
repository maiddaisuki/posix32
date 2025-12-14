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

/**
 * Functions strtok_s and _mbstok_s[_l] are available starting with msvcr80.dll.
 * For older CRTs we always use our implementation of strtok_r.
 */

#if P32_CRT >= P32_MSVCR80
static char *p32_strtok_r_sbcs (char *str, const char *delim, char **context, locale_t locale) {
  return strtok_s (str, delim, context);
  UNREFERENCED_PARAMETER (locale);
}
#endif

char *p32_private_strtok_r_l (char *str, const char *delim, char **context, locale_t locale) {
  /**
   * Start of the next token.
   */
  char *start = NULL;

  if (str != NULL) {
    start = str;
  } else {
    start = *context;
  }

  /**
   * Return value.
   */
  char *ret = NULL;

  /**
   * Next character to examine.
   */
  char *next = start;

  /**
   * Conversion state for `next`.
   */
  mbstate_t strState = {0};

  while (1) {
    const size_t length = p32_private_mbrlen_l (next, locale->Charset.MaxLength, &strState, locale);
    assert (length != (size_t) -2);

    /**
     * `next` points to invalid character.
     */
    if (length == (size_t) -1 || length == (size_t) -2) {
      goto stop;
    }

    /**
     * `next` points to terminating NUL.
     */
    if (length == 0) {
      /**
       * We increment `start` if a character from `delim` was found at the
       * beginning of `start`.
       *
       * If `next == start`, then token pointed by `start` consists entirely
       * of characters in `delim`.
       */
      if (next != start) {
        ret = start;
      }

      break;
    }

    bool found = false;

    /**
     * Next character in `delim` to examine.
     */
    const char *ptr = delim;

    /**
     * Conversion state for `delim`
     */
    mbstate_t delimState = {0};

    while (1) {
      const size_t delimLength = p32_private_mbrlen_l (ptr, locale->Charset.MaxLength, &delimState, locale);
      assert (delimLength != (size_t) -2);

      /**
       * `ptr` points to invalid character.
       */
      if (delimLength == (size_t) -1 || delimLength == (size_t) -2) {
        goto stop;
      }

      /**
       * `ptr` points to terminating NUL.
       */
      if (delimLength == 0) {
        break;
      }

      if (length == delimLength && !memcmp (next, ptr, delimLength)) {
        found = true;
        break;
      }

      ptr += delimLength;
    }

    /**
     * `next` points to a character in `delim`.
     */
    if (found) {
      /**
       * Overwrite found separator.
       */
      memset (next, 0, length);

      /**
       * If separator is found at the beginning of `start`, skip over it
       * and increment `start` by `length`.
       */
      if (next == start) {
        start += length;
      } else {
        next += length;
        ret   = start;
        break;
      }
    }

    next += length;
  }

  *context = next;

stop:
  return ret;
}

static void P32LocaleFunction_strtok_r (LocaleFunctions *functions, Charset *charset, Locale *locale) {
#if P32_CRT >= P32_MSVCR80
  if (P32_IS_POSIX (locale) || P32_IS_SBCS (charset)) {
    functions->F_strtok_r = p32_strtok_r_sbcs;
  } else {
    functions->F_strtok_r = p32_private_strtok_r_l;
  }
#else
  functions->F_strtok_r = p32_private_strtok_r_l;
#endif

  return;
  UNREFERENCED_PARAMETER (charset);
  UNREFERENCED_PARAMETER (locale);
}

char *p32_strtok_r (char *str, const char *delim, char **context) {
  locale_t activeLocale = p32_active_locale ();

#if defined(LIBPOSIX32_TEST)
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return activeLocale->Functions.F_strtok_r (str, delim, context, activeLocale);
}
