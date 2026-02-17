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

#ifndef LIBPOSIX32_WINDOWS_LOCALE_H_INCLUDED
#define LIBPOSIX32_WINDOWS_LOCALE_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "locale_types/locale_map.h"

/**
 * CALID values.
 */
typedef enum Calendar {
  Calendar_ArabicLunar_Hijri = CAL_HIJRI,
#ifdef CAL_UMALQURA
  Calendar_ArabicLunar_UmAlQura = CAL_UMALQURA,
#endif
  Calendar_Gregorian                   = CAL_GREGORIAN,
  Calendar_Gregorian_Arabic            = CAL_GREGORIAN_ARABIC,
  Calendar_Gregorian_MiddleEast_French = CAL_GREGORIAN_ME_FRENCH,
  Calendar_Gregorian_Translit_English  = CAL_GREGORIAN_XLIT_ENGLISH,
  Calendar_Gregorian_Translit_French   = CAL_GREGORIAN_XLIT_FRENCH,
  Calendar_Gregorian_UnitedStates      = CAL_GREGORIAN_US,
  Calendar_JapaneseEmperorEra          = CAL_JAPAN,
  Calendar_KoreanTangunEra             = CAL_KOREA,
  Calendar_Lunar_Hebrew                = CAL_HEBREW,
  Calendar_Taiwan                      = CAL_TAIWAN,
  Calendar_Thai                        = CAL_THAI,
} Calendar;

/**
 * Represents Windows locale object.
 */
typedef struct Locale {
  LocaleType Type;
  /**
   * Locale name
   */
  wchar_t *LocaleName;
#if !P32_LOCALE_NAMES
  /**
   * LCID
   */
  uint32_t LocaleId;
#endif
#if P32_REGION_NAMES
  /**
   * Country/Region name
   */
  wchar_t *GeoName;
#else
  /**
   * GEOID
   */
  int32_t GeoId;
#endif
  /**
   * Locale's default calendar.
   */
  Calendar Calendar;
  /**
   * Locale's alternative calendar, if any.
   */
  Calendar AlternativeCalendar;
  struct {
    /**
     * Locale's default ANSI code page.
     */
    uint32_t Ansi;
    /**
     * Locale's default OEM code page.
     */
    uint32_t Oem;
  } CodePage;
  struct {
    LanguageIndex Language;
    ScriptIndex   Script;
    CountryIndex  Country;
    ModifierIndex Modifier;
  } Map;
} Locale;

/**
 * Locale's default code page as used by CRT.
 *
 * crtdll.dll and msvcrt10.dll use locale's default OEM code page.
 * All later CRTs use locale's default ANSI code page.
 */
#if P32_CRT >= P32_MSVCRT20
#define P32_LOCALE_CODEPAGE(l) ((l).CodePage.Ansi)
#else
#define P32_LOCALE_CODEPAGE(l) ((l).CodePage.Oem)
#endif

/**
 * `Locale` objects for each locale category.
 */
typedef struct WindowsLocale {
  Locale LcCollate;
  Locale LcCtype;
  Locale LcMessages;
  Locale LcMonetary;
  Locale LcNumeric;
  Locale LcTime;
} WindowsLocale;

#if P32_CRT >= P32_MSVCR80 && defined(LIBPOSIX32_TEST)
/**
 * `_locale_t` objects for each locale category supported by CRT.
 *
 * NOTE: posix32 no longer uses `_locale_t`. Code which attempts to create
 * `_locale_t` objects is kept to aid testing the library.
 */
typedef struct CrtLocale {
  _locale_t LcCollate;
  _locale_t LcCtype;
  /**
   * CRT does not support `LC_MESSAGES` locale category.
   */
  /* _locale_t LcMessages; */
  _locale_t LcMonetary;
  _locale_t LcNumeric;
  _locale_t LcTime;
} CrtLocale;
#endif

#if defined(LIBPOSIX32_TEST) && !P32_LOCALE_NAMES
/**
 * Attempt to construct `Locale` object from existing valid `localeId`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_from_lcid (Locale *locale, uintptr_t heap, uint32_t localeId);
#endif

/**
 * Construct `Locale` object corresponding to System Default Locale.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_system_default (Locale *locale, uintptr_t heap);

/**
 * Construct `Locale` object corresponding to User Default Locale.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_user_default (Locale *locale, uintptr_t heap);

/**
 * Construct best-fitting `Locale` object from `localeMap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_resolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap);

/**
 * Create copy of `oldLocale` and store it in `newLocale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_copy (Locale *newLocale, uintptr_t heap, Locale *oldLocale);

/**
 * Destroy `Locale` object `locale`.
 */
P32_TEST_DECL void p32_winlocale_destroy (Locale *locale, uintptr_t heap);

/**
 * Return `true` if `l1` and `l2` are the same locale.
 */
P32_TEST_DECL bool p32_winlocale_equal (Locale *l1, Locale *l2);

/**
 * Retrieve locale information as an `uint32_t` value instead of a string.
 *
 * This flag can be set only when retrieving locale information using
 * `LOCALE_I*` and `CAL_I*` constants.
 */
#define P32_LOCALE_INFO_REQUEST_NUMERIC          (1)
/**
 * Convert locale information.
 */
