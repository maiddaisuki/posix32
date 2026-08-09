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

#if (P32_LOCALE_API & P32_LOCALE_API_LN)
#define KNOWN_LOCALE(type, acp, oemcp, locale_string, locale_name) {type, 0, acp, oemcp, locale_string, locale_name}
#else
#define KNOWN_LOCALE(type, acp, oemcp, locale_string, locale_name) {type, 0, acp, oemcp, locale_string}
#endif

#define KNOWN_LOCALE_NAME(type, locale_string, locale_name, acp, oemcp, ll, ss, cc, xx, mm) \
  {KNOWN_LOCALE (type, acp, oemcp, locale_string, locale_name), ll, ss, cc, xx, mm}

/**
 * Known Locale names.
 */
static const KnownLocaleName KnownLocaleNames[] = {
  /* clang-format off */
  KNOWN_LOCALE_NAME (LocaleType_POSIX,        L"POSIX",     L"en-US", P32_CODEPAGE_POSIX, P32_CODEPAGE_POSIX, en, -1, US, -1, -1),
  KNOWN_LOCALE_NAME (LocaleType_PseudoLocale, L"qps-ploc",  L"en-US", 1250,               852,                en, -1, US, -1, -1),
  KNOWN_LOCALE_NAME (LocaleType_PseudoLocale, L"qps-ploca", L"ja-JP", 932,                932,                ja, -1, JP, -1, -1),
  KNOWN_LOCALE_NAME (LocaleType_PseudoLocale, L"qps-plocm", L"ar-SA", 1256,               720,                ar, -1, SA, -1, -1),
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
   * For Winodws pseudo locales, we want to use their default ANSI code pages;
   * we emulate them with `LCID` locales, which means we need to supply
   * code page to use explicitly.
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
