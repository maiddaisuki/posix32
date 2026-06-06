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
#include <pthread.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-atomic.h"
#include "core-heap.h"
#include "core-loader.h"
#include "core-norm.h"
#include "core-runtime.h"
#include "core-winver.h"

/**
 * File Summary:
 *
 * This file defines `p32_normalize_unicode_string` function.
 * This function is used to normalize Unicode strings.
 */

/*******************************************************************************
 * Structures, functions and macros used to lookup functions at runtime.
 */

#if P32_WINNT < P32_WINNT_VISTA
#define DYNAMIC_CHECKS
#define DYNAMIC_IMPLEMENTATION

/**
 * Function type corresponding to `IsNormalizedString`.
 */
typedef BOOL (WINAPI *FuncIsNormalizedString) (NORM_FORM, LPCWSTR, INT);

/**
 * Function type corresponding to `NormalizeString`.
 */
typedef INT (WINAPI *FuncNormalizeString) (NORM_FORM, LPCWSTR, INT, LPWSTR, INT);
#endif /* P32_WINNT < Windows Vista */

#ifdef DYNAMIC_CHECKS
/**
 * Pointers to Normalization APIs which are looked up at runtime.
 */
typedef struct NormApi {
  pthread_once_t Init;

#if P32_WINNT < P32_WINNT_VISTA
  /**
   * According to Microsoft documentation:
   *
   * <https://learn.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-isnormalizedstring>
   * <https://learn.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-normalizestring>
   *
   * functions `IsNormalizedString` and `NormalizeString` are located in
   * Normaliz.dll. We will first try to look them up in kernel32.dll, and if it
   * fails, we will try to look them up in Normaliz.dll.
   *
   * Since we may load Normaliz.dll, it is our responsibility to unload it when
   * it is no longer needed.
   */
  uintptr_t              NormalizDll;
  FuncIsNormalizedString PtrIsNormalizedString;
  FuncNormalizeString    PtrNormalizeString;
#endif /* P32_WINNT < Windows Vista */
} NormApi;

/**
 * Normalization APIs.
 */
static NormApi P32NormApi = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WINNT < P32_WINNT_VISTA
  .NormalizDll           = 0,
  .PtrIsNormalizedString = NULL,
  .PtrNormalizeString    = NULL,
#endif /* P32_WINNT < Windows Vista */
};

/**
 * Release resources used by `P32NormApi`.
 */
static int __cdecl P32FiniNormApi (void) {
#if P32_WINNT < P32_WINNT_VISTA
  if (P32NormApi.NormalizDll != 0) {
    p32_free_library (P32NormApi.NormalizDll);
  }
#endif

  return EXIT_SUCCESS;
}

/**
 * Initialize `P32NormApi`.
 */
static void P32InitNormApi (void) {
  uintptr_t kernel32 = p32_get_module_handle (P32_MODULE_NAME ("kernel32.dll"));
  assert (kernel32 != 0);

#if P32_WINNT < P32_WINNT_VISTA
  /**
   * Lookup `IsNormalizedString` and `NormalizeString`.
   */
  FuncIsNormalizedString ptrIsNormalizedString = NULL;
  FuncNormalizeString    ptrNormalizeString    = NULL;

  if (P32_WINNT_CHECK (P32_WINNT_VISTA, WindowsNtVista)) {
    if (kernel32 != 0) {
      ptrIsNormalizedString = p32_get_proc_address (kernel32, IsNormalizedString);
      ptrNormalizeString    = p32_get_proc_address (kernel32, NormalizeString);
    }

    assert ((ptrIsNormalizedString == NULL) == (ptrNormalizeString == NULL));

    if (ptrIsNormalizedString == NULL || ptrNormalizeString == NULL) {
      uintptr_t normaliz = p32_load_library (P32_MODULE_NAME ("Normaliz.dll"));

      if (normaliz != 0) {
        ptrIsNormalizedString = p32_get_proc_address (normaliz, IsNormalizedString);
        ptrNormalizeString    = p32_get_proc_address (normaliz, NormalizeString);
      }

      assert ((ptrIsNormalizedString == NULL) == (ptrNormalizeString == NULL));

      if (ptrIsNormalizedString != NULL && ptrNormalizeString != NULL) {
        P32NormApi.NormalizDll = normaliz;
      } else {
        p32_free_library (normaliz);
      }
    }

    if (ptrIsNormalizedString != NULL && ptrNormalizeString != NULL) {
      p32_atomic_exchange_fpointer (&P32NormApi.PtrIsNormalizedString, ptrIsNormalizedString);
      p32_atomic_exchange_fpointer (&P32NormApi.PtrNormalizeString, ptrNormalizeString);
    }
  }
#endif /* P32_WINNT < Windows Vista */

  if (_onexit (P32FiniNormApi) == NULL) {
    p32_terminate (L"Failed to register clean up function.");
  }
}
#endif /* DYNAMIC_CHECKS */

