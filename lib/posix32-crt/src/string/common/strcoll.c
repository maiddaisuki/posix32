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

#include "string-internal.h"

/**
 * In "C" and "POSIX" locales, `strcoll` is the same as `strcmp`.
 */
static int p32_strcoll_posix (const char *str1, const char *str2, locale_t locale) {
  return strcmp (str1, str2);
  UNREFERENCED_PARAMETER (locale);
}

#if !P32_LOCALE_NAMES
static int p32_strcoll_ansi (const char *str1, const char *str2, locale_t locale) {
  DWORD flags = locale->LocaleInfo.LcCollate.StringCompareFlags;
  return P32CompareStringA (&locale->WinLocale.LcCollate, flags, str1, -1, str2, -1);
}
#endif

int p32_private_strcoll_l (const char *str1, const char *str2, locale_t locale) {
  /**
   * Return value.
   */
  int diff = _NLSCMPERROR;

  wchar_t *wcs1 = NULL;
  wchar_t *wcs2 = NULL;

  int wcs1Length = 0;
  int wcs2Length = 0;

  wcs1Length = P32MbsToWcs (&wcs1, str1, locale);

  if (wcs1Length == -1) {
    goto fail;
  }

  wcs2Length = P32MbsToWcs (&wcs2, str2, locale);

  if (wcs2Length == -1) {
    goto fail;
  }

  /**
   * Locale-specific flags for `StringCompare[Ex]`.
   */
  DWORD flags = locale->LocaleInfo.LcCollate.StringCompareFlags;

  diff = P32CompareStringW (&locale->WinLocale.LcCollate, flags, wcs1, wcs1Length, wcs2, wcs2Length);

fail:
  free (wcs1);
  free (wcs2);

  return diff;
}

static void P32LocaleFunction_strcoll (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (P32_IS_POSIX (locale)) {
    functions->F_strcoll = p32_strcoll_posix;
#if !P32_LOCALE_NAMES
  } else if (P32_IS_ANSI (locale, charset)) {
    functions->F_strcoll = p32_strcoll_ansi;
#endif
  } else {
    functions->F_strcoll = p32_private_strcoll_l;
  }

  return;
  UNREFERENCED_PARAMETER (charset);
}

int p32_strcoll_l (const char *str1, const char *str2, locale_t locale) {
  return locale->Functions.F_strcoll (str1, str2, locale);
}

int p32_strcoll (const char *str1, const char *str2) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return strcoll (str1, str2);
  }
#endif

  return p32_strcoll_l (str1, str2, activeLocale);
}
