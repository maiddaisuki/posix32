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
 * POSIX says that all bytes in range [0,255] are valid.
 */
static wint_t p32_btowc_posix (int c, Charset *charset) {
  assert (charset->CodePage == P32_CODEPAGE_POSIX);

  if (c == EOF) {
    return WEOF;
  }

  return (uint8_t) c;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * When `MultiByteToWideChar` is used with code page 20127 (ASCII),
 * it handles input bytes in range [128,255] as if it has called `toascii`
 * before performing conversion.
 */
static wint_t p32_btowc_ascii (int c, Charset *charset) {
  assert (charset->CodePage == P32_CODEPAGE_ASCII || charset->CodePage == CP_UTF8);

  if (c == EOF) {
    return WEOF;
  }

  return (uint8_t) c <= 0x7F ? (uint8_t) c : WEOF;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Generic implementation around `MultiByteToWideChar`.
 */
static wint_t p32_btowc_generic (int c, Charset *charset) {
  if (c == EOF) {
    return WEOF;
  }

  CHAR  byte = (BYTE) c;
  WCHAR wc   = WEOF;

  INT ret = MultiByteToWideChar (charset->CodePage, charset->ToWideChar, &byte, 1, &wc, 1);

  if (ret != 1) {
    return WEOF;
  }

  return wc;
}

static void P32LocaleFunction_btowc (LocaleFunctions *functions, Locale *locale, Charset *charset) {
  if (locale->Type == LOCALE_TYPE_POSIX) {
    functions->F_btowc = p32_btowc_posix;
  } else if (charset->CodePage == P32_CODEPAGE_ASCII || charset->CodePage == CP_UTF8) {
    functions->F_btowc = p32_btowc_ascii;
  } else {
    functions->F_btowc = p32_btowc_generic;
  }
}

wint_t p32_btowc_l (int c, locale_t locale) {
  return locale->Functions.F_btowc (c, &locale->Charset);
}

wint_t p32_btowc (int c) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
#if P32_CRT >= P32_MSVCR80
    return btowc (c);
#else
    activeLocale = p32_posix_locale ();
#endif
  }
#endif

  return p32_btowc_l (c, activeLocale);
}
