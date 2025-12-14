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

#include "p32_stdlib.h"

/**
 * Private `mbstate_t` object for `mbtowc`.
 */
static mbstate_t P32MbState_mbtowc = {0};

static int p32_mbtowc_generic (wchar_t *wc, const char *mbc, size_t count, locale_t locale) {
  /**
   * When `mbc` is NULL, `mbtowc` must:
   *
   * 1. Reset its internal state to the initial state.
   * 2. Return non-zero if encoding is stateful, and zero otherwise.
   */
  if (mbc == NULL) {
    memset (&P32MbState_mbtowc, 0, sizeof (mbstate_t));
    return 0;
  }

  size_t length = p32_private_mbrtowc_l (wc, mbc, count, &P32MbState_mbtowc, locale);

  /**
   * `mbc` point to invalid or incomplete multibyte character.
   */
  if (length == (size_t) -1 || length == (size_t) -2) {
    return -1;
  }

  assert (length <= locale->Charset.MaxLength);
  return (int) length;
}

static void P32LocaleFunction_mbtowc (LocaleFunctions *functions, Charset *charset) {
  functions->F_mbtowc = p32_mbtowc_generic;
  return;
  UNREFERENCED_PARAMETER (charset);
}

int p32_mbtowc_l (wchar_t *wc, const char *mbc, size_t count, locale_t locale) {
  return locale->Functions.F_mbtowc (wc, mbc, count, locale);
}

int p32_mbtowc (wchar_t *wc, const char *mbc, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return mbtowc (wc, mbc, count);
  }
#endif

  return p32_mbtowc_l (wc, mbc, count, activeLocale);
}
