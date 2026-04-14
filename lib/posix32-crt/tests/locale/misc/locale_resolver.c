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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * This is test for locale resolver (`p32_winlocale_resolve`).
 *
 * Given a locale string contructed from information obtained from
 * `GetLocaleInfo[Ex]` function and `LocaleMap` constructed from that locale
 * string, locale resolver must produce locale equivalent to that from which
 * original locale string was constructed.
 *
 * Example:
 *
 * Given `en-US` locale, the following strings can be constructed:
 *
 *  - English_United States
 *  - English_US
 *  - English_USA
 *  - en_United States
 *  - en_US
 *  - en_USA
 *  - eng_United States
 *  - eng_US
 *  - eng_USA
 *
 * Locale resolver must procude the very same `en-US` locale they were
 * constructed from.
 *
 * However, this is not always possible. Consider `sr-Latn-RS` locale:
 *
 *  - Serbian (Latin)_Serbia
 *  - Serbian (Latin)_RS
 *  - Serbian (Latin)_SRB
 *  - sr_Serbia
 *  - sr_RS
 *  - sr_SRB
 *  - srp_Serbia
 *  - srp_RS
 *  - srp_SRB
 *
 * Here, usage of language codes (`sr` and `srp`) result in lose of important
 * bit of information preventing us from constructing original locale.
 */

static int exit_code = EXIT_SUCCESS;

typedef bool (*FuncIsEqualLocale) (Locale *, Locale *);
typedef bool (*FuncIsKnownFailure) (Locale *, Locale *);

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
#define KNOWN_FAILURE_LCID(langid, sublang_from, sublang_to) \
  {MAKELCID (MAKELANGID (langid, sublang_from), SORT_DEFAULT), MAKELCID (MAKELANGID (langid, sublang_to), SORT_DEFAULT)}

/**
 * A pair of `LCID` locales.
 */
typedef struct KnownLCIDFailure {
  /**
   * Original locale used to construct locale strings passed to
   * `p32_locale_map`.
   */
  LCID From;
  /**
   * Locale resolved by `p32_winlocale_resolve`.
   */
  LCID To;
} KnownLCIDFailure;

/**
 * Known cases when `p32_winlocale_resolve` may construct locale different
 * from one originally used to construct locale string.
 */
static KnownLCIDFailure KnownLCIDFailures[] = {
  /**
   * Spanish_Spain (0A 01 -> 0A 03)
   */
  KNOWN_FAILURE_LCID (LANG_SPANISH, SUBLANG_SPANISH, SUBLANG_SPANISH_MODERN),
};

static bool IsKnownLCIDFailure (Locale *from, Locale *to) {
  for (size_t i = 0; i < _countof (KnownLCIDFailures); ++i) {
    if (KnownLCIDFailures[i].From == from->LocaleId && KnownLCIDFailures[i].To == to->LocaleId) {
      return true;
    }
  }

  return false;
}

static bool IsEqualLCIDLocale (Locale *from, Locale *to) {
  LANGID fromLangId = LANGIDFROMLCID (from->LocaleId);
  LANGID toLangId   = LANGIDFROMLCID (to->LocaleId);

  if (fromLangId != toLangId) {
    return false;
  }

  WORD fromSortingId = SORTIDFROMLCID (from->LocaleId);
  WORD toSortingId   = SORTIDFROMLCID (to->LocaleId);

  if (fromSortingId != toSortingId) {
    fwprintf (stdout, L"UNRESOLVED: %s -> %s\n", from->LocaleName, to->LocaleName);
  }

  return true;
}
#endif /* `LCID` locales */

#if (P32_LOCALE_API & P32_LOCALE_API_LN)
#define KNOWN_FAILURE_LN(from, to) {from, to}

/**
 * A pair of locale names.
 */
typedef struct KnownLNFailure {
  const wchar_t *From;
  const wchar_t *To;
} KnownLNFailure;

/**
 * Known cases when `p32_winlocale_resolve` may construct locale different
 * from one originally used to construct locale string.
 */
