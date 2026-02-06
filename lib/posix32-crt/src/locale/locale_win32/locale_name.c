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
 * String returned by `GetLocaleInfoEx (LOCALE_SENGLISHLANGUAGENAME)`
 * for an invalid locale name.
 */
static WCHAR UnknownLanguage[] = TEXT ("Unknown Language");

/**
 * String returned by `GetLocaleInfoEx (LOCALE_SENGLISHCOUNTRYNAME)`
 * for an invalid locale name.
 */
static WCHAR UnknownCountry[] = TEXT ("Unknown Region");

/**
 * Known Default Locales.
 */
typedef enum DefaultLocaleType {
  SystemDefaultLocale,
  UserDefaultLocale,
} DefaultLocaleType;

/**
 * Structure used during locale resolution.
 */
typedef struct ResolvedLocaleMap {
  LPWSTR        Locale;
  LanguageIndex Language;
  ScriptIndex   Script;
  CountryIndex  Country;
  SortingIndex  Sorting;
} ResolvedLocaleMap;

static int P32GetLocaleInfo (Locale *locale, uint32_t info, wchar_t *buffer, int bufferSize) {
  return GetLocaleInfoEx (locale->LocaleName, info, buffer, bufferSize);
}

/**
 * Check whether `localeName` is a valid supported locale.
 *
 * The `IsValidLocaleName` function introduced in Windows Vista cannot be
 * used to determine whether passed locale name is a valid supported locale.
 *
 * This function only verifies that the passed locale name is formed correctly.
 *
 * Thr `ResolveLocaleName` function introduced in Windows 7 helps with above
 * issue but does not solve the problem completely:
 *
 * 1. It may discard some parts of the string, such as sorting order name or
 *    replace invalid country code with the default for the language.
 *
 * 2. It resolves "ca-ES-valencia" to "ca-ES" which is a different locale.
 *
 * The behavior in item 1 is desired in case when we supply plain language name
 * (e.g. "en"), but not when we supply an invalid locale and it just replaces
 * the country code with the default for the language.
 *
 * Instead, we rely on behavior that `GetLocaleInfoEx` function returns
 *
 * - "Unknown Language (`ll[-Ssss]`)", with `LOCALE_SENGLISHLANGUAGENAME`
 * - "Unknown Region (`CC`)", with `LOCALE_SENGLISHCOUNTRYNAME`
 *
 * if passed locale name is not a valid supported locale.
 *
 * Return values:
 *
 *   `1`: `localeName` is not a valid locale
 *   `0`: `localeName` is a valid locale
 *  `-1`: an error has occured
 */
static int P32IsValidLocaleName (LPWSTR localeName, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  int success = 1;

  /**
   * Partial `Locale` object for use with `p32_winlocale_getinfo`.
   */
  Locale locale = {0};

  locale.LocaleName = localeName;

  LPWSTR language = NULL;
  LPWSTR country  = NULL;

  if (!p32_winlocale_getinfo (&language, heap, &locale, LOCALE_SENGLISHLANGUAGENAME)) {
    success = -1;
    goto fail;
  }

  if (!p32_winlocale_getinfo (&country, heap, &locale, LOCALE_SENGLISHCOUNTRYNAME)) {
    success = -1;
    goto fail;
  }

  if (!_wcsnicmp (language, UnknownLanguage, _countof (UnknownLanguage) - 1)) {
    goto fail;
  }

  if (!_wcsnicmp (country, UnknownCountry, _countof (UnknownCountry) - 1)) {
    goto fail;
  }

  success = 0;

fail:
  if (language != NULL) {
    HeapFree (heapHandle, 0, language);
  }
  if (country != NULL) {
    HeapFree (heapHandle, 0, country);
  }

  return success;
}

/**
 * Return `CountryIndex` for locale `localeName`.
 *
 * On success, returns valid `CountryIndex` value. Otherwise, returns
 * `CountryIndex_invalid`.
 */
