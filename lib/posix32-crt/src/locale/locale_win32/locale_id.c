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

#include "locale-internal.h"

/**
 * Strucutre used for locale resolution.
 */
typedef struct ResolvedLocaleMap {
  /**
   * Resolved `LCID` locale.
   */
  LCID Locale;
  /**
   * `SublanguageIndex` corresponding to resolved locale.
   */
  SublanguageIndex Sublanguage;
  /**
   * `SortingIndex` corrsponding to sorting order applied to resolved locale.
   *
   * If this field is set to `SortingIndex_invalid`, then no sorting order was
   * applied.
   */
  SortingIndex Sorting;
} ResolvedLocaleMap;

static int P32GetLocaleInfo (Locale *locale, uint32_t info, wchar_t *buffer, int bufferSize) {
  return GetLocaleInfoW (locale->LocaleId, info, buffer, bufferSize);
}

/**
 * Attempt to construct `LCID` object from `ll`, `ss` and `cc`.
 *
 * This functions returns `false` only if an error has occured.
 * Failure to consturct `LCID` object is not considered an error.
 *
 * In order to check whether this function has constructed `LCID` object,
 * set `locale->Locale` to `0` and check it afterwards.
 */
static bool P32TryResolve (
  ResolvedLocaleMap *locale,
  uintptr_t          heap,
  LanguageIndex      ll,
  ScriptIndex        ss,
  CountryIndex       cc
) {
  HANDLE heapHandle = (HANDLE) heap;

  Language language = {0};

  p32_language (ll, &language);

  Script script = {0};

  if (ss != ScriptIndex_invalid) {
    p32_script (ss, &script);
  }

  Country country = {0};

  if (cc != CountryIndex_invalid) {
    p32_country (cc, &country);
  }

  char *string = NULL;
  int   length = 0;

  if (ss != ScriptIndex_invalid && cc != CountryIndex_invalid) {
    assert (script.Name != NULL && country.Code != NULL);
    length = p32_private_asprintf (&string, heap, L"%s-%s-%s", language.Code, script.Name, country.Code);
  } else if (ss != ScriptIndex_invalid) {
    assert (script.Name != NULL);
    length = p32_private_asprintf (&string, heap, L"%s-%s", language.Code, script.Name);
  } else if (cc != CountryIndex_invalid) {
    assert (country.Code != NULL);
    length = p32_private_asprintf (&string, heap, L"%s-%s", language.Code, country.Code);
  } else {
    length = p32_private_asprintf (&string, heap, L"%s", language.Code);
  }

  if (length == -1) {
    return false;
  }

  SubLangInfo info = {0};

  info.String = string;
  info.Length = length;

  if (p32_lookup_sublanguage (&info)) {
    SubLanguage subLanguage = {0};

    p32_sublanguage (info.SubLanguage, &subLanguage);

    LANGID langId   = MAKELANGID (language.LangId, subLanguage.SubLangId);
    LCID   localeId = MAKELCID (langId, SORT_DEFAULT);

    if (IsValidLocale (localeId, LCID_INSTALLED)) {
      locale->Locale      = localeId;
      locale->Sublanguage = info.SubLanguage;
    }
  }

  HeapFree (heapHandle, 0, string);

  return true;
}

/**
 * Attempt to apply `ss` to `LCID` object `Locale->locale`.
 *
 * If `ss` has been successfully applied, new `LCID` object with applied
 * sorting order is stored in `Locale->locale`.
 *
 * This functions returns `false` only if an error has occured.
 * Failure to apply `ss` is not considered an error.
 */
static bool P32TrySortOrder (ResolvedLocaleMap *locale, uintptr_t heap, LanguageIndex ll, SortingIndex ss) {
  HANDLE heapHandle = (HANDLE) heap;

  Language language = {0};

  p32_language (ll, &language);

  Sorting sorting = {0};

  p32_sorting (ss, &sorting);

  char *string = NULL;
  int   length = p32_private_asprintf (&string, heap, L"%s_%s", language.Code, sorting.Name);

  if (length == -1) {
    return false;
  }

  SortingIdInfo info = {0};

  info.String = string;
  info.Length = length;

  if (p32_lookup_sorting_id (&info)) {
    LANGID langId   = LANGIDFROMLCID (locale->Locale);
    LCID   localeId = MAKELCID (langId, info.SortId);

    if (IsValidLocale (localeId, LCID_INSTALLED)) {
      locale->Locale  = localeId;
      locale->Sorting = ss;
    }
  }

  HeapFree (heapHandle, 0, string);

  return true;
}

