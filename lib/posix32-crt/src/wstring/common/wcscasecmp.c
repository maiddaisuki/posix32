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
 * In "C" and "POSIX" locales, perform comparison as if each wide character
 * has been converted to lowercase.
 */
static int p32_wcscasecmp_posix (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  while (1) {
    wchar_t wc1 = p32_private_towlower_l (*wcs1, locale);
    wchar_t wc2 = p32_private_towlower_l (*wcs2, locale);

    if (wc1 == L'\0' || wc1 != wc2) {
      return wc1 - wc2;
    }

    wcs1 += 1;
    wcs2 += 1;
  }
}

/**
 * Implementation using `CompareStringW`/`CompareStringEx`.
 */
static int p32_wcscasecmp_unicode (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  Locale      *lcCtype     = &locale->WinLocale.LcCtype;
  LcCtypeInfo *lcCtypeInfo = &locale->LocaleInfo.LcCtype;

  /**
   * Return value.
   */
  int diff = _NLSCMPERROR;

  /**
   * Locale-specific flags for `CompareStringW`/`CompareStringEx`.
   */
  uint32_t flags = lcCtypeInfo->CaseCmpFlags;

  diff = p32_winlocale_compare_unicode_string (lcCtype, flags, wcs1, -1, wcs2, -1);

  if (diff == 0) {
    diff = _NLSCMPERROR;
  } else {
    diff -= 2;
  }

  return diff;
}

static void P32LocaleFunction_wcscasecmp (LocaleFunctions *functions, Locale *locale) {
  if (locale->Type == LocaleType_POSIX) {
    functions->F_wcscasecmp = p32_wcscasecmp_posix;
  } else {
    functions->F_wcscasecmp = p32_wcscasecmp_unicode;
  }
}

int p32_private_wcscasecmp_l (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  return locale->Functions.F_wcscasecmp (wcs1, wcs2, locale);
}

int p32_wcscasecmp_l (const wchar_t *wcs1, const wchar_t *wcs2, locale_t locale) {
  return p32_private_wcscasecmp_l (wcs1, wcs2, locale);
}

int p32_wcscasecmp (const wchar_t *wcs1, const wchar_t *wcs2) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return _wcsicmp (wcs1, wcs2);
  }
#endif

  return p32_private_wcscasecmp_l (wcs1, wcs2, activeLocale);
}
