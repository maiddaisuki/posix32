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

static void P32LocaleFunction_mbsnrtowcs (LocaleFunctions *functions, Charset *charset) {
  if (charset->CodePage == P32_CODEPAGE_ASCII) {
    functions->F_mbsnrtowcs = p32_private_mbsnrtowcs_cp20127;
  } else if (charset->CodePage == P32_CODEPAGE_ISO_8859_1) {
    functions->F_mbsnrtowcs = p32_private_mbsnrtowcs_cp28591;
  } else if (charset->CodePage == CP_UTF8) {
    functions->F_mbsnrtowcs = p32_private_mbsnrtowcs_cp65001;
  } else if (charset->MaxLength == 1) {
    functions->F_mbsnrtowcs = p32_private_mbsnrtowcs_sbcs;
  } else if (charset->MaxLength == 2) {
    functions->F_mbsnrtowcs = p32_private_mbsnrtowcs_dbcs;
  }
  assert (functions->F_mbsnrtowcs != NULL);
}

size_t p32_private_mbsnrtowcs_l (
  wchar_t     *wcs,
  const char **mbs,
  size_t       count,
  size_t       size,
  mbstate_t   *state,
  locale_t     locale
) {
  return locale->Functions.F_mbsnrtowcs (wcs, mbs, count, size, state, &locale->Charset);
}

/**
 * Private `mbstate_t` to use if caller did not supply one.
 */
static mbstate_t P32ConversionState_mbsnrtowcs = {0};

size_t p32_mbsnrtowcs_l (wchar_t *wcs, const char **mbs, size_t count, size_t size, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32ConversionState_mbsnrtowcs;
  }

  return p32_private_mbsnrtowcs_l (wcs, mbs, count, size, state, locale);
}

size_t p32_mbsnrtowcs (wchar_t *wcs, const char **mbs, size_t count, size_t size, mbstate_t *state) {
  locale_t locale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (locale == NULL) {
    locale = p32_posix_locale ();
  }
#endif

  return p32_mbsnrtowcs_l (wcs, mbs, count, size, state, locale);
}
