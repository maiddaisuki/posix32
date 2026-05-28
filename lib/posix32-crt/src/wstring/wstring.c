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
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"
#include "wctype-internal.h"
#include "wstring-internal.h"

/**
 * File Summary:
 *
 * This file defines functions declared in wchar.h.
 */

/**
 * Locale independant functions.
 */
#include "common/wcsdup.c"
#include "common/wcsndup.c"

/**
 * Functions dependant on LC_CTYPE locale category.
 */
#include "common/wcscasecmp.c"
#include "common/wcsncasecmp.c"

/**
 * Functions dependant on LC_COLLATE locale category.
 */
#include "common/wcscoll.c"
#include "common/wcsxfrm.c"

/*******************************************************************************
 * External Functions.
 */

void p32_wstring_functions (locale_t locale) {
  P32LocaleFunction_wcscasecmp (&locale->Functions, &locale->WinLocale.LcCtype);
  P32LocaleFunction_wcsncasecmp (&locale->Functions, &locale->WinLocale.LcCtype);
  P32LocaleFunction_wcscoll (&locale->Functions, &locale->WinLocale.LcCollate);
  P32LocaleFunction_wcsxfrm (&locale->Functions, &locale->WinLocale.LcCollate);
}
