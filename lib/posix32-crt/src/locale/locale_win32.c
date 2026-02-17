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
 * Wrapper around `GetCalendarInfo[Ex]` which uses `Locale` object instead of
 * LCID object or locale name.
 *
 * Return value is the same as for `GetLocaleInfo[Ex]`.
 */
static int P32GetCalendarInfo (Locale *, Calendar, uint32_t, wchar_t *, int, uint32_t *);

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

/*******************************************************************************
 * Functions to obtain locale information.
 */

/**
 * Retrieve locale information as a string.
 *
 * Retrieved string is stored in `*request->OutputW`.
 *
 * If `P32_LOCALE_INFO_REQUEST_CONVERT` flag is set in `request->Flags`, then
 * string converted to `request->CodePage` is stored in `request->OutputA`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetTextualLocaleInfo (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  HANDLE heapHandle = (HANDLE) heap;

  LPWSTR buffer     = NULL;
  INT    bufferSize = 0;

  bufferSize = P32GetLocaleInfo (locale, request->Info, NULL, 0);

  if (bufferSize == 0) {
    goto fail;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (WCHAR));

  if (buffer == NULL) {
    goto fail;
  }

  INT written = P32GetLocaleInfo (locale, request->Info, buffer, bufferSize);

  if (written != bufferSize) {
    goto fail_free;
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT) {
    bool bestFit = !!(request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT_BEST_FIT);

    if (p32_private_wcstombs (request->OutputA, buffer, heap, request->CodePage, bestFit) == -1) {
      /**
       * TODO: we should fallback to locale information used for "POSIX" locale.
       */
      if (request->Flags & (P32_LOCALE_INFO_REQUEST_CONVERT_FALLBACK)) {
        if (p32_private_strdup (request->OutputA, "", heap) == -1) {
          goto fail_free;
        }
      } else if ((request->Flags & (P32_LOCALE_INFO_REQUEST_CONVERT_NO_ERROR)) == 0) {
        goto fail_free;
      }
    }
  }

  *request->OutputW = buffer;

  return true;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return false;
}

/**
 * Retrieve locale information as an integer value.
 *
 * Retrieved value is stored in `*request->Output`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetNumericLocaleInfo (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  return P32GetLocaleInfo (locale, LOCALE_RETURN_NUMBER | request->Info, (LPWSTR) request->Output, 2) == 2;
  UNREFERENCED_PARAMETER (heap);
}

bool p32_winlocale_get_locale_info (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  if (request->Flags & P32_LOCALE_INFO_REQUEST_NUMERIC) {
    return P32GetNumericLocaleInfo (request, heap, locale);
  }

  return P32GetTextualLocaleInfo (request, heap, locale);
}

/**
 * Retrieve calendar information as a string.
 *
 * Retrieved string is stored in `*request->OutputW`.
 *
 * If `P32_LOCALE_INFO_REQUEST_CONVERT` flag is set in `request->Flags`, then
 * string converted to `request->CodePage` is stored in `request->OutputA`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetTextualCalendarInfo (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  HANDLE heapHandle = (HANDLE) heap;

  Calendar calendar = locale->Calendar;

  if (request->Flags & P32_CALENDAR_INFO_REQUEST_ALTERNATIVE) {
    assert (locale->AlternativeCalendar != 0);
    calendar = locale->AlternativeCalendar;
  }

  LPWSTR buffer     = NULL;
  INT    bufferSize = 0;

  bufferSize = P32GetCalendarInfo (locale, calendar, request->Info, buffer, bufferSize, NULL);

  if (bufferSize == 0) {
    goto fail;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (WCHAR));

  if (buffer == NULL) {
    goto fail;
  }

  INT written = P32GetCalendarInfo (locale, calendar, request->Info, buffer, bufferSize, NULL);

  if (written != bufferSize) {
    goto fail_free;
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT) {
    bool bestFit = !!(request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT_BEST_FIT);

    if (p32_private_wcstombs (request->OutputA, buffer, heap, request->CodePage, bestFit) == -1) {
      /**
       * TODO: we should fallback to locale information used for "POSIX" locale.
       */
      if (request->Flags & (P32_LOCALE_INFO_REQUEST_CONVERT_FALLBACK)) {
        if (p32_private_strdup (request->OutputA, "", heap) == -1) {
          goto fail_free;
        }
      } else if ((request->Flags & (P32_LOCALE_INFO_REQUEST_CONVERT_NO_ERROR)) == 0) {
        goto fail_free;
      }
    }
  }

  *request->OutputW = buffer;

  return true;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return false;
}

/**
 * Retrieve calendar information as an integer value.
 *
 * Retrieved value is stored in `*request->Output`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetNumericCalendarInfo (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  Calendar calendar = locale->Calendar;

  if (request->Flags & P32_CALENDAR_INFO_REQUEST_ALTERNATIVE) {
    assert (locale->AlternativeCalendar != 0);
    calendar = locale->AlternativeCalendar;
  }

  return P32GetCalendarInfo (locale, calendar, CAL_RETURN_NUMBER | request->Info, NULL, 0, request->Output) == 2;
  UNREFERENCED_PARAMETER (heap);
}

bool p32_winlocale_get_calendar_info (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  if (request->Flags & P32_LOCALE_INFO_REQUEST_NUMERIC) {
    return P32GetNumericCalendarInfo (request, heap, locale);
  }

  return P32GetTextualCalendarInfo (request, heap, locale);
}

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

  LocaleInfoRequest infoRequest = {0};
  infoRequest.Flags             = (P32_LOCALE_INFO_REQUEST_NUMERIC);

  /**
   * Locale's default calendar.
   */
  infoRequest.Info   = LOCALE_ICALENDARTYPE;
  infoRequest.Output = &calendar;

  if (!p32_winlocale_get_locale_info (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's alternative calendar.
   *
   * If there is no alternative calendar, then value `(CALID)0` is returned.
   */
  infoRequest.Info   = LOCALE_IOPTIONALCALENDAR;
  infoRequest.Output = &alternativeCalendar;

  if (!p32_winlocale_get_locale_info (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's default ANSI code page.
   * For an Unicode Locales, ANSI code page is returned as `CP_ACP`.
   */
  infoRequest.Info   = LOCALE_IDEFAULTANSICODEPAGE;
  infoRequest.Output = &ansiCodePage;

  if (!p32_winlocale_get_locale_info (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's default OEM code page.
   * For an Unicode Locales, OEM code page is returned as `CP_OEMCP`.
   */
  infoRequest.Info   = LOCALE_IDEFAULTCODEPAGE;
  infoRequest.Output = &oemCodePage;

  if (!p32_winlocale_get_locale_info (&infoRequest, heap, locale)) {
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
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = info;
  infoRequest.OutputW = address;

  return p32_winlocale_get_locale_info (&infoRequest, heap, locale);
}

bool p32_winlocale_getinfo_number (uint32_t *address, Locale *locale, uint32_t info) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = info;
  infoRequest.Flags  = P32_LOCALE_INFO_REQUEST_NUMERIC;
  infoRequest.Output = address;

  return p32_winlocale_get_locale_info (&infoRequest, 0, locale);
}