static KnownLNFailure KnownLNFailures[] = {
  /**
   * German_Germany
   */
  KNOWN_FAILURE_LN (L"de-DE_phoneb", L"de-DE"),
  /**
   * Spanish_Spain
   */
  KNOWN_FAILURE_LN (L"es-ES_tradnl", L"es-ES"),
  /**
   * Hungarian_Hungary
   */
  KNOWN_FAILURE_LN (L"hu-HU_technl", L"hu-HU"),
  /**
   * Japanese_Japan
   */
  KNOWN_FAILURE_LN (L"ja-JP_radstr", L"ja-JP"),
  /**
   * Georgian_Georgia
   */
  KNOWN_FAILURE_LN (L"ka-GE_modern", L"ka-GE"),
  /**
   * Mongolian_Mongolia
   */
  KNOWN_FAILURE_LN (L"mn-MN", L"mn-Cyrl-MN"),
  /**
   * Manipuri (Bangla)_India
   */
  KNOWN_FAILURE_LN (L"mni-IN", L"mni-Beng-IN"),
  /**
   * Norwegian_Norway -> Norwegian Bokmål_Norway
   */
  KNOWN_FAILURE_LN (L"no", L"nb-NO"),
  /**
   * Punjabi_India
   */
  KNOWN_FAILURE_LN (L"pa-Guru", L"pa-IN"),
  /**
   * Chinese (Simplified)_China
   */
  KNOWN_FAILURE_LN (L"zh-CN", L"zh-Hans-CN"),
  KNOWN_FAILURE_LN (L"zh-CN_phoneb", L"zh-Hans-CN"),
  KNOWN_FAILURE_LN (L"zh-CN_stroke", L"zh-Hans-CN"),
  /**
   * Chinese (Simplified)_Singapore
   */
  KNOWN_FAILURE_LN (L"zh-SG", L"zh-Hans-SG"),
  KNOWN_FAILURE_LN (L"zh-SG_phoneb", L"zh-Hans-SG"),
  KNOWN_FAILURE_LN (L"zh-SG_stroke", L"zh-Hans-SG"),
  /**
   * Chinese (Traditional)_Hong Kong SAR
   */
  KNOWN_FAILURE_LN (L"zh-HK", L"zh-Hant-HK"),
  KNOWN_FAILURE_LN (L"zh-HK_radstr", L"zh-Hant-HK"),
  /**
   * Chinese (Traditional)_Macao SAR
   */
  KNOWN_FAILURE_LN (L"zh-MO", L"zh-Hant-MO"),
  KNOWN_FAILURE_LN (L"zh-MO_radstr", L"zh-Hant-MO"),
  KNOWN_FAILURE_LN (L"zh-MO_stroke", L"zh-Hant-MO"),
  /**
   * Chinese (Traditional)_Taiwan
   */
  KNOWN_FAILURE_LN (L"zh-TW", L"zh-Hant-TW"),
  KNOWN_FAILURE_LN (L"zh-TW_pronun", L"zh-Hant-TW"),
  KNOWN_FAILURE_LN (L"zh-TW_radstr", L"zh-Hant-TW"),
};

static bool IsKnownLNFailure (Locale *from, Locale *to) {
  for (size_t i = 0; i < _countof (KnownLNFailures); ++i) {
    if (!wcscmp (KnownLNFailures[i].From, from->LocaleName) && !wcscmp (KnownLNFailures[i].To, to->LocaleName)) {
      return true;
    }
  }

  return false;
}

static bool IsEqualLNLocale (Locale *from, Locale *to) {
  size_t length  = wcslen (from->LocaleName);
  size_t compare = 0;

  if (length == 2 || length == 3) {
    compare = length;
  } else if (length == 7 && from->LocaleName[2] == L'-') {
    compare = length;
  } else if (length == 8 && from->LocaleName[3] == L'-') {
    compare = length;
  }

  if (compare) {
    WCHAR LocaleName[LOCALE_NAME_MAX_LENGTH];

    if (p32_winlocale_resolve_locale_name (from->LocaleName, LocaleName, LOCALE_NAME_MAX_LENGTH) != 0) {
      if (wcscmp (LocaleName, to->LocaleName) == 0) {
        return true;
      }

      fwprintf (stdout, L"UNRESOLVED: %s (%s) -> %s\n", from->LocaleName, LocaleName, to->LocaleName);
    } else {
      fwprintf (stdout, L"UNRESOLVED: %s -> %s\n", from->LocaleName, to->LocaleName);
    }

    return wcsncmp (from->LocaleName, to->LocaleName, compare) == 0;
  }

  return wcscmp (from->LocaleName, to->LocaleName) == 0;
}
#endif /* Locale Names */

/**
 * Pointer to `IsEqualLCIDLocale` or `IsEqualLNLocale`.
 */
static FuncIsEqualLocale IsEqualLocale;

/**
 * Pointer to `IsKnownLCIDFailure` or `IsKnownLNFailure`.
 */
static FuncIsKnownFailure IsKnownFailure;

/**
 * Resolve locale from `ll` and `cc`.
 *
 * Resolved locale must be equivalent to `originalLocale`.
 */
