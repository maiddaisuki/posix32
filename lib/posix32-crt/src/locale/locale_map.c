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
#include <ctype.h> /* iswascii */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h> /* _countof */
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This functions defines functions to parse locale strings.
 * They construct `LocaleMap` objects used during locale resolution.
 */

/*******************************************************************************
 * Locale Names
 *
 * Locale string may specify locale names such as "C" and "POSIX" instead of
 * explicit locales like "en-US".
 *
 * In addition to "C" and "POSIX" locales, we also support Windows pseudo
 * locales, which were introduced in Windows Vista.
 */

#define KNOWN_LOCALE(locale, type) {type, locale}
#define BASE_LANGUAGE(ll, ss, cc)  {ll, ss, cc}
#define LOCALE_MAP(type, locale, ll, ss, cc, cp)                                \
  {KNOWN_LOCALE (type, locale), BASE_LANGUAGE (ll, ss, cc), -1, -1, -1, -1, cp}

#if P32_LOCALE_NAMES
#define PSEUDO_LOCALE_CODEPAGE(cp) P32_CODEPAGE_ACP
#else
#define PSEUDO_LOCALE_CODEPAGE(cp) cp
#endif

/**
 * Known locale names.
 */
static const LocaleMap LocaleNames[] = {
  /* clang-format off */
  LOCALE_MAP (L"C",         LOCALE_TYPE_POSIX,             en, -1, US, P32_CODEPAGE_POSIX),
  LOCALE_MAP (L"POSIX",     LOCALE_TYPE_POSIX,             en, -1, US, P32_CODEPAGE_POSIX),
  LOCALE_MAP (L"qps-ploc",  LOCALE_TYPE_PSEUDO_LOCALE, en, -1, US, PSEUDO_LOCALE_CODEPAGE (1250)),
  LOCALE_MAP (L"qps-ploca", LOCALE_TYPE_PSEUDO_LOCALE, ja, -1, JP, PSEUDO_LOCALE_CODEPAGE (932)),
  LOCALE_MAP (L"qps-plocm", LOCALE_TYPE_PSEUDO_LOCALE, ar, -1, SA, PSEUDO_LOCALE_CODEPAGE (1256)),
  /* clang-format on */
};

/**
 * Check if `localeString` is a known locale name.
 */
static bool P32LocaleName (const wchar_t *localeString, LocaleMap *localeMap) {
  for (size_t i = 0; i < _countof (LocaleNames); ++i) {
    if (wcscmp (localeString, LocaleNames[i].KnownLocale.LocaleName) == 0) {
      *localeMap = LocaleNames[i];
      return true;
    }
  }

  return false;
}

/*******************************************************************************
 * Locale String Normalization
 *
 * The parser requires that locale string contains only ASCII characters.
 * This is also important since we use gperf-generated lookup functions to
 * construct `LocaleMap`.
 *
 * Unfortunately, for some locales, `GetLocaleInfo[Ex]` returns a
 * language name and/or country name that contain non-ascii characters.
 *
 * In addition, some language names and country names contain characters
 * (in particular '.') which may confuse the parser.
 *
 * To work around this, we normalize the locale string so it can be parsed
 * and be used to perform lookups using gperf-generated functions.
 *
 * For string which contain non-ascii characters, we simply substitute them
 * with a reasonable ASCII character.
 */

typedef struct ReplacementPair {
  const wchar_t *const String;
  const size_t         StringLength;
  const wchar_t *const Replacament;
  const size_t         ReplacamentLength;
} ReplacementPair;

/**
 * Problematic language names and replacements for them.
 */
static const ReplacementPair Languages[] = {
  {L"Kʼicheʼ",          7,  L"K'iche'",          7 },
  {L"Māori",            5,  L"Maori",            5 },
  {L"Metaʼ",            5,  L"Meta'",            5 },
  {L"Norwegian Bokmål", 16, L"Norwegian Bokmal", 16},
  {L"Volapük",          7,  L"Volapuk",          7 },
  /**
   * We also handle it here...
   */
  {L"ca-ES-valencia",   14, L"Valencian",        9 }
};

/**
 * Problematic country names and replacements for them.
 */
