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

#include "p32_stdlib.h"

/**
 * Private `mbstate_t` object for `wctomb`.
 */
static mbstate_t P32MbState_wctomb = {0};

static int p32_wctomb_generic (char *mbc, wchar_t wc, locale_t locale) {
  /**
   * When `mbc` is NULL, `wctomb` must:
   *
   * 1. Reset its internal state to the initial state.
   * 2. Return non-zero if encoding is stateful, and zero otherwise.
   */
  if (mbc == NULL) {
    memset (&P32MbState_wctomb, 0, sizeof (mbstate_t));
    return 0;
  }

  size_t length = p32_private_wcrtomb_l (mbc, wc, &P32MbState_wctomb, locale);

  /**
   * Conversion failed.
   */
  if (length == (size_t) -1) {
    return -1;
  }

  assert (length <= locale->Charset.MaxLength);
  return (int) length;
}

static void P32LocaleFunction_wctomb (LocaleFunctions *functions, Charset *charset) {
  functions->F_wctomb = p32_wctomb_generic;
  return;
  UNREFERENCED_PARAMETER (charset);
}

int p32_wctomb_l (char *mbc, wchar_t wc, locale_t locale) {
  return locale->Functions.F_wctomb (mbc, wc, locale);
}

int p32_wctomb (char *mbc, wchar_t wc) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return wctomb (mbc, wc);
  }
#endif

  return p32_wctomb_l (mbc, wc, activeLocale);
}