static CountryIndex P32GetCountryIndex (LPCWSTR localeName, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Partial `Locale` object for use with `p32_winlocale_getinfo`.
   */
  Locale locale = {0};

  locale.LocaleName = (LPWSTR) localeName;

  /**
   * ISO-3166 country code for `localeName`.
   */
  LPWSTR countryCode = NULL;

  if (!p32_winlocale_getinfo (&countryCode, heap, &locale, LOCALE_SISO3166CTRYNAME)) {
    return CountryIndex_invalid;
  }

  CountryIndex cc = p32_country_by_cc (countryCode);

  HeapFree (heapHandle, 0, countryCode);

  return cc;
}

/**
 * Attempt to resolve locale from `ll`, `ss` and `cc`.
 *
 * `ss` can be `ScriptIndex_invalid`, in which case no script will be applied.
 *
 * `cc` can be `CountryIndex_invalid`, in which case locale's default country
 * will be applied.
 *
 * This function returns `false` only if an error has occured.
 * Failure to resolve locale is not considered an error.
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

  /**
   * Locale name constructed from `ll`, `ss` and `cc`.
   */
  LPWSTR localeName = NULL;

  if (ss != ScriptIndex_invalid && cc != CountryIndex_invalid) {
    assert (script.Name != NULL && country.Code != NULL);

    if (p32_private_aswprintf (&localeName, heap, L"%s-%s-%s", language.Code, script.Name, country.Code) == -1) {
      return false;
    }
  } else if (ss != ScriptIndex_invalid) {
    assert (script.Name != NULL);

    if (p32_private_aswprintf (&localeName, heap, L"%s-%s", language.Code, script.Name) == -1) {
      return false;
    }
  } else if (cc != CountryIndex_invalid) {
    assert (country.Code != NULL);

    if (p32_private_aswprintf (&localeName, heap, L"%s-%s", language.Code, country.Code) == -1) {
      return false;
    }
  } else {
    if (p32_private_wcsdup (&localeName, language.Code, heap) == -1) {
      return false;
    }
  }

  /**
   * Function's return value.
   */
  bool success = false;

  int isValidLocale = P32IsValidLocaleName (localeName, heap);

  if (isValidLocale != 0) {
    success = (isValidLocale == 1);
    goto fail_free;
  }

  /**
   * Figure out locale's default country.
   */
  if (cc == CountryIndex_invalid) {
    cc = P32GetCountryIndex (localeName, heap);

    if (cc == CountryIndex_invalid) {
      goto fail_free;
    }

    HeapFree (heapHandle, 0, localeName);

    p32_country (cc, &country);

    if (ss != ScriptIndex_invalid) {
      assert (script.Name != NULL && country.Code != NULL);

      if (p32_private_aswprintf (&localeName, heap, L"%s-%s-%s", language.Code, script.Name, country.Code) == -1) {
        goto fail;
      }
    } else {
      assert (country.Code != NULL);

      if (p32_private_aswprintf (&localeName, heap, L"%s-%s", language.Code, country.Code) == -1) {
        goto fail;
      }
    }

    isValidLocale = P32IsValidLocaleName (localeName, heap);

    if (isValidLocale != 0) {
      success = (isValidLocale == 1);
      goto fail_free;
    }
  }

  locale->Locale   = localeName;
  locale->Language = ll;
  locale->Script   = ss;
  locale->Country  = cc;

  success = true;

fail_free:
  if (isValidLocale != 0 || !success) {
    HeapFree (heapHandle, 0, localeName);
  }

fail:
  return success;
}

/**
 * Attempt to apply sorting order `xx` to locale `locale->Locale`.
 *
 * If sorting order has been applied, `locale->Locale` is set to locale name
 * with applied sorting order.
 *
 * This function returns `false` only if an error has occured.
 * Failure to apply sorting order is not considered an error.
 */
