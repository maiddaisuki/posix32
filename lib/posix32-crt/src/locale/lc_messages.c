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

#include "core-heap.h"

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
static bool P32LcMessagesInfo (LcMessagesInfo *lcMessagesInfo, uintptr_t heap, Locale *lcMessages, locale_t locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Flags   |= P32_LOCALE_INFO_REQUEST_CONVERT;
  infoRequest.charset  = &locale->Charset;

  infoRequest.Info    = NOSTR;
  infoRequest.OutputA = &lcMessagesInfo->NoStr.A;
  infoRequest.OutputW = &lcMessagesInfo->NoStr.W;

  if (p32_posix_get_locale_info (&infoRequest, heap, P32_POSIX_LOCALE_INFO_NL_ITEM) != 0) {
    goto fail;
  }

  infoRequest.Info    = YESSTR;
  infoRequest.OutputA = &lcMessagesInfo->YesStr.A;
  infoRequest.OutputW = &lcMessagesInfo->YesStr.W;

  if (p32_posix_get_locale_info (&infoRequest, heap, P32_POSIX_LOCALE_INFO_NL_ITEM) != 0) {
    goto fail;
  }

  infoRequest.Info    = NOEXPR;
  infoRequest.OutputA = &lcMessagesInfo->NoExpr.A;
  infoRequest.OutputW = &lcMessagesInfo->NoExpr.W;

  if (p32_posix_get_locale_info (&infoRequest, heap, P32_POSIX_LOCALE_INFO_NL_ITEM) != 0) {
    goto fail;
  }

  infoRequest.Info    = YESEXPR;
  infoRequest.OutputA = &lcMessagesInfo->YesExpr.A;
  infoRequest.OutputW = &lcMessagesInfo->YesExpr.W;

  if (p32_posix_get_locale_info (&infoRequest, heap, P32_POSIX_LOCALE_INFO_NL_ITEM) != 0) {
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
  if (p32_heap_wcsdup (&dest->NoStr.W, heap, src->NoStr.W) == -1) {
    goto fail;
  }
  if (p32_heap_wcsdup (&dest->YesStr.W, heap, src->YesStr.W) == -1) {
    goto fail;
  }
  if (p32_heap_wcsdup (&dest->NoExpr.W, heap, src->NoExpr.W) == -1) {
    goto fail;
  }
  if (p32_heap_wcsdup (&dest->YesExpr.W, heap, src->YesExpr.W) == -1) {
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
  p32_heap_free (heap, 0, lcMessagesInfo->NoStr.W);
  lcMessagesInfo->NoStr.W = NULL;
  p32_heap_free (heap, 0, lcMessagesInfo->YesStr.W);
  lcMessagesInfo->YesStr.W = NULL;
  p32_heap_free (heap, 0, lcMessagesInfo->NoExpr.W);
  lcMessagesInfo->NoExpr.W = NULL;
  p32_heap_free (heap, 0, lcMessagesInfo->YesExpr.W);
  lcMessagesInfo->YesExpr.W = NULL;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcMessagesInfoA (LcMessagesInfo *dest, uintptr_t heap, LcMessagesInfo *src) {
  if (p32_heap_strdup (&dest->NoStr.A, heap, src->NoStr.A) == -1) {
    goto fail;
  }
  if (p32_heap_strdup (&dest->YesStr.A, heap, src->YesStr.A) == -1) {
    goto fail;
  }
  if (p32_heap_strdup (&dest->NoExpr.A, heap, src->NoExpr.A) == -1) {
    goto fail;
  }
  if (p32_heap_strdup (&dest->YesExpr.A, heap, src->YesExpr.A) == -1) {
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
  p32_heap_free (heap, 0, lcMessagesInfo->NoStr.A);
  lcMessagesInfo->NoStr.A = NULL;
  p32_heap_free (heap, 0, lcMessagesInfo->YesStr.A);
  lcMessagesInfo->YesStr.A = NULL;
  p32_heap_free (heap, 0, lcMessagesInfo->NoExpr.A);
  lcMessagesInfo->NoExpr.A = NULL;
  p32_heap_free (heap, 0, lcMessagesInfo->YesExpr.A);
  lcMessagesInfo->YesExpr.A = NULL;
}

/*******************************************************************************
 * External Functions.
 */

bool p32_localeinfo_messages (locale_t locale, uintptr_t heap) {
  Locale         *lcMessages     = &locale->WinLocale.LcMessages;
  LcMessagesInfo *lcMessagesInfo = &locale->LocaleInfo.LcMessages;

  if (!P32LcMessagesInfo (lcMessagesInfo, heap, lcMessages, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"LC_MESSAGES(%s): failed to obtain locale information\n", lcMessages->LocaleName);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail;
  }

  return true;

fail:
  P32FreeLcMessagesInfoA (lcMessagesInfo, heap);
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