#if P32_WINNT < P32_WINNT_VISTA
#define IsNormalizedString P32NormApi.PtrIsNormalizedString
#define NormalizeString    P32NormApi.PtrNormalizeString
#endif /* P32_WINNT < Windows Vista */

/*******************************************************************************
 * Implementation.
 *
 * p32_normalize_unicode_string:
 *
 *  Windows 9x:
 *
 *    Unicode support is missing and normalization is not available.
 *
 *    While function `FoldStringA` is available on all Windows 9x systems,
 *    its functionality is limited to active ANSI code page.
 *
 *  <= Windows XP
 *
 *    Use `FoldStringW` to normalize Unicode strings.
 *
 *  >= Windows Vista
 *
 *    Use `NormalizeString` to normalize Unicode strings.
 */

#if P32_WINNT < P32_WINNT_VISTA
/**
 * Function type corresponding to `p32_normalize_unicode_string`.
 */
typedef int (*FuncNormalizeUnicodeString) (NormalizationRequest *, uintptr_t);

/**
 * Initialization thunk for `p32_normalize_unicode_string`.
 */
static int P32InitNormalizeUnicodeString (NormalizationRequest *request, uintptr_t heap);

/**
 * Implementation for `p32_normalize_unicode_string` using `FoldStringW`.
 */
static int P32FoldString (NormalizationRequest *request, uintptr_t heap);
#endif /* P32_WINNT < Windows Vista */

/**
 * Implementation for `p32_normalize_unicode_string` using `NormalizeString`.
 */
static int P32NormalizeString (NormalizationRequest *request, uintptr_t heap);

#ifdef DYNAMIC_IMPLEMENTATION
/**
 * Pointers to implementation.
 */
typedef struct NormImpl {
  pthread_once_t Init;

#if P32_WINNT < P32_WINNT_VISTA
  /**
   * Implementation for `p32_normalize_unicode_string`.
   */
  FuncNormalizeUnicodeString PtrNormalizeUnicodeString;
#endif /* P32_WINNT < Windows Vista */
} NormImpl;

/**
 * Implementation.
 */
static NormImpl P32NormImpl = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WINNT < P32_WINNT_VISTA
  .PtrNormalizeUnicodeString = P32InitNormalizeUnicodeString,
#endif /* P32_WINNT < Windows Vista */
};

/**
 * Initialize `P32NormImpl`.
 */
static void P32InitNormImpl (void) {
#ifdef DYNAMIC_CHECKS
  pthread_once (&P32NormApi.Init, P32InitNormApi);
#endif

#if P32_WINNT < P32_WINNT_VISTA
  if (P32NormApi.PtrIsNormalizedString != NULL && P32NormApi.PtrNormalizeString != NULL) {
    p32_atomic_exchange_fpointer (&P32NormImpl.PtrNormalizeUnicodeString, P32NormalizeString);
  } else {
    p32_atomic_exchange_fpointer (&P32NormImpl.PtrNormalizeUnicodeString, P32FoldString);
  }
#endif
}
#endif /* DYNAMIC_IMPLEMENTATION */

#if P32_WINNT < P32_WINNT_VISTA
#define NormalizeUnicodeString P32NormImpl.PtrNormalizeUnicodeString

static int P32InitNormalizeUnicodeString (NormalizationRequest *request, uintptr_t heap) {
  pthread_once (&P32NormImpl.Init, P32InitNormImpl);
  return NormalizeUnicodeString (request, heap);
}

static int P32FoldString (NormalizationRequest *request, uintptr_t heap) {
  /**
   * Flags for `FoldStringW` corresponding to `request->Form`.
   */
  DWORD flags;

  switch (request->Form) {
    case NormForm_C:
      flags = MAP_PRECOMPOSED;
      break;
    case NormForm_D:
      flags = MAP_COMPOSITE;
      break;
    case NormForm_KC:
      flags = MAP_PRECOMPOSED | MAP_FOLDCZONE;
      break;
    case NormForm_KD:
      flags = MAP_COMPOSITE | MAP_FOLDCZONE;
      break;
    default:
      request->Status = NormalizationRequestFailure;
      return -1;
  }

  INT    bufferSize = 0;
  LPWSTR buffer     = NULL;

  bufferSize = FoldStringW (flags, request->Input, -1, NULL, 0);

  if (bufferSize == 0) {
    switch (GetLastError ()) {
      case ERROR_INVALID_DATA:
        request->Status = NormalizationRequestInvalidUnicode;
        return -1;
      case ERROR_OUTOFMEMORY:
        request->Status = NormalizationRequestOutOfMemory;
        return -1;
#if P32_WIN9X
      /**
       * Function `FoldStringW` is available in kernel32.dll on Windows 9x
       * systems, but it always fails with `ERROR_CALL_NOT_IMPLEMENTED`.
       */
      case ERROR_CALL_NOT_IMPLEMENTED:
#endif
      case ERROR_MOD_NOT_FOUND:
      case ERROR_PROC_NOT_FOUND:
        request->Status = NormalizationRequestNotSupported;
        return -1;
      default:
        request->Status = NormalizationRequestFailure;
        return -1;
    }
  }

  if (request->Flags & P32_NORMALIZATION_REQUEST_MALLOC) {
    buffer = malloc (bufferSize * sizeof (wchar_t));
  } else {
    buffer = p32_heap_alloc (heap, 0, bufferSize * sizeof (wchar_t));
  }

  if (buffer == NULL) {
    request->Status = NormalizationRequestOutOfMemory;
    return -1;
  }

  INT written = FoldStringW (flags, request->Input, -1, buffer, bufferSize);
  assert (written != 0 && written <= bufferSize);

  if (written == 0 || written > bufferSize) {
    request->Status = NormalizationRequestFailure;
    goto fail;
  }

  /**
   * Check if normalized string is binary equivalent to `request->Input`.
   *
   * On pre-Vista systems, there is no mechanism to check whether string is
   * already normalized, so we have to normalize it first and then compare it
   * to the original string.
   */
  if (wcscmp (request->Input, buffer) == 0) {
    request->Status = NormalizationRequestNormalized;
    goto fail;
  }

  *request->Output = buffer;
  request->Status  = NormalizationRequestSuccess;

  return written - 1;

fail:
  if (request->Flags & P32_NORMALIZATION_REQUEST_MALLOC) {
    free (buffer);
  } else {
    p32_heap_free (heap, 0, buffer);
  }

  return -1;
}

