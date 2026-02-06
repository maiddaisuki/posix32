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

#include <stdarg.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines some internal functions.
 */

/**
 * Provide external definitions.
 */
extern inline int p32_private_strdup (char **address, const char *str, uintptr_t heap);
extern inline int p32_private_wcsdup (wchar_t **address, const wchar_t *wcs, uintptr_t heap);

/*******************************************************************************
 * Internal functions `p32_private_asprintf` and `p32_private_aswprintf`.
 */

/**
 * Get buffer size required for string formatted according to `format`.
 */
static int P32AswprintfBufferSize (const wchar_t *format, va_list list) {
#if P32_CRT >= P32_MSVCR80
  return _vscwprintf (format, list);
#else
  wchar_t *buffer     = NULL;
  size_t   bufferSize = 0;

#if P32_CRT < P32_MSVCRT60
  /**
   * Ancient CRTs, including some historical versions of msvcrt.dll, do not
   * support calling `_vsnwprintf (NULL, 0, ...)` to obtain length of formatted
   * string.
   */
  bufferSize = 4096;
  buffer     = _alloca (bufferSize);
#endif

  return _vsnwprintf (buffer, bufferSize / sizeof (buffer[0]), format, list);
#endif
}

/**
 * Format string according to `format` and store it in `buffer`.
 */
static int P32AswprintfFormatString (wchar_t *buffer, size_t bufferSize, const wchar_t *format, va_list list) {
#if P32_CRT >= P32_MSVCR80
  return vswprintf_s (buffer, bufferSize, format, list);
#else
  /**
   * CRT's vswprintf incorrectly sets errno.
   */
  return _vswprintf (buffer, format, list);
  UNREFERENCED_PARAMETER (bufferSize);
#endif
}

/**
 * Internal implementation of `p32_private_aswprintf`.
 */
static int P32Aswprintf (wchar_t **address, uintptr_t heap, const wchar_t *format, va_list list) {
  HANDLE heapHandle = (HANDLE) heap;

  wchar_t *buffer     = NULL;
  int      bufferSize = 0;

  bufferSize = P32AswprintfBufferSize (format, list);

  if (bufferSize == -1) {
    goto fail;
  }

  bufferSize += 1;
  buffer      = (wchar_t *) HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    goto fail;
  }

  int written = P32AswprintfFormatString (buffer, bufferSize, format, list);

  if (written == -1 || written >= bufferSize) {
    goto fail_free;
  }

  *address = buffer;

  return written;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return -1;
}

int p32_private_asprintf (char **address, uintptr_t heap, const wchar_t *format, ...) {
  HANDLE heapHandle = (HANDLE) heap;

  va_list va;
  va_start (va, format);

  wchar_t *wcs       = NULL;
  char    *str       = NULL;
  int      strLength = -1;

  if (P32Aswprintf (&wcs, heap, format, va) == -1) {
    goto fail;
  }

  strLength = p32_private_wcstombs (&str, wcs, heap, 20127, false);

  if (strLength == -1) {
    goto fail_free;
  }

  *address = str;

fail_free:
  HeapFree (heapHandle, 0, wcs);

fail:
  va_end (va);

  return strLength;
}

int p32_private_aswprintf (wchar_t **address, uintptr_t heap, const wchar_t *format, ...) {
  va_list va;
  va_start (va, format);

  wchar_t *wcs       = NULL;
  int      wcsLength = -1;

  wcsLength = P32Aswprintf (&wcs, heap, format, va);

  if (wcsLength == -1) {
    goto fail;
  }

  *address = wcs;

fail:
  va_end (va);

  return wcsLength;
}