#define P32_LOCALE_INFO_REQUEST_CONVERT          (1 << 8)
/**
 * Allow best fit conversion of locale infromation.
 *
 * TODO: this flag should be removed once `p32_private_wcstombs` is refactored
 *   to have interface similar to `p32_winlocale_get_locale_info`.
 */
#define P32_LOCALE_INFO_REQUEST_CONVERT_BEST_FIT (1 << 9)
/**
 * If locale information cannot be converted, fallback to some default value.
 *
 * TODO: we should fallback to locale information used for "POSIX" locale;
 *   currently, we simply store an empty string.
 */
#define P32_LOCALE_INFO_REQUEST_CONVERT_FALLBACK (1 << 10)
/**
 * Do not fail if locale information cannot be converted.
 */
#define P32_LOCALE_INFO_REQUEST_CONVERT_NO_ERROR (1 << 11)

/**
 * Structure used with `p32_winlocale_get_locale_info` function.
 */
typedef struct LocaleInfoRequest {
  /**
   * Locale information to retrieve.
   * This must be one of `LOCALE_*` constants.
   */
  uint32_t Info;
  /**
   * Any combination of `P32_LOCALE_INFO_REQUEST_*` flags defined above.
   */
  uint32_t Flags;
  /**
   * Code page to use when converting retrieved locale information.
   *
   * When `P32_LOCALE_INFO_REQUEST_CONVERT` flag is set, this field must be set
   * to a valid code page. Otherwise, this field is ignored.
   */
  uint32_t CodePage;
  /**
   * The output field when `P32_LOCALE_INFO_REQUEST_NUMERIC` flag is set.
   */
  uint32_t *Output;
  /**
   * The output field when `P32_LOCALE_INFO_REQUEST_CONVERT` flag is set.
   *
   * `*OutputA` will recieve address of the buffer allocated to hold
   * retrieved locale information.
   */
  char **OutputA;
  /**
   * The output field unless `P32_LOCALE_INFO_REQUEST_NUMERIC` flag is set.
   *
   * `*OutputW` will recieve address of the buffer allocated to hold
   * retrieved locale information.
   */
  wchar_t **OutputW;
} LocaleInfoRequest;

/**
 * Retrieve locale information for `locale`.
 *
 * The `request` argument must point to a properly initialized
 * `LocaleInfoRequest` structure.
 *
 * Buffers to store retrieved locale information are allocated from `heap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_get_locale_info (LocaleInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Use locale's default calendar (default).
 */
#define P32_CALENDAR_INFO_REQUEST_DEFAULT     (1 << 24)
/**
 * Use locale's alternative calendar.
 */
#define P32_CALENDAR_INFO_REQUEST_ALTERNATIVE (1 << 25)

/**
 * Structure used with `p32_winlocale_get_calendar_info` function.
 *
 * Unless mentioned, this structure is equivalent to `LocaleInfoRequest`.
 */
typedef struct CalendarInfoRequest {
  /**
   * Calendar information to retrieve.
   * This must be one of `CAL_*` constants.
   */
  uint32_t Info;
  /**
   * All `P32_CALENDAR_INFO_REQUEST_*` flags can be used in addition
   * to all `P32_LOCALE_INFO_REQUEST_*` flags accepted by `LocaleInfoRequest`.
   */
  uint32_t  Flags;
  uint32_t  CodePage;
  uint32_t *Output;
  char    **OutputA;
  wchar_t **OutputW;
} CalendarInfoRequest;

/**
 * Retrieve calendar information for `locale`.
 *
 * The `request` argument must point to a properly initialized
 * `CalendarInfoRequest` structure.
 *
 * By default, information for `locale->Calendar` is retrieved.
 *
 * If `P32_CALENDAR_INFO_REQUEST_ALTERNATIVE` flag is set in `request->Flags`,
 * then information for `locale->AlternativeCalendar` is retrieved instead.
 *
 * Buffers to store retrieved calendar information are allocated from `heap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_get_calendar_info (CalendarInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Get language name for `locale`.
 *
 * Buffer to store retrieved string is allocated from `heap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_get_language_name (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get country name for `locale`.
 *
 * Buffer to store retrieved string is allocated from `heap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_get_country_name (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get ISO-639 language code for `locale`.
 *
 * Buffer to store retrieved string is allocated from `heap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_get_language_code (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get ISO-3166 country code for `locale`.
 *
 * Buffer to store retrieved string is allocated from `heap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_get_country_code (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Wrapper around `GetLocaleInfo[Ex]` which uses `Locale` object instead of
 * `LCID` or `Locale Name`.
 *
 * Buffer to store retrieved information is allocated from `heap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_getinfo (wchar_t **address, uintptr_t heap, Locale *locale, uint32_t info);

/**
 * Wrapper around `GetLocaleInfo[Ex]` which used `Locale` object instead of
 * LCID object or locale name.
 *
 * This is a convenience function to obtain locale information with LOCALE_I*
 * constants which return information as an integer value.
 *
 * Retrieved value is stored in `*address`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_winlocale_getinfo_number (uint32_t *address, Locale *locale, uint32_t info);

#endif /* LIBPOSIX32_WINDOWS_LOCALE_H_INCLUDED */
