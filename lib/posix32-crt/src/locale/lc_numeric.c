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
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to gather locale information dependant on
 * LC_NUMERIC locale category.
 */

/**
 * Get locale information dependant on `LC_NUMERIC` locale category.
 */
static bool P32LcNumericInfo (LcNumericInfo *info, uintptr_t heap, Locale *locale) {
  if (!p32_winlocale_getinfo (&info->RadixChar.W, heap, locale, LOCALE_SDECIMAL)) {
    goto fail;
  }
  if (!p32_winlocale_getinfo (&info->ThousandsSeparator.W, heap, locale, LOCALE_STHOUSAND)) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcNumericInfoW (LcNumericInfo *dest, uintptr_t heap, LcNumericInfo *src) {
  if (p32_private_wcsdup (&dest->RadixChar.W, src->RadixChar.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->ThousandsSeparator.W, src->ThousandsSeparator.W, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale infromation stored in `info`.
 */
static void P32FreeLcNumericInfoW (LcNumericInfo *info, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (info->RadixChar.W != NULL) {
    HeapFree (heapHandle, 0, info->RadixChar.W);
    info->RadixChar.W = NULL;
  }
  if (info->ThousandsSeparator.W != NULL) {
    HeapFree (heapHandle, 0, info->ThousandsSeparator.W);
    info->ThousandsSeparator.W = NULL;
  }
}

/**
 * Convert locale information in `info` to `locale->Charset.CodePage`.
 */
static bool P32ConverLcNumericInfo (LcNumericInfo *info, uintptr_t heap, locale_t locale) {
  /**
   * Code page to use during conversion.
   */
  uint32_t codePage = locale->Charset.CodePage;

  /**
   * Do not allow best-fit conversion for ASCII.
   */
  bool bestFit = codePage != P32_CODEPAGE_ASCII;

  if (p32_private_wcstombs (&info->RadixChar.A, info->RadixChar.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->ThousandsSeparator.A, info->ThousandsSeparator.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcNumericInfoA (LcNumericInfo *dest, uintptr_t heap, LcNumericInfo *src) {
  if (p32_private_strdup (&dest->RadixChar.A, src->RadixChar.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->ThousandsSeparator.A, src->ThousandsSeparator.A, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale infromation stored in `info`.
 */
static void P32FreeLcNumericInfoA (LcNumericInfo *info, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (info->RadixChar.A != NULL) {
    HeapFree (heapHandle, 0, info->RadixChar.A);
    info->RadixChar.A = NULL;
  }
  if (info->ThousandsSeparator.A != NULL) {
    HeapFree (heapHandle, 0, info->ThousandsSeparator.A);
    info->ThousandsSeparator.A = NULL;
  }
}

bool p32_localeinfo_numeric (locale_t locale, uintptr_t heap) {
  Locale        *lcNumeric     = &locale->WinLocale.LcNumeric;
  LcNumericInfo *lcNumericInfo = &locale->LocaleInfo.LcNumeric;

  if (!P32LcNumericInfo (lcNumericInfo, heap, lcNumeric)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_NUMERIC): failed to obtain locale information\n", lcNumeric->LocaleName);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail;
  }

  if (!P32ConverLcNumericInfo (lcNumericInfo, heap, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_NUMERIC): failed to convert locale information\n", lcNumeric->LocaleName);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_convert;
  }

  return true;

fail_convert:
  P32FreeLcNumericInfoA (lcNumericInfo, heap);
fail:
  P32FreeLcNumericInfoW (lcNumericInfo, heap);

  return false;
}

bool p32_localeinfo_numeric_copy (LcNumericInfo *dest, uintptr_t heap, LcNumericInfo *src) {
  if (!P32CopyLcNumericInfoW (dest, heap, src)) {
    goto fail_w;
  }

  if (!P32CopyLcNumericInfoA (dest, heap, src)) {
    goto fail_a;
  }

  return true;

fail_a:
  P32FreeLcNumericInfoA (dest, heap);
fail_w:
  P32FreeLcNumericInfoW (dest, heap);

  return false;
}

void p32_localeinfo_numeric_free (LcNumericInfo *lcNumericInfo, uintptr_t heap) {
  P32FreeLcNumericInfoA (lcNumericInfo, heap);
  P32FreeLcNumericInfoW (lcNumericInfo, heap);
}
