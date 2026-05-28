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
 * In "C" and "POSIX" locales, wcscoll is the same as wcscmp.
 */
static int p32_wcscoll_posix (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  return wcscmp (wcs1, wcs2);
  UNREFERENCED_PARAMETER (locale);
}

/**
 * Implementation using `CompareStringW`/`CompareStringEx`.
 */
static int p32_wcscoll_unicode (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  Locale        *lcCollate     = &locale->WinLocale.LcCtype;
  LcCollateInfo *lcCollateInfo = &locale->LocaleInfo.LcCollate;

  /**
   * Return value.
   */
  int diff = _NLSCMPERROR;

  /**
   * Locale-specific flags for `CompareStringW`/`CompareStringEx`.
   */
  uint32_t flags = lcCollateInfo->StringCompareFlags;

  diff = p32_winlocale_compare_unicode_string (lcCollate, flags, wcs1, -1, wcs2, -1);

  if (diff == 0) {
    diff = _NLSCMPERROR;
  } else {
    diff -= 2;
  }

  return diff;
}

static void P32LocaleFunction_wcscoll (LocaleFunctions *functions, Locale *locale) {
  if (locale->Type == LocaleType_POSIX) {
    functions->F_wcscoll = p32_wcscoll_posix;
  } else {
    functions->F_wcscoll = p32_wcscoll_unicode;
  }
}

int p32_private_wcscoll_l (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  return locale->Functions.F_wcscoll (wcs1, wcs2, locale);
}

int p32_wcscoll_l (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  return p32_private_wcscoll_l (wcs1, wcs2, locale);
}

int p32_wcscoll (const wchar_t *wcs1, const wchar_t *wcs2) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return wcscoll (wcs1, wcs2);
  }
#endif

  return p32_private_wcscoll_l (wcs1, wcs2, activeLocale);
}
