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
 * LC_MESSAGES locale category.
 */

/**
 * Get locale information dependant on `LC_MESSAGES` locale category.
 */
static bool P32LcMessagesInfo (LcMessagesInfo *lcMessagesInfo, uintptr_t heap, Locale *lcMessages) {
  if (p32_private_wcsdup (&lcMessagesInfo->NoStr.W, L"no", heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&lcMessagesInfo->YesStr.W, L"yes", heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&lcMessagesInfo->NoExpr.W, L"^[nN]", heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&lcMessagesInfo->YesExpr.W, L"^[yY]", heap) == -1) {
    goto fail;
  }

  return true;
  UNREFERENCED_PARAMETER (lcMessages);

fail:
  return false;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcMessagesInfoW (LcMessagesInfo *dest, uintptr_t heap, LcMessagesInfo *src) {
  if (p32_private_wcsdup (&dest->NoStr.W, src->NoStr.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->YesStr.W, src->YesStr.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->NoExpr.W, src->NoExpr.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->YesExpr.W, src->YesExpr.W, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale infromation stored in `lcMessagesInfo`.
 */
static void P32FreeLcMessagesInfoW (LcMessagesInfo *lcMessagesInfo, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (lcMessagesInfo->NoStr.W != NULL) {
    HeapFree (heapHandle, 0, lcMessagesInfo->NoStr.W);
    lcMessagesInfo->NoStr.W = NULL;
  }
  if (lcMessagesInfo->YesStr.W != NULL) {
    HeapFree (heapHandle, 0, lcMessagesInfo->YesStr.W);
    lcMessagesInfo->YesStr.W = NULL;
  }
  if (lcMessagesInfo->NoExpr.W != NULL) {
    HeapFree (heapHandle, 0, lcMessagesInfo->NoExpr.W);
    lcMessagesInfo->NoExpr.W = NULL;
  }
  if (lcMessagesInfo->YesExpr.W != NULL) {
    HeapFree (heapHandle, 0, lcMessagesInfo->YesExpr.W);
    lcMessagesInfo->YesExpr.W = NULL;
  }
}

/**
 * Convert locale information in `lcMessagesInfo` to `locale->Charset.CodePage`.
 */
static bool P32ConvertLcMessagesInfo (LcMessagesInfo *lcMessagesInfo, uintptr_t heap, locale_t locale) {
  /**
   * Code page to use during conversion.
   */
  uint32_t codePage = locale->Charset.CodePage;

  /**
   * Do not allow best-fit conversion for ASCII.
   */
  bool bestFit = codePage != P32_CODEPAGE_ASCII;

  if (p32_private_wcstombs (&lcMessagesInfo->NoStr.A, lcMessagesInfo->NoStr.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&lcMessagesInfo->YesStr.A, lcMessagesInfo->YesStr.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  if (p32_private_wcstombs (&lcMessagesInfo->NoExpr.A, lcMessagesInfo->NoExpr.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&lcMessagesInfo->YesExpr.A, lcMessagesInfo->YesExpr.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcMessagesInfoA (LcMessagesInfo *dest, uintptr_t heap, LcMessagesInfo *src) {
  if (p32_private_strdup (&dest->NoStr.A, src->NoStr.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->YesStr.A, src->YesStr.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->NoExpr.A, src->NoExpr.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->YesExpr.A, src->YesExpr.A, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale infromation stored in `lcMessagesInfo`.
 */
static void P32FreeLcMessagesInfoA (LcMessagesInfo *lcMessagesInfo, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (lcMessagesInfo->NoStr.A != NULL) {
    HeapFree (heapHandle, 0, lcMessagesInfo->NoStr.A);
    lcMessagesInfo->NoStr.A = NULL;
  }
  if (lcMessagesInfo->YesStr.A != NULL) {
    HeapFree (heapHandle, 0, lcMessagesInfo->YesStr.A);
    lcMessagesInfo->YesStr.A = NULL;
  }
  if (lcMessagesInfo->NoExpr.A != NULL) {
    HeapFree (heapHandle, 0, lcMessagesInfo->NoExpr.A);
    lcMessagesInfo->NoExpr.A = NULL;
  }
  if (lcMessagesInfo->YesExpr.A != NULL) {
    HeapFree (heapHandle, 0, lcMessagesInfo->YesExpr.A);
    lcMessagesInfo->YesExpr.A = NULL;
  }
}

bool p32_localeinfo_messages (locale_t locale, uintptr_t heap) {
  Locale         *lcMessages     = &locale->WinLocale.LcMessages;
  LcMessagesInfo *lcMessagesInfo = &locale->LocaleInfo.LcMessages;

  if (!P32LcMessagesInfo (lcMessagesInfo, heap, lcMessages)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_MESSAGES): failed to obtain locale information\n", lcMessages->LocaleName);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_w;
  }

  if (!P32ConvertLcMessagesInfo (lcMessagesInfo, heap, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_MESSAGES): failed to convert locale information\n", lcMessages->LocaleName);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_a;
  }

  return true;

fail_a:
  P32FreeLcMessagesInfoA (lcMessagesInfo, heap);
fail_w:
  P32FreeLcMessagesInfoW (lcMessagesInfo, heap);

  return false;
}

bool p32_localeinfo_messages_copy (LcMessagesInfo *dest, uintptr_t heap, LcMessagesInfo *src) {
  if (!P32CopyLcMessagesInfoW (dest, heap, src)) {
    goto fail_w;
  }

  if (!P32CopyLcMessagesInfoA (dest, heap, src)) {
    goto fail_a;
  }

  return true;

fail_a:
  P32FreeLcMessagesInfoA (dest, heap);
fail_w:
  P32FreeLcMessagesInfoW (dest, heap);

  return false;
}

void p32_localeinfo_messages_free (LcMessagesInfo *lcMessagesInfo, uintptr_t heap) {
  P32FreeLcMessagesInfoA (lcMessagesInfo, heap);
  P32FreeLcMessagesInfoW (lcMessagesInfo, heap);
}
