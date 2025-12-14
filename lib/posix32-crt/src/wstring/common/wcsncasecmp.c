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
 * In "C" and "POSIX" locales, perform comparison as if each wide character
 * has been converted to lowercase.
 */
static int p32_wcsncasecmp_posix (const wchar_t *wcs1, const wchar_t *wcs2, size_t count, locale_t locale) {
  /**
   * Two zero-length strings are equal.
   */
  if (count == 0) {
    return 0;
  }

  for (size_t i = 0; i < count; ++i) {
    wchar_t wc1 = p32_towlower_l (wcs1[i], locale);
    wchar_t wc2 = p32_towlower_l (wcs2[i], locale);

    if (wc1 == L'\0' || wc1 != wc2) {
      return wc1 - wc2;
    }
  }

  return 0;
}

/**
 * Generic implementation using `CompareString[Ex]` functions.
 */
static int p32_wcsncasecmp_generic (const wchar_t *wcs1, const wchar_t *wcs2, size_t count, locale_t locale) {
  /**
   * Two zero-length strings are equal.
   */
  if (count == 0) {
    return 0;
  }

  int wcs1Length = (int) wcsnlen (wcs1, __min (count, INT_MAX));
  int wcs2Length = (int) wcsnlen (wcs2, __min (count, INT_MAX));

  if (wcs1Length > 0 && IS_HIGH_SURROGATE (wcs1[wcs1Length - 1])) {
    wcs1Length -= 1;
  }

  if (wcs2Length > 0 && IS_HIGH_SURROGATE (wcs2[wcs2Length - 1])) {
    wcs2Length -= 1;
  }

  /**
   * Locale-specific flags for `StringCompare[Ex]`.
   */
  DWORD flags = locale->LocaleInfo.LcCtype.CaseCmpFlags;

  return P32CompareString (&locale->WinLocale.LcCtype, flags, wcs1, wcs1Length, wcs2, wcs2Length);
}

static void P32LocaleFunction_wcsncasecmp (LocaleFunctions *functions, Locale *locale) {
  if (locale->Type == LOCALE_TYPE_POSIX) {
    functions->F_wcsncasecmp = p32_wcsncasecmp_posix;
  } else {
    functions->F_wcsncasecmp = p32_wcsncasecmp_generic;
  }
}

int p32_wcsncasecmp_l (const wchar_t *wcs1, const wchar_t *wcs2, size_t count, locale_t locale) {
  return locale->Functions.F_wcsncasecmp (wcs1, wcs2, count, locale);
}

int p32_wcsncasecmp (const wchar_t *wcs1, const wchar_t *wcs2, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return _wcsnicmp (wcs1, wcs2, count);
  }
#endif

  return p32_wcsncasecmp_l (wcs1, wcs2, count, activeLocale);
}
