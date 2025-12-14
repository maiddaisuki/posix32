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
 * In "C" and "POSIX" locales, just copy `src` to `dest`.
 */
static size_t p32_wcsxfrm_posix (wchar_t *dest, const wchar_t *src, size_t size, locale_t locale) {
  size_t bufferSize = wcslen (src);

  if (size > bufferSize) {
    wmemcpy (dest, src, bufferSize + 1);
  }

  return bufferSize;
  UNREFERENCED_PARAMETER (locale);
}

/**
 * Generic implementation using `P32LCMapString[Ex]` functions.
 */
static size_t p32_wcsxfrm_generic (wchar_t *dest, const wchar_t *src, size_t size, locale_t locale) {
  INT destSize = (INT) __min (size, INT_MAX);

  /**
   * Locale specific flags for `LcMapString[Ex]`.
   */
  DWORD flags = locale->LocaleInfo.LcCollate.StringTransformationFlags;

  /**
   * NOTE: `LCMapString(LCMAP_SORTKEY)` returns number of bytes, not number of
   * characters.
   */
  INT bufferSize = P32LCMapSortKey (&locale->WinLocale.LcCollate, flags, src, -1, NULL, 0);

  if (bufferSize == 0) {
    goto einval;
  }

  if (bufferSize > destSize) {
    return bufferSize - 1;
  }

  INT written = P32LCMapSortKey (&locale->WinLocale.LcCollate, flags, src, -1, dest, destSize);
  assert (written == bufferSize);

  for (INT i = written - 1; i >= 0; --i) {
    ((uint8_t *) dest)[(i * 2) + 1] = 0;
    ((uint8_t *) dest)[i * 2]       = ((uint8_t *) dest)[i];
  }

  return written - 1;

einval:
  _set_errno (EINVAL);
  return 0;
}

static void P32LocaleFunction_wcsxfrm (LocaleFunctions *functions, Locale *locale) {
  if (locale->Type == LOCALE_TYPE_POSIX) {
    functions->F_wcsxfrm = p32_wcsxfrm_posix;
  } else {
    functions->F_wcsxfrm = p32_wcsxfrm_generic;
  }
}

size_t p32_wcsxfrm_l (wchar_t *dest, const wchar_t *src, size_t size, locale_t locale) {
  return locale->Functions.F_wcsxfrm (dest, src, size, locale);
}

size_t p32_wcsxfrm (wchar_t *dest, const wchar_t *src, size_t size) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return wcsxfrm (dest, src, size);
  }
#endif

  return p32_wcsxfrm_l (dest, src, size, activeLocale);
}
