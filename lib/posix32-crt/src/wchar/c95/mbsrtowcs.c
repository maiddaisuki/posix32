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

static void P32LocaleFunction_mbsrtowcs (LocaleFunctions *functions, Charset *charset) {
  if (charset->CodePage == P32_CODEPAGE_ASCII) {
    functions->F_mbsrtowcs = p32_private_mbsrtowcs_cp20127;
  } else if (charset->CodePage == P32_CODEPAGE_POSIX) {
    functions->F_mbsrtowcs = p32_private_mbsrtowcs_cp28591;
  } else if (charset->CodePage == CP_UTF8) {
    functions->F_mbsrtowcs = p32_private_mbsrtowcs_cp65001;
  } else if (charset->MaxLength == 1) {
    functions->F_mbsrtowcs = p32_private_mbsrtowcs_sbcs;
  } else if (charset->MaxLength == 2) {
    functions->F_mbsrtowcs = p32_private_mbsrtowcs_dbcs;
  }
  assert (functions->F_mbsrtowcs != NULL);
}

size_t p32_private_mbsrtowcs_l (wchar_t *wcs, const char **mbs, size_t size, mbstate_t *state, locale_t locale) {
  return locale->Functions.F_mbsrtowcs (wcs, mbs, size, state, &locale->Charset);
}

/**
 * Private `mbstate_t` to use if caller did not supply one.
 */
static mbstate_t P32ConversionState_mbsrtowcs = {0};

size_t p32_mbsrtowcs_l (wchar_t *wcs, const char **mbs, size_t count, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32ConversionState_mbsrtowcs;
  }

  return p32_private_mbsrtowcs_l (wcs, mbs, count, state, locale);
}

size_t p32_mbsrtowcs (wchar_t *wcs, const char **mbs, size_t count, mbstate_t *state) {
  locale_t locale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (locale == NULL) {
    locale = p32_posix_locale ();
  }
#endif

  return p32_mbsrtowcs_l (wcs, mbs, count, state, locale);
}