/**
 * Construct `ResolvedLocaleMap` from existing `LCID` locale.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32LocaleFromLCID (ResolvedLocaleMap *map, uintptr_t heap, Locale *locale) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * We have to take both primary language ID (`LANG_*`) and ISO-639 language
   * code into account when looking up `LanguageIndex` for `locale->LocaleId`.
   *
   * The are languages IDs which may be used to construct `LCID` which have
   * different ISO-639 language codes.
   *
   * For example, `LANG_NORWEGIAN` may be used to construct `LCID` locales with
   * `no` and `nb` ISO-639 language code.
   *
   * Similarly, there are ISO-639 language codes which are with more than one
   * language ID.
   *
   * For example, both `LANG_CATALAN` and `LANG_VALENCIAN` are used to
   * construct `LCID` locale with ISO-639 language code "ca".
   */
  LPWSTR ll = NULL;

  if (!p32_winlocale_getinfo (&ll, heap, locale, LOCALE_SISO639LANGNAME)) {
    return false;
  }

  bool success = false;

  LCID   localeId      = locale->LocaleId;
  LANGID langId        = LANGIDFROMLCID (localeId);
  LANGID primaryLangId = PRIMARYLANGID (langId);
  LANGID subLangId     = SUBLANGID (langId);
  WORD   sortingId     = SORTIDFROMLCID (localeId);

  LanguageIndex language = p32_language_from_id (primaryLangId, ll);

  if (language == LanguageIndex_invalid) {
    goto fail;
  }

  SublanguageIndex sublanguage = p32_sublanguage_from_id (subLangId, language);

  if (sublanguage == SublanguageIndex_invalid) {
    goto fail;
  }

  SortingIndex sorting = p32_sorting_from_id (sortingId, language);

  if (sorting == SortingIndex_invalid) {
    goto fail;
  }

  map->Locale      = localeId;
  map->Sublanguage = sublanguage;
  map->Sorting     = (sorting == SortingIndex_default ? SortingIndex_invalid : sorting);

  success = true;

fail:
  HeapFree (heapHandle, 0, ll);

  return success;
}

/**
 * Construct `LCID` object for langauge described by `map->Language`.
 *
 * If this functions fails, then `ll` is not supported by the operating system.
 * This may happen if `ll` is a language which does not have assigned `LANG_*`
 * constant (which means it is only usable with locale names).
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32Ll (ResolvedLocaleMap *locale, LocaleMap *map, uintptr_t heap) {
  Language language = {0};

  p32_language (map->Language.Language, &language);

  /**
   * We store `(uint16_t)-1` in `LangId` for languages which do not have
   * assigned `LANG_*` constant
   *
   * Such languages cannot be used with `LCID` locales.
   */
  if (language.LangId == (uint16_t) -1) {
    return false;
  }

  Script script = {0};

  if (map->Language.Script != ScriptIndex_invalid) {
    p32_script (map->Language.Script, &script);
  }

  Country country = {0};

  if (map->Language.Country != CountryIndex_invalid) {
    p32_country (map->Language.Country, &country);
  }

  /**
   * Attempt to apply both `map->Language.Script` and `map->Language.Country`.
   */
  if (map->Language.Script != ScriptIndex_invalid && map->Language.Country != CountryIndex_invalid) {
    if (!P32TryResolve (locale, heap, map->Language.Language, map->Language.Script, map->Language.Country)) {
      return false;
    }

    if (locale->Locale != 0) {
      return true;
    }
  }

  /**
   * Attempt to apply `map->Language.Script`.
   */
  if (map->Language.Script != ScriptIndex_invalid) {
    if (!P32TryResolve (locale, heap, map->Language.Language, map->Language.Script, CountryIndex_invalid)) {
      return false;
    }

    if (locale->Locale != 0) {
      return true;
    }
  }

  /**
   * Attempt to apply `map->Language.Country`.
   */
  if (map->Language.Country != CountryIndex_invalid) {
    if (!P32TryResolve (locale, heap, map->Language.Language, ScriptIndex_invalid, map->Language.Country)) {
      return false;
    }

    if (locale->Locale != 0) {
      return true;
    }
  }

  /**
   * Attempt to constuct default locale for `map->Language.Language`.
   *
   * There are locales for which `SUBLANG_DEFAULT` cannot be used to construct
   * valid `LCID` locale.
   *
   * There are also languge IDs which, when used with `SUBLANG_DEFAULT`, result
   * in different locale from `ResolveLocaleName` called on that locale's
   * language code.
   *
   * In order to provide consistent result, we define default `SublanuageIndex`
   * for each supported language.
   */
  if (1) {
    if (!P32TryResolve (locale, heap, map->Language.Language, ScriptIndex_invalid, CountryIndex_invalid)) {
      return false;
    }

    if (locale->Locale != 0) {
      return true;
    }
  }

  /**
   * If everyting above failed, try to use `SUBLANG_DEFAULT`.
   *
   * If it result in valid `LCID` locale, attempt to figure out
   * `SublanguageIndex` for that locale.
   *
   * Currently, there is only one known case:
   *
   * when
   *
   *  `MAKELCID (MAKELANGID (LANG_SPANISH, SUBLANG_SPANISH_MODERN), 0)`
   *
   * does not result in valid `LCID` locale, but
   *
   *  `MAKELCID (MAKELANGID (LANG_SPANISH, SUBLANG_SPANISH), 0)`
   *
   * does.
   */
  LANGID langId   = MAKELANGID (language.LangId, SUBLANG_DEFAULT);
  LCID   localeId = MAKELCID (langId, SORT_DEFAULT);

  if (IsValidLocale (localeId, LCID_INSTALLED)) {
    Locale l = {0};

    l.LocaleId = localeId;

    return P32LocaleFromLCID (locale, heap, &l);
  }

  return false;
}

