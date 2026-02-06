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

static size_t p32_mbstowcs_generic (wchar_t *wcs, const char *mbs, size_t count, locale_t locale) {
  /**
   * POSIX says that `mbstowcs` starts conversion in the initial state.
   *
   * Nothing in POSIX specification points that we should keep track of
   * shift state between calls to `mbstowcs`.
   *
   * For this reason we use private `mbstate_t` object allocated on the stack.
   * This effectively makes `mbstowcs` thread safe.
   */
  mbstate_t state = {0};

  return p32_private_mbsrtowcs_l (wcs, &mbs, count, &state, locale);
}

static void P32LocaleFunction_mbstowcs (LocaleFunctions *functions, Charset *charset) {
  functions->F_mbstowcs = p32_mbstowcs_generic;
  return;
  UNREFERENCED_PARAMETER (charset);
}

size_t p32_mbstowcs_l (wchar_t *wcs, const char *mbs, size_t count, locale_t locale) {
  return locale->Functions.F_mbstowcs (wcs, mbs, count, locale);
}

size_t p32_mbstowcs (wchar_t *wcs, const char *mbs, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return mbstowcs (wcs, mbs, count);
  }
#endif

  return p32_mbstowcs_l (wcs, mbs, count, activeLocale);
}
