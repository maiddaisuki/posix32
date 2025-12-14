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

/**
 * In "C" and "POSIX" locales, wcscoll is the same as wcscmp.
 */
static int p32_wcscoll_posix (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  return wcscmp (wcs1, wcs2);
  UNREFERENCED_PARAMETER (locale);
}

/**
 * Generic implementation using `CompareString[Ex]` functions.
 */
static int p32_wcscoll_generic (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  /**
   * Locale-specific flags for `StringCompare[Ex]`.
   */
  DWORD flags = locale->LocaleInfo.LcCollate.StringCompareFlags;

  return P32CompareString (&locale->WinLocale.LcCollate, flags, wcs1, -1, wcs2, -1);
}

static void P32LocaleFunction_wcscoll (LocaleFunctions *functions, Locale *locale) {
  if (locale->Type == LOCALE_TYPE_POSIX) {
    functions->F_wcscoll = p32_wcscoll_posix;
  } else {
    functions->F_wcscoll = p32_wcscoll_generic;
  }
}

int p32_wcscoll_l (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  return locale->Functions.F_wcscoll (wcs1, wcs2, locale);
}

int p32_wcscoll (const wchar_t *wcs1, const wchar_t *wcs2) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return wcscoll (wcs1, wcs2);
  }
#endif

  return p32_wcscoll_l (wcs1, wcs2, activeLocale);
}
