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
 * In "C" and "POSIX" locales, perform comparison as if each character
 * has been converted to lowercase.
 */
static int p32_strncasecmp_posix (const char *str1, const char *str2, size_t count, locale_t locale) {
  /**
   * Two zero-length strings are equal.
   */
  if (count == 0) {
    return 0;
  }

  for (size_t i = 0; i < count; ++i) {
    int c1 = p32_tolower_l ((uint8_t) str1[i], locale);
    int c2 = p32_tolower_l ((uint8_t) str2[i], locale);

    if (c1 == '\0' || c1 != c2) {
      return c1 - c2;
    }
  }

  return 0;
}

#if !P32_LOCALE_NAMES
static int p32_strncasecmp_ansi (const char *str1, const char *str2, size_t count, locale_t locale) {
  /**
   * Two zero-length strings are equal.
   */
  if (count == 0) {
    return 0;
  }

  size_t str1Length = p32_private_strnlen_l (str1, __min (count, INT_MAX), locale);

  if (str1Length == (size_t) -1) {
    return _NLSCMPERROR;
  }

  assert (str1Length <= INT_MAX);

  size_t str2Length = p32_private_strnlen_l (str2, __min (count, INT_MAX), locale);

  if (str2Length == (size_t) -1) {
    return _NLSCMPERROR;
  }

  assert (str2Length <= INT_MAX);

  /**
   * Locale-specific flags for `StringCompare[Ex]`.
   */
  DWORD flags = LINGUISTIC_IGNORECASE | NORM_LINGUISTIC_CASING | SORT_STRINGSORT;

  return P32CompareStringA (&locale->WinLocale.LcCtype, flags, str1, (INT) str1Length, str2, (INT) str2Length);
}
#endif

int p32_private_strncasecmp_l (const char *str1, const char *str2, size_t count, locale_t locale) {
  /**
   * Two zero-length string are equal.
   */
  if (count == 0) {
    return 0;
  }

  /**
   * Return value.
   */
  int diff = _NLSCMPERROR;

  wchar_t *wcs1 = NULL;
  wchar_t *wcs2 = NULL;

  int wcs1Length = 0;
  int wcs2Length = 0;

  wcs1Length = P32MbsToWcsS (&wcs1, str1, count, locale);

  if (wcs1Length == -1) {
    goto fail;
  }

  wcs2Length = P32MbsToWcsS (&wcs2, str2, count, locale);

  if (wcs2Length == -1) {
    goto fail;
  }

  /**
   * Locale-specific flags for `StringCompare[Ex]`.
   */
  DWORD flags = LINGUISTIC_IGNORECASE | NORM_LINGUISTIC_CASING | SORT_STRINGSORT;

  diff = P32CompareStringW (&locale->WinLocale.LcCtype, flags, wcs1, wcs1Length, wcs2, wcs2Length);

fail:
  free (wcs1);
  free (wcs2);

  return diff;
}

static void P32LocaleFunction_strncasecmp (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (P32_IS_POSIX (locale)) {
    functions->F_strncasecmp = p32_strncasecmp_posix;
#if !P32_LOCALE_NAMES
  } else if (P32_IS_ANSI (locale, charset)) {
    functions->F_strncasecmp = p32_strncasecmp_ansi;
#endif
  } else {
    functions->F_strncasecmp = p32_private_strncasecmp_l;
  }

  return;
  UNREFERENCED_PARAMETER (charset);
}

int p32_strncasecmp_l (const char *str1, const char *str2, size_t count, locale_t locale) {
  return locale->Functions.F_strncasecmp (str1, str2, count, locale);
}

int p32_strncasecmp (const char *str1, const char *str2, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return _strnicmp (str1, str2, count);
  }
#endif

  return p32_strncasecmp_l (str1, str2, count, activeLocale);
}
