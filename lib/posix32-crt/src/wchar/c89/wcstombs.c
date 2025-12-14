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

static size_t p32_wcstombs_generic (char *mbs, const wchar_t *wcs, size_t count, locale_t locale) {
  /**
   * POSIX says that `wcstombs` starts conversion in the initial state.
   *
   * Nothing in POSIX specification points that we should keep track of
   * shift state between calls to `wcstombs`.
   *
   * For this reason we use private `mbstate_t` object allocated on the stack.
   * This effectively makes `wcstombs` thread safe.
   */
  mbstate_t state = {0};

  return p32_private_wcsrtombs_l (mbs, &wcs, count, &state, locale);
}

static void P32LocaleFunction_wcstombs (LocaleFunctions *functions, Charset *charset) {
  functions->F_wcstombs = p32_wcstombs_generic;
  return;
  UNREFERENCED_PARAMETER (charset);
}

size_t p32_wcstombs_l (char *mbs, const wchar_t *wcs, size_t count, locale_t locale) {
  return locale->Functions.F_wcstombs (mbs, wcs, count, locale);
}

size_t p32_wcstombs (char *mbs, const wchar_t *wcs, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return wcstombs (mbs, wcs, count);
  }
#endif

  return p32_wcstombs_l (mbs, wcs, count, activeLocale);
}