static bool P32TrySortOrder (ResolvedLocaleMap *locale, uintptr_t heap, SortingIndex xx) {
  HANDLE heapHandle = (HANDLE) heap;

  Sorting sorting = {0};

  p32_sorting (xx, &sorting);

  /**
   * Locale name with applied sorting order.
   */
  LPWSTR localeName = NULL;

  if (p32_private_aswprintf (&localeName, heap, L"%s_%s", locale->Locale, sorting.Name) == -1) {
    return false;
  }

  int isValidLocale = P32IsValidLocaleName (localeName, heap);

  if (isValidLocale == 0) {
    HeapFree (heapHandle, 0, locale->Locale);
    locale->Locale = localeName;
  } else {
    HeapFree (heapHandle, 0, localeName);
  }

  return isValidLocale != -1;
}

/**
 * Resolve default locale for language described by `map->Language`.
 *
 * If this function fails, then language `map->Language.Language` is
 * not supported by the operating system.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32Ll (ResolvedLocaleMap *locale, uintptr_t heap, LocaleMap *map) {
  /**
   * Try to use both `map->Language.Script` and `map->Language.Country`.
   */
  if (map->Language.Script != ScriptIndex_invalid && map->Language.Country != CountryIndex_invalid) {
    if (!P32TryResolve (locale, heap, map->Language.Language, map->Language.Script, map->Language.Country)) {
      return false;
    }

    if (locale->Locale != NULL) {
      return true;
    }
  }

  /**
   * Try to use only `map->Language.Script`.
   */
  if (map->Language.Script != ScriptIndex_invalid) {
    if (!P32TryResolve (locale, heap, map->Language.Language, map->Language.Script, CountryIndex_invalid)) {
      return false;
    }

    if (locale->Locale != NULL) {
      return true;
    }
  }

  /**
   * Try to use only `map->Language.Country`.
   */
  if (map->Language.Country != CountryIndex_invalid) {
    if (!P32TryResolve (locale, heap, map->Language.Language, ScriptIndex_invalid, map->Language.Country)) {
      return false;
    }

    if (locale->Locale != NULL) {
      return true;
    }
  }

  /**
   * Try to resolve default locale for language `map->Language.Language`.
   */
  if (!P32TryResolve (locale, heap, map->Language.Language, ScriptIndex_invalid, CountryIndex_invalid)) {
    return false;
  }

  return locale->Locale != NULL;
}

/**
 * Try to resolve locale name from `map->Language` and `map->Country`.
 *
 * This function returns `false` only if an error has occured.
 * Failure to resolve locale name is not considered an error.
 */
static bool P32LlCc (ResolvedLocaleMap *locale, uintptr_t heap, LocaleMap *map, ResolvedLocaleMap *base) {
  /**
   * If `base->Script` is set, try to use it with other explicit information
   * from `map`.
   */
  if (base->Script != ScriptIndex_invalid) {
    if (!P32TryResolve (locale, heap, map->Language.Language, base->Script, map->Country)) {
      return false;
    }
  }

  if (locale->Locale != NULL) {
    return true;
  }

  /**
   * Try to use only explicit informaion from `map`.
   */
  if (!P32TryResolve (locale, heap, map->Language.Language, ScriptIndex_invalid, map->Country)) {
    return false;
  }

  return true;
}

/**
 * Try to resolve locale name from `map->Language` and `map->Script`.
 *
 * This function returns `false` only if an error has occured.
 * Failure to resolve locale name is not considered an error.
 */
static bool P32LlSs (ResolvedLocaleMap *locale, uintptr_t heap, LocaleMap *map, ResolvedLocaleMap *base) {
  /**
   * If `base->Country` is set, try to use it with other explicit information
   * from `map`.
   */
  if (base->Country != CountryIndex_invalid) {
    if (!P32TryResolve (locale, heap, map->Language.Language, map->Script, base->Country)) {
      return false;
    }
  }

  if (locale->Locale != NULL) {
    return true;
  }

  /**
   * Try to use only explicit informaion from `map`.
   */
  if (!P32TryResolve (locale, heap, map->Language.Language, map->Script, CountryIndex_invalid)) {
    return false;
  }

  return true;
}

