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

#include "core-heap.h"
#include "core-norm.h"

#include "locale-internal.h"
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

/**
 * Structure to keep track of conversion progress.
 */
typedef struct ConversionState {
  /**
   * Set to `true` to signal success.
   */
  bool Success;
  /**
   * Set to `true` to signal failure.
   */
  bool Failure;
  /**
   * Set to true if `p32_normalize_unicode_string` failed with status code
   * `NormalizationRequestNormalized`.
   *
   * It prevents us from attempting to convert the same, already normalized
   * string multiple times if first conversion attempt failed.
   */
  bool Normalized;
  /**
   * Set to true if `p32_normalize_unicode_string` failed with status code
   * `NormalizationRequestNotSupported`.
   *
   * We will attempt to convert string as-is, without any normalization.
   * If conversion succeeds, converted string is returned.
   *
   * Ohterwise, this flag prevents us from attempting to convert the same
   * string multiple times if first conversion attempt failed.
   */
  bool NotSupported;
  /**
   * Length of converted/normalized string.
   *
   * On success, this is the value returned by `p32_charset_convert`.
   */
  int Length;
  /**
   * Buffer to store intermediate results.
   */
  wchar_t *Buffer;
  /**
   * Input.
   */
  union {
    const char    *A;
    const wchar_t *W;
  } Input;
  /**
   * Output.
   */
  union {
    char    **A;
    wchar_t **W;
  } Output;
  /**
   * For use with internal conversion functions.
   */
  CharsetConversionRequest ConvRequest;
  /**
   * For use with `p32_normalize_unicode_string`.
   */
  NormalizationRequest NormRequest;
} ConversionState;

#define CONV_FLAGS(state, convFlag, cmp, charsetFlag)                                             \
  (((state).ConvRequest.Flags & convFlag) cmp ((state).ConvRequest.Charset->Flags & charsetFlag))

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
    buffer = p32_heap_alloc (heap, 0, bufferSize * sizeof (wchar_t));
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
    p32_heap_free (heap, 0, buffer);
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
    buffer = p32_heap_alloc (heap, 0, bufferSize * sizeof (wchar_t));
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
    p32_heap_free (heap, 0, buffer);
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
  } else if (charset->CodePage == P32_CODEPAGE_ISO_8859_1) {
    func = p32_private_mbsrtowcs_cp28591;
  } else if (charset->CodePage == CP_UTF8) {
    func = p32_private_mbsrtowcs_cp65001;
  } else if (charset->MaxLength == 1) {
    func = p32_private_mbsrtowcs_sbcs;
  } else if (charset->MaxLength == 2) {
    func = p32_private_mbsrtowcs_dbcs;
  }

  /**
   * Local structure for intermediate results.
   */
  ConversionState convState = {0};

  convState.Input.A  = request->Input.A;
  convState.Output.W = request->Output.W;

  convState.ConvRequest.Flags    = request->Flags;
  convState.ConvRequest.Charset  = charset;
  convState.ConvRequest.Input.A  = convState.Input.A;
  convState.ConvRequest.Output.W = &convState.Buffer;

  /**
   * If we implement conversion function for `charset->CodePage`, then use
   * `func` to perform conversion.
   *
   * Otherwise, use `MultiByteToWideChar`.
   */
  if (func != NULL) {
    convState.Length = P32MbsToWcs (&convState.ConvRequest, heap, func);
  } else {
    convState.Length = P32MbsToWcsFallback (&convState.ConvRequest, heap);
  }

  if (convState.Length == -1) {
    request->Status = convState.ConvRequest.Status;
    goto fail;
  }

  assert (convState.ConvRequest.Status == CharsetConversionRequestSuccess);

  /**
   * Normalize converted string to form C.
   */
  convState.NormRequest.Form   = NormForm_C;
  convState.NormRequest.Input  = convState.Buffer;
  convState.NormRequest.Output = convState.Output.W;

  if (convState.ConvRequest.Flags & P32_CHARSET_CONVERSION_MALLOC) {
    convState.NormRequest.Flags |= P32_NORMALIZATION_REQUEST_MALLOC;
  }

  int oldLength    = convState.Length;
  convState.Length = p32_normalize_unicode_string (&convState.NormRequest, heap);

  if (convState.Length == -1) {
    switch (convState.NormRequest.Status) {
      /**
       * String is already normalized.
       */
      case NormalizationRequestNormalized:
      /**
       * Normalization is not supported.
       */
      case NormalizationRequestNotSupported:
        *convState.Output.W = convState.Buffer;
        convState.Buffer    = NULL;
        convState.Length    = oldLength;
        break;
      case NormalizationRequestOutOfMemory:
        request->Status = CharsetConversionRequestOutOfMemory;
        goto fail_free;
      default:
        request->Status = CharsetConversionRequestFailure;
        goto fail_free;
    }
  }

  request->Status = CharsetConversionRequestSuccess;