static const ReplacementPair Countries[] = {
  {L"Åland Islands",                          13, L"Aland Islands",                             13},
  {L"Côte d’Ivoire",                          13, L"Cote d'Ivoire",                             13},
  {L"Curaçao",                                7,  L"Curacao",                                   7 },
  {L"Réunion",                                7,  L"Reunion",                                   7 },
  {L"São Tomé & Príncipe",                    19, L"Sao Tome & Principe",                       19},
  {L"St. Barthélemy",                         14, L"Saint Barthelemy",                          16},
  {L"Türkiye",                                7,  L"Turkiye",                                   7 },
  /**
   * Following country names do not require character set tricks,
   * but need to modified before parsing them.
   */
  {L"St Helena, Ascension, Tristan da Cunha", 38, L"Saint Helena, Ascension, Tristan da Cunha", 41},
  {L"St. Kitts & Nevis",                      17, L"Saint Kitts & Nevis",                       19},
  {L"St. Lucia",                              9,  L"Saint Lucia",                               11},
  {L"St. Martin",                             10, L"Saint Martin",                              12},
  {L"St. Pierre & Miquelon",                  21, L"Saint Pierre & Miquelon",                   23},
  {L"St. Vincent & Grenadines",               24, L"Saint Vincent & Grenadines",                26},
  {L"U.S. Outlying Islands",                  21, L"US Outlying Islands",                       19},
  {L"U.S. Virgin Islands",                    19, L"US Virgin Islands",                         17},
};

/**
 * This structure contains information used by `P32NormalizeLocaleString`
 * to perform modification of locale string.
 */
typedef struct ReplacementMap {
  /* Input fields */
  struct {
    /* String to be modified */
    const wchar_t *String;
    /* Length of the string */
    size_t Length;
    /* Length of language name in the string */
    size_t LanguageLength;
    /* Length of country name in the string */
    size_t CountryLength;
    /* Position of country name in the string */
    size_t CountryOffset;
  } I;

  /* Output fields */
  struct {
    /* Modified string */
    wchar_t *String;
    /* Length of the string */
    size_t Length;
    /* Language string to insert into the string (if non-NULL) */
    const wchar_t *Language;
    /* Length of the language string */
    size_t LanguageLength;
    /* Country string to insert into the string (if non-NULL) */
    const wchar_t *Country;
    /* Length of the country string */
    size_t CountryLength;
  } O;
} ReplacementMap;

/**
 * Normalize locale string `localeString`.
 *
 * If normalization is perfomed, normalized string is stored in `*address`.
 * Buffer to store normalized string is allocated from `heap`.
 *
 * Returns `true` on success, and `false` otherwise.
 *
 * This function also returns `true` if normalization was not required,
 * in which case `*address` is kept intact.
 */
