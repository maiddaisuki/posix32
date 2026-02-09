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
 * These functions use `WideCharToMultiByte()` and `MultiByteToWideChar()`
 * for conversion, which means they are independent of CRT.
 */

/**
 * Set `errno` based on Win32 error code returned by `GetLastError`.
 */
static void P32SetErrno (void) {
  DWORD errorCode = GetLastError ();

  switch (errorCode) {
    case ERROR_NOT_ENOUGH_MEMORY:
      _set_errno (ENOMEM);
      return;

    case ERROR_INVALID_PARAMETER:
    case ERROR_INVALID_FLAGS:
#ifdef LIBPOSIX32_TEST
      _RPTFW1 (_CRT_ERROR, L"Unexpected error %X.\n", errorCode);

      if (IsDebuggerPresent ()) {
        DebugBreak ();
      }
#endif
      _set_errno (EINVAL);
      return;

    case ERROR_NO_UNICODE_TRANSLATION:
    /**
     * Assume that `P32WcsToMbsBufferSize` or `P32MbsToWcsBufferSize`
     * returned -1 to indicate an error.
     */
    default:
      _set_errno (EILSEQ);
      return;
  }
}

/**
 * Returns buffer size required to hold wide character string `wcs` converted
 * to multibyte character string.
 *
 * On success, this function returns buffer size, in bytes, required to hold
 * converted string.
 *
 * Otherwise, this function returns `-1`.
 */
static INT P32WcsToMbsBufferSize (LPCWSTR wcs, Charset *charset) {
  BOOL defaultCharUsed = FALSE;
  INT  bufferSize      = 0;

  /**
   * Last two arguments must be `NULL` with `CP_UTF7` and `CP_UTF8`.
   */
  if (charset->CodePage == CP_UTF7 || charset->CodePage == CP_UTF8) {
    bufferSize = WideCharToMultiByte (charset->CodePage, charset->ToMultiByte, wcs, -1, NULL, 0, NULL, NULL);
  } else {
    bufferSize =
      WideCharToMultiByte (charset->CodePage, charset->ToMultiByte, wcs, -1, NULL, 0, NULL, &defaultCharUsed);
  }

  if (bufferSize == 0 || defaultCharUsed) {
    return -1;
  }

  return bufferSize;
}

/**
 * Returns buffer size required to hold multibyte character string `mbs`
 * converted to wide character string.
 *
 * On success, this function returns buffer size, in wide characters,
 * required to hold converted string.
 *
 * Otherwise, this function returns `-1`.
 */
static INT P32MbsToWcsBufferSize (LPCSTR mbs, Charset *charset) {
  INT bufferSize = 0;

  /**
   * When MultiByteToWideChar is used with code page 20127 (ASCII), it performs
   * conversion as if it has called `toascii` on each byte in input string.
   *
   * This means that input strings which contain bytes outside of ASCII range
   * will be converted incorrectly.
   *
   * Note that passing `MB_ERR_INVALID_CHARS` to `MultiByteToWideChar`
   * does not prevent this from happening.
   *
   * For this reason, we validate the input string and calculate required
   * buffer size ourselves.
   */
  if (charset->CodePage == P32_CODEPAGE_ASCII) {
    for (size_t i = 0;; ++i) {
      if (!__isascii (mbs[i])) {
        return -1;
      }

      bufferSize += 1;

      if (mbs[i] == '\0') {
        break;
      }
    }
  } else {
    bufferSize = MultiByteToWideChar (charset->CodePage, charset->ToWideChar, mbs, -1, NULL, 0);
  }

  if (bufferSize == 0) {
    return -1;
  }

  return bufferSize;
}

/**
 * Convert multibyte characters string `mbs` to wide character string.
 */
static INT P32MbsToWcs (wchar_t **address, const char *mbs, Charset *charset) {
  wchar_t *buffer     = NULL;
  int      bufferSize = 0;

  bufferSize = P32MbsToWcsBufferSize (mbs, charset);

  if (bufferSize == -1) {
    P32SetErrno ();
    return -1;
  }

  if (address == NULL) {
    return bufferSize - 1;
  }

  buffer = malloc (bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    _set_errno (ENOMEM);
    return -1;
  }

  INT written = MultiByteToWideChar (charset->CodePage, charset->ToWideChar, mbs, -1, buffer, bufferSize);

  if (written == 0 || written > bufferSize) {
    P32SetErrno ();
    free (buffer);
    return -1;
  }

  *address = buffer;

  return written - 1;
}

/**
 * Convert wide characters string `wcs` to multibyte character string.
 */
