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

#if !1
/**
 * Implementation using `LCMapStringA`.
 */
static size_t p32_strxfrm_ansi (char *dest, const char *src, size_t size, locale_t locale) {
  Locale        *lcCollate     = &locale->WinLocale.LcCollate;
  LcCollateInfo *lcCollateInfo = &locale->LocaleInfo.LcCollate;

  /**
   * Locale-specific flags for `LCMapStringA`.
   */
  uint32_t flags = (LCMAP_SORTKEY | lcCollateInfo->StringTransformationFlags);

  /**
   * NOTE: LCMapString(LCMAP_SORTKEY) returns number of bytes, not number
   * of characters.
   */
  int bufferSize = p32_winlocale_map_ansi_string (lcCollate, flags, src, -1, NULL, 0);

  if (bufferSize == 0) {
    goto einval;
  }

  int destSize = (int) __min (size, INT_MAX);

  if (bufferSize > destSize) {
    return bufferSize - 1;
  }

  int written = p32_winlocale_map_ansi_string (lcCollate, flags, src, -1, dest, destSize);
  assert (written == bufferSize);

  return written - 1;

einval:
  _set_errno (EINVAL);
  return 0;
}
#endif

/**
 * Implementation using `LCMapStringW`/`LCMapStringEx`.
 */
size_t p32_private_strxfrm_l (char *dest, const char *src, size_t size, locale_t locale) {
  Locale        *lcCollate     = &locale->WinLocale.LcCollate;
  LcCollateInfo *lcCollateInfo = &locale->LocaleInfo.LcCollate;

  /**
   * Convert `src` to wide character string.
   */
  wchar_t *wcs = NULL;

  int wcsLength = P32MbsToWcs (&wcs, src, SIZE_MAX, locale);

  if (wcsLength == -1) {
    return 0;
  }

  /**
   * Locale-specific flags for `LCMapStringW`/`LCMapStringEx`.
   */
  uint32_t flags = (LCMAP_SORTKEY | lcCollateInfo->StringTransformationFlags);

  /**
   * NOTE: LCMapString(LCMAP_SORTKEY) returns number of bytes, not number
   * of characters.
   */
  int bufferSize = p32_winlocale_map_unicode_string (lcCollate, flags, wcs, wcsLength, NULL, 0);

  if (bufferSize == 0) {
    free (wcs);
    goto einval;
  }

  int destSize = (int) __min (size, INT_MAX);

  if (bufferSize > destSize) {
    free (wcs);
    return bufferSize - 1;
  }

  int written = p32_winlocale_map_unicode_string (lcCollate, flags, wcs, wcsLength, (wchar_t *) dest, destSize);
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