#else /* P32_WINNT >= Windows Vista */
#define NormalizeUnicodeString P32NormalizeString
#endif /* P32_WINNT >= Windows Vista */

static int P32NormalizeString (NormalizationRequest *request, uintptr_t heap) {
  /**
   * `NORM_FORM` value corresponding to `request->Form`.
   */
  NORM_FORM normForm;

  switch (request->Form) {
    case NormForm_C:
      normForm = NormalizationC;
      break;
    case NormForm_D:
      normForm = NormalizationD;
      break;
    case NormForm_KC:
      normForm = NormalizationKC;
      break;
    case NormForm_KD:
      normForm = NormalizationKD;
      break;
    default:
      request->Status = NormalizationRequestFailure;
      return -1;
  }

  /**
   * This is required in order to properly distinguish case when
   * `request->Input` is not normalized from an actual error.
   */
  SetLastError (ERROR_SUCCESS);

  /**
   * Check if `request->Input` is already normalized.
   */
  if (IsNormalizedString (normForm, request->Input, -1)) {
    /**
     * Normalization is not required; `request->Input` is already normalized.
     */
    request->Status = NormalizationRequestNormalized;
    return -1;
  } else {
    switch (GetLastError ()) {
      case ERROR_SUCCESS:
        break;
      case ERROR_NO_UNICODE_TRANSLATION:
        request->Status = NormalizationRequestInvalidUnicode;
        return -1;
      default:
        request->Status = NormalizationRequestFailure;
        return -1;
    }
  }

  INT    bufferSize = 0;
  LPWSTR buffer     = NULL;

  bufferSize = NormalizeString (normForm, request->Input, -1, NULL, 0);
  assert (bufferSize > 0);

  if (bufferSize <= 0) {
    request->Status = NormalizationRequestFailure;
    return -1;
  }

  if (request->Flags & P32_NORMALIZATION_REQUEST_MALLOC) {
    buffer = malloc (bufferSize * sizeof (wchar_t));
  } else {
    buffer = p32_heap_alloc (heap, 0, bufferSize * sizeof (wchar_t));
  }

  if (buffer == NULL) {
    request->Status = NormalizationRequestOutOfMemory;
    return -1;
  }

  INT written = NormalizeString (normForm, request->Input, -1, buffer, bufferSize);
  assert (written > 0 && written <= bufferSize);

  if (written <= 0 || written > bufferSize) {
    goto fail;
  }

  /**
   * First call to `NormalizeString` may return buffer size larger than
   * actually required; realloc if needed.
   */
  if (written < bufferSize) {
    if (request->Flags & P32_NORMALIZATION_REQUEST_MALLOC) {
      *request->Output = realloc (buffer, written * sizeof (wchar_t));
    } else {
      *request->Output = p32_heap_realloc (heap, 0, buffer, written * sizeof (wchar_t));
    }

    if (*request->Output == NULL) {
      *request->Output = buffer;
    }
  } else {
    *request->Output = buffer;
  }

  request->Status = NormalizationRequestSuccess;
  return written - 1;

fail:
  if (request->Flags & P32_NORMALIZATION_REQUEST_MALLOC) {
    free (buffer);
  } else {
    p32_heap_free (heap, 0, buffer);
  }

  request->Status = NormalizationRequestFailure;
  return -1;
}

/*******************************************************************************
 * External Functions.
 */

int p32_normalize_unicode_string (NormalizationRequest *request, uintptr_t heap) {
  return NormalizeUnicodeString (request, heap);
}
