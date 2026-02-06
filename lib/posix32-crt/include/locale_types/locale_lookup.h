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

#ifndef LIBPOSIX32_LOCALE_LOOKUP_H_INCLUDED
#define LIBPOSIX32_LOCALE_LOOKUP_H_INCLUDED

#include "locale_types/locale_map.h"

typedef struct LanguageInfo {
  const char       *String;
  size_t            Length;
  BaseLanguageIndex BaseLanguage;
} LanguageInfo;

bool p32_lookup_language_code (LanguageInfo *info);
bool p32_lookup_language_name (LanguageInfo *info);

typedef struct CountryInfo {
  const char  *String;
  size_t       Length;
  CountryIndex Country;
} CountryInfo;

bool p32_lookup_country_code (CountryInfo *info);
bool p32_lookup_country_name (CountryInfo *info);

typedef struct ScriptInfo {
  const char *String;
  size_t      Length;
  ScriptIndex Script;
} ScriptInfo;

bool p32_lookup_script (ScriptInfo *info);

typedef struct CharsetInfo {
  const char *String;
  size_t      Length;
  uint32_t    CodePage;
} CharsetInfo;

bool p32_lookup_charset (CharsetInfo *info);

typedef struct {
  const char  *String;
  size_t       Length;
  SortingIndex Sorting;
} SortingInfo;

bool p32_lookup_sorting (SortingInfo *info);

typedef struct ModifierInfo {
  const char   *String;
  size_t        Length;
  ModifierIndex Modifier;
} ModifierInfo;

bool p32_lookup_modifier (ModifierInfo *info);

#if !P32_LOCALE_NAMES
typedef struct SubLangInfo {
  const char      *String;
  size_t           Length;
  SublanguageIndex SubLanguage;
} SubLangInfo;

bool p32_lookup_sublanguage (SubLangInfo *info);

typedef struct SortingIdInfo {
  const char    *String;
  size_t         Length;
  SortingIdIndex SortingId;
} SortingIdInfo;

bool p32_lookup_sorting_id (SortingIdInfo *info);
#endif

#endif /* LIBPOSIX32_LOCALE_LOOKUP_H_INCLUDED */