/**
 * Attempt to cunstruct `LCID` object from `map->Language` and `map->Country`.
 *
 * This functions returns `false` only if an error has occured.
 * Failure to consturct `LCID` object is not considered an error.
 *
 * In order to check whether this function has constructed `LCID` object,
 * set `locale->Locale` to `0` and check it afterwards.
 */
static bool P32LlCc (ResolvedLocaleMap *locale, LocaleMap *map, uintptr_t heap, ResolvedLocaleMap *base) {
  /**
   * Try to construct `LCID` locale by applying `map->Contry`.
   */
  if (!P32TryResolve (locale, heap, map->Language.Language, ScriptIndex_invalid, map->Country)) {
    return false;
  }

  if (locale->Locale == 0) {
    return true;
  }

  /**
   * If we constructed `LCID` locale, try to apply script from `base`.
   */
  if (locale->Sublanguage != base->Sublanguage) {
    SubLanguage sublanguage = {0};

    p32_sublanguage (base->Sublanguage, &sublanguage);

    if (sublanguage.Map.Script != ScriptIndex_invalid) {
      if (!P32TryResolve (locale, heap, map->Language.Language, sublanguage.Map.Script, map->Country)) {
        return false;
      }
    }
  }

  return true;
}

/**
 * Attempt to cunstruct `LCID` object from `map->Language` and `map->Script`.
 *
 * This functions returns `false` only if an error has occured.
 * Failure to consturct `LCID` object is not considered an error.
 *
 * In order to check whether this function has constructed `LCID` object,
 * set `locale->Locale` to `0` and check it afterwards.
 */
static bool P32LlSs (ResolvedLocaleMap *locale, LocaleMap *map, uintptr_t heap, ResolvedLocaleMap *base) {
  /**
   * Try to construct `LCID` locale by applying `map->Script`.
   */
  if (!P32TryResolve (locale, heap, map->Language.Language, map->Script, CountryIndex_invalid)) {
    return false;
  }

  if (locale->Locale == 0) {
    return true;
  }

  /**
   * If we constructed `LCID` locale, try to apply country from `base`.
   */
  if (locale->Sublanguage != base->Sublanguage) {
    SubLanguage sublanguage = {0};

    p32_sublanguage (base->Sublanguage, &sublanguage);

    if (sublanguage.Map.Country != CountryIndex_invalid) {
      if (!P32TryResolve (locale, heap, map->Language.Language, map->Script, sublanguage.Map.Country)) {
        return false;
      }
    }
  }

  return true;
}

