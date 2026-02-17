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
 * LC_MONETARY locale category.
 */

/**
 * Get locale information dependant on `LC_MONETARY` locale category.
 */
static bool P32LcMonetaryInfo (LcMonetaryInfo *info, uintptr_t heap, Locale *lcMonetaryInfo, locale_t locale) {
  LocaleInfoRequest infoRequest      = {0};
  uint32_t          infoRequestFlags = P32_LOCALE_INFO_REQUEST_CONVERT;

  infoRequest.CodePage = locale->Charset.CodePage;

  if (infoRequest.CodePage != P32_CODEPAGE_ASCII) {
    infoRequestFlags |= (P32_LOCALE_INFO_REQUEST_CONVERT_BEST_FIT);
  }

  /**
   * Locale's currency symbol.
   *
   * Situation when currency symbol cannot be converted to ANSI/OEM code pages
   * is very common; we handle this situation gracefully.
   */
  infoRequest.Info    = LOCALE_SCURRENCY;
  infoRequest.Flags   = (infoRequestFlags | P32_LOCALE_INFO_REQUEST_CONVERT_FALLBACK);
  infoRequest.OutputA = &info->CurrencyString.A;
  infoRequest.OutputW = &info->CurrencyString.W;

  if (!p32_winlocale_get_locale_info (&infoRequest, heap, lcMonetaryInfo)) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcMonetaryInfoW (LcMonetaryInfo *dest, uintptr_t heap, LcMonetaryInfo *src) {
  if (p32_private_wcsdup (&dest->CurrencyString.W, src->CurrencyString.W, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale information stored in `info`.
 */
static void P32FreeLcMonetaryInfoW (LcMonetaryInfo *info, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (info->CurrencyString.W != NULL) {
    HeapFree (heapHandle, 0, info->CurrencyString.W);
    info->CurrencyString.W = NULL;
  }
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcMonetaryInfoA (LcMonetaryInfo *dest, uintptr_t heap, LcMonetaryInfo *src) {
  if (p32_private_strdup (&dest->CurrencyString.A, src->CurrencyString.A, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale information stored in `info`.
 */
static void P32FreeLcMonetaryInfoA (LcMonetaryInfo *info, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (info->CurrencyString.A != NULL) {
    HeapFree (heapHandle, 0, info->CurrencyString.A);
    info->CurrencyString.A = NULL;
  }
}

bool p32_localeinfo_monetary (locale_t locale, uintptr_t heap) {
  Locale         *lcMonetary     = &locale->WinLocale.LcMonetary;
  LcMonetaryInfo *lcMonetaryInfo = &locale->LocaleInfo.LcMonetary;

  if (!P32LcMonetaryInfo (lcMonetaryInfo, heap, lcMonetary, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (
      _CRT_ERROR, L"%s(LC_MONETARY): failed to obtain locale information\n", locale->WindowsLocaleStrings.W.LcMonetary
    );

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail;
  }

  return true;

fail:
  P32FreeLcMonetaryInfoA (lcMonetaryInfo, heap);
  P32FreeLcMonetaryInfoW (lcMonetaryInfo, heap);
  return false;
}

bool p32_localeinfo_monetary_copy (LcMonetaryInfo *dest, uintptr_t heap, LcMonetaryInfo *src) {
  if (!P32CopyLcMonetaryInfoW (dest, heap, src)) {
    goto fail_w;
  }

  if (!P32CopyLcMonetaryInfoA (dest, heap, src)) {
    goto fail_a;
  }

  return true;

fail_a:
  P32FreeLcMonetaryInfoA (dest, heap);
fail_w:
  P32FreeLcMonetaryInfoW (dest, heap);

  return false;
}

void p32_localeinfo_monetary_free (LcMonetaryInfo *lcMonetaryInfo, uintptr_t heap) {
  P32FreeLcMonetaryInfoA (lcMonetaryInfo, heap);
  P32FreeLcMonetaryInfoW (lcMonetaryInfo, heap);
}
