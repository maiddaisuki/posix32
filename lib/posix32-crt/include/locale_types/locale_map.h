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

#ifndef LIBPOSIX32_LOCALE_MAP_H_INCLUDED
#define LIBPOSIX32_LOCALE_MAP_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "locale_types/base_language.h"
#include "locale_types/country.h"
#include "locale_types/language.h"
#include "locale_types/modifier.h"
#include "locale_types/script.h"
#include "locale_types/sorting.h"
#include "locale_types/sorting_id.h"
#include "locale_types/sublanguage.h"

/**
 * `LocaleType`.
 */
#include "locale_types/locale_type.h"

/**
 * A known locale.
 */
typedef struct KnownLocale {
  /**
   * Known locale's type.
   */
  LocaleType Type;
  /**
   * Known locale's name.
   */
  wchar_t *LocaleName;
} KnownLocale;

/**
 * Basic information about language.
 *
 * This information is used during locale resolution to construct "default"
 * locale for requested language.
 */
typedef struct BaseLanguage {
  LanguageIndex Language;
  ScriptIndex   Script;
  CountryIndex  Country;
} BaseLanguage;

/**
 * Retrieve `BaseLanguage` structure for `index`.
 */
void p32_base_language (BaseLanguageIndex index, BaseLanguage *baseLanguage);

/**
 * Information about language.
 */
typedef struct Language {
  /**
   * Language name in English (e.g. "English").
   */
  const wchar_t *Name;
  /**
   * ISO 3166 language code (e.g. "en").
   */
  const wchar_t *Code;
#if !P32_LOCALE_NAMES
  /**
   * `LANGID`.
   *
   * This field is set to `(uint16_t)-1` if language cannot be used with
   * `LCID` locales.
   */
  uint16_t LangId;
#endif
} Language;

/**
 * Retrieve `Language` structure for `index`.
 */
void p32_language (LanguageIndex index, Language *language);

#if !P32_LOCALE_NAMES
/**
 * Lookup `LanguageIndex` for primary language ID (`LANGID`) `langId` and
 * language code `ll`.
 *
 * On success, returns valid `LanguageIndex` value.
 * On failure, returns `LanguageIndex_invalid`.
 */
LanguageIndex p32_language_from_id (uint16_t langId, const wchar_t *ll);
#endif

/**
 * Information about script.
 */
typedef struct Script {
  /**
   * Script name as used by Windows locale names (e.g. `Latn`).
   */
  const wchar_t *Name;
} Script;

/**
 * Retrieve `Script` structure for `index`.
 */
void p32_script (ScriptIndex index, Script *script);

/**
 * Information about country.
 */
typedef struct Country {
  /**
   * Country name in english (e.g. "United States").
   */
  const wchar_t *Name;
  /**
   * ISO-639 country codes (e.g. "US").
   */
  const wchar_t *Code;
} Country;

/**
 * Retrieve `Country` structure for `index`.
 */
void p32_country (CountryIndex index, Country *country);

#if P32_LOCALE_NAMES
/**
 * Lookup `CountryIndex` for country code `cc`.
 *
 * On success, returns valid `CountryIndex` value.
 * On failure, returns `CountryIndex_invalid`.
 */
CountryIndex p32_country_by_cc (const wchar_t *cc);
#endif

/**
 * Information about sorting order.
 */
typedef struct Sorting {
  /**
   * Sorting order name as used by Windows locale names (e.g. `radstr`).
   */
  const wchar_t *Name;
} Sorting;

/**
 * Retrieve `Sorting` structure for `index`.
 */
void p32_sorting (SortingIndex index, Sorting *sorting);

#if !P32_LOCALE_NAMES
/**
 * Lookup `SortingIndex` for sorting order ID `sortingId` and language `ll`.
 *
 * On success, returns either
 *
 *  1. `SortingIndex_default`, if `sortingId` is the default sorting order
 *    for language `ll`
 *  2. valid `SortingIndex` value, if `sortingId` is not default sorting order
 *    for language `ll`
 *
 * On failure, returns `SortingIndex_invalid`.
 */
SortingIndex p32_sorting_from_id (uint16_t sortingId, LanguageIndex ll);
#endif

/**
 * Information about locale modifier.
 */
typedef struct Modifier {
  /**
   * Modifier string that follows modifier separator (e.g. "valencia" in
   * locale string "es_CA@valencia").
   */
  const wchar_t *Name;
} Modifier;

/**
 * Retrieve `Modifier` structure for `index`.
 */
void p32_modifier (ModifierIndex index, Modifier *modifier);

#if !P32_LOCALE_NAMES
/**
 * Complete information about some known `LCID` locale.
 */
typedef struct {
  /**
   * Sublanguage ID (`LANGID`) required to construct `LCID` locale.
   */
  uint16_t     SubLangId;
  BaseLanguage Map;
} SubLanguage;

/**
 * Retrieve `SubLanguage` structure for `index`.
 */
void p32_sublanguage (SublanguageIndex index, SubLanguage *subLanguage);

/**
 * Lookup `SublanguageIndex` for sublanguage ID `subLangId` and language `ll`.
 *
 * On success, returns valid `SublanguageIndex` value.
 * On failure, returns `SublanguageIndex_invalid`.
 */
SublanguageIndex p32_sublanguage_from_id (uint16_t subLangId, LanguageIndex ll);
#endif

/**
 * Information required to construct `Locale` object.
 *
 * This structure is filled by `p32_locale_map` function.
 * This structure does not need to be freed.
 *
 * This structure is used by `p32_winlocale_resolve`.
 */
typedef struct LocaleMap {
  KnownLocale   KnownLocale;
  BaseLanguage  Language;
  ScriptIndex   Script;
  CountryIndex  Country;
  SortingIndex  Sorting;
  ModifierIndex Modifier;
  uint32_t      CodePage;
} LocaleMap;

/**
 * Parse locale string `localeString` and fill in `localeMap`.
 *
 * Return `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_locale_map (LocaleMap *localeMap, const wchar_t *localeString, uintptr_t heap);

#endif /* LIBPOSIX32_LOCALE_MAP_H_INCLUDED */
