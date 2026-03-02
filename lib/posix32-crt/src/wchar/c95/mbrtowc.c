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

#include "wchar-internal.h"

/**
 * Implementation of `mbrtowc`.
 */
#include "common/mbrtowc.c"

static void P32LocaleFunction_mbrtowc (LocaleFunctions *functions, Charset *charset) {
  functions->F_mbrtowc = p32_private_mbrtowc_l;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Private `mbstate_t` to use if caller did not supply one.
 */
static mbstate_t P32ConversionState_mbrtowc = {0};

size_t p32_mbrtowc_l (wchar_t *wc, const char *mbc, size_t count, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32ConversionState_mbrtowc;
  }

  return locale->Functions.F_mbrtowc (wc, mbc, count, state, locale);
}

size_t p32_mbrtowc (wchar_t *wc, const char *mbc, size_t count, mbstate_t *state) {
  locale_t locale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (locale == NULL) {
    locale = p32_posix_locale ();
  }
#endif

  return p32_mbrtowc_l (wc, mbc, count, state, locale);
}