/**
 * Attempt to cunstruct `LCID` object from `map->Language`, `map->Script`
 * and `map->Country`.
 *
 * This functions returns `false` only if an error has occured.
 * Failure to consturct `LCID` object is not considered an error.
 *
 * In order to check whether this function has constructed `LCID` object,
 * set `locale->Locale` to `0` and check it afterwards.
 */
static bool P32LlSsCc (ResolvedLocaleMap *locale, LocaleMap *map, uintptr_t heap) {
  /**
   * Try to construct `LCID` locale by applying both `map->Script` and
   * `map->Contry`.
   */
  if (!P32TryResolve (locale, heap, map->Language.Language, map->Script, map->Country)) {
    return false;
  }

  return true;
}

/**
 * Construct locale name for `Locale->LocaleId` and store it in
 * `Locale->LocaleName`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32LocaleName (Locale *locale, uintptr_t heap, ResolvedLocaleMap *resolved, LocaleMap *localeMap) {
  /**
   * Windows Pseudo locales were added in Windows Vista and are only supported
   * with locale names. We emulate them with `LCID` locales.
   *
   * Simply copy locale name from `localeMap`.
   */
  if (localeMap != NULL && localeMap->KnownLocale.Type == LOCALE_TYPE_PSEUDO_LOCALE) {
    assert (localeMap->KnownLocale.LocaleName != NULL);
    return p32_private_wcsdup (&locale->LocaleName, localeMap->KnownLocale.LocaleName, heap) != -1;
  }

  SubLanguage sublanguage = {0};

  assert (resolved->Sublanguage != SublanguageIndex_invalid);
  p32_sublanguage (resolved->Sublanguage, &sublanguage);

  Language language = {0};

  assert (sublanguage.Map.Language != LanguageIndex_invalid);
  p32_language (sublanguage.Map.Language, &language);

  Script script = {0};

  if (sublanguage.Map.Script != ScriptIndex_invalid) {
    p32_script (sublanguage.Map.Script, &script);
  }

  Country country = {0};

  assert (sublanguage.Map.Country != CountryIndex_invalid);
  p32_country (sublanguage.Map.Country, &country);

  Sorting sorting = {0};

  if (resolved->Sorting != SortingIndex_invalid) {
    p32_sorting (resolved->Sorting, &sorting);
  }

  LPCWSTR ll = language.Code;
  LPCWSTR ss = script.Name;
  LPCWSTR cc = country.Code;
  LPCWSTR xx = sorting.Name;

  if (ss != NULL && xx != NULL) {
    if (p32_private_aswprintf (&locale->LocaleName, heap, L"%s-%s-%s_%s", ll, ss, cc, xx) == -1) {
      goto fail;
    }
  } else if (ss != NULL) {
    if (p32_private_aswprintf (&locale->LocaleName, heap, L"%s-%s-%s", ll, ss, cc) == -1) {
      goto fail;
    }
  } else if (xx != NULL) {
    if (p32_private_aswprintf (&locale->LocaleName, heap, L"%s-%s_%s", ll, cc, xx) == -1) {
      goto fail;
    }
  } else {
    if (p32_private_aswprintf (&locale->LocaleName, heap, L"%s-%s", ll, cc) == -1) {
      goto fail;
    }
  }

  return true;

fail:
  return false;
}

bool p32_winlocale_default (Locale *locale, uintptr_t heap) {
  ResolvedLocaleMap map = {0};

  locale->LocaleId = GetUserDefaultLCID ();

  if (!P32LocaleFromLCID (&map, heap, locale)) {
    goto fail;
  }

  if (!P32LocaleName (locale, heap, &map, NULL)) {
    goto fail;
  }

  if (!P32FillLocaleInfo (locale, heap)) {
    goto fail;
  }

  return true;

fail:
  p32_winlocale_destroy (locale, heap);

  return false;
}

