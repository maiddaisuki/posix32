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
#include <stdlib.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"
#include "wchar-internal.h"

/**
 * File Summary:
 *
 * This file defines C89 conversion functions declared in stdlib.h:
 *
 *  - mblen
 *  - mbtowc
 *  - mbstowcs
 *  - wctomb
 *  - wcstombs
 */

#include "c89/mb_cur_max.c"
#include "c89/mblen.c"
#include "c89/mbstowcs.c"
#include "c89/mbtowc.c"
#include "c89/wcstombs.c"
#include "c89/wctomb.c"

void p32_wchar_c89_functions (locale_t locale) {
  P32LocaleFunction_mblen (&locale->Functions, &locale->Charset);
  P32LocaleFunction_mbtowc (&locale->Functions, &locale->Charset);
  P32LocaleFunction_mbstowcs (&locale->Functions, &locale->Charset);
  P32LocaleFunction_wctomb (&locale->Functions, &locale->Charset);
  P32LocaleFunction_wcstombs (&locale->Functions, &locale->Charset);
}
