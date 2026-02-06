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

static void P32LocaleFunction_mbrtoc16 (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (locale->Type == LOCALE_TYPE_POSIX) {
    functions->F_mbrtoc16 = p32_private_mbrtoc16_posix;
  } else if (charset->CodePage == P32_CODEPAGE_ASCII) {
    functions->F_mbrtoc16 = p32_private_mbrtoc16_ascii;
  } else if (charset->MaxLength == 1) {
    functions->F_mbrtoc16 = p32_private_mbrtoc16_sbcs;
  } else if (charset->MaxLength == 2) {
    functions->F_mbrtoc16 = p32_private_mbrtoc16_dbcs;
  } else {
    assert (charset->CodePage == CP_UTF8);
    functions->F_mbrtoc16 = p32_private_mbrtoc16_utf8;
  }
}

/**
 * Private `mbstate_t` if called did not supply one.
 */
static mbstate_t P32MbState_mbrtoc16 = {0};

size_t p32_mbrtoc16_l (char16_t *u16char, const char *u8str, size_t count, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32MbState_mbrtoc16;
  }

  return locale->Functions.F_mbrtoc16 (u16char, u8str, count, state, &locale->Charset);
}

size_t p32_mbrtoc16 (char16_t *u16char, const char *u8str, size_t count, mbstate_t *state) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return p32_mbrtoc16_l (u16char, u8str, count, state, activeLocale);
}