static bool P32NormalizeLocaleString (wchar_t **address, uintptr_t heap, const wchar_t *localeString) {
  ReplacementMap map = {0};

  map.I.String = localeString;
  map.I.Length = wcslen (localeString);

  /**
   * Check if language string needs to be replaced
   */
  for (size_t i = 0; i < _countof (Languages); ++i) {
    if (!_wcsnicmp (map.I.String, Languages[i].String, Languages[i].StringLength)) {
      map.O.Language       = Languages[i].Replacament;
      map.O.LanguageLength = Languages[i].ReplacamentLength;
      map.I.LanguageLength = Languages[i].StringLength;
      break;
    }
  }

  wchar_t *separator = wcschr (map.I.String, L'_');

  /**
   * Invalid locale string.
   */
  if (separator == map.I.String) {
    return false;
  }

  /**
   * Check if country string needs to be replaced
   */
  if (separator != NULL) {
    wchar_t *countryPart = separator + 1;

    for (size_t i = 0; i < _countof (Countries); ++i) {
      if (!_wcsnicmp (countryPart, Countries[i].String, Countries[i].StringLength)) {
        map.O.Country       = Countries[i].Replacament;
        map.O.CountryLength = Countries[i].ReplacamentLength;
        map.I.CountryOffset = countryPart - map.I.String;
        map.I.CountryLength = Countries[i].StringLength;
        break;
      }
    }
  }

  /**
   * No modifications needed.
   */
  if (map.O.Country == NULL && map.O.Language == NULL) {
    return true;
  }

  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Difference, in characters, between language/country name in
   * the input string and in the replacement string
   */
  ptrdiff_t languageLengthAdjustment = 0;
  ptrdiff_t countryLengthAdjustment  = 0;

  if (map.O.LanguageLength != 0) {
    languageLengthAdjustment = map.O.LanguageLength - map.I.LanguageLength;
  }

  if (map.O.CountryLength != 0) {
    countryLengthAdjustment = map.O.CountryLength - map.I.CountryLength;
  }

  map.O.Length = map.I.Length + languageLengthAdjustment + countryLengthAdjustment;
  map.O.String = (wchar_t *) HeapAlloc (heapHandle, 0, (map.O.Length + 1) * sizeof (wchar_t));

  if (map.O.String == NULL) {
    return false;
  }

  wchar_t *newString = map.O.String;

  /**
   * If language string needs to be replaced, copy replacement string into
   * the output buffer.
   *
   * Otherwise, copy everything up to country string into the output buffer.
   */
  if (map.O.Language != NULL) {
    wmemcpy (newString, map.O.Language, map.O.LanguageLength);
    newString += map.O.LanguageLength;
  } else {
    wmemcpy (newString, map.I.String, map.I.CountryOffset);
    newString += map.I.CountryOffset;
  }

  /**
   * If both language and country strings need to be replaced, copy content
   * of the string between them into the output buffer.
   */
  if (map.O.Language != NULL && map.O.Country != NULL) {
    size_t length = map.I.CountryOffset - map.I.LanguageLength;
    assert (length != 0);

    wmemcpy (newString, map.I.String + map.I.LanguageLength, length);
    newString += length;
  }

  /**
   * If country string needs to be replaced, copy replacement string into the
   * output buffer.
   *
   * Otherwise, do nothing.
   */
  if (map.O.Country != NULL) {
    wmemcpy (newString, map.O.Country, map.O.CountryLength);
    newString += map.O.CountryLength;
  }

  /**
   * If string contains anything after last replaced part, copy it into the
   * output buffer.
   */
  size_t tailOffset = 0;
  size_t tailLength = 0;

  if (map.I.CountryOffset != 0) {
    tailOffset = map.I.CountryOffset + map.I.CountryLength;
  } else {
    tailOffset = map.I.LanguageLength;
  }

  tailLength = map.I.Length - tailOffset;

  if (tailLength > 0) {
    wmemcpy (newString, map.I.String + tailOffset, tailLength);
    newString += tailLength;
  }

  *newString = L'\0';
  *address   = map.O.String;

  return true;
}

/*******************************************************************************
 * Locale String Parser
 *
 * See `locale_map/locale_parser.c` for details.
 */

/**
 * Structure to represent split locale string.
 *
 * Only `Language` member is allocated, which points to beginning of the
 * locale string.
 *
 * The '\0' is inserted into `Language` buffer to separate parts of the string
 * and other non-NULL fields point to corresponding part of the string.
 */
typedef struct LocaleStringMap {
  char *Language;
  char *Script;
  char *Country;
  char *Sorting;
  char *Charset;
  char *Modifier;
} LocaleStringMap;

/**
 * Convert wide character string to ASCII.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WcsToAscii (char *dest, const wchar_t *src, size_t length) {
  for (size_t i = 0; i <= length; ++i) {
    if (!iswascii (src[i])) {
      return false;
    }

    dest[i] = (char) src[i];
  }

  return true;
}

/**
 * Lookup `BaseLanguageIndex` for language `info->String`.
 *
 * If `singleLookup` is `true`, this function only attempts to lookup
 * `info->String` as language code.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32LookupLanguage (LanguageInfo *info, bool singleLookup) {
  if (p32_lookup_language_code (info)) {
    return true;
  }

  if (singleLookup) {
    return false;
  }

  return p32_lookup_language_name (info);
}

/**
 * Lookup `CountryIndex` for country `info->String`.
 *
 * If `singleLookup` is `true`, this function only attempts to lookup
 * `info->String` as country code.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32LookupCountry (CountryInfo *info, bool singleLookup) {
  if (p32_lookup_country_code (info)) {
    return true;
  }

  if (singleLookup) {
    return false;
  }

  return p32_lookup_country_name (info);
}

/**
 * Parse `stringMap->Modifier` and apply it to `localeMap`.
 */