/**
 * Try to resolve locale name from `map->Language`, `map->Script` and
 * `map->Country`.
 *
 * This function returns `false` only if an error has occured.
 * Failure to resolve locale name is not considered an error.
 */
static bool P32LlSsCc (ResolvedLocaleMap *locale, uintptr_t heap, LocaleMap *map) {
  /**
   * Try to use all explicit information from `map`.
   */
  if (!P32TryResolve (locale, heap, map->Language.Language, map->Script, map->Country)) {
    return false;
  }

  return true;
}

/**
 * Construct `Locale` object for a known `defaultLocaleType`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32DefaultLocale (Locale *locale, uintptr_t heap, DefaultLocaleType defaultLocaleType) {
  HANDLE heapHandle = (HANDLE) heap;

  locale->LocaleName = HeapAlloc (heapHandle, 0, LOCALE_NAME_MAX_LENGTH * sizeof (WCHAR));

  if (locale->LocaleName == NULL) {
    goto fail;
  }

  INT written = 0;

  if (defaultLocaleType == SystemDefaultLocale) {
    written = GetSystemDefaultLocaleName (locale->LocaleName, LOCALE_NAME_MAX_LENGTH);
  } else if (defaultLocaleType == UserDefaultLocale) {
    written = GetUserDefaultLocaleName (locale->LocaleName, LOCALE_NAME_MAX_LENGTH);
  }

  if (written == 0) {
    goto fail_free;
  }

  PVOID address = HeapReAlloc (heapHandle, 0, locale->LocaleName, written * sizeof (WCHAR));

  if (address == NULL) {
    goto fail_free;
  }

  locale->LocaleName = address;

  if (!P32FillLocaleInfo (locale, heap)) {
    goto fail_free;
  }

  return true;

fail_free:
  p32_winlocale_destroy (locale, heap);

fail:
  return false;
}

bool p32_winlocale_system_default (Locale *locale, uintptr_t heap) {
  return P32DefaultLocale (locale, heap, SystemDefaultLocale);
}

bool p32_winlocale_user_default (Locale *locale, uintptr_t heap) {
  return P32DefaultLocale (locale, heap, UserDefaultLocale);
}

bool p32_winlocale_resolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * We do not need to resolve Windows pseudo locales.
   */
  if (localeMap->KnownLocale.Type == LOCALE_TYPE_PSEUDO_LOCALE) {
    if (P32IsValidLocaleName (localeMap->KnownLocale.LocaleName, heap) != 0) {
      return false;
    }

    if (p32_private_wcsdup (&locale->LocaleName, localeMap->KnownLocale.LocaleName, heap) == -1) {
      return false;
    }

    if (!P32FillLocaleInfo (locale, heap)) {
      HeapFree (heapHandle, 0, locale->LocaleName);
      locale->LocaleName = NULL;

      return false;
    }

    locale->Map.Language = localeMap->Language.Language;
    locale->Map.Script   = localeMap->Language.Script;
    locale->Map.Country  = localeMap->Language.Country;
    locale->Map.Modifier = localeMap->Modifier;

    locale->Type = LOCALE_TYPE_PSEUDO_LOCALE;

    return true;
  }

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
  ResolvedLocaleMap defaultLocale = {NULL, -1, -1, -1, -1};

  if (!P32Ll (&defaultLocale, heap, localeMap)) {
    return false;
  }

  /**
   * Locale resolved from explicit information in `localeMap`.
   *
   * If we fail to resolve locale using explicit information,
   * then resolved locale in `defaultLocale` will be used.
   */
  ResolvedLocaleMap resolvedLocale = {NULL, -1, -1, -1, -1};

  if (localeMap->Script != ScriptIndex_invalid && localeMap->Country != CountryIndex_invalid) {
    if (!P32LlSsCc (&resolvedLocale, heap, localeMap)) {
      goto fail;
    }
  }

  if (resolvedLocale.Locale == NULL && localeMap->Script != ScriptIndex_invalid) {
    if (!P32LlSs (&resolvedLocale, heap, localeMap, &defaultLocale)) {
      goto fail;
    }
  }

  if (resolvedLocale.Locale == NULL && localeMap->Country != CountryIndex_invalid) {
    if (!P32LlCc (&resolvedLocale, heap, localeMap, &defaultLocale)) {
      goto fail;
    }
  }

  /**
   * If we failed to resolve locale using explicit information,
   * store resolved locale name from `defaultLocale` in `resolvedLocale`.
   *
   * Otherwise, free resolved locale name in `defaultLocale`.
   */
  if (resolvedLocale.Locale == NULL) {
    resolvedLocale       = defaultLocale;
    defaultLocale.Locale = NULL;
  } else {
    HeapFree (heapHandle, 0, defaultLocale.Locale);
    defaultLocale.Locale = NULL;
  }

  /**
   * Try to apply sorting order to resolved locale.
   */
  if (localeMap->Sorting != SortingIndex_invalid) {
    if (!P32TrySortOrder (&resolvedLocale, heap, localeMap->Sorting)) {
      goto fail;
    }
  }

  /**
   * This makes `locale` usable with `p32_winlocale_getinfo`.
   */
  locale->LocaleName    = resolvedLocale.Locale;
  resolvedLocale.Locale = NULL;

  /**
   * Gather basic infromation for `locale->LocaleName`.
   */
  if (!P32FillLocaleInfo (locale, heap)) {
    goto fail_destroy;
  }

  locale->Map.Language = resolvedLocale.Language;
  locale->Map.Script   = resolvedLocale.Script;
  locale->Map.Country  = resolvedLocale.Country;
  locale->Map.Modifier = localeMap->Modifier;

  locale->Type = localeMap->KnownLocale.Type;

  return true;