fail_free:
  if (convState.Buffer != NULL) {
    if (request->Flags & P32_CHARSET_CONVERSION_MALLOC) {
      free (convState.Buffer);
    } else {
      p32_heap_free (heap, 0, convState.Buffer);
    }
  }

fail:
  assert ((convState.Length != -1) == (request->Status == CharsetConversionRequestSuccess));
  return convState.Length;
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
    buffer = p32_heap_alloc (heap, 0, bufferSize * sizeof (char));
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
    p32_heap_free (heap, 0, buffer);
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
    buffer = p32_heap_alloc (heap, 0, bufferSize * sizeof (char));
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
    p32_heap_free (heap, 0, buffer);
  }

fail:
  _set_errno (saveErrno);
  return -1;
}

/**
 * Convert wide character string without normalization.
 */
static void P32WcsToMbsSimple (ConversionState *convState, uintptr_t heap, WcsToMbsFunc func) {
  /**
   * Prepare `convState->ConvRequest`.
   */
  convState->ConvRequest.Input.W  = convState->Input.W;
  convState->ConvRequest.Output.A = convState->Output.A;

  convState->Length = P32WcsToMbs (&convState->ConvRequest, heap, func);

  if (convState->Length == -1) {
    /**
     * If `P32WcsToMbs` failed for any reason other than conversion error,
     * treat it as a hard error and fail immediately.
     */
    if (convState->ConvRequest.Status != CharsetConversionRequestNoConversion) {
      convState->Failure = true;
      return;
    }

    goto cleanup;
  }

  convState->Success = true;

cleanup:
  convState->ConvRequest.Input.W  = NULL;
  convState->ConvRequest.Output.A = NULL;
}

/**
 * Convert wide character string with normalization.
 */
