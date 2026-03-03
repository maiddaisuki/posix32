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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#include <errno.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "posix32-ext/p32_wchar-ext.h"

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to convert between multibyte and wide character
 * strings. These functions convert the entire string and allocate buffer
 * to hold converted string.
 *
 * These functions are wrappers around internal function `p32_charset_convert`.
 */

int p32_ext_mbstowcs (wchar_t **address, const char *mbs) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_posix_locale ();
  }
#endif

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MB_TO_WC;
  conversionRequest.Charset   = &activeLocale->Charset;
  conversionRequest.Input.A   = mbs;
  conversionRequest.Output.W  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_wcstombs (char **address, const wchar_t *wcs) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_posix_locale ();
  }
#endif

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_WC_TO_MB;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
  conversionRequest.Charset   = &activeLocale->Charset;
  conversionRequest.Input.W   = wcs;
  conversionRequest.Output.A  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_mbstowcs_cp (wchar_t **address, const char *mbs, unsigned codePage) {
  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_CP;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MB_TO_WC;
  conversionRequest.CodePage  = codePage;
  conversionRequest.Input.A   = mbs;
  conversionRequest.Output.W  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_wcstombs_cp (char **address, const wchar_t *wcs, unsigned codePage) {
  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_CP;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_WC_TO_MB;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
  conversionRequest.CodePage  = codePage;
  conversionRequest.Input.W   = wcs;
  conversionRequest.Output.A  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_wcstombs_ansi (char **address, const wchar_t *wcs) {
  locale_t locale = p32_ansi_locale ();

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_WC_TO_MB;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
  conversionRequest.Charset   = &locale->Charset;
  conversionRequest.Input.W   = wcs;
  conversionRequest.Output.A  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_mbstowcs_ansi (wchar_t **address, const char *mbs) {
  locale_t locale = p32_ansi_locale ();

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MB_TO_WC;
  conversionRequest.Charset   = &locale->Charset;
  conversionRequest.Input.A   = mbs;
  conversionRequest.Output.W  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_wcstombs_oem (char **address, const wchar_t *wcs) {
  locale_t locale = p32_oem_locale ();

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_WC_TO_MB;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
  conversionRequest.Charset   = &locale->Charset;
  conversionRequest.Input.W   = wcs;
  conversionRequest.Output.A  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_mbstowcs_oem (wchar_t **address, const char *mbs) {
  locale_t locale = p32_oem_locale ();

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MB_TO_WC;
  conversionRequest.Charset   = &locale->Charset;
  conversionRequest.Input.A   = mbs;
  conversionRequest.Output.W  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_wcstombs_fs (char **address, const wchar_t *wcs) {
  locale_t locale = p32_fileapi_locale ();

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_WC_TO_MB;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
  conversionRequest.Charset   = &locale->Charset;
  conversionRequest.Input.W   = wcs;
  conversionRequest.Output.A  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

int p32_ext_mbstowcs_fs (wchar_t **address, const char *mbs) {
  locale_t locale = p32_fileapi_locale ();

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MB_TO_WC;
  conversionRequest.Charset   = &locale->Charset;
  conversionRequest.Input.A   = mbs;
  conversionRequest.Output.W  = address;

  return p32_charset_convert (&conversionRequest, 0);
}

/**
 * Internal Functions
 *
 * These internal functions are the same as public version with `_cp` suffix,
 * with the following exceptions:
 *
 * - buffer to hold converted string is allocated using `HeapAlloc` instead of
 *   `malloc`
 *
 * - `WC_NO_BEST_FIT_CHARS` may be stripped from conversion flags passed to
 *   `WideCharToMultiByte`
 *
 * These functions are mostly used to convert locale information.
 *
 * There are some locales for which default code page cannot be used to
 * losslessly convert all locale information returned by `GetLocaleInfo[Ex]`.
 *
 * Stripping `WC_NO_BEST_FIT_CHARS` allows us to use such locales with their
 * default code page. Otherwise, they will be unusable.
 */

int p32_private_wcstombs (char **address, const wchar_t *wcs, uintptr_t heap, uint32_t codePage, bool bestFit) {
  CharsetConversionRequest req = {0};

  req.Flags    |= P32_CHARSET_CONVERSION_CP;
  req.Flags    |= P32_CHARSET_CONVERSION_WC_TO_MB;
  req.CodePage  = codePage;
  req.Input.W   = wcs;
  req.Output.A  = address;

  if (!bestFit) {
    req.Flags |= (P32_CHARSET_CONVERSION_NO_BEST_FIT);
  }

  return p32_charset_convert (&req, heap);
}

int p32_private_mbstowcs (wchar_t **address, const char *mbs, uintptr_t heap, uint32_t codePage) {
  CharsetConversionRequest req = {0};

  req.Flags    |= P32_CHARSET_CONVERSION_CP;
  req.Flags    |= P32_CHARSET_CONVERSION_MB_TO_WC;
  req.CodePage  = codePage;
  req.Input.A   = mbs;
  req.Output.W  = address;

  return p32_charset_convert (&req, heap);
}
