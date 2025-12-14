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
 * LC_MONETARY locale category.
 */

/**
 * Get locale information dependant on `LC_MONETARY` locale category.
 */
static bool P32LcMonetaryInfo (LcMonetaryInfo *info, uintptr_t heap, Locale *locale) {
  if (!p32_winlocale_getinfo (&info->CurrencyString.W, heap, locale, LOCALE_SCURRENCY)) {
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
 * Convert locale information in `info` to `codePage`.
 */
static bool P32ConvertLcMonetaryInfo (LcMonetaryInfo *info, uintptr_t heap, locale_t locale) {
  /**
   * Code page to use during conversion.
   */
  uint32_t codePage = locale->Charset.CodePage;

  /**
   * Do not allow best-fit conversion for ASCII.
   */
  bool bestFit = codePage != P32_CODEPAGE_ASCII;

  /**
   * FIXME: some locales cannot represent currency symbol with their default
   * code page, but otherwise completely usable.
   */
  if (p32_private_wcstombs (&info->CurrencyString.A, info->CurrencyString.W, heap, codePage, bestFit) == -1) {
    /**
     * ASCII locales should be able to represent their currency symbols.
     */
    if (codePage == P32_CODEPAGE_ASCII) {
      goto fail;
    }

#ifdef LIBPOSIX32_TEST
    _RPTW2 (
      _CRT_WARN, L"%s(LC_NUMERIC): cannot convert currency symbol '%s'\n", locale->WindowsLocaleStrings.W.LcMonetary,
      info->CurrencyString.W
    );
#endif

    /**
     * Use empty string if currency symbol cannot be converted.
     */
    if (p32_private_strdup (&info->CurrencyString.A, "", heap) == -1) {
      goto fail;
    }
  }

  return true;

fail:
  return false;
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

/**
 * External Functions
 */

bool p32_localeinfo_monetary (locale_t locale, uintptr_t heap) {
  Locale         *lcMonetary     = &locale->WinLocale.LcMonetary;
  LcMonetaryInfo *lcMonetaryInfo = &locale->LocaleInfo.LcMonetary;

  if (!P32LcMonetaryInfo (lcMonetaryInfo, heap, lcMonetary)) {
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

  if (!P32ConvertLcMonetaryInfo (lcMonetaryInfo, heap, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (
      _CRT_ERROR, L"%s(LC_MONETARY): failed to convert locale information\n", locale->WindowsLocaleStrings.W.LcMonetary
    );

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_convert;
  }

  return true;

fail_convert:
  P32FreeLcMonetaryInfoA (lcMonetaryInfo, heap);
fail:
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
