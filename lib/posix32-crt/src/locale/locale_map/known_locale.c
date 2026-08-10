/**
 * Copyright 2026 Kirill Makurin
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
 * This file contains information about Known Locales.
 */

/**
 * Information about some Known Locale name.
 */
typedef struct KnownLocaleName {
  /**
   * `KnownLocale` structure for this locale.
   */
  KnownLocale KnownLocale;
  /**
   * Language to use when resolving this locale.
   */
  LanguageIndex Language;
  /**
   * Script to use when resolving this locale.
   */
  ScriptIndex Script;
  /**
   * Country to use when resolving this locale.
   */
  CountryIndex Country;
  /**
   * Sorting to use when resolving this locale.
   */
  SortingIndex Sorting;
  /**
   * Modifier required for this locale.
   */
  ModifierIndex Modifier;
} KnownLocaleName;

#define KNOWN_LOCALE(type, acp, ocp, locale, locale_name, index) {type, index, acp, ocp, locale, locale_name}
#define KNOWN_LOCALE_NAME(type, locale, locale_name, index, acp, ocp, ll, ss, cc, xx, mm) \
  {KNOWN_LOCALE (type, acp, ocp, locale, locale_name, index), ll, ss, cc, xx, mm}

/**
 * Known Locale names.
 */
static const KnownLocaleName KnownLocaleNames[] = {
  /* clang-format off */
  KNOWN_LOCALE_NAME (LocaleType_POSIX, L"POSIX", L"en-US", en_US, P32_CODEPAGE_POSIX, P32_CODEPAGE_POSIX, en, -1, US, -1, -1),
  /**
   * Locale name "ca-ES-valencia" does not follow the same format as other
   * locale names, which makes it hard to parse properly.
   */
  KNOWN_LOCALE_NAME (LocaleType_WindowsLocale, L"ca-ES-valencia", L"ca-ES-valencia", ca_ES_valencia, P32_CODEPAGE_ACP, P32_CODEPAGE_OCP, ca, -1, ES_valencia, -1, -1),
  /**
   * Invariant locale "x-IV_mathan".
   *
   * There are two `LCID` locales for this locale:
   *
   * 1. MAKELCID (MAKELANGID (LANG_INVARIANT, SUBLANG_NEUTRAL), SORT_DEFAULT);
   * 2. MAKELCID (MAKELANGID (LANG_INVARIANT, SUBLANG_NEUTRAL), SORT_INVARIANT_MATH);
   *
   * Only the second one has corresponding locale name - "x-IV_mathan".
   *
   * When used with Locale Name NLS APIs, only locale name "x-IV_mathan"
   * can be used; all other combinations are invalid:
   *
   * - "x"
   * - "x-IV"
   * - "iv"
   * - "iv-IV"
   * - "iv-IV_mathan"
   *
   * This makes it impossible to resolve locale name "x-IV_mathan" following
   * resolution logic used for other locale names.
   *
   * Otherwise, "x-IV_mathan" is a regular Windows locale.
   */
  KNOWN_LOCALE_NAME (LocaleType_WindowsLocale, L"x-IV_mathan", L"x-IV_mathan", iv_IV_mathan, P32_CODEPAGE_ACP, P32_CODEPAGE_OCP, iv, -1, IV, mathan, -1),
  /**
   * Windows pseudo locales.
   *
   * They were introduced in Windows Vista.
   *
   * For older Windows versions, which support only `LCID` locales,
   * we emulate Windows pseudo locales using similar locales as substitutes.
   *
   * When we emulate Windows pseudo locales, we have to provide correct default
   * code pages for them; otherwise, we might end up using substitute locales'
   * default code pages.
   */
  KNOWN_LOCALE_NAME (LocaleType_PseudoLocale, L"qps-ploc",  L"en-US", en_US, 1250, 852, qps_ploc,  -1, CountryIndex_ploc,  -1, -1),
  KNOWN_LOCALE_NAME (LocaleType_PseudoLocale, L"qps-ploca", L"ja-JP", ja_JP, 932,  932, qps_ploca, -1, CountryIndex_ploca, -1, -1),
  KNOWN_LOCALE_NAME (LocaleType_PseudoLocale, L"qps-plocm", L"ar-SA", ar_SA, 1256, 720, qps_plocm, -1, CountryIndex_plocm, -1, -1),
  /* clang-format on */
};

void p32_known_locale (KnownLocaleIndex index, KnownLocale *knownLocale) {
  assert (index >= KnownLocaleIndex_MIN && index <= KnownLocaleIndex_MAX);
  *knownLocale = KnownLocaleNames[index].KnownLocale;
}

void p32_known_locale_map (LocaleMap *localeMap, KnownLocaleIndex index) {
  assert (index >= KnownLocaleIndex_MIN && index <= KnownLocaleIndex_MAX);

  localeMap->KnownLocale = index;

  /**
   * Copy information required during locale resolution.
   * This information is also required to properly format locale strings.
   */
  localeMap->Language.Language = KnownLocaleNames[index].Language;
  localeMap->Language.Script   = KnownLocaleNames[index].Script;
  localeMap->Language.Country  = KnownLocaleNames[index].Country;
  localeMap->Script            = ScriptIndex_invalid;
  localeMap->Country           = CountryIndex_invalid;
  localeMap->Sorting           = KnownLocaleNames[index].Sorting;
  localeMap->Modifier          = KnownLocaleNames[index].Modifier;

  /**
   * Default code page to use with locale.
   *
   * For "POSIX" locale, we want to use code page 28591 (ISO-8859-1) as
   * default code page.
   *
   * For Winodws pseudo locales, we want to use their default ANSI code pages.
   */
  if (KnownLocaleNames[index].KnownLocale.Type == LocaleType_POSIX) {
    localeMap->CodePage = KnownLocaleNames[index].KnownLocale.AnsiCodePage;
  } else if (KnownLocaleNames[index].KnownLocale.Type == LocaleType_PseudoLocale) {
#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
    localeMap->CodePage = KnownLocaleNames[index].KnownLocale.AnsiCodePage;
#else
    localeMap->CodePage = P32_CODEPAGE_ACP;
#endif
  }
}
