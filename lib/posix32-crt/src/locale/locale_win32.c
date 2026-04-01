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

/*******************************************************************************
 * Declarations.
 */

/**
 * Functions defined in `locale_win32/locale_id.c`.
 */
#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
/**
 * Wrapper around `GetLocaleInfoW` which used `Locale` object instead of
 * `LCID` object.
 *
 * Return value is the same as for `GetLocaleInfoW`.
 */
static int P32WinlocaleLCIDGetLocaleInfoW (Locale *, uint32_t, wchar_t *, int);

/**
 * Wrapper around `GetCalendarInfoW` which uses `Locale` object instead of
 * `LCID` object.
 *
 * Return value is the same as for `GetCalendarInfoW`.
 */
static int P32WinlocaleLCIDGetCalendarInfoW (Locale *, Calendar, uint32_t, wchar_t *, int, uint32_t *);

/**
 * Implementation for `p32_winlocale_system_default`.
 */
static bool P32WinlocaleLCIDSystemDefault (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_user_default`.
 */
static bool P32WinlocaleLCIDUserDefault (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_resolve`.
 */
static bool P32WinlocaleLCIDResolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap);

/**
 * Implementation for `p32_winlocale_copy`.
 */
static bool P32WinlocaleLCIDCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Implementation for `p32_winlocale_destroy`.
 */