static void P32Modifier (LocaleMap *localeMap, LocaleStringMap *stringMap) {
  localeMap->Modifier = ModifierIndex_invalid;

  /**
   * No modifier in the locale string.
   */
  if (stringMap->Modifier == NULL) {
    return;
  }

  /**
   * Check if `stringMap->Modifier` is an actual modifier.
   */
  ModifierInfo info = {0};

  info.String = stringMap->Modifier;
  info.Length = strlen (info.String);

  if (p32_lookup_modifier (&info)) {
    localeMap->Modifier = info.Modifier;
    return;
  }

  /**
   * Check if `stringMap->Modifier` is a script name.
   */
  if (localeMap->Script == ScriptIndex_invalid) {
    ScriptInfo scriptInfo = {0};

    scriptInfo.String = stringMap->Modifier;
    scriptInfo.Length = strlen (info.String);

    if (p32_lookup_script (&scriptInfo)) {
      localeMap->Script = scriptInfo.Script;
      return;
    }
  }

  /**
   * Check if `stringMap->Modifier` is a sorting order name.
   */
  if (localeMap->Sorting == SortingIndex_invalid) {
    SortingInfo sortingInfo = {0};

    sortingInfo.String = stringMap->Modifier;
    sortingInfo.Length = strlen (info.String);

    if (p32_lookup_sorting (&sortingInfo)) {
      localeMap->Sorting = sortingInfo.Sorting;
      return;
    }
  }
}

/**
 * Construct `LocaleMap` object from `stringMap`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32LocaleMap (LocaleMap *localeMap, LocaleStringMap *stringMap, bool singleLookup) {
  assert (stringMap->Language != NULL);

  LanguageInfo languageInfo = {0};

  languageInfo.String = stringMap->Language;
  languageInfo.Length = strlen (languageInfo.String);

  if (!P32LookupLanguage (&languageInfo, singleLookup)) {
    return false;
  }

  p32_base_language (languageInfo.BaseLanguage, &localeMap->Language);

  if (stringMap->Script != NULL) {
    ScriptInfo scriptInfo = {0};

    scriptInfo.String = stringMap->Script;
    scriptInfo.Length = strlen (scriptInfo.String);

    if (!p32_lookup_script (&scriptInfo)) {
      return false;
    }

    localeMap->Script = scriptInfo.Script;
  } else {
    localeMap->Script = ScriptIndex_invalid;
  }

  if (stringMap->Country != NULL) {
    CountryInfo countryInfo = {0};

    countryInfo.String = stringMap->Country;
    countryInfo.Length = strlen (countryInfo.String);

    if (!P32LookupCountry (&countryInfo, singleLookup)) {
      return false;
    }

    localeMap->Country = countryInfo.Country;
  } else {
    localeMap->Country = CountryIndex_invalid;
  }

  if (stringMap->Sorting != NULL) {
    SortingInfo sortingInfo = {0};

    sortingInfo.String = stringMap->Sorting;
    sortingInfo.Length = strlen (sortingInfo.String);

    if (!p32_lookup_sorting (&sortingInfo)) {
      return false;
    }

    localeMap->Sorting = sortingInfo.Sorting;
  } else {
    localeMap->Sorting = SortingIndex_invalid;
  }

  if (stringMap->Charset != NULL) {
    CharsetInfo charsetInfo = {0};

    charsetInfo.String = stringMap->Charset;
    charsetInfo.Length = strlen (charsetInfo.String);

    if (!p32_lookup_charset (&charsetInfo)) {
      return false;
    }

    localeMap->CodePage = charsetInfo.CodePage;
  }

  P32Modifier (localeMap, stringMap);

  /**
   * ca-ES-valencia.
   *
   * The following locale string can be used for this locale:
   *
   *  - ca-ES-valencia
   *  - ca[_ES]@valencia
   *  - Valencian[_Spain]
   *
   * With information in `localeMap`, they would always resolve to `ca-ES`.
   */
  if (localeMap->Language.Language == ca) {
    if (localeMap->Language.Country == ES_valencia || localeMap->Modifier == Modifier_valencia) {
      localeMap->Country  = ES_valencia;
      localeMap->Modifier = Modifier_valencia;
    }

    /**
     * Locale strings `Punjabi_India` and `Punjabi_Pakistan` correspond to
     * locales `pa[-Guru]-IN` and `pa-Arab-PK` respectively.
     *
     * Locale name `pa-PK`, which is resolved from `Punjabi_Pakistan`, is not
     * a valid locale. This will cause `Punjabi_Pakistan` always resolve to
     * `pa-IN`.
     *
     * Give resolver a hint to resolve `Punjabi_Pakistan` to `pa-Arab-PK`.
     */
  } else if (localeMap->Language.Language == pa) {
    if (localeMap->Country == PK && localeMap->Language.Script == ScriptIndex_invalid) {
      localeMap->Language.Script = Arab;
    }

#if !P32_LOCALE_NAMES
    /**
     * fa-AF.
     *
     * Locale names for `Persian_Afganistan` and `Persian_Iran` locales
     * use the same language code `fa` (`fa-AF` and `fa-IR` respectively).
     *
     * However, `LCID` locales use different primary language IDs (`LANG_*`).
     */
  } else if (localeMap->Language.Language == fa) {
    if (localeMap->Country == AF) {
      localeMap->Language.Language = prs;
    }
#endif
  }

  switch (localeMap->Modifier) {
    case Modifier_valencia:
      if (localeMap->Language.Language != ca) {
        localeMap->Modifier = ModifierIndex_invalid;
      }
      break;
    default:
      assert (localeMap->Modifier == ModifierIndex_invalid);
      break;
  }

  return true;
}

