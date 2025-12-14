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

#define P32_NEWLOCALE_TEST_ASCII  1
#define P32_NEWLOCALE_TEST_ANSI   2
#define P32_NEWLOCALE_TEST_OEM    3
#define P32_NEWLOCALE_TEST_MAC    4
#define P32_NEWLOCALE_TEST_EBCDIC 5
#define P32_NEWLOCALE_TEST_UTF8   6

#define P32_FLAG_ASCII   1
#define P32_FLAG_ANSI    (1 << 1)
#define P32_FLAG_OEM     (1 << 2)
#define P32_FLAG_MAC     (1 << 3)
#define P32_FLAG_EBCDIC  (1 << 4)
#define P32_FLAG_UNICODE (1 << 5)

#if P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_ASCII
#define DO_TEST(ansi, oem, mac) 1
#define TEST_FAILED(status)     (void) 0
#define P32_FLAG                P32_FLAG_ASCII
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_ANSI
#define DO_TEST(ansi, oem, mac) (ansi != CP_ACP && oem != CP_OEMCP && mac != CP_MACCP)
#define TEST_FAILED(status)     status = EXIT_FAILURE
#define P32_FLAG                P32_FLAG_ANSI
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_OEM
#define DO_TEST(ansi, oem, mac) (ansi != CP_ACP && oem != CP_OEMCP && mac != CP_MACCP)
#define TEST_FAILED(status)     status = EXIT_FAILURE
#define P32_FLAG                P32_FLAG_OEM
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_MAC
#define DO_TEST(ansi, oem, mac) (ansi != CP_ACP && oem != CP_OEMCP && mac != CP_MACCP)
#define TEST_FAILED(status)     (void) 0
#define P32_FLAG                P32_FLAG_MAC
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_EBCDIC
#define DO_TEST(ansi, oem, mac) (ansi != CP_ACP && oem != CP_OEMCP && mac != CP_MACCP)
#define TEST_FAILED(status)     (void) 0
#define P32_FLAG                P32_FLAG_EBCDIC
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_UTF8
#define DO_TEST(ansi, oem, mac) 1
#define TEST_FAILED(status)     status = EXIT_FAILURE
#define P32_FLAG                P32_FLAG_UNICODE
#else
#error P32_NEWLOCALE_TEST is set to invalid value
#endif

/**
 * Test's exit code.
 */
static int exit_code = EXIT_SUCCESS;

typedef struct {
#if P32_LOCALE_NAMES
  LPCWSTR Locale;
#else
  LCID Locale;
#endif
  DWORD Flags;
} ExpectedFailure;

#if P32_LOCALE_NAMES
#define EXPECTED_FAILURE(locale_name, langid, sublangid) TEXT (locale_name)
#define EQUAL_LOCALE(l1, l2)                             wcscmp (l1->LocaleName, l2.Locale) == 0
#else
#define EXPECTED_FAILURE(locale_name, langid, sublangid) MAKELCID (MAKELANGID (langid, sublangid), SORT_DEFAULT)
#define EQUAL_LOCALE(l1, l2)                             l1->LocaleId == l2.Locale
#endif

/**
 * Creation of `locale_t` object for the following locales with their
 * default ANSI/OEM code pages is known to fail.
 */