static void P32WinlocaleLCIDDestroy (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_equal`.
 */
static bool P32WinlocaleLCIDEqual (Locale *l1, Locale *l2);
#endif

/**
 * Functions defined in `locale_win32/locale_name.c`.
 */
#if (P32_LOCALE_API & P32_LOCALE_API_LN)
/**
 * Wrapper around `GetLocaleInfoEx` which used `Locale` object instead of
 * Locale Name.
 *
 * Return value is the same as for `GetLocaleInfoEx`.
 */
static int P32WinlocaleLNGetLocaleInfoW (Locale *, uint32_t, wchar_t *, int);

/**
 * Wrapper around `GetCalendarInfoEx` which uses `Locale` object instead of
 * Locale Name.
 *
 * Return value is the same as for `GetCalendarInfoEx`.
 */
static int P32WinlocaleLNGetCalendarInfoW (Locale *, Calendar, uint32_t, wchar_t *, int, uint32_t *);

/**
 * Implementation for `p32_winlocale_system_default`.
 */
static bool P32WinlocaleLNSystemDefault (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_user_default`.
 */
static bool P32WinlocaleLNUserDefault (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_resolve`.
 */
static bool P32WinlocaleLNResolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap);

/**
 * Implementation for `p32_winlocale_copy`.
 */
static bool P32WinlocaleLNCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Implementation for `p32_winlocale_destroy`.
 */
static void P32WinlocaleLNDestroy (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_equal`.
 */
static bool P32WinlocaleLNEqual (Locale *l1, Locale *l2);

/**
 * Implementation for `p32_winlocale_get_language_name`.
 */
static bool P32WinlocaleLNGetLanguageName (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Implementation for `p32_winlocale_get_country_name`.
 */
static bool P32WinlocaleLNGetCountryName (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Implementation for `p32_winlocale_get_language_code`.
 */
static bool P32WinlocaleLNGetLanguageCode (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Implementation for `p32_winlocale_get_country_code`.
 */
static bool P32WinlocaleLNGetCountryCode (wchar_t **address, uintptr_t heap, Locale *locale);
#endif

/**
 * Functions defined in `locale_win32/region_id.c`.
 */
#if (P32_GEO_API & P32_GEO_API_GEOID)
/**
 * Store `GEOID` object in `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleGeo (Locale *locale, uintptr_t heap);

/**
 * Copy `GEOID` object from `srcLocale` to `destLocale`,
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleGeoCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Destroy `GEOID` object stored in `locale`.
 */
static void P32WinlocaleGeoDestroy (Locale *locale, uintptr_t heap);
#endif

/**
 * Functions defined in `locale_win32/region_name.c`.
 */
#if (P32_GEO_API & P32_GEO_API_RN)
/**
 * Store Region Name in `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleRegionName (Locale *locale, uintptr_t heap);

/**
 * Copy Region Name from `srcLocale` to `destLocale`,
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleRegionNameCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Destroy Region Name stored in `locale`.
 */
static void P32WinlocaleRegionNameDestroy (Locale *locale, uintptr_t heap);
#endif

/*******************************************************************************
 * Structures, functions and macros to call appropriate implementation.
 *
 * This segment defines `Winlocale*` macros; these macros redirect to
 * appropriate implementation of specific functions for the current library
 * configuration.
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
static bool P32GetTextualLocaleInfoW (LocaleInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Retrieve locale information as an integer value.
 *
 * Retrieved value is stored in `*request->Output`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetNumericLocaleInfoW (LocaleInfoRequest *request, uintptr_t heap, Locale *locale);

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
static bool P32GetTextualCalendarInfoW (CalendarInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Retrieve calendar information as an integer value.
 *
 * Retrieved value is stored in `*request->Output`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetNumericCalendarInfoW (CalendarInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Get language name for `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetLanguageNameFromLocale (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get country name for `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetCountryNameFromLocale (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get ISO-639 language code for `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetLanguageCodeFromLocale (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get ISO-3166 country code for `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetCountryCodeFromLocale (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Finish construction of `Locale` object `locale`.
 *
 * Store the following information in `locale`:
 *
 * - Default ANSI and OEM code page used by `locale`
 * - Default and alternative calendar used by `locale`
 * - Geological information (`GEOID` or Region Name)
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleInfo (Locale *locale, uintptr_t heap);

#define WinlocaleGetTextualLocaleInfo P32GetTextualLocaleInfoW
#define WinlocaleGetNumericLocaleInfo P32GetNumericLocaleInfoW

#define WinlocaleGetTextualCalendarInfo P32GetTextualCalendarInfoW
#define WinlocaleGetNumericCalendarInfo P32GetNumericCalendarInfoW

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
#define WinlocaleGetLocaleInfoW   P32WinlocaleLCIDGetLocaleInfoW
#define WinlocaleGetCalendarInfoW P32WinlocaleLCIDGetCalendarInfoW
#define WinlocaleGetLanguageName  P32GetLanguageNameFromLocale
#define WinlocaleGetCountryName   P32GetCountryNameFromLocale
#define WinlocaleGetLanguageCode  P32GetLanguageCodeFromLocale
#define WinlocaleGetCountryCode   P32GetCountryCodeFromLocale
#define WinlocaleSystemDefault    P32WinlocaleLCIDSystemDefault
#define WinlocaleUserDefault      P32WinlocaleLCIDUserDefault
#define WinlocaleResolve          P32WinlocaleLCIDResolve
#define WinlocaleCopy             P32WinlocaleLCIDCopy
#define WinlocaleEqual            P32WinlocaleLCIDEqual
#define WinlocaleDestroy          P32WinlocaleLCIDDestroy
#else
#define WinlocaleGetLocaleInfoW   P32WinlocaleLNGetLocaleInfoW
#define WinlocaleGetCalendarInfoW P32WinlocaleLNGetCalendarInfoW
#define WinlocaleGetLanguageName  P32WinlocaleLNGetLanguageName
#define WinlocaleGetCountryName   P32WinlocaleLNGetCountryName
#define WinlocaleGetLanguageCode  P32WinlocaleLNGetLanguageCode
#define WinlocaleGetCountryCode   P32WinlocaleLNGetCountryCode
#define WinlocaleSystemDefault    P32WinlocaleLNSystemDefault
#define WinlocaleUserDefault      P32WinlocaleLNUserDefault
#define WinlocaleResolve          P32WinlocaleLNResolve
#define WinlocaleCopy             P32WinlocaleLNCopy
#define WinlocaleEqual            P32WinlocaleLNEqual
#define WinlocaleDestroy          P32WinlocaleLNDestroy
#endif

#if (P32_GEO_API & P32_GEO_API_GEOID)
#define WinlocaleGeo        P32WinlocaleGeo
#define WinlocaleGeoCopy    P32WinlocaleGeoCopy
#define WinlocaleGeoDestroy P32WinlocaleGeoDestroy
#else
#define WinlocaleGeo        P32WinlocaleRegionName
#define WinlocaleGeoCopy    P32WinlocaleRegionNameCopy
#define WinlocaleGeoDestroy P32WinlocaleRegionNameDestroy
#endif

/*******************************************************************************
 * Functions to obtain locale information.
 */

static bool P32GetTextualLocaleInfoW (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  HANDLE heapHandle = (HANDLE) heap;

  LPWSTR buffer     = NULL;
  INT    bufferSize = 0;

  bufferSize = WinlocaleGetLocaleInfoW (locale, request->Info, NULL, 0);

  if (bufferSize == 0) {
    goto fail;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (WCHAR));

  if (buffer == NULL) {
    goto fail;
  }

  INT written = WinlocaleGetLocaleInfoW (locale, request->Info, buffer, bufferSize);

  if (written != bufferSize) {
    goto fail_free;
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT) {
    CharsetConversionRequest conversionRequest = {0};

    conversionRequest.Flags    |= (P32_CHARSET_CONVERSION_WC_TO_MB);
    conversionRequest.Charset   = request->charset;
    conversionRequest.Input.W   = buffer;
    conversionRequest.Output.A  = request->OutputA;

    if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT_NO_BEST_FIT) {
      conversionRequest.Flags |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
    }

    if (p32_charset_convert (&conversionRequest, heap) == -1) {
      /**
       * We may be allowed to return success if `p32_charset_convert` failed
       * because it is unable to convert retrieved locale information.
       */
      if (conversionRequest.Status != CharsetConversionRequestNoConversion) {
        goto fail_free;
      }

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

static bool P32GetNumericLocaleInfoW (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  return WinlocaleGetLocaleInfoW (locale, LOCALE_RETURN_NUMBER | request->Info, (LPWSTR) request->Output, 2) == 2;
  UNREFERENCED_PARAMETER (heap);
}

static bool P32GetTextualCalendarInfoW (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  HANDLE heapHandle = (HANDLE) heap;

  Calendar calendar = locale->Calendar;

  if (request->Flags & P32_CALENDAR_INFO_REQUEST_ALTERNATIVE) {
    assert (locale->AlternativeCalendar != 0);
    calendar = locale->AlternativeCalendar;
  }

  LPWSTR buffer     = NULL;
  INT    bufferSize = 0;

  bufferSize = WinlocaleGetCalendarInfoW (locale, calendar, request->Info, buffer, bufferSize, NULL);

  if (bufferSize == 0) {
    goto fail;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (WCHAR));

  if (buffer == NULL) {
    goto fail;
  }

  INT written = WinlocaleGetCalendarInfoW (locale, calendar, request->Info, buffer, bufferSize, NULL);

  if (written != bufferSize) {
    goto fail_free;
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT) {
    CharsetConversionRequest conversionRequest = {0};

    conversionRequest.Flags    |= (P32_CHARSET_CONVERSION_WC_TO_MB);
    conversionRequest.Charset   = request->charset;
    conversionRequest.Input.W   = buffer;
    conversionRequest.Output.A  = request->OutputA;

    if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT_NO_BEST_FIT) {
      conversionRequest.Flags |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
    }

    if (p32_charset_convert (&conversionRequest, heap) == -1) {
      /**
       * We may be allowed to return success if `p32_charset_convert` failed
       * because it is unable to convert retrieved locale information.
       */
      if (conversionRequest.Status != CharsetConversionRequestNoConversion) {
        goto fail_free;
      }

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

static bool P32GetNumericCalendarInfoW (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  Calendar calendar = locale->Calendar;

  if (request->Flags & P32_CALENDAR_INFO_REQUEST_ALTERNATIVE) {
    assert (locale->AlternativeCalendar != 0);
    calendar = locale->AlternativeCalendar;
  }

  return WinlocaleGetCalendarInfoW (locale, calendar, CAL_RETURN_NUMBER | request->Info, NULL, 0, request->Output) == 2;
  UNREFERENCED_PARAMETER (heap);
}

static bool P32GetLanguageNameFromLocale (wchar_t **address, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = LOCALE_SENGLANGUAGE;
  infoRequest.OutputW = address;

  return WinlocaleGetTextualLocaleInfo (&infoRequest, heap, locale);
}

static bool P32GetCountryNameFromLocale (wchar_t **address, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = LOCALE_SENGCOUNTRY;
  infoRequest.OutputW = address;

  return WinlocaleGetTextualLocaleInfo (&infoRequest, heap, locale);
}

static bool P32GetLanguageCodeFromLocale (wchar_t **address, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = LOCALE_SISO639LANGNAME;
  infoRequest.OutputW = address;

  return WinlocaleGetTextualLocaleInfo (&infoRequest, heap, locale);
}

static bool P32GetCountryCodeFromLocale (wchar_t **address, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = LOCALE_SISO3166CTRYNAME;
  infoRequest.OutputW = address;

  return WinlocaleGetTextualLocaleInfo (&infoRequest, heap, locale);
}

static bool P32WinlocaleInfo (Locale *locale, uintptr_t heap) {
  uint32_t calendar            = 0;
  uint32_t alternativeCalendar = 0;
  uint32_t ansiCodePage        = 0;
  uint32_t oemCodePage         = 0;

  LocaleInfoRequest infoRequest = {0};

  /**
   * Locale's default calendar.
   */
  infoRequest.Info   = LOCALE_ICALENDARTYPE;
  infoRequest.Output = &calendar;

  if (!WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's alternative calendar.
   *
   * If there is no alternative calendar, then value `(CALID)0` is returned.
   */
  infoRequest.Info   = LOCALE_IOPTIONALCALENDAR;
  infoRequest.Output = &alternativeCalendar;

  if (!WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's default ANSI code page.
   * For an Unicode Locales, ANSI code page is returned as `CP_ACP`.
   */
  infoRequest.Info   = LOCALE_IDEFAULTANSICODEPAGE;
  infoRequest.Output = &ansiCodePage;

  if (!WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's default OEM code page.
   * For an Unicode Locales, OEM code page is returned as `CP_OEMCP`.
   */
  infoRequest.Info   = LOCALE_IDEFAULTCODEPAGE;
  infoRequest.Output = &oemCodePage;

  if (!WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale)) {
    goto fail;
  }

  if (!WinlocaleGeo (locale, heap)) {
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

/*******************************************************************************
 * Implementation.
 */

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
#include "locale_win32/locale_id.c"
#endif

#if (P32_GEO_API & P32_GEO_API_GEOID)
#include "locale_win32/region_id.c"
#endif

#if (P32_LOCALE_API & P32_LOCALE_API_LN)
#include "locale_win32/locale_name.c"
#endif

#if (P32_GEO_API & P32_GEO_API_RN)
#include "locale_win32/region_name.c"
#endif

/*******************************************************************************
 * External Functions.
 */

bool p32_winlocale_system_default (Locale *locale, uintptr_t heap) {
  return WinlocaleSystemDefault (locale, heap);
}

bool p32_winlocale_user_default (Locale *locale, uintptr_t heap) {
  return WinlocaleUserDefault (locale, heap);
}

bool p32_winlocale_resolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap) {
  return WinlocaleResolve (locale, heap, localeMap);
}

bool p32_winlocale_copy (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  return WinlocaleCopy (destLocale, heap, srcLocale);
}

void p32_winlocale_destroy (Locale *locale, uintptr_t heap) {
  WinlocaleDestroy (locale, heap);
}

bool p32_winlocale_equal (Locale *l1, Locale *l2) {
  return WinlocaleEqual (l1, l2);
}

bool p32_winlocale_get_language_name (wchar_t **address, uintptr_t heap, Locale *locale) {
  return WinlocaleGetLanguageName (address, heap, locale);
}

bool p32_winlocale_get_country_name (wchar_t **address, uintptr_t heap, Locale *locale) {
  return WinlocaleGetCountryName (address, heap, locale);
}

bool p32_winlocale_get_language_code (wchar_t **address, uintptr_t heap, Locale *locale) {
  return WinlocaleGetLanguageCode (address, heap, locale);
}

bool p32_winlocale_get_country_code (wchar_t **address, uintptr_t heap, Locale *locale) {
  return WinlocaleGetCountryCode (address, heap, locale);
}

bool p32_winlocale_get_locale_info (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  if (request->Flags & P32_LOCALE_INFO_REQUEST_NUMERIC) {
    return WinlocaleGetNumericLocaleInfo (request, heap, locale);
  }

  return WinlocaleGetTextualLocaleInfo (request, heap, locale);
}

bool p32_winlocale_get_calendar_info (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  if (request->Flags & P32_LOCALE_INFO_REQUEST_NUMERIC) {
    return WinlocaleGetNumericCalendarInfo (request, heap, locale);
  }

  return WinlocaleGetTextualCalendarInfo (request, heap, locale);
}

#ifdef LIBPOSIX32_TEST
bool p32_winlocale_get_ansi_code_page (uint32_t *codePage, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IDEFAULTANSICODEPAGE;
  infoRequest.Output = codePage;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}

bool p32_winlocale_get_oem_code_page (uint32_t *codePage, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IDEFAULTCODEPAGE;
  infoRequest.Output = codePage;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}

bool p32_winlocale_get_mac_code_page (uint32_t *codePage, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IDEFAULTMACCODEPAGE;
  infoRequest.Output = codePage;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}

bool p32_winlocale_get_ebcdic_code_page (uint32_t *codePage, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IDEFAULTEBCDICCODEPAGE;
  infoRequest.Output = codePage;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}
#endif
