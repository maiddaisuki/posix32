/**
 * Copyright 2026 Kirill Makurin
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

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "wchar-internal.h"

/**
 * File Summary:
 *
 * This file defines internal function `p32_charset_convert`.
 *
 * This function is used to perform conversion between multibyte and wide
 * character strings.
 *
 * Whenever possible, it uses our own `p32_private_mbsrtowcs_*` and
 * `p32_private_wcsrtombs_*` functions to perform conversion.
 *
 * This allows us to always provide support for ASCII (code page 20127),
 * ISO-8859-1 (code page 28591) and UTF-8 (code page 65001) even if they are
 * not supported by the operating system.
 *
 * For example, code page 65001 (`CP_UTF8`) is only supported since Windows XP.
 *
 * If we do not implement conversion functions for some code page,
 * it will use `MultiByteToWideChar` and `WideCharToMultiByte` functions.
 */

/*******************************************************************************
 * Conversion from Multibyte to Wide character strings.
 */

/**
 * Pointer type corresponding to `p32_private_mbsrtowcs_*` functions.
 */
typedef size_t (*MbsToWcsFunc) (wchar_t *, const char **, size_t, mbstate_t *, Charset *);

/**
 * Convenience wrapper around `MultiByteToWideChar`.
 */
static INT P32MultiByteToWideChar (LPWSTR buffer, INT bufferSize, LPCSTR mbs, Charset *charset) {
  assert ((buffer == NULL) == (bufferSize == 0));
  return MultiByteToWideChar (charset->CodePage, charset->ToWideChar, mbs, -1, buffer, bufferSize);
}

/**
 * Perform conversion using `MultiByteToWideChar`.
 *
 * On success, returns length, excluding terminating '\0', of the string
 * stored in `request->Output.W`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
static int P32MbsToWcsFallback (CharsetConversionRequest *request, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  int      bufferSize = 0;
  wchar_t *buffer     = NULL;

  bufferSize = P32MultiByteToWideChar (buffer, bufferSize, request->Input.A, request->Charset);

  if (bufferSize == 0) {
    request->Status = CharsetConversionRequestNoConversion;
    goto fail;
  }

  if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
    buffer = malloc (bufferSize * sizeof (wchar_t));
  } else {
    buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));
  }

  if (buffer == NULL) {
    request->Status = CharsetConversionRequestOutOfMemory;
    goto fail;
  }

  int written = P32MultiByteToWideChar (buffer, bufferSize, request->Input.A, request->Charset);
  assert (written == bufferSize);

  if (written == 0 || written > bufferSize) {
    request->Status = CharsetConversionRequestFailure;
    goto fail_free;
  }

  *request->Output.W = buffer;
  request->Status    = CharsetConversionRequestSuccess;

  return written - 1;

fail_free:
  if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
    free (buffer);
  } else {
    HeapFree (heapHandle, 0, buffer);
  }

fail:
  return -1;
}

/**
 * Perform conversion using `func`.
 *
 * On success, returns length, excluding terminating '\0', of the string
 * stored in `request->Output.W`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
static int P32MbsToWcs (CharsetConversionRequest *request, uintptr_t heap, MbsToWcsFunc func) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Since `func` may set `errno` on failure, save current `errno` value
   * and restore it on failure.
   */
  errno_t saveErrno = 0;
  _get_errno (&saveErrno);

  /**
   * String to convert.
   */
  const char *str = request->Input.A;

  /**
   * Conversion state for `str`.
   */
  mbstate_t state = {0};

  wchar_t *buffer     = NULL;
  size_t   bufferSize = 0;

  bufferSize = func (buffer, &str, bufferSize, &state, request->Charset);
  assert (p32_mbsinit (&state));

  if (bufferSize == (size_t) -1) {
    request->Status = CharsetConversionRequestNoConversion;
    goto fail;
  }

  if (bufferSize >= INT_MAX) {
    request->Status = CharsetConversionRequestTooBig;
    goto fail;
  }

  bufferSize += 1;

  if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
    buffer = malloc (bufferSize * sizeof (wchar_t));
  } else {
    buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));
  }

  if (buffer == NULL) {
    request->Status = CharsetConversionRequestOutOfMemory;
    goto fail;
  }

  size_t written = func (buffer, &str, bufferSize, &state, request->Charset);
  assert (written == bufferSize - 1 && str == NULL);

  if (written == (size_t) -1 || str != NULL) {
    request->Status = CharsetConversionRequestFailure;
    goto fail_free;
  }

  *request->Output.W = buffer;
  request->Status    = CharsetConversionRequestSuccess;

  return (int) written;

