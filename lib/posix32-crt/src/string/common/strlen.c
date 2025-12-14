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

static size_t p32_strlen_sbcs (const char *str, locale_t locale) {
  return strlen (str);
  UNREFERENCED_PARAMETER (locale);
}

size_t p32_private_strlen_l (const char *str, locale_t locale) {
  size_t stringLength = 0;

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
      return (size_t) -1;
    }

    /**
     * `str` points to terminating '\0'.
     */
    if (length == 0) {
      break;
    }

    stringLength += length;
    str          += length;
  }

  return stringLength;
}

static void P32LocaleFunction_strlen (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  functions->F_strlen = p32_strlen_sbcs;
  return;
  UNREFERENCED_PARAMETER (charset);
  UNREFERENCED_PARAMETER (locale);
}

size_t p32_strlen (const char *str) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return activeLocale->Functions.F_strlen (str, activeLocale);
}