static void DoTest (Locale *originaLocale, LPCWSTR ll, LPCWSTR cc) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  LPWSTR    LocaleString   = NULL;
  LocaleMap localeMap      = {0};
  Locale    resolvedLocale = {0};

  assert (p32_private_aswprintf (&LocaleString, heap, L"%s_%s", ll, cc) != -1);
  assert (p32_locale_map (&localeMap, LocaleString, heap));

  if (p32_winlocale_resolve (&resolvedLocale, heap, &localeMap)) {
    assert (resolvedLocale.Type != LocaleType_Invalid);

    if (!IsEqualLocale (originaLocale, &resolvedLocale) && !IsKnownFailure (originaLocale, &resolvedLocale)) {
      exit_code = EXIT_FAILURE;
      fwprintf (stderr, L"FAIL: %s -> %s -> %s\n", originaLocale->LocaleName, LocaleString, resolvedLocale.LocaleName);
    }

    p32_winlocale_destroy (&resolvedLocale, heap);
    assert (resolvedLocale.Type == LocaleType_Invalid);
  } else {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"ERROR: %s (%s)\n", originaLocale->LocaleName, LocaleString);
  }

  assert (HeapFree (heapHandle, 0, LocaleString));
}

/**
 * Resolve locale from `originalLocale->LocaleName`.
 *
 * Resolved locale must be equivalent to `originalLocale`.
 */
static void DoTestLocaleName (Locale *originalLocale) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  LocaleMap localeMap      = {0};
  Locale    resolvedLocale = {0};

  assert (p32_locale_map (&localeMap, originalLocale->LocaleName, heap));

  if (p32_winlocale_resolve (&resolvedLocale, heap, &localeMap)) {
    assert (resolvedLocale.Type != LocaleType_Invalid);

    if (!IsEqualLocale (originalLocale, &resolvedLocale) && !IsKnownFailure (originalLocale, &resolvedLocale)) {
      exit_code = EXIT_FAILURE;
      fwprintf (stderr, L"FAIL: %s -> %s\n", originalLocale->LocaleName, resolvedLocale.LocaleName);
    }

    p32_winlocale_destroy (&resolvedLocale, heap);
    assert (resolvedLocale.Type == LocaleType_Invalid);
  } else {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"ERROR: %s\n", originalLocale->LocaleName);
  }
}

static bool __cdecl Test (Locale *locale) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  DoTestLocaleName (locale);

  /**
   * Language name
   */
  LPWSTR LanguageName = NULL;

  /**
   * Country name
   */
  LPWSTR CountryName = NULL;

  /**
   * ISO 639 language code
   */
  LPWSTR Ll = NULL;

  /**
   * ISO 3166 country code
   */
  LPWSTR Cc = NULL;

  /**
   * ISO 639-2 language code
   */
  LPWSTR Lll = NULL;

  /**
   * ISO 3166-2 country code
   */
  LPWSTR Ccc = NULL;

  assert (p32_winlocale_get_language_name (&LanguageName, heap, locale));
  assert (p32_winlocale_get_country_name (&CountryName, heap, locale));
  assert (p32_winlocale_get_language_code (&Ll, heap, locale));
  assert (p32_winlocale_get_country_code (&Cc, heap, locale));

  if (P32_WINNT_CHECK (P32_WINNT_VISTA, WindowsNtVista)) {
    LocaleInfoRequest infoRequest = {0};

    infoRequest.Info    = LOCALE_SISO639LANGNAME2;
    infoRequest.OutputW = &Lll;

    assert (p32_winlocale_get_locale_info (&infoRequest, heap, locale));

    infoRequest.Info    = LOCALE_SISO3166CTRYNAME2;
    infoRequest.OutputW = &Ccc;

    assert (p32_winlocale_get_locale_info (&infoRequest, heap, locale));
  }

  DoTest (locale, LanguageName, CountryName);
  // DoTest (locale, LanguageName, Cc);
  // DoTest (locale, Ll, Cc);
  // DoTest (locale, Ll, CountryName);

  if (P32_WINNT_CHECK (P32_WINNT_VISTA, WindowsNtVista)) {
    // DoTest (locale, LanguageName, Ccc);
    // DoTest (locale, Ll, Ccc);
    // DoTest (locale, Lll, Ccc);
    // DoTest (locale, Lll, CountryName);
    // DoTest (locale, Lll, Cc);
  }

  assert (HeapFree (heapHandle, 0, LanguageName));
  assert (HeapFree (heapHandle, 0, CountryName));
  assert (HeapFree (heapHandle, 0, Ll));
  assert (HeapFree (heapHandle, 0, Cc));

  if (P32_WINNT_CHECK (P32_WINNT_VISTA, WindowsNtVista)) {
    assert (HeapFree (heapHandle, 0, Lll));
    assert (HeapFree (heapHandle, 0, Ccc));
  }

  return true;
}

int main (void) {
  p32_test_init ();

#if (P32_LOCALE_API & P32_LOCALE_API_LN)
  IsKnownFailure = IsKnownLNFailure;
  IsEqualLocale  = IsEqualLNLocale;
#else
  IsKnownFailure = IsKnownLCIDFailure;
  IsEqualLocale  = IsEqualLCIDLocale;
#endif

  assert (IsKnownFailure != NULL);
  assert (IsEqualLocale != NULL);

  /**
   * Test all supported locales.
   */
  p32_locale_test_func1 (Test);

  return exit_code;
}
