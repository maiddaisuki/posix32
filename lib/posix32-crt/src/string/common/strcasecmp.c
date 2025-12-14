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

#include "string-internal.h"

/**
 * In "C" and "POSIX" locales, perform comparison as if each character
 * has been converted to lowercase.
 */
static int p32_strcasecmp_posix (const char *str1, const char *str2, locale_t locale) {
  while (1) {
    int c1 = p32_tolower_l ((uint8_t) *str1, locale);
    int c2 = p32_tolower_l ((uint8_t) *str2, locale);

    if (c1 == '\0' || c1 != c2) {
      return c1 - c2;
    }

    str1 += 1;
    str2 += 1;
  }
}

#if !P32_LOCALE_NAMES
static int p32_strcasecmp_ansi (const char *str1, const char *str2, locale_t locale) {
  DWORD flags = locale->LocaleInfo.LcCtype.CaseCmpFlags;
  return P32CompareStringA (&locale->WinLocale.LcCtype, flags, str1, -1, str2, -1);
}
#endif

int p32_private_strcasecmp_l (const char *str1, const char *str2, locale_t locale) {
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
  DWORD flags = locale->LocaleInfo.LcCtype.CaseCmpFlags;

  diff = P32CompareStringW (&locale->WinLocale.LcCtype, flags, wcs1, wcs1Length, wcs2, wcs2Length);

fail:
  free (wcs1);
  free (wcs2);

  return diff;
}

static void P32LocaleFunction_strcasecmp (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (P32_IS_POSIX (locale)) {
    functions->F_strcasecmp = p32_strcasecmp_posix;
#if !P32_LOCALE_NAMES
  } else if (P32_IS_ANSI (locale, charset)) {
    functions->F_strcasecmp = p32_strcasecmp_ansi;
#endif
  } else {
    functions->F_strcasecmp = p32_private_strcasecmp_l;
  }

  return;
  UNREFERENCED_PARAMETER (charset);
}

int p32_strcasecmp_l (const char *str1, const char *str2, locale_t locale) {
  return locale->Functions.F_strcasecmp (str1, str2, locale);
}

int p32_strcasecmp (const char *str1, const char *str2) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return _stricmp (str1, str2);
  }
#endif

  return p32_strcasecmp_l (str1, str2, activeLocale);
}