static ExpectedFailure ExpectedFailures[] = {
#if P32_LOCALE_NAMES
  /**
   * Locale: Arabic_world
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-001",      0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Djibouti
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-DJ",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Eritrea
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-ER",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Israel
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-IL",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Comoros
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-KM",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Mauritania
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-MR",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Palestinian Authority
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-PS",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Sudan
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-SD",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Somalia
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-SO",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_South Sudan
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-SS",       0x00,                 0x00),                                    P32_FLAG_OEM                },
  /**
   * Locale: Arabic_Chad
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ar-TD",       0x00,                 0x00),                                    P32_FLAG_OEM                },
#endif
  /**
   * Locale: Azerbaijani (Cyrillic)_Azerbaijan
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("az",          LANG_AZERI,           SUBLANG_AZERBAIJANI_AZERBAIJAN_CYRILLIC), P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("az-Cyrl",     LANG_AZERI,           SUBLANG_AZERBAIJANI_AZERBAIJAN_CYRILLIC), P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("az-Cyrl-AZ",  LANG_AZERI,           SUBLANG_AZERBAIJANI_AZERBAIJAN_CYRILLIC), P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Azerbaijani (Latin)_Azerbaijan
   * ACP: 1254
   * OCP: 857
   */
  {EXPECTED_FAILURE ("az-Latn",     LANG_AZERI,           SUBLANG_AZERBAIJANI_AZERBAIJAN_LATIN),    P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("az-Latn-AZ",  LANG_AZERI,           SUBLANG_AZERBAIJANI_AZERBAIJAN_LATIN),    P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Bashkir_Russia
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("ba",          LANG_BASHKIR,         SUBLANG_BASHKIR_RUSSIA),                  P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ba-RU",       LANG_BASHKIR,         SUBLANG_BASHKIR_RUSSIA),                  P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Belarusian_Belarus
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("be",          LANG_BELARUSIAN,      SUBLANG_BELARUSIAN_BELARUS),              P32_FLAG_OEM                },
  {EXPECTED_FAILURE ("be-BY",       LANG_BELARUSIAN,      SUBLANG_BELARUSIAN_BELARUS),              P32_FLAG_OEM                },
  /**
   * Locale: Persian_Iran
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("fa",          LANG_PERSIAN,         SUBLANG_PERSIAN_IRAN),                    P32_FLAG_OEM                },
  {EXPECTED_FAILURE ("fa-IR",       LANG_PERSIAN,         SUBLANG_PERSIAN_IRAN),                    P32_FLAG_OEM                },
  /**
   * Locale: Persian_Afghanistan
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("fa-AF",       LANG_DARI,            SUBLANG_DARI_AFGHANISTAN),                P32_FLAG_OEM                },
  /**
   * Locale: Fulah (Latin)_Senegal
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("ff",          LANG_FULAH,           SUBLANG_FULAH_SENEGAL),                   P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ff-Latn",     LANG_FULAH,           SUBLANG_FULAH_SENEGAL),                   P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ff-Latn-SN",  LANG_FULAH,           SUBLANG_FULAH_SENEGAL),                   P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Fula (Latin)_Nigeria
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("ff-Latn-NG",  LANG_FULAH,           0x01),                                    P32_FLAG_ANSI | P32_FLAG_OEM},
#if P32_LOCALE_NAMES
  /**
   * Locale: Fula (Latin)_Cameroon
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("ff-Latn-CM",  0x00,                 0x00),                                    P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Fula (Latin)_Ghana
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("ff-Latn-GN",  0x00,                 0x00),                                    P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Fula (Latin)_Mauritania
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("ff-Latn-MR",  0x00,                 0x00),                                    P32_FLAG_ANSI | P32_FLAG_OEM},
#endif
  /**
   * Locale: haw-US
   * ACP: 1252
   * OCP: 437
   */
  {EXPECTED_FAILURE ("haw",         LANG_HAWAIIAN,        SUBLANG_HAWAIIAN_US),                     P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("haw-US",      LANG_HAWAIIAN,        SUBLANG_HAWAIIAN_US),                     P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Igbo_Nigeria
   * ACP: 1252
   * OCP: 437
   */
  {EXPECTED_FAILURE ("ig",          LANG_IGBO,            SUBLANG_IGBO_NIGERIA),                    P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ig-NG",       LANG_IGBO,            SUBLANG_IGBO_NIGERIA),                    P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Central Kurdish_Iraq
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ku",          LANG_CENTRAL_KURDISH, SUBLANG_CENTRAL_KURDISH_IRAQ),            P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ku-Arab",     LANG_CENTRAL_KURDISH, SUBLANG_CENTRAL_KURDISH_IRAQ),            P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ku-Arab-IQ",  LANG_CENTRAL_KURDISH, SUBLANG_CENTRAL_KURDISH_IRAQ),            P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Kyrgyz_Kyrgyzstan
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("ky",          LANG_KYRGYZ,          SUBLANG_KYRGYZ_KYRGYZSTAN),               P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ky-KG",       LANG_KYRGYZ,          SUBLANG_KYRGYZ_KYRGYZSTAN),               P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Macedonian_North Macedonia
   * ACP: ???
   * OCP: 720
   */
  {EXPECTED_FAILURE ("mk",          LANG_MACEDONIAN,      SUBLANG_MACEDONIAN_MACEDONIA),            P32_FLAG_OEM                },
  {EXPECTED_FAILURE ("mk-MK",       LANG_MACEDONIAN,      SUBLANG_MACEDONIAN_MACEDONIA),            P32_FLAG_OEM                },
  /**
   * Locale: Mongolian_Mongolia
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("mn",          LANG_MONGOLIAN,       SUBLANG_MONGOLIAN_CYRILLIC_MONGOLIA),     P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("mn-Cyrl",     LANG_MONGOLIAN,       SUBLANG_MONGOLIAN_CYRILLIC_MONGOLIA),     P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("mn-MN",       LANG_MONGOLIAN,       SUBLANG_MONGOLIAN_CYRILLIC_MONGOLIA),     P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Punjabi_Pakistan
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("pa-Arab",     LANG_PUNJABI,         SUBLANG_PUNJABI_PAKISTAN),                P32_FLAG_OEM                },
  {EXPECTED_FAILURE ("pa-Arab-PK",  LANG_PUNJABI,         SUBLANG_PUNJABI_PAKISTAN),                P32_FLAG_OEM                },
  /**
   * Locale: Romanian_Romania
   * ACP: 1250
   * OCP: 852
   */
  {EXPECTED_FAILURE ("ro",          LANG_ROMANIAN,        SUBLANG_ROMANIAN_ROMANIA),                P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ro-RO",       LANG_ROMANIAN,        SUBLANG_ROMANIAN_ROMANIA),                P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Romanian_Moldova
   * ACP: 1250
   * OCP: 852
   */
  {EXPECTED_FAILURE ("ro-MD",       LANG_ROMANIAN,        0x02),                                    P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Sakha_Russia
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("sah",         LANG_SAKHA,           SUBLANG_SAKHA_RUSSIA),                    P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("sah-RU",      LANG_SAKHA,           SUBLANG_SAKHA_RUSSIA),                    P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Sindhi_Pakistan
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("sd",          LANG_SINDHI,          SUBLANG_SINDHI_PAKISTAN),                 P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("sd-Arab",     LANG_SINDHI,          SUBLANG_SINDHI_PAKISTAN),                 P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("sd-Arab-PK",  LANG_SINDHI,          SUBLANG_SINDHI_PAKISTAN),                 P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Northern Sami_Norway
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("se",          LANG_SAMI,            SUBLANG_SAMI_NORTHERN_NORWAY),            P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("se-NO",       LANG_SAMI,            SUBLANG_SAMI_NORTHERN_NORWAY),            P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Sami (Northern)_Finland
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("se-FI",       LANG_SAMI,            SUBLANG_SAMI_NORTHERN_FINLAND),           P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Sami (Northern)_Sweden
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("se-SE",       LANG_SAMI,            SUBLANG_SAMI_NORTHERN_SWEDEN),            P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Sami (Inari)_Finland
   * ACP: 1252
   * OCP: 850
   */
  {EXPECTED_FAILURE ("smn",         LANG_SAMI,            SUBLANG_SAMI_INARI_FINLAND),              P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("smn-FI",      LANG_SAMI,            SUBLANG_SAMI_INARI_FINLAND),              P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Tajik (Cyrillic)_Tajikistan
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("tg",          LANG_TAJIK,           SUBLANG_TAJIK_TAJIKISTAN),                P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("tg-Cyrl",     LANG_TAJIK,           SUBLANG_TAJIK_TAJIKISTAN),                P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("tg-Cyrl-TJ",  LANG_TAJIK,           SUBLANG_TAJIK_TAJIKISTAN),                P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Tatar_Russia
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("tt",          LANG_TATAR,           SUBLANG_TATAR_RUSSIA),                    P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("tt-RU",       LANG_TATAR,           SUBLANG_TATAR_RUSSIA),                    P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Central Atlas Tamazight (Arabic)_Morocco
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("tzm-Arab",    LANG_TAMAZIGHT,       0x01),                                    P32_FLAG_OEM                },
  {EXPECTED_FAILURE ("tzm-Arab-MA", LANG_TAMAZIGHT,       0x01),                                    P32_FLAG_OEM                },
  /**
   * Locale: Uyghur_China
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ug",          LANG_UIGHUR,          SUBLANG_UIGHUR_PRC),                      P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("ug-CN",       LANG_UIGHUR,          SUBLANG_UIGHUR_PRC),                      P32_FLAG_ANSI | P32_FLAG_OEM},
  /**
   * Locale: Ukrainian_Ukraine
   * ACP: 1251
   * OCP: 866
   */
  {EXPECTED_FAILURE ("uk",          LANG_UKRAINIAN,       SUBLANG_UKRAINIAN_UKRAINE),               P32_FLAG_OEM                },
  {EXPECTED_FAILURE ("uk-UA",       LANG_UKRAINIAN,       SUBLANG_UKRAINIAN_UKRAINE),               P32_FLAG_OEM                },
  /**
   * Locale: Urdu_India
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ur",          LANG_URDU,            SUBLANG_URDU_INDIA),                      P32_FLAG_OEM                },
  {EXPECTED_FAILURE ("ur-IN",       LANG_URDU,            SUBLANG_URDU_INDIA),                      P32_FLAG_OEM                },
  /**
   * Locale: Urdu_Pakistan
   * ACP: 1256
   * OCP: 720
   */
  {EXPECTED_FAILURE ("ur-PK",       LANG_URDU,            SUBLANG_URDU_PAKISTAN),                   P32_FLAG_OEM                },
  /**
   * Locale: Yoruba_Nigeria
   * ACP: 1252
   * OCP: 437
   */
  {EXPECTED_FAILURE ("yo",          LANG_YORUBA,          SUBLANG_YORUBA_NIGERIA),                  P32_FLAG_ANSI | P32_FLAG_OEM},
  {EXPECTED_FAILURE ("yo-NG",       LANG_YORUBA,          SUBLANG_YORUBA_NIGERIA),                  P32_FLAG_ANSI | P32_FLAG_OEM},
#if P32_LOCALE_NAMES
  /**
   * Locale: Yoruba_Benin
   * ACP: 1252
   * OCP: 437
   */
  {EXPECTED_FAILURE ("yo-BJ",       0x00,                 0x00),                                    P32_FLAG_ANSI | P32_FLAG_OEM},
#endif
};

/**
 * Return non-zero if creation of `locale_t` object for specified `locale` is
 * expected to fail.
 */
static DWORD IsExpectedFailure (Locale *locale) {
  for (size_t i = 0; i < _countof (ExpectedFailures); ++i) {
    if (EQUAL_LOCALE (locale, ExpectedFailures[i])) {
      return ExpectedFailures[i].Flags & P32_FLAG;
    }
  }

  return FALSE;
}

/**
 * Try to create `locale_t` object for `localeString`.
 */
static void TestLocale (Locale *locale, LPCSTR localeString) {
  DWORD    expectedFailure = IsExpectedFailure (locale);
  locale_t testLocale      = newlocale (LC_ALL_MASK, localeString, NULL);

  if (testLocale == NULL) {
    if (expectedFailure) {
      fwprintf (stderr, L"XFAIL: %hs\n", localeString);
    } else {
      TEST_FAILED (exit_code);
      fwprintf (stderr, L"FAIL: %hs\n", localeString);
    }
  } else {
    if (expectedFailure) {
      TEST_FAILED (exit_code);
      fwprintf (stderr, L"XPASS: %hs\n", localeString);
    } else {
      fwprintf (stdout, L"PASS: %hs\n", localeString);
    }

    freelocale (testLocale);
  }
}

static bool __cdecl Test (Locale *locale) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

#if P32_LOCALE_NAMES
  if (locale->LocaleName[0] == L'\0') {
    return true;
  } else if (wcscmp (locale->LocaleName, L"x-IV_mathan") == 0) {
    return true;
  }

  switch (wcslen (locale->LocaleName)) {
    case 2:
    case 3:
      return true;
    case 7:
      if (locale->LocaleName[2] == L'-') {
        return true;
      }
      break;
    case 8:
      if (locale->LocaleName[3] == L'-') {
        return true;
      }
      break;
  }
#else
  /**
   * Language name
   */
  LPWSTR LanguageName = NULL;

  /**
   * Country name
   */
  LPWSTR LanguageCode = NULL;

  /**
   * ISO 639 language code
   */
  LPWSTR Ll = NULL;

  /**
   * ISO 3166 country code
   */
  LPWSTR Cc = NULL;

  assert (p32_winlocale_getinfo (&LanguageName, heap, locale, LOCALE_SENGLANGUAGE));
  assert (p32_winlocale_getinfo (&LanguageCode, heap, locale, LOCALE_SENGCOUNTRY));
  assert (p32_winlocale_getinfo (&Ll, heap, locale, LOCALE_SISO639LANGNAME));
  assert (p32_winlocale_getinfo (&Cc, heap, locale, LOCALE_SISO3166CTRYNAME));
#endif

  UINT ansiCodePage   = 0;
  UINT oemCodePage    = 0;
  UINT macCodePage    = 0;
  UINT ebcdicCodePage = 0;

  assert (p32_winlocale_getinfo_number (&ansiCodePage, locale, LOCALE_IDEFAULTANSICODEPAGE));
  assert (p32_winlocale_getinfo_number (&oemCodePage, locale, LOCALE_IDEFAULTCODEPAGE));
  assert (p32_winlocale_getinfo_number (&macCodePage, locale, LOCALE_IDEFAULTMACCODEPAGE));
  assert (p32_winlocale_getinfo_number (&ebcdicCodePage, locale, LOCALE_IDEFAULTEBCDICCODEPAGE));

  UINT codePage = 0;

  bool doTest        = DO_TEST (ansiCodePage, oemCodePage, macCodePage);
  bool usableCharset = false;

#if P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_ASCII
  codePage      = P32_CODEPAGE_ASCII;
  usableCharset = true;
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_ANSI
  codePage = ansiCodePage;

  if (codePage != CP_ACP) {
    usableCharset = p32_charset_usable (codePage, 0, 0);
  }
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_OEM
  codePage = oemCodePage;

  if (codePage != CP_OEMCP) {
    usableCharset = p32_charset_usable (codePage, 0, 0);
  }
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_MAC
  codePage = macCodePage;

  if (codePage != CP_MACCP) {
    usableCharset = p32_charset_usable (codePage, 0, 0);
  }
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_EBCDIC
  codePage      = ebcdicCodePage;
  usableCharset = p32_charset_usable (codePage, 0, 0);
#elif P32_NEWLOCALE_TEST == P32_NEWLOCALE_TEST_UTF8
  codePage      = CP_UTF8;
  usableCharset = true;
#endif

  if (usableCharset && doTest) {
    LPSTR localeString = NULL;

#if P32_LOCALE_NAMES
    assert (p32_private_asprintf (&localeString, heap, L"%s.%u", locale->LocaleName, codePage) != -1);
#else
    if (p32_private_asprintf (&localeString, heap, L"%s_%s.%u", LanguageName, LanguageCode, codePage) == -1) {
      assert (p32_private_asprintf (&localeString, heap, L"%s_%s.%u", Ll, Cc, codePage) != -1);
    }
#endif

    TestLocale (locale, localeString);

    assert (HeapFree (heapHandle, 0, localeString));
  } else if (!usableCharset && doTest) {
#if P32_LOCALE_NAMES
    _RPTW2 (_CRT_WARN, L"UNSUPPORTED: %s.%u\n", locale->LocaleName, codePage);
#else
    _RPTW3 (_CRT_WARN, L"UNSUPPORTED: %s-%s.%u\n", Ll, Cc, codePage);
#endif
  } else {
#if P32_LOCALE_NAMES
    _RPTW1 (_CRT_WARN, L"SKIP: %s\n", locale->LocaleName);
#else
    _RPTW2 (_CRT_WARN, L"SKIP: %s-%s\n", Ll, Cc);
#endif
  }

#if P32_LOCALE_NAMES
#else
  assert (HeapFree (heapHandle, 0, LanguageName));
  assert (HeapFree (heapHandle, 0, LanguageCode));
  assert (HeapFree (heapHandle, 0, Ll));
  assert (HeapFree (heapHandle, 0, Cc));
#endif

  return true;
}

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, "en-US") != NULL);

  /**
   * Test all supported locales.
   */
  p32_locale_test_func1 (Test);

  return exit_code;
}
