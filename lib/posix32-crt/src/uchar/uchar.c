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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-internal.h"

/**
 * File Summary:
 *
 * This file defines functions declared in uchar.h:
 *
 *  - mbrtoc8
 *  - c8rtomb
 *  - mbrtoc16
 *  - c16rtomb
 *  - mbrtoc32
 *  - c32rtomb
 */

#include "common/c16rtomb.c"
#include "common/c32rtomb.c"
#include "common/c8rtomb.c"
#include "common/mbrtoc16.c"
#include "common/mbrtoc32.c"
#include "common/mbrtoc8.c"

void p32_uchar_functions (locale_t locale) {
  P32LocaleFunction_c8rtomb (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_mbrtoc8 (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_c16rtomb (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_mbrtoc16 (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_c32rtomb (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_mbrtoc32 (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
}