fail_free:
  if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
    free (buffer);
  } else {
    HeapFree (heapHandle, 0, buffer);
  }

fail:
  _set_errno (saveErrno);
  return -1;
}

/**
 * Convert multibyte character string `request->Input.A` to wide character
 * string. Conversion is performed using information in `charset`.
 *
 * On success, returns length, excluding terminating '\0', of the string
 * stored in `request->Output.W`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
static int P32MbsToWcsMain (CharsetConversionRequest *request, uintptr_t heap, Charset *charset) {
  /**
   * If we implement conversion function for `charset->CodePage`, this is the
   * pointer to that function.
   */
  MbsToWcsFunc func = NULL;

  if (charset->CodePage == P32_CODEPAGE_ASCII) {
    func = p32_private_mbsrtowcs_cp20127;
  } else if (charset->CodePage == P32_CODEPAGE_POSIX) {
    func = p32_private_mbsrtowcs_posix;
  } else if (charset->MaxLength == 1) {
    func = p32_private_mbsrtowcs_sbcs;
  } else if (charset->MaxLength == 2) {
    func = p32_private_mbsrtowcs_dbcs;
  } else if (charset->CodePage == CP_UTF8) {
    func = p32_private_mbsrtowcs_utf8;
  }

  wchar_t *buffer = NULL;
  int      length = 0;

  /**
   * Use local structure for intermediate results.
   */
  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    = request->Flags;
  conversionRequest.Charset  = charset;
  conversionRequest.Input    = request->Input;
  conversionRequest.Output.W = &buffer;

  /**
   * If we implement conversion function for `charset->CodePage`, then use
   * `func` to perform conversion.
   *
   * Otherwise, use `MultiByteToWideChar`.
   */
  if (func != NULL) {
    length = P32MbsToWcs (&conversionRequest, heap, func);
  } else {
    length = P32MbsToWcsFallback (&conversionRequest, heap);
  }

  if (length == -1) {
    request->Status = conversionRequest.Status;
    return -1;
  }

  assert (conversionRequest.Status == CharsetConversionRequestSuccess);

  *request->Output.W = buffer;
  request->Status    = CharsetConversionRequestSuccess;

  return length;
}

/*******************************************************************************
 * Conversion from Wide to Multibyte character strings.
 */

/**
 * Pointer type corresponding to `p32_private_wcsrtombs_*` functions.
 */
typedef size_t (*WcsToMbsFunc) (char *, const wchar_t **, size_t, mbstate_t *, Charset *);

/**
 * Convenience wrapper around `WideCharToMultiByte`.
 */
static INT P32WideCharToMultiByte (LPSTR buffer, INT bufferSize, LPCWSTR wcs, Charset *charset) {
  assert ((buffer == NULL) == (bufferSize == 0));

  /**
   * If `charset->CodePage` is a code page for which we have appropriate
   * `p32_private_wcsrtombs_*` function, then we are using `WideCharToMultiByte`
   * to attempt best-fit conversion.
   *
   * Otherwise, `charset->CodePage` is a code page for which we do not
   * implement our own conversion functions. This includes all code pages
   * for which `charset->MaxLength` is greater than 2, with exception of
   * code page 65001 (UTF-8), which we fully support. All of them do not
   * allow using `WC_NO_BEST_FIT_CHARS` with `WideCharToMultiByte`.
   *
   * As such, stripping `WC_NO_BEST_FIT_CHARS` is appropriate in both cases.
   */
  DWORD flags = (charset->ToMultiByte & ~WC_NO_BEST_FIT_CHARS);

  /**
   * If any character in `wcs` cannot be represented by `charset->CodePage`,
   * `WideCharToMultiByte` will set this variable to `TRUE`.
   */
  BOOL defaultCharUsed = FALSE;

  /**
   * The last two arguments to `WideCharToMultiByte` must be `NULL` with
   * `CP_UTF7` and `CP_UTF8`.
   */
  LPBOOL defaultCharUsedPtr = &defaultCharUsed;

  if (charset->CodePage == CP_UTF7 || charset->CodePage == CP_UTF8) {
    assert (charset->CodePage != CP_UTF8);
    defaultCharUsedPtr = NULL;
  }

  INT written = WideCharToMultiByte (charset->CodePage, flags, wcs, -1, buffer, bufferSize, NULL, defaultCharUsedPtr);

  if (written == 0 || defaultCharUsed) {
    return 0;
  }

  return written;
}