static void P32WcsToMbsNorm (ConversionState *convState, uintptr_t heap, WcsToMbsFunc func) {
  /**
   * Prepare `convState->NormRequest`.
   */
  convState->NormRequest.Input  = convState->Input.W;
  convState->NormRequest.Output = &convState->Buffer;

  if (p32_normalize_unicode_string (&convState->NormRequest, heap) == -1) {
    switch (convState->NormRequest.Status) {
      /**
       * String `convState->Input.W` is already normalized.
       *
       * If we already tried to convert it (`convState->Normalized` is `true`),
       * then clean up `convState` and continue.
       */
      case NormalizationRequestNormalized:
        if (convState->Normalized) {
          goto cleanup_norm;
        }

        convState->Normalized = true;
        break;
      /**
       * Normalization is not supported.
       *
       * If we already tried to convert it (`convState->NotSupported` is `true`),
       * then clean up `convState` and continue.
       */
      case NormalizationRequestNotSupported:
        if (convState->NotSupported) {
          goto cleanup_norm;
        }

        convState->NotSupported = true;
        break;
      case NormalizationRequestOutOfMemory:
        convState->ConvRequest.Status = CharsetConversionRequestOutOfMemory;
        convState->Failure            = true;
        return;
      default:
        convState->ConvRequest.Status = CharsetConversionRequestFailure;
        convState->Failure            = true;
        return;
    }
  }

  /**
   * Prepare `convState->ConvRequest`.
   */
  convState->ConvRequest.Input.W  = (convState->Buffer != NULL ? convState->Buffer : convState->Input.W);
  convState->ConvRequest.Output.A = convState->Output.A;

  convState->Length = P32WcsToMbs (&convState->ConvRequest, heap, func);

  if (convState->Length == -1) {
    switch (convState->ConvRequest.Status) {
      /**
       * Any error other than conversion error is a hard error.
       */
      default:
        convState->Failure = true;
      /**
       * If conversion failed, we may try with another normalization form
       * or without normalization. Clean up `convState` and continue.
       */
      case CharsetConversionRequestNoConversion:
        goto cleanup_conv;
    }
  }

  convState->Success = true;

cleanup_conv:
  if (convState->Buffer != NULL) {
    if (convState->NormRequest.Flags & P32_NORMALIZATION_REQUEST_MALLOC) {
      free (convState->Buffer);
    } else {
      p32_heap_free (heap, 0, convState->Buffer);
    }

    convState->Buffer = NULL;
  }

  convState->ConvRequest.Input.W  = NULL;
  convState->ConvRequest.Output.A = NULL;

cleanup_norm:
  convState->NormRequest.Input  = NULL;
  convState->NormRequest.Output = NULL;
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
  } else if (charset->CodePage == P32_CODEPAGE_ISO_8859_1) {
    func = p32_private_wcsrtombs_cp28591;
  } else if (charset->CodePage == CP_UTF8) {
    func = p32_private_wcsrtombs_cp65001;
  } else if (charset->MaxLength == 1) {
    func = p32_private_wcsrtombs_sbcs;
  } else if (charset->MaxLength == 2) {
    func = p32_private_wcsrtombs_dbcs;
  }

  /**
   * Local structure for intermediate results.
   */
  ConversionState convState = {0};

  convState.Input.W             = request->Input.W;
  convState.Output.A            = request->Output.A;
  convState.ConvRequest.Flags   = request->Flags;
  convState.ConvRequest.Charset = charset;

  if (convState.ConvRequest.Flags & P32_CHARSET_CONVERSION_MALLOC) {
    convState.NormRequest.Flags |= P32_NORMALIZATION_REQUEST_MALLOC;
  }

  /**
   * If we implement conversion function for `charset->CodePage`, then try
   * to use `func` to perform conversion first.
   *
   * Since `func` does not allow best-fit conversion, we may try to use
   * `WideCharToMultiByte` to do best-fit conversion if we are allowed to.
   */
  if (func != NULL) {
    /**
     * Try using normalization form C if request and allowed.
     */
    if (CONV_FLAGS (convState, P32_CHARSET_CONVERSION_NORM_C, &&, P32_CHARSET_NORM_C)) {
      convState.NormRequest.Form = NormForm_C;
      P32WcsToMbsNorm (&convState, heap, func);

      if (convState.Failure) {
        goto fail;
      }

      if (convState.Success) {
        goto done;
      }
    }

    /**
     * Try using normalization form D if request and allowed.
     */
    if (CONV_FLAGS (convState, P32_CHARSET_CONVERSION_NORM_D, &&, P32_CHARSET_NORM_D)) {
      convState.NormRequest.Form = NormForm_D;
      P32WcsToMbsNorm (&convState, heap, func);

      if (convState.Failure) {
        goto fail;
      }

      if (convState.Success) {
        goto done;
      }
    }

    /**
     * Try using normalization form KC if request and allowed.
     */
    if (CONV_FLAGS (convState, P32_CHARSET_CONVERSION_NORM_KC, &&, P32_CHARSET_NORM_KC)) {
      convState.NormRequest.Form = NormForm_KC;
      P32WcsToMbsNorm (&convState, heap, func);

      if (convState.Failure) {
        goto fail;
      }

      if (convState.Success) {
        goto done;
      }
    }

    /**
     * Try using normalization form KD if request and allowed.
     */
    if (CONV_FLAGS (convState, P32_CHARSET_CONVERSION_NORM_KD, &&, P32_CHARSET_NORM_KD)) {
      convState.NormRequest.Form = NormForm_KD;
      P32WcsToMbsNorm (&convState, heap, func);

      if (convState.Failure) {
        goto fail;
      }

      if (convState.Success) {
        goto done;
      }
    }

    /**
     * Try to convert `request->Input.W` as-is, if we have not tried yet.
     */
    if (!convState.Normalized && !convState.NotSupported) {
      P32WcsToMbsSimple (&convState, heap, func);

      if (convState.Failure) {
        goto fail;
      }

      if (convState.Success) {
        goto done;
      }
    }

    /**
     * Check whether we are allowed to attempt best-fit conversion.
     */
    if (CONV_FLAGS (convState, P32_CHARSET_CONVERSION_NO_BEST_FIT, ||, P32_CHARSET_CONV_NO_BEST_FIT)) {
      goto fail;
    }
  }

  assert (func == NULL || convState.ConvRequest.Status == CharsetConversionRequestNoConversion);

  /**
   * Try to perform conversion using `WideCharToMultiByte`.
   */
  convState.ConvRequest.Input.W  = convState.Input.W;
  convState.ConvRequest.Output.A = convState.Output.A;

  convState.Length = P32WcsToMbsFallback (&convState.ConvRequest, heap);

  if (convState.Length == -1) {
    goto fail;
  }

done:
  assert (convState.ConvRequest.Status == CharsetConversionRequestSuccess);
  request->Status = convState.ConvRequest.Status;

  return convState.Length;

fail:
  assert (convState.ConvRequest.Status != CharsetConversionRequestSuccess);
  request->Status = convState.ConvRequest.Status;

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