/**
 * Try to parse `localeString` as locale string in ISO Format.
 *
 * Return values:
 *
 *   1: locale string has other format
 *   0: locale string is valid
 *  -1: locale string is invalid
 */
static int P32ParseIsoLocaleString (LocaleMap *localeMap, const wchar_t *localeString, uintptr_t heap);

#define P32_LOCALE_PARSER P32_LOCALE_PARSER_ISO
#include "locale_map/locale_parser.c"
#undef P32_LOCALE_PARSER

/**
 * Try to parse `localeString` as locale string in Windows Format.
 *
 * Return values:
 *
 *   1: locale string has other format
 *   0: locale string is valid
 *  -1: locale string is invalid
 */
static int P32ParseWindowsLocaleString (LocaleMap *localeMap, const wchar_t *localeString, uintptr_t heap);

#define P32_LOCALE_PARSER P32_LOCALE_PARSER_WINDOWS
#include "locale_map/locale_parser.c"
#undef P32_LOCALE_PARSER

/**
 * Try to parse `localeString` as locale string in CRT Format.
 *
 * Return values:
 *
 *   0: locale string is valid
 *  -1: locale string is invalid
 */
static int P32ParseCrtLocaleString (LocaleMap *localeMap, const wchar_t *localeString, uintptr_t heap);

#define P32_LOCALE_PARSER P32_LOCALE_PARSER_CRT
#include "locale_map/locale_parser.c"
#undef P32_LOCALE_PARSER

/**
 * External Functions
 */

bool p32_locale_map (LocaleMap *localeMap, const wchar_t *localeString, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Check if `localeString` is a known locale name.
   */
  if (P32LocaleName (localeString, localeMap)) {
    return true;
  }

  /**
   * Check if `localeString` needs to be normalized.
   */
  wchar_t *normalizedLocaleString = NULL;

  if (!P32NormalizeLocaleString (&normalizedLocaleString, heap, localeString)) {
    return false;
  }

  if (normalizedLocaleString != NULL) {
    localeString = normalizedLocaleString;
  }

  /**
   * Try to parse locale string.
   */
  bool success = false;

  switch (P32ParseIsoLocaleString (localeMap, localeString, heap)) {
    case 0:
      success = true;
    case -1:
      goto free;
  }

  switch (P32ParseWindowsLocaleString (localeMap, localeString, heap)) {
    case 0:
      success = true;
    case -1:
      goto free;
  }

  switch (P32ParseCrtLocaleString (localeMap, localeString, heap)) {
    case 0:
      success = true;
    case -1:
      goto free;
  }

free:
  if (normalizedLocaleString != NULL) {
    HeapFree (heapHandle, 0, normalizedLocaleString);
  }

  return success;
}