/**
 * Perform conversion using `WideCharToMultiByte`.
 *
 * On success, returns length, excluding terminating '\0', of the string
 * stored in `request->Output.A`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
static int P32WcsToMbsFallback (CharsetConversionRequest *request, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  char *buffer     = NULL;
  int   bufferSize = 0;

  bufferSize = P32WideCharToMultiByte (buffer, bufferSize, request->Input.W, request->Charset);

  if (bufferSize == 0) {
    request->Status = CharsetConversionRequestNoConversion;
    goto fail;
  }

  if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
    buffer = malloc (bufferSize * sizeof (char));
  } else {
    buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (char));
  }

  if (buffer == NULL) {
    request->Status = CharsetConversionRequestOutOfMemory;
    goto fail;
  }

  int written = P32WideCharToMultiByte (buffer, bufferSize, request->Input.W, request->Charset);
  assert (written == bufferSize);

  if (written == 0 || written > bufferSize) {
    request->Status = CharsetConversionRequestFailure;
    goto fail_free;
  }

  *request->Output.A = buffer;
  request->Status    = CharsetConversionRequestSuccess;

  return written - 1;

fail_free:
  if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
    free (buffer);
  } else {
    HeapFree (heapHandle, 0, buffer);
  }

fail:
  return -1;
}

/**
 * Perform conversion using `func`.
 *
 * On success, returns length, excluding terminating '\0', of the string
 * stored in `request->Output.A`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
static int P32WcsToMbs (CharsetConversionRequest *request, uintptr_t heap, WcsToMbsFunc func) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Since `func` may set `errno` on failure, save current `errno` value
   * and restore it on failure.
   */
  errno_t saveErrno = 0;
  _get_errno (&saveErrno);

  /**
   * String to convert.
   */
  const wchar_t *str = request->Input.W;

  /**
   * Conversion state for `str`.
   */
  mbstate_t state = {0};

  char  *buffer     = NULL;
  size_t bufferSize = 0;

  bufferSize = func (buffer, &str, bufferSize, &state, request->Charset);
  assert (p32_mbsinit (&state));

  if (bufferSize == (size_t) -1) {
    request->Status = CharsetConversionRequestNoConversion;
    goto fail;
  }

  if (bufferSize >= INT_MAX) {
    request->Status = CharsetConversionRequestTooBig;
    goto fail;
  }

  bufferSize += 1;

  if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
    buffer = malloc (bufferSize * sizeof (char));
  } else {
    buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (char));
  }

  if (buffer == NULL) {
    request->Status = CharsetConversionRequestOutOfMemory;
    goto fail;
  }

  size_t written = func (buffer, &str, bufferSize, &state, request->Charset);
  assert (written == bufferSize - 1 && str == NULL);

  if (written == (size_t) -1 || str != NULL) {
    request->Status = CharsetConversionRequestFailure;
    goto fail_free;
  }

  *request->Output.A = buffer;
  request->Status    = CharsetConversionRequestSuccess;

  return (int) written;

fail_free:
  if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
    free (buffer);
  } else {
    HeapFree (heapHandle, 0, buffer);
  }

fail:
  _set_errno (saveErrno);
  return -1;
}

