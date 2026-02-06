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

typedef struct {
#if P32_LOCALE_NAMES
#define KNOWN_FAILURE(name_from, name_to, langid, sublang_from, sublang_to) name_from, name_to
  const wchar_t *From;
  const wchar_t *To;
#else
#define KNOWN_FAILURE(name_from, name_to, langid, sublang_from, sublang_to)                                            \
  MAKELCID (MAKELANGID (langid, sublang_from), SORT_DEFAULT), MAKELCID (MAKELANGID (langid, sublang_to), SORT_DEFAULT)
  LCID From;
  LCID To;
#endif
} KnownFailure;

#if P32_LOCALE_NAMES
#define EQUAL_LOCALE(l, n) (wcscmp (l->LocaleName, n) == 0)
#else
#define EQUAL_LOCALE(l, i) (l->LocaleId == i)
#endif

KnownFailure KnownFailures[] = {
#if P32_LOCALE_NAMES
  /**
   * German_Germany
   */
  {KNOWN_FAILURE (L"de-DE_phoneb", L"de-DE", 0x00, 0x00, 0x00)},
  /**
   * Spanish_Spain
   */
  {KNOWN_FAILURE (L"es-ES_tradnl", L"es-ES", 0x00, 0x00, 0x00)},
  /**
   * Hungarian_Hungary
   */
  {KNOWN_FAILURE (L"hu-HU_technl", L"hu-HU", 0x00, 0x00, 0x00)},
  /**
   * Japanese_Japan
   */
  {KNOWN_FAILURE (L"ja-JP_radstr", L"ja-JP", 0x00, 0x00, 0x00)},
  /**
   * Georgian_Georgia
   */
  {KNOWN_FAILURE (L"ka-GE_modern", L"ka-GE", 0x00, 0x00, 0x00)},
  /**
   * Mongolian_Mongolia
   */
  {KNOWN_FAILURE (L"mn-MN", L"mn-Cyrl-MN", 0x00, 0x00, 0x00)},
  /**
   * Manipuri (Bangla)_India
   */
  {KNOWN_FAILURE (L"mni-IN", L"mni-Beng-IN", 0x00, 0x00, 0x00)},
  /**
   * Norwegian_Norway -> Norwegian Bokmål_Norway
   */
  {KNOWN_FAILURE (L"no", L"nb-NO", 0x00, 0x00, 0x00)},
  /**
   * Punjabi_India
   */
  {KNOWN_FAILURE (L"pa-Guru", L"pa-IN", 0x00, 0x00, 0x00)},
  /**
   * Chinese (Simplified)_China
   */
  {KNOWN_FAILURE (L"zh-CN", L"zh-Hans-CN", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-CN_phoneb", L"zh-Hans-CN", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-CN_stroke", L"zh-Hans-CN", 0x00, 0x00, 0x00)},
  /**
   * Chinese (Simplified)_Singapore
   */
  {KNOWN_FAILURE (L"zh-SG", L"zh-Hans-SG", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-SG_phoneb", L"zh-Hans-SG", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-SG_stroke", L"zh-Hans-SG", 0x00, 0x00, 0x00)},
  /**
   * Chinese (Traditional)_Hong Kong SAR
   */
  {KNOWN_FAILURE (L"zh-HK", L"zh-Hant-HK", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-HK_radstr", L"zh-Hant-HK", 0x00, 0x00, 0x00)},
  /**
   * Chinese (Traditional)_Macao SAR
   */
  {KNOWN_FAILURE (L"zh-MO", L"zh-Hant-MO", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-MO_radstr", L"zh-Hant-MO", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-MO_stroke", L"zh-Hant-MO", 0x00, 0x00, 0x00)},
  /**
   * Chinese (Traditional)_Taiwan
   */
  {KNOWN_FAILURE (L"zh-TW", L"zh-Hant-TW", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-TW_pronun", L"zh-Hant-TW", 0x00, 0x00, 0x00)},
  {KNOWN_FAILURE (L"zh-TW_radstr", L"zh-Hant-TW", 0x00, 0x00, 0x00)},
#else
  /**
   * Spanish_Spain (0A 01 -> 0A 03)
   */
  {KNOWN_FAILURE (L"es-ES", L"es-ES", LANG_SPANISH, SUBLANG_SPANISH, SUBLANG_SPANISH_MODERN)}
#endif
};

static bool IsKnownFailure (Locale *from, Locale *to) {
  for (size_t i = 0; i < _countof (KnownFailures); ++i) {
    if (EQUAL_LOCALE (from, KnownFailures[i].From) && EQUAL_LOCALE (to, KnownFailures[i].To)) {
      return true;
    }
  }

  return false;
}

/**
 * Compare two `Locale` object.
 *
 * Return `true` if they represent the same locale. Otherwise, return `false`
 */
static bool IsEqualLocale (Locale *from, Locale *to) {
#if P32_LOCALE_NAMES
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
#if _WIN32_WINNT >= _WIN32_WINNT_WIN7
    WCHAR LocaleName[LOCALE_NAME_MAX_LENGTH];

    assert (ResolveLocaleName (from->LocaleName, LocaleName, LOCALE_NAME_MAX_LENGTH) != 0);

    if (wcscmp (LocaleName, to->LocaleName) == 0) {
      return true;
    }

    fwprintf (stdout, L"UNRESOLVED: %s (%s) -> %s\n", from->LocaleName, LocaleName, to->LocaleName);
#else
    fwprintf (stdout, L"UNRESOLVED: %s -> %s\n", from->LocaleName, to->LocaleName);
#endif
    return wcsncmp (from->LocaleName, to->LocaleName, compare) == 0;
  } else {
    return wcscmp (from->LocaleName, to->LocaleName) == 0;
  }
#else /* !P32_LOCALE_NAMES */
  LANGID fromLangId        = LANGIDFROMLCID (from->LocaleId);
  LANGID fromPrimaryLangId = PRIMARYLANGID (fromLangId);
  LANGID fromSubLangId     = SUBLANGID (fromLangId);

  LANGID toLangId        = LANGIDFROMLCID (to->LocaleId);
  LANGID toPrimaryLangId = PRIMARYLANGID (toLangId);
  LANGID toSubLangId     = SUBLANGID (toLangId);

  return fromPrimaryLangId == toPrimaryLangId && fromSubLangId == toSubLangId;
#endif
}

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
    if (!IsEqualLocale (originaLocale, &resolvedLocale) && !IsKnownFailure (originaLocale, &resolvedLocale)) {
      exit_code = EXIT_FAILURE;
#if P32_LOCALE_NAMES
      fwprintf (stderr, L"FAIL: %s -> %s -> %s\n", originaLocale->LocaleName, LocaleString, resolvedLocale.LocaleName);
#else
      fwprintf (stderr, L"FAIL: %s -> %s\n", LocaleString, resolvedLocale.LocaleName);
#endif
    }

    p32_winlocale_destroy (&resolvedLocale, heap);
  } else {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"ERROR: %s (%s)\n", originaLocale->LocaleName, LocaleString);
  }

  assert (HeapFree (heapHandle, 0, LocaleString));
}

