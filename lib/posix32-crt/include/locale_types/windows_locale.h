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
