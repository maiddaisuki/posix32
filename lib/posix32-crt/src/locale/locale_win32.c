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
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions which manipulate `Locale` objects.
 * These objects are used to represent Windows locales.
 *
 * Windows provides two different sets of NLS APIs:
 *
 * 1. Locale IDs (`LCID`). This is the older mechanism to represent locales
 * which was used prior to Windows Vista.
 *
 * 2. Locale names. This is a newer mechanism which was intoduced in
 * Windows Vista.
 *
 * While locale names are preferred over `LCID` locales, locale name APIs were
 * only introduced in Windows Vista. Support for locale names in CRT was
 * first added in msvcr110.dll.
 *
 * If we're building for CRT which does not support locale names, we always
 * use `LCID` locales regardless of targeted Windows version.
 *
 * If we're targeting Windows version older than Windows Vista, we will always
 * use `LCID` locales regardless of CRT.
 */

/**
 * Wrapper around `GetLocaleInfo[Ex]` which used `Locale` object instead of
 * LCID object or locale name.
 *
 * Return value is the same as for `GetLocaleInfo[Ex]`.
 */
static int P32GetLocaleInfo (Locale *locale, uint32_t info, wchar_t *buffer, int bufferSize);

/**
 * Store geological infromation in `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32Geo (Locale *locale, uintptr_t heap);

/**
 * Copy geological information from `srcLocale` to `destLocale`,
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GeoDuplicate (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Destroy geological information stored in `locale`.
 */
static void P32GeoDestroy (Locale *locale, uintptr_t heap);

/**
 * Fill `locale` with information about locale it represents.
 *
 * Currently this information includes:
 *
 * - Default and alternative calendar used by locale
 * - Default ANSI and OEM code page used by locale
 * - Geological information
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32FillLocaleInfo (Locale *locale, uintptr_t heap);

#if P32_REGION_NAMES
#include "locale_win32/region_name.c"
#else
#include "locale_win32/region_id.c"
#endif

#if P32_LOCALE_NAMES
#include "locale_win32/locale_name.c"
#else
#include "locale_win32/locale_id.c"
#endif

static bool P32FillLocaleInfo (Locale *locale, uintptr_t heap) {
  uint32_t calendar            = 0;
  uint32_t alternativeCalendar = 0;
  uint32_t ansiCodePage        = 0;
  uint32_t oemCodePage         = 0;

  if (!p32_winlocale_getinfo_number (&calendar, locale, LOCALE_ICALENDARTYPE)) {
    goto fail;
  }
  if (!p32_winlocale_getinfo_number (&alternativeCalendar, locale, LOCALE_IOPTIONALCALENDAR)) {
    goto fail;
  }
  if (!p32_winlocale_getinfo_number (&ansiCodePage, locale, LOCALE_IDEFAULTANSICODEPAGE)) {
    goto fail;
  }
  if (!p32_winlocale_getinfo_number (&oemCodePage, locale, LOCALE_IDEFAULTCODEPAGE)) {
    goto fail;
  }

  if (!P32Geo (locale, heap)) {
    goto fail;
  }

  locale->Calendar            = calendar;
  locale->AlternativeCalendar = alternativeCalendar;
  locale->CodePage.Ansi       = ansiCodePage;
  locale->CodePage.Oem        = oemCodePage;

  return true;

fail:
  return false;
}

bool p32_winlocale_getinfo (wchar_t **address, uintptr_t heap, Locale *locale, uint32_t info) {
  HANDLE heapHandle = (HANDLE) heap;

  wchar_t *buffer     = NULL;
  int      bufferSize = 0;

  bufferSize = P32GetLocaleInfo (locale, info, buffer, bufferSize);

  if (bufferSize == 0) {
    goto fail;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    goto fail;
  }

  int written = P32GetLocaleInfo (locale, info, buffer, bufferSize);

  if (written == 0 || written > bufferSize) {
    goto fail_free;
  }

  *address = buffer;

  return true;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return false;
}

bool p32_winlocale_getinfo_number (uint32_t *address, Locale *locale, uint32_t info) {
  int written = P32GetLocaleInfo (locale, info | LOCALE_RETURN_NUMBER, (LPWSTR) address, 2);

  assert (written == 2);

  return written == 2;
}