#if P32_LOCALE_NAMES
/**
 * Resolve locale from `originalLocale->LocaleName.
 *
 * Resolved locale must be equivalent to `originalLocale`.
 */
static void DoTestLocaleName (Locale *originalLocale) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  LocaleMap localeMap      = {0};
  Locale    resolvedLocale = {0};

  assert (p32_locale_map (&localeMap, originalLocale->LocaleName, heap));
  assert (p32_winlocale_resolve (&resolvedLocale, heap, &localeMap));

  if (!IsEqualLocale (originalLocale, &resolvedLocale) && !IsKnownFailure (originalLocale, &resolvedLocale)) {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"%s -> %s\n", originalLocale->LocaleName, resolvedLocale.LocaleName);
  }

  p32_winlocale_destroy (&resolvedLocale, heap);
}
#endif

static bool __cdecl Test (Locale *locale) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

#if P32_LOCALE_NAMES
  DoTestLocaleName (locale);
#endif

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

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
  /**
   * ISO 639-2 language code
   */
  LPWSTR Lll = NULL;

  /**
   * ISO 3166-2 country code
   */
  LPWSTR Ccc = NULL;
#endif

  assert (p32_winlocale_getinfo (&LanguageName, heap, locale, LOCALE_SENGLANGUAGE));
  assert (p32_winlocale_getinfo (&CountryName, heap, locale, LOCALE_SENGCOUNTRY));
  assert (p32_winlocale_getinfo (&Ll, heap, locale, LOCALE_SISO639LANGNAME));
  assert (p32_winlocale_getinfo (&Cc, heap, locale, LOCALE_SISO3166CTRYNAME));
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
  assert (p32_winlocale_getinfo (&Lll, heap, locale, LOCALE_SISO639LANGNAME2));
  assert (p32_winlocale_getinfo (&Ccc, heap, locale, LOCALE_SISO3166CTRYNAME2));
#endif

  DoTest (locale, LanguageName, CountryName);
  // DoTest (locale, LanguageName, Cc);
  // DoTest (locale, Ll, Cc);
  // DoTest (locale, Ll, CountryName);
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
  // DoTest (locale, LanguageName, Ccc);
  // DoTest (locale, Ll, Ccc);
  // DoTest (locale, Lll, Ccc);
  // DoTest (locale, Lll, CountryName);
  // DoTest (locale, Lll, Cc);
#endif

  assert (HeapFree (heapHandle, 0, LanguageName));
  assert (HeapFree (heapHandle, 0, CountryName));
  assert (HeapFree (heapHandle, 0, Ll));
  assert (HeapFree (heapHandle, 0, Cc));
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
  assert (HeapFree (heapHandle, 0, Lll));
  assert (HeapFree (heapHandle, 0, Ccc));
#endif

  return true;
}

int main (void) {
  p32_test_init ();

  /**
   * Test all supported locales.
   */
  p32_locale_test_func1 (Test);

  return exit_code;
}
