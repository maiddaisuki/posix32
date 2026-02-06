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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#include <assert.h>
#include <errno.h>
#include <limits.h> /* MB_LEN_MAX */
#include <stdbool.h>
#include <stdio.h> /* EOF */
#include <string.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"
#include "uchar-internal.h"
#include "wchar-internal.h"

/**
 * File Summary:
 *
 * This file defines C95 conversion functions declared in wchar.h:
 *
 *  - btowc
 *  - wctob
 *  - mbsinit
 *  - mbrlen
 *  - mbrtowc
 *  - mbsrtowcs
 *  - wcrtomb
 *  - wcsrtombs
 *
 * This file also defines following functions added in SUSv4:
 *
 *  - mbsnrtowcs
 *  - wcsnrtombs
 */

#include "c95/btowc.c"
#include "c95/mbrlen.c"
#include "c95/mbrtowc.c"
#include "c95/mbsnrtowcs.c"
#include "c95/mbsrtowcs.c"
#include "c95/wcrtomb.c"
#include "c95/wcsnrtombs.c"
#include "c95/wcsrtombs.c"
#include "c95/wctob.c"

/**
 * Provide external definition.
 */
P32_INLINE_DEFN int __cdecl p32_mbsinit (const mbstate_t *state);

/**
 * Internal Functions
 */

void p32_wchar_c95_functions (locale_t locale) {
  P32LocaleFunction_btowc (&locale->Functions, &locale->WinLocale.LcCtype, &locale->Charset);
  P32LocaleFunction_wctob (&locale->Functions, &locale->WinLocale.LcCtype, &locale->Charset);
  P32LocaleFunction_mbrlen (&locale->Functions, &locale->Charset);
  P32LocaleFunction_mbrtowc (&locale->Functions, &locale->Charset);
  P32LocaleFunction_mbsrtowcs (&locale->Functions, &locale->Charset);
  P32LocaleFunction_mbsnrtowcs (&locale->Functions, &locale->Charset);
  P32LocaleFunction_wcrtomb (&locale->Functions, &locale->Charset);
  P32LocaleFunction_wcsrtombs (&locale->Functions, &locale->Charset);
  P32LocaleFunction_wcsnrtombs (&locale->Functions, &locale->Charset);
}