static INT P32WcsToMbs (char **address, const wchar_t *wcs, Charset *charset) {
  char *buffer     = NULL;
  int   bufferSize = 0;

  bufferSize = P32WcsToMbsBufferSize (wcs, charset);

  if (bufferSize == -1) {
    P32SetErrno ();
    return -1;
  }

  if (address == NULL) {
    return bufferSize - 1;
  }

  buffer = malloc (bufferSize);

  if (buffer == NULL) {
    _set_errno (ENOMEM);
    return -1;
  }

  INT written = WideCharToMultiByte (charset->CodePage, charset->ToMultiByte, wcs, -1, buffer, bufferSize, NULL, NULL);

  if (written == 0 || written > bufferSize) {
    P32SetErrno ();
    free (buffer);
    return -1;
  }

  *address = buffer;

  return written - 1;
}

int p32_ext_mbstowcs (wchar_t **address, const char *mbs) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return P32MbsToWcs (address, mbs, &activeLocale->Charset);
}

int p32_ext_wcstombs (char **address, const wchar_t *wcs) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return P32WcsToMbs (address, wcs, &activeLocale->Charset);
}

int p32_ext_mbstowcs_cp (wchar_t **address, const char *mbs, unsigned codePage) {
  Charset charset  = {0};
  charset.CodePage = codePage;

  p32_charset_conversion_flags (&charset, false);

  return P32MbsToWcs (address, mbs, &charset);
}

int p32_ext_wcstombs_cp (char **address, const wchar_t *wcs, unsigned codePage) {
  Charset charset  = {0};
  charset.CodePage = codePage;

  p32_charset_conversion_flags (&charset, false);

  return P32WcsToMbs (address, wcs, &charset);
}

int p32_ext_wcstombs_ansi (char **address, const wchar_t *wcs) {
  UINT codePage = GetACP ();
  return p32_ext_wcstombs_cp (address, wcs, codePage);
}

int p32_ext_mbstowcs_ansi (wchar_t **address, const char *mbs) {
  UINT codePage = GetACP ();
  return p32_ext_mbstowcs_cp (address, mbs, codePage);
}

int p32_ext_wcstombs_oem (char **address, const wchar_t *wcs) {
#ifdef LIBPOSIX32_UWP
  UINT codePage = CP_UTF8;
#else
  UINT codePage = GetOEMCP ();
#endif
  return p32_ext_wcstombs_cp (address, wcs, codePage);
}

int p32_ext_mbstowcs_oem (wchar_t **address, const char *mbs) {
#ifdef LIBPOSIX32_UWP
  UINT codePage = CP_UTF8;
#else
  UINT codePage = GetOEMCP ();
#endif
  return p32_ext_mbstowcs_cp (address, mbs, codePage);
}

int p32_ext_wcstombs_fs (char **address, const wchar_t *wcs) {
#ifdef LIBPOSIX32_UWP
  UINT codePage = GetACP ();
#else
  UINT codePage = (AreFileApisANSI () ? GetACP () : GetOEMCP ());
#endif
  return p32_ext_wcstombs_cp (address, wcs, codePage);
}

int p32_ext_mbstowcs_fs (wchar_t **address, const char *mbs) {
#ifdef LIBPOSIX32_UWP
  UINT codePage = GetACP ();
#else
  UINT codePage = (AreFileApisANSI () ? GetACP () : GetOEMCP ());
#endif
  return p32_ext_mbstowcs_cp (address, mbs, codePage);
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
  HANDLE heapHandle = (HANDLE) heap;

  Charset charset  = {0};
  charset.CodePage = codePage;

  p32_charset_conversion_flags (&charset, bestFit);

  char *buffer     = NULL;
  int   bufferSize = 0;

  bufferSize = P32WcsToMbsBufferSize (wcs, &charset);

  if (bufferSize == -1) {
    goto fail;
  }

  if (address == NULL) {
    return bufferSize - 1;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize);

  if (buffer == NULL) {
    goto fail;
  }

  INT written = WideCharToMultiByte (charset.CodePage, charset.ToMultiByte, wcs, -1, buffer, bufferSize, NULL, NULL);

  if (written == 0 || written > bufferSize) {
    goto fail_free;
  }

  *address = buffer;

  return written - 1;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return -1;
}

int p32_private_mbstowcs (wchar_t **address, const char *mbs, uintptr_t heap, uint32_t codePage) {
  HANDLE heapHandle = (HANDLE) heap;

  Charset charset  = {0};
  charset.CodePage = codePage;

  p32_charset_conversion_flags (&charset, false);

  wchar_t *buffer     = NULL;
  int      bufferSize = 0;

  bufferSize = P32MbsToWcsBufferSize (mbs, &charset);

  if (bufferSize == -1) {
    goto fail;
  }

  if (address == NULL) {
    return bufferSize - 1;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    goto fail;
  }

  INT written = MultiByteToWideChar (charset.CodePage, charset.ToWideChar, mbs, -1, buffer, bufferSize);

  if (written == 0 || written > bufferSize) {
    goto fail_free;
  }

  *address = buffer;

  return written - 1;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return -1;
}