fail_destroy:
  p32_winlocale_destroy (locale, heap);

fail:
  if (resolvedLocale.Locale != NULL) {
    HeapFree (heapHandle, 0, resolvedLocale.Locale);
  }

  if (defaultLocale.Locale != NULL) {
    HeapFree (heapHandle, 0, defaultLocale.Locale);
  }

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
  P32GeoDestroy (locale, heap);

  locale->Calendar            = 0;
  locale->AlternativeCalendar = 0;
  locale->CodePage.Ansi       = CP_ACP;
  locale->CodePage.Oem        = CP_OEMCP;
  locale->Map.Language        = LanguageIndex_invalid;
  locale->Map.Script          = ScriptIndex_invalid;
  locale->Map.Country         = CountryIndex_invalid;
  locale->Map.Modifier        = ModifierIndex_invalid;

  if (locale->LocaleName != NULL) {
    HeapFree ((HANDLE) heap, 0, locale->LocaleName);
    locale->LocaleName = NULL;
  }
}

bool p32_winlocale_equal (Locale *l1, Locale *l2) {
  if (l1->Type != l2->Type) {
    return false;
  }

  if (l1->Type == LOCALE_TYPE_POSIX) {
    assert (wcscmp (l1->LocaleName, L"en-US") == 0);
    assert (wcscmp (l2->LocaleName, L"en-US") == 0);
  }

#if _WIN32_WINNT >= _WIN32_WINNT_WIN7
  LPWSTR localeName1 = _alloca (LOCALE_NAME_MAX_LENGTH * sizeof (WCHAR));
  LPWSTR localeName2 = _alloca (LOCALE_NAME_MAX_LENGTH * sizeof (WCHAR));

  if (ResolveLocaleName (l1->LocaleName, localeName1, LOCALE_NAME_MAX_LENGTH) != 0
      && ResolveLocaleName (l2->LocaleName, localeName2, LOCALE_NAME_MAX_LENGTH) != 0) {
    return wcscmp (localeName1, localeName2) == 0;
  }
#endif

  return wcscmp (l1->LocaleName, l2->LocaleName) == 0;
}
