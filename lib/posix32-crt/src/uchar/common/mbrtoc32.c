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

#include "uchar-internal.h"

static void P32LocaleFunction_mbrtoc32 (LocaleFunctions *functions, Charset *charset) {
  if (charset->CodePage == P32_CODEPAGE_ASCII) {
    functions->F_mbrtoc32 = p32_private_mbrtoc32_cp20127;
  } else if (charset->CodePage == P32_CODEPAGE_ISO_8859_1) {
    functions->F_mbrtoc32 = p32_private_mbrtoc32_cp28591;
  } else if (charset->CodePage == CP_UTF8) {
    functions->F_mbrtoc32 = p32_private_mbrtoc32_cp65001;
  } else if (charset->MaxLength == 1) {
    functions->F_mbrtoc32 = p32_private_mbrtoc32_sbcs;
  } else if (charset->MaxLength == 2) {
    functions->F_mbrtoc32 = p32_private_mbrtoc32_dbcs;
  }
  assert (functions->F_mbrtoc32 != NULL);
}

/**
 * Private `mbstate_t` if called did not supply one.
 */
static mbstate_t P32MbState_mbrtoc32 = {0};

size_t p32_mbrtoc32_l (char32_t *u32char, const char *u8str, size_t count, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32MbState_mbrtoc32;
  }

  return locale->Functions.F_mbrtoc32 (u32char, u8str, count, state, &locale->Charset);
}

size_t p32_mbrtoc32 (char32_t *u32char, const char *u8str, size_t count, mbstate_t *state) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_posix_locale ();
  }
#endif

  return p32_mbrtoc32_l (u32char, u8str, count, state, activeLocale);
}
