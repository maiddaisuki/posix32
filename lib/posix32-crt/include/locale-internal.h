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

#ifndef LIBPOSIX32_LOCALE_INTERNAL_H_INCLUDED
#define LIBPOSIX32_LOCALE_INTERNAL_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

#include "p32_langinfo.h"
#include "p32_locale.h"

/**
 * Locale names were introduced in Windows Vista.
 * CRT supports locale names since msvcr110.dll.
 */
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA && P32_CRT >= P32_MSVCR110
#define P32_LOCALE_NAMES 1
#else
#define P32_LOCALE_NAMES 0
#endif

/**
 * `GetGeoInfoEx` and related APIs are available since Windows 10 1709.
 */
#if defined(LIBPOSIX32_WIN10PLUS)
#define P32_REGION_NAMES 1
#else
#define P32_REGION_NAMES 0
#endif

/**
 * `Locale`.
 */
#include "locale_types/windows_locale.h"

/**
 * Request to use locale's default ANSI code page.
 */
#define P32_CODEPAGE_ACP   ((uint16_t) -1)
/**
 * Request to use locale's default OEM code page.
 */
#define P32_CODEPAGE_OCP   ((uint16_t) -2)
/**
 * Code page for ASCII.
 */
#define P32_CODEPAGE_ASCII (20127)
/**
 * Code page used with "POSIX" locale.
 */
#define P32_CODEPAGE_POSIX (28591)

/**
 * `Charset`.
 */
#include "locale_types/charset.h"

/**
 * `LocaleStrings`.
 */
#include "locale_types/locale_strings.h"

/**
 * `LocaleInfo`.
 */
#include "locale_types/locale_info.h"

/**
 * `LocaleFunctions`.
 */
#include "locale_types/locale_functions.h"

/*
 * `locale_t`
 */
#include "locale_types/locale_t.h"

/**
 * Lookup functions used by locale string parser.
 */
#include "locale_types/locale_lookup.h"

/**
 * Returns `locale_t` object representing "POSIX" locale.
 */
locale_t p32_posix_locale (void);

/**
 * Returns `locale_t` object representing Unicode locale.
 */
locale_t p32_unicode_locale (void);

/**
 * Returns `locale_t` object representing ANSI locale.
 */
locale_t p32_ansi_locale (void);

/**
 * Returns `locale_t` object representing active global locale.
 */
locale_t p32_global_locale (void);

/**
 * Returns `locale_t` object representing active locale.
 */
locale_t p32_active_locale (void);

/**
 * Format ASCII String using `format`.
 *
 * Buffer to store formatted string is allocated from `heap`.
 * Pointer to allocated buffer is stored in `*address`.
 *
 * On success, returns number of bytes (excluding terminating '\0')
 * in the string which was stored in `*address`.
 *
 * On failure, returns `-1`;
 */
P32_TEST_DECL int p32_private_asprintf (char **address, uintptr_t heap, const wchar_t *format, ...);

/**
 * Format Wide Character String using `format`.
 *
 * Buffer to store formatted string is allocated from `heap`.
 * Pointer to allocated buffer is stored in `*address`.
 *
 * On success, returns number of wide character (excluding terminating L'\0')
 * in the string which was stored in `*address`.
 *
 * On failure, returns `-1`;
 */
P32_TEST_DECL int p32_private_aswprintf (wchar_t **address, uintptr_t heap, const wchar_t *format, ...);

/**
 * Convert Wide Character String to Multibyte Character String using
 * specified `codePage`.
 *
 * Buffer to store converted string is allocated from `heap`.
 *
 * Pointer to converted string is stored in `*address`.
 * If `address` is `NULL`, no actual conversion performed.
 *
 * If `bestFit` is `true`, allow best fit conversion.
 *
 * On success, returns number of wide character (excluding terminating L'\0')
 * in the string which was stored in `*address`.
 *
 * On failure, returns `-1`;
 */
P32_TEST_DECL int p32_private_wcstombs (
  char         **address,
  const wchar_t *string,
  uintptr_t      heap,
  uint32_t       codePage,
  bool           bestFit
);

/**
 * Convert Multibyte Character String to Wide Character String using
 * specified `codePage`.
 *
 * Buffer to store converted string is allocated from `heap`.
 *
 * Pointer to converted string is stored in `*address`.
 * If `address` is `NULL`, no actual conversion performed.
 *
 * On success, returns number of wide character (excluding terminating L'\0')
 * in the string which was stored in `*address`.
 *
 * On failure, returns `-1`;
 */
P32_TEST_DECL int p32_private_mbstowcs (wchar_t **address, const char *string, uintptr_t heap, uint32_t codePage);

/**
 * Internal version of strdup which allocates memory using `HeapAlloc`
 * instead of `malloc`.
 */
inline int p32_private_strdup (char **address, const char *str, uintptr_t heap) {
  size_t bufferSize = strlen (str);

  bufferSize += 1;
  *address    = HeapAlloc ((HANDLE) heap, 0, bufferSize);

  if (*address == NULL) {
    return -1;
  }

  memcpy (*address, str, bufferSize);
  return (int) (bufferSize - 1);
}

/**
 * Internal version of wcsdup which allocates memory using `HeapAlloc`
 * instead of `malloc`.
 */
inline int p32_private_wcsdup (wchar_t **address, const wchar_t *wcs, uintptr_t heap) {
  size_t bufferSize = wcslen (wcs);

  bufferSize += 1;
  *address    = HeapAlloc ((HANDLE) heap, 0, bufferSize * sizeof (wchar_t));

  if (*address == NULL) {
    return -1;
  }

  wmemcpy (*address, wcs, bufferSize);
  return (int) (bufferSize - 1);
}

#endif /* LIBPOSIX32_LOCALE_INTERNAL_H_INCLUDED */