bool p32_winlocale_resolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap) {
  /**
   * Locale resolved from implicit information in `localeMap->Language`.
   *
   * This locale is used as a base when resolving locale using explicit
   * information in `localeMap`.
   *
   * This base locale is always complete, which means it includes country
   * information and, for locales which have script-specific variant,
   * script information.
   *
   * If explicit information does not specify country or script information,
   * we will first try to inherit this information from this base locale.
   */
  ResolvedLocaleMap defaultLocale = {0, SublanguageIndex_invalid, SortingIndex_invalid};

  if (!P32Ll (&defaultLocale, localeMap, heap)) {
    return false;
  }

  /**
   * Locale resolved from explicit information in `localeMap`.
   *
   * If we fail to resolve locale using explicit information,
   * then resolved locale in `defaultLocale` will be used.
   */
  ResolvedLocaleMap resolvedLocale = {0, SublanguageIndex_invalid, SortingIndex_invalid};

  if (localeMap->Country != CountryIndex_invalid && localeMap->Script != ScriptIndex_invalid) {
    if (!P32LlSsCc (&resolvedLocale, localeMap, heap)) {
      return false;
    }
  }

  if (resolvedLocale.Locale == 0 && localeMap->Script != ScriptIndex_invalid) {
    if (!P32LlSs (&resolvedLocale, localeMap, heap, &defaultLocale)) {
      return false;
    }
  }

  if (resolvedLocale.Locale == 0 && localeMap->Country != CountryIndex_invalid) {
    if (!P32LlCc (&resolvedLocale, localeMap, heap, &defaultLocale)) {
      return false;
    }
  }

  /**
   * If we failed to resolve locale using explicit information,
   * use resolved locale from `defaultLocale`.
   */
  if (resolvedLocale.Locale == 0) {
    resolvedLocale = defaultLocale;
  }

  /**
   * Try to apply sorting order to resolved locale.
   */
  if (localeMap->Sorting != SortingIndex_invalid) {
    if (!P32TrySortOrder (&resolvedLocale, heap, localeMap->Language.Language, localeMap->Sorting)) {
      return false;
    }
  }

  /**
   * This makes `locale` usable with `p32_winlocale_getinfo`.
   */
  locale->LocaleId = resolvedLocale.Locale;

  /**
   * Store locale name in `locale->LocaleName`.
   */
  if (!P32LocaleName (locale, heap, &resolvedLocale, localeMap)) {
    goto fail;
  }

  /**
   * Gather basic infromation for `locale->LocaleId`.
   */
  if (!P32FillLocaleInfo (locale, heap)) {
    goto fail;
  }

  SubLanguage sublanguage = {0};

  p32_sublanguage (resolvedLocale.Sublanguage, &sublanguage);

  locale->Map.Language = sublanguage.Map.Language;
  locale->Map.Script   = sublanguage.Map.Script;
  locale->Map.Country  = sublanguage.Map.Country;
  locale->Map.Modifier = localeMap->Modifier;

  locale->Type = localeMap->KnownLocale.Type;

  return true;

fail:
  p32_winlocale_destroy (locale, heap);

  return false;
}

bool p32_winlocale_copy (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  if (p32_private_wcsdup (&destLocale->LocaleName, srcLocale->LocaleName, heap) == -1) {
    goto fail;
  }

  if (!P32GeoDuplicate (destLocale, heap, srcLocale)) {
    goto fail_destroy;
  }

  destLocale->Type                = srcLocale->Type;
  destLocale->LocaleId            = srcLocale->LocaleId;
  destLocale->Calendar            = srcLocale->Calendar;
  destLocale->AlternativeCalendar = srcLocale->AlternativeCalendar;
  destLocale->CodePage            = srcLocale->CodePage;
  destLocale->Map                 = srcLocale->Map;

  return true;

fail_destroy:
  p32_winlocale_destroy (destLocale, heap);

fail:
  return false;
}

void p32_winlocale_destroy (Locale *locale, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  P32GeoDestroy (locale, heap);

  locale->LocaleId            = 0;
  locale->Calendar            = 0;
  locale->AlternativeCalendar = 0;
  locale->CodePage.Ansi       = 0;
  locale->CodePage.Oem        = 0;

  if (locale->LocaleName != NULL) {
    HeapFree (heapHandle, 0, locale->LocaleName);
    locale->LocaleName = NULL;
  }
}

bool p32_winlocale_equal (Locale *l1, Locale *l2) {
  if (l1->Type != l2->Type) {
    return false;
  }

  if (l1->Type == LOCALE_TYPE_POSIX) {
    assert (l1->LocaleId == l2->LocaleId);
    assert (wcscmp (l1->LocaleName, L"en-US") == 0);
    assert (wcscmp (l2->LocaleName, L"en-US") == 0);
  }

  if (l1->LocaleId != l2->LocaleId) {
    return false;
  }

  assert (wcscmp (l1->LocaleName, l2->LocaleName) == 0);

  return true;
}
