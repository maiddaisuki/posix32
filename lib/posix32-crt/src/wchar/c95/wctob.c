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

#include "wchar-internal.h"

static int p32_wctob_posix (wint_t wc, Charset *charset) {
  assert (charset->CodePage == P32_CODEPAGE_POSIX);
  return wc <= 0xFF ? wc : EOF;
  UNREFERENCED_PARAMETER (charset);
}

static int p32_wctob_ascii (wint_t wc, Charset *charset) {
  assert (charset->CodePage == P32_CODEPAGE_ASCII || charset->CodePage == CP_UTF8);
  return wc <= 0x7F ? wc : EOF;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Generic implementation around `WideCharToMultiByte`.
 */
static int p32_wctob_generic (wint_t wc, Charset *charset) {
  BOOL defaultCharacterUsed = FALSE;
  CHAR c                    = EOF;

  INT ret = WideCharToMultiByte (charset->CodePage, charset->ToMultiByte, &wc, 1, &c, 1, NULL, &defaultCharacterUsed);

  if (ret != 1 || defaultCharacterUsed) {
    return EOF;
  }

  return (BYTE) c;
}

static void P32LocaleFunction_wctob (LocaleFunctions *functions, Locale *locale, Charset *charset) {
  if (locale->Type == LOCALE_TYPE_POSIX) {
    functions->F_wctob = p32_wctob_posix;
  } else if (charset->CodePage == P32_CODEPAGE_ASCII || charset->CodePage == CP_UTF8) {
    functions->F_wctob = p32_wctob_ascii;
  } else {
    functions->F_wctob = p32_wctob_generic;
  }
}

int p32_wctob_l (wint_t wc, locale_t locale) {
  return locale->Functions.F_wctob (wc, &locale->Charset);
}

int p32_wctob (wint_t wc) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
#if P32_CRT >= P32_MSVCR80
    return wctob (wc);
#else
    activeLocale = p32_default_locale ();
#endif
  }
#endif

  return p32_wctob_l (wc, activeLocale);
}
