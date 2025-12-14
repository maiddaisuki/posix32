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
 * In "C" and "POSIX" locales, just copy `src` to `dest`.
 */
static size_t p32_strxfrm_posix (char *dest, const char *src, size_t size, locale_t locale) {
  size_t bufferSize = strlen (src);

  if (size > bufferSize) {
    memcpy (dest, src, bufferSize + 1);
  }

  return bufferSize;
  UNREFERENCED_PARAMETER (locale);
}

#if !P32_LOCALE_NAMES
static size_t p32_strxfrm_ansi (char *dest, const char *src, size_t size, locale_t locale) {
  INT destSize = (INT) __min (size, INT_MAX);

  /**
   * Locale-specific flags for `LCMapString[Ex]`.
   */
  DWORD flags = locale->LocaleInfo.LcCollate.StringTransformationFlags;

  /**
   * NOTE: LCMapString(LCMAP_SORTKEY) returns number of bytes, not number
   * of characters.
   */
  INT bufferSize = P32LCMapSortKeyA (&locale->WinLocale.LcCollate, flags, src, -1, NULL, 0);

  if (bufferSize == 0) {
    goto einval;
  }

  if (bufferSize > destSize) {
    return bufferSize - 1;
  }

  INT written = P32LCMapSortKeyA (&locale->WinLocale.LcCollate, flags, src, -1, dest, destSize);
  assert (written == bufferSize);

  return written - 1;

einval:
  _set_errno (EINVAL);
  return 0;
}
#endif

size_t p32_private_strxfrm_l (char *dest, const char *src, size_t size, locale_t locale) {
  INT destSize = (INT) __min (size, INT_MAX);

  /**
   * `src` converted to wide character string.
   */
  wchar_t *wcs = NULL;

  if (P32MbsToWcs (&wcs, src, locale) == -1) {
    return 0;
  }

  /**
   * Locale-specific flags for `LCMapString[Ex]`.
   */
  DWORD flags = locale->LocaleInfo.LcCollate.StringTransformationFlags;

  /**
   * NOTE: LCMapString(LCMAP_SORTKEY) returns number of bytes, not number
   * of characters.
   */
  INT bufferSize = P32LCMapSortKeyW (&locale->WinLocale.LcCollate, flags, wcs, -1, NULL, 0);

  if (bufferSize == 0) {
    free (wcs);
    goto einval;
  }

  if (bufferSize > destSize) {
    free (wcs);
    return bufferSize - 1;
  }

  INT written = P32LCMapSortKeyW (&locale->WinLocale.LcCollate, flags, wcs, -1, (wchar_t *) dest, destSize);
  assert (written == bufferSize);

  free (wcs);

  return written - 1;

einval:
  _set_errno (EINVAL);
  return 0;
}

static void P32LocaleFunction_strxfrm (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  if (P32_IS_POSIX (locale)) {
    functions->F_strxfrm = p32_strxfrm_posix;
#if !P32_LOCALE_NAMES
  } else if (P32_IS_ANSI (locale, charset)) {
    functions->F_strxfrm = p32_strxfrm_ansi;
#endif
  } else {
    functions->F_strxfrm = p32_private_strxfrm_l;
  }

  return;
  UNREFERENCED_PARAMETER (charset);
}

size_t p32_strxfrm_l (char *dest, const char *src, size_t size, locale_t locale) {
  return locale->Functions.F_strxfrm (dest, src, size, locale);
}

size_t p32_strxfrm (char *dest, const char *src, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return strxfrm (dest, src, count);
  }
#endif

  return p32_strxfrm_l (dest, src, count, activeLocale);
}