/**
 * Convert wide character string `request->Input.W` to multibyte character
 * string. Conversion is performed using information in `charset`.
 *
 * On success, returns length, excluding terminating '\0', of the string
 * stored in `request->Output.A`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
static int P32WcsToMbsMain (CharsetConversionRequest *request, uintptr_t heap, Charset *charset) {
  /**
   * If we implement conversion function for `charset->CodePage`, this is the
   * pointer to that function.
   */
  WcsToMbsFunc func = NULL;

  if (charset->CodePage == P32_CODEPAGE_ASCII) {
    func = p32_private_wcsrtombs_cp20127;
  } else if (charset->CodePage == P32_CODEPAGE_POSIX) {
    func = p32_private_wcsrtombs_posix;
  } else if (charset->MaxLength == 1) {
    func = p32_private_wcsrtombs_sbcs;
  } else if (charset->MaxLength == 2) {
    func = p32_private_wcsrtombs_dbcs;
  } else if (charset->CodePage == CP_UTF8) {
    func = p32_private_wcsrtombs_utf8;
  }

  char *buffer = NULL;
  int   length = 0;

  /**
   * Use local structure for intermediate results.
   */
  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    = request->Flags;
  conversionRequest.Charset  = charset;
  conversionRequest.Input    = request->Input;
  conversionRequest.Output.A = &buffer;

  /**
   * If we implement conversion function for `charset->CodePage`, then try
   * to use `func` to perform conversion first.
   *
   * Since `func` does not allow best-fit conversion, we may try to use
   * `WideCharToMultiByte` to do best-fit conversion if we are allowed to.
   */
  if (func != NULL) {
    length = P32WcsToMbs (&conversionRequest, heap, func);

    if (length == -1) {
      /**
       * If `P32WcsToMbs` failed for any reason other than conversion error,
       * treat it as a hard error and fail immediately.
       */
      if (conversionRequest.Status != CharsetConversionRequestNoConversion) {
        request->Status = conversionRequest.Status;
        goto fail;
      }

      /**
       * Check whether we are allowed to attempt best-fit conversion.
       */
      if ((charset->Flags & P32_CHARSET_CONV_NO_BEST_FIT) || (request->Flags & P32_CHARSET_CONVERSION_NO_BEST_FIT)) {
        request->Status = CharsetConversionRequestNoConversion;
        goto fail;
      }
    }
  }

  /**
   * Try to perform conversion using `WideCharToMultiByte`.
   */
  if (buffer == NULL) {
    assert (func == NULL || conversionRequest.Status == CharsetConversionRequestNoConversion);

    length = P32WcsToMbsFallback (&conversionRequest, heap);

    if (length == -1) {
      request->Status = conversionRequest.Status;
      goto fail;
    }
  }

  assert (conversionRequest.Status == CharsetConversionRequestSuccess);

  *request->Output.A = buffer;
  request->Status    = CharsetConversionRequestSuccess;

  return length;

fail:
  return -1;
}

/*******************************************************************************
 * External functions.
 */

int p32_charset_convert (CharsetConversionRequest *request, uintptr_t heap) {
  /**
   * If `requset->Flags` contains `P32_CHARSET_CONVERSION_CP` flag, then
   * we need to call `p32_charset_info` to obtain information about
   * `request->CodePage`.
   *
   * Otherwise, we will use local copy of `request->Charset`.
   */
  Charset charset = {0};

  if (request->Flags & P32_CHARSET_CONVERSION_CP) {
    charset.CodePage = request->CodePage;

    if (!p32_charset_info (&charset)) {
      request->Status = CharsetConversionRequestFailure;
      return -1;
    }
  } else {
    assert (request->Charset != NULL);
    memcpy (&charset, request->Charset, sizeof (Charset));
  }

  assert ((request->Flags & (P32_CHARSET_CONVERSION_MB_TO_WC | P32_CHARSET_CONVERSION_WC_TO_MB)) != 0);

  if (request->Flags & P32_CHARSET_CONVERSION_MB_TO_WC) {
    assert ((request->Flags & P32_CHARSET_CONVERSION_WC_TO_MB) == 0);
    return P32MbsToWcsMain (request, heap, &charset);
  }

  if (request->Flags & P32_CHARSET_CONVERSION_WC_TO_MB) {
    assert ((request->Flags & P32_CHARSET_CONVERSION_MB_TO_WC) == 0);
    return P32WcsToMbsMain (request, heap, &charset);
  }

  return -1;
}
