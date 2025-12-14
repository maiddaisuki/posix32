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

#include "ctype-internal.h"
#include "locale-internal.h"
#include "wchar-internal.h"

/**
 * File Summary:
 *
 * This file defines functions declared in wchar.h.
 */

#if P32_LOCALE_NAMES
static int P32CompareString (Locale *locale, DWORD flags, LPCWSTR wcs1, INT len1, LPCWSTR wcs2, INT len2) {
  int ret = CompareStringEx (locale->LocaleName, flags, wcs1, len1, wcs2, len2, NULL, NULL, 0);

#ifdef LIBPOSIX32_TEST
  _ASSERT_EXPR (ret != 0, L"Call to CompareStringEx has failed.\n");
#endif

  if (ret == 0) {
    return _NLSCMPERROR;
  }

  return ret - 2;
}

static int P32LCMapSortKey (Locale *locale, DWORD flags, LPCWSTR src, INT srcSize, LPWSTR dest, INT destSize) {
  return LCMapStringEx (locale->LocaleName, flags | LCMAP_SORTKEY, src, srcSize, dest, destSize, NULL, NULL, 0);
}
#else
static int P32CompareString (Locale *locale, DWORD flags, LPCWSTR wcs1, INT len1, LPCWSTR wcs2, INT len2) {
  int ret = CompareStringW (locale->LocaleId, flags, wcs1, len1, wcs2, len2);

#ifdef LIBPOSIX32_TEST
  _ASSERT_EXPR (ret != 0, L"Call to CompareStringW has failed.\n");
#endif

  if (ret == 0) {
    return _NLSCMPERROR;
  }

  return ret - 2;
}

static int P32LCMapSortKey (Locale *locale, DWORD flags, LPCWSTR src, INT srcSize, LPWSTR dest, INT destSize) {
  return LCMapStringW (locale->LocaleId, flags | LCMAP_SORTKEY, src, srcSize, dest, destSize);
}
#endif

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
 *
 * While msvcr80.dll and later support thread locales, we still provide our own
 * implementation for `wcscoll` and `wcsxfrm` functions.
 *
 * If string passed to `setlocale` or `newlocale` was in Windows format and
 * specified sorting order (e.g. ja-JP_radstr) and that sorting order
 * was applied during locale resolution, this bit of information will be lost
 * when we call CRT's `[_w]setlocale` and `_[w]create_locale` functions.
 *
 * The reason for this is that the string we pass to `[_w]setlocale` and
 * `_[w]create_locale` has "Language_Country" format.
 *
 * Except for "C" locale, our implementation always calls Windows NLS functions
 * and this bit of information will be used.
 */
#include "common/wcscoll.c"
#include "common/wcsxfrm.c"

/**
 * Internal Functions
 */

void p32_wchar_functions (locale_t locale) {
  P32LocaleFunction_wcscasecmp (&locale->Functions, &locale->WinLocale.LcCtype);
  P32LocaleFunction_wcsncasecmp (&locale->Functions, &locale->WinLocale.LcCtype);
  P32LocaleFunction_wcscoll (&locale->Functions, &locale->WinLocale.LcCollate);
  P32LocaleFunction_wcsxfrm (&locale->Functions, &locale->WinLocale.LcCollate);
}
