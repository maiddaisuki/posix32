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

static void P32LocaleFunction_c16rtomb (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (locale->Type == LOCALE_TYPE_POSIX) {
    functions->F_c16rtomb = p32_private_c16rtomb_posix;
  } else if (charset->CodePage == P32_CODEPAGE_ASCII) {
    functions->F_c16rtomb = p32_private_c16rtomb_ascii;
  } else if (charset->MaxLength == 1) {
    functions->F_c16rtomb = p32_private_c16rtomb_sbcs;
  } else if (charset->MaxLength == 2) {
    functions->F_c16rtomb = p32_private_c16rtomb_dbcs;
  } else {
    assert (charset->CodePage == CP_UTF8);
    functions->F_c16rtomb = p32_private_c16rtomb_utf8;
  }
}

/**
 * Private `mbstate_t` if called did not supply one.
 */
static mbstate_t P32MbState_c16rtomb = {0};

size_t p32_c16rtomb_l (char *u8str, char16_t u16char, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32MbState_c16rtomb;
  }

  return locale->Functions.F_c16rtomb (u8str, u16char, state, &locale->Charset);
}

size_t p32_c16rtomb (char *u8str, char16_t u16char, mbstate_t *state) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_posix_locale ();
  }
#endif

  return p32_c16rtomb_l (u8str, u16char, state, activeLocale);
}
