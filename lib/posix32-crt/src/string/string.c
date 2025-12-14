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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <mbstring.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ctype-internal.h"
#include "locale-internal.h"
#include "string-internal.h"
#include "wchar-internal.h"

/**
 * File Summary:
 *
 * This file defines functions declared in string.h.
 */

/**
 * We need to handle "POSIX" locale specially in some cases.
 */
#define P32_IS_POSIX(l) (l->Type == LOCALE_TYPE_POSIX)

/**
 * Evaluates to non-zero if `Charset` is a SBCS code page where all 256 bytes
 * are valid characters.
 */
#define P32_IS_SBCS(c) (c->MaxLength == 1 && (c->Flags & P32_CHARSET_FULL))

/**
 * When we use `LCID` locales, in some cases we can use ANSI versions of NLS
 * functions.
 *
 * Since all Windows locales are ASCII-compatible, we also can use them for
 * locales which use code page 20127 (ASCII).
 */
#define P32_IS_ANSI(l, c) (l->CodePage.Ansi == c->CodePage || c->CodePage == P32_CODEPAGE_ASCII)

#if P32_LOCALE_NAMES
static int P32CompareStringW (Locale *locale, DWORD flags, LPCWSTR wcs1, INT len1, LPCWSTR wcs2, INT len2) {
  int ret = CompareStringEx (locale->LocaleName, flags, wcs1, len1, wcs2, len2, NULL, NULL, 0);

#ifdef LIBPOSIX32_TEST
  _ASSERT_EXPR (ret != 0, L"Call to CompareStringEx has failed.\n");
#endif

  if (ret == 0) {
    return _NLSCMPERROR;
  }

  return ret - 2;
}
#else
static int P32CompareStringA (Locale *locale, DWORD flags, LPCSTR str1, INT len1, LPCSTR str2, INT len2) {
  int ret = CompareStringA (locale->LocaleId, flags, str1, len1, str2, len2);

#ifdef LIBPOSIX32_TEST
  _ASSERT_EXPR (ret != 0, L"Call to CompareStringA has failed.\n");
#endif

  if (ret == 0) {
    return _NLSCMPERROR;
  }

  return ret - 2;
}

static int P32CompareStringW (Locale *locale, DWORD flags, LPCWSTR wcs1, INT len1, LPCWSTR wcs2, INT len2) {
  int ret = CompareStringW (locale->LocaleId, flags, wcs1, len1, wcs2, len2);

#ifdef LIBPOSIX32_TEST
  _ASSERT_EXPR (ret != 0, L"Call to CompareStringW has failed.\n");
#endif

  if (ret == 0) {
    return _NLSCMPERROR;
  }

  return ret - 2;
}
#endif

#if P32_LOCALE_NAMES
static int P32LCMapSortKeyW (Locale *locale, DWORD flags, LPCWSTR src, INT srcSize, LPWSTR dest, INT destSize) {
  return LCMapStringEx (locale->LocaleName, flags | LCMAP_SORTKEY, src, srcSize, dest, destSize, NULL, NULL, 0);
}
#else
static int P32LCMapSortKeyA (Locale *locale, DWORD flags, LPCSTR src, INT srcSize, LPSTR dest, INT destSize) {
  return LCMapStringA (locale->LocaleId, flags | LCMAP_SORTKEY, src, srcSize, dest, destSize);
}

static int P32LCMapSortKeyW (Locale *locale, DWORD flags, LPCWSTR src, INT srcSize, LPWSTR dest, INT destSize) {
  return LCMapStringW (locale->LocaleId, flags | LCMAP_SORTKEY, src, srcSize, dest, destSize);
}
#endif

/**
 * Convert multibyte character string to wide character string.
 *
 * This function allocates buffer to store converted string using `malloc`.
 * Converted string must be freed with `free` when no longer needed.
 *
 * On success, this funtions returns length of converted string.
 * On failure, this function returns `-1`.
 */
static int P32MbsToWcs (wchar_t **address, const char *mbs, locale_t locale) {
  /**
   * Conversion state for `mbs`.
   */
  mbstate_t state = {0};

  wchar_t *buffer     = NULL;
  size_t   bufferSize = 0;

  bufferSize = p32_private_mbsrtowcs_l (buffer, &mbs, bufferSize, &state, locale);

  if (bufferSize == (size_t) -1 && bufferSize >= INT_MAX) {
    return -1;
  }

  bufferSize += 1;
  buffer      = malloc (bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    return -1;
  }

  size_t written = p32_private_mbsrtowcs_l (buffer, &mbs, bufferSize, &state, locale);

  if (written == (size_t) -1 || mbs != NULL) {
    free (buffer);
    return -1;
  }

  *address = buffer;

  return (int) written;
}

/**
 * Convert up to `count` bytes of multibyte character string `mbs`
 * to wide character string.
 *
 * This function allocates buffer to store converted string using `malloc`.
 * Converted string must be freed with `free` when no longer needed.
 *
 * On success, this funtions returns length of converted string.
 * On failure, this function returns `-1`.
 */
static int P32MbsToWcsS (wchar_t **address, const char *mbs, size_t count, locale_t locale) {
  /**
   * Conversion state for `mbs`.
   */
  mbstate_t state = {0};

  wchar_t *buffer     = NULL;
  size_t   bufferSize = 0;

  bufferSize = p32_private_mbsnrtowcs_l (buffer, &mbs, count, bufferSize, &state, locale);

  if (bufferSize == (size_t) -1 && bufferSize >= INT_MAX) {
    return -1;
  }

  bufferSize += 1;
  buffer      = malloc (bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    return -1;
  }

  size_t written = p32_private_mbsnrtowcs_l (buffer, &mbs, count, bufferSize, &state, locale);

  if (written == (size_t) -1 || written >= bufferSize) {
    free (buffer);
    return -1;
  }

  *address = buffer;

  return (int) written;
}

/**
 * Locale independant functions.
 */
#include "common/strdup.c"
#include "common/strndup.c"

/**
 * Functions dependant on character set (LC_CTYPE).
 */
#include "common/strcat.c"
#include "common/strchr.c"
#include "common/strcmp.c"
#include "common/strcpy.c"
#include "common/strcspn.c"
#include "common/strlen.c"
#include "common/strncat.c"
#include "common/strncmp.c"
#include "common/strncpy.c"
#include "common/strnlen.c"
#include "common/strpbrk.c"
#include "common/strrchr.c"
#include "common/strspn.c"
#include "common/strstr.c"
#include "common/strtok.c"
#include "common/strtok_r.c"

/**
 * Functions dependant on LC_CTYPE locale category.
 */
#include "common/strcasecmp.c"
#include "common/strncasecmp.c"

/**
 * Functions dependant on LC_COLLATE locale category.
 */
#include "common/strcoll.c"
#include "common/strxfrm.c"

void p32_string_functions (locale_t locale) {
  /**
   * Following functions depend on character set in use (LC_CTYPE).
   */
  P32LocaleFunction_strlen (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strnlen (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strcpy (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strncpy (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strcat (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strncat (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strcmp (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strncmp (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strchr (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strrchr (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strstr (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strpbrk (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strspn (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strcspn (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strtok (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strtok_r (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  /**
   * Following functions depend on LC_CTYPE locale category.
   */
  P32LocaleFunction_strcasecmp (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  P32LocaleFunction_strncasecmp (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCtype);
  /**
   * Following functions depend on LC_COLLATE locale category.
   */
  P32LocaleFunction_strcoll (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCollate);
  P32LocaleFunction_strxfrm (&locale->Functions, &locale->Charset, &locale->WinLocale.LcCollate);
}
