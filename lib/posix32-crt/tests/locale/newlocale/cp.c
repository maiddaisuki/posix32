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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Attempt to create `locale_t` with each code page supported by the
 * operating system.
 */

static int exit_code = EXIT_SUCCESS;

static bool __cdecl Test (uint32_t codePage) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  if (!p32_charset_usable (codePage, 0, 0)) {
    return true;
  }

  char    *localeStringA = NULL;
  wchar_t *localeStringW = NULL;

  switch (codePage) {
#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    case 708:   /* ASMO-708   */
    case 720:   /* OEM Arabic */
    case 1256:  /* CP1256     */
    case 10004: /* MAC Arabic */
    case 28596: /* ISO-8859-6 */
      assert (p32_private_aswprintf (&localeStringW, heap, L"ar-SA.%u", codePage) != -1);
      break;
#endif
    case 852:   /* OEM Latin 2        */
    case 870:   /* IBM Latin 2        */
    case 1250:  /* CP1250             */
    case 10029: /* MAC Central Europe */
    case 28592: /* ISO-8859-2         */
      assert (p32_private_aswprintf (&localeStringW, heap, L"cs-CZ.%u", codePage) != -1);
      break;
    case 1142:  /* IBM Denmark/Norway (+ Euro) */
    case 20277: /* IBM Denmark/Norway          */
      assert (p32_private_aswprintf (&localeStringW, heap, L"da-DK.%u", codePage) != -1);
      break;
    case 850:   /* OEM Latin 1          */
    case 858:   /* OEM Latin 1 (+ Euro) */
    case 1047:  /* IBM Latin 1          */
    case 20924: /* IBM Latin 1 (+ Euro) */
    case 20273: /* IBM Germany          */
    case 1141:  /* IBM Germany (+ Euro) */
    case 1252:  /* CP1252 (Latin 1)     */
    case 10000: /* Mac Roman            */
    case 28591: /* ISO-8859-1 (Latin 1) */
    case 28593: /* ISO-8859-3 (Latin 3) */
      assert (p32_private_aswprintf (&localeStringW, heap, L"de-DE.%u", codePage) != -1);
      break;
    case 737:   /* OEM Greek        */
    case 869:   /* OEM Modern Greek */
    case 875:   /* IBM Greek        */
    case 1253:  /* CP1253           */
    case 10006: /* MAC Greek        */
    case 28597: /* ISO-8859-7       */
      assert (p32_private_aswprintf (&localeStringW, heap, L"el-GR.%u", codePage) != -1);
      break;
    case 37:   /* IBM U.S./Canada            */
    case 1140: /* IBM U.S./Canada (+ Euro)   */
    case 500:  /* IBM International          */
    case 1148: /* IBM International (+ Euro) */
      assert (p32_private_aswprintf (&localeStringW, heap, L"en-CA.%u", codePage) != -1);
      break;
    case 20285: /* IBM United Kingdom          */
    case 1146:  /* IBM United Kingdom (+ Euro) */
      assert (p32_private_aswprintf (&localeStringW, heap, L"en-GB.%u", codePage) != -1);
      break;
    case 20284: /* IBM Latin America/Spain          */
    case 1145:  /* IBM Latin America/Spain (+ Euro) */
      assert (p32_private_aswprintf (&localeStringW, heap, L"es-ES.%u", codePage) != -1);
      break;
#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    case 1257:  /* CP1257      */
    case 28603: /* IBM-8859-13 */
      assert (p32_private_aswprintf (&localeStringW, heap, L"et-EE.%u", codePage) != -1);
      break;
#endif
    case 20278: /* IBM Finland/Sweden          */
    case 1143:  /* IBM Finland/Sweden (+ Euro) */
      assert (p32_private_aswprintf (&localeStringW, heap, L"fi-FI.%u", codePage) != -1);
      break;
    case 863: /* OEM French Canadian  */
      assert (p32_private_aswprintf (&localeStringW, heap, L"fr-CA.%u", codePage) != -1);
      break;
#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    case 862:   /* OEM Hebrew   */
    case 1255:  /* CP1252       */
    case 10005: /* MAC Hebrew   */
    case 20424: /* IBM Hebrew   */
    case 28598: /* ISO-8859-8   */
    case 38598: /* ISO-8859-8-I */
      assert (p32_private_aswprintf (&localeStringW, heap, L"he-IL.%u", codePage) != -1);
      break;
#endif
    case 10082: /* MAC Croatian */
      assert (p32_private_aswprintf (&localeStringW, heap, L"hr-HR.%u", codePage) != -1);
      break;
#if _WIN32_WINNT >= _WIN32_WINNT_WIN2K
    case 65001: /* UTF-8 */
      assert (p32_private_aswprintf (&localeStringW, heap, L"hy-AM.%u", codePage) != -1);
      break;
#endif
    case 861:   /* OEM Icelandic          */
    case 20871: /* IBM Icelandic          */
    case 1149:  /* IBM Icelandic (+ Euro) */
    case 10079: /* MAC Icelandic          */
      assert (p32_private_aswprintf (&localeStringW, heap, L"is-IS.%u", codePage) != -1);
      break;
    case 20280: /* IBM Italy          */
    case 1144:  /* IBM Italy (+ Euro) */
      assert (p32_private_aswprintf (&localeStringW, heap, L"it-IT.%u", codePage) != -1);
      break;
    case 932:   /* SHIFT_JIS                */
    case 10001: /* MAC Japanese (SHIFT_JIS) */
    case 20932: /* EUC-JP                   */
      assert (p32_private_aswprintf (&localeStringW, heap, L"ja-JP.%u", codePage) != -1);
      break;
    case 949:   /* Unified Hangul Code */
    case 1361:  /* Johab               */
    case 10003: /* MAC Korean (EUC-KR) */
    case 20949: /* EUC-KR              */
    case 51949: /* EUC-KR              */
      assert (p32_private_aswprintf (&localeStringW, heap, L"ko-KR.%u", codePage) != -1);
      break;
#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    case 775:   /* OEM Baltic */
    case 28594: /* ISO-8859-4 */
      assert (p32_private_aswprintf (&localeStringW, heap, L"lv-LV.%u", codePage) != -1);
      break;
#endif
    case 865: /* OEM Nordic */
      assert (p32_private_aswprintf (&localeStringW, heap, L"nb-NO.%u", codePage) != -1);
      break;
    case 860: /* OEM Portuguese */
      assert (p32_private_aswprintf (&localeStringW, heap, L"pt-PT.%u", codePage) != -1);
      break;
    case 855:   /* OEM Cyrillic */
    case 866:   /* OEM Russian  */
    case 1251:  /* CP1252       */
    case 10007: /* MAC Cyrillic */
    case 20866: /* KOI8-R       */
    case 20880: /* IBM Russian  */
    case 28595: /* ISO-8859-5   */
      assert (p32_private_aswprintf (&localeStringW, heap, L"ru-RU.%u", codePage) != -1);
      break;
#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    case 21025: /* IBM Serbian/Bulgarian (Cyrillic) */
      assert (p32_private_aswprintf (&localeStringW, heap, L"bg-BG.%u", codePage) != -1);
      break;
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    case 874:   /* Thai     */
    case 10021: /* MAC Thai */
    case 20838: /* IBM Thai */
      assert (p32_private_aswprintf (&localeStringW, heap, L"th-TH.%u", codePage) != -1);
      break;
#endif
    case 857:   /* OEM Turkish */
    case 1026:  /* IBM Turkish */
    case 20905: /* IBM Turkish */
    case 1254:  /* CP1254      */
    case 10081: /* MAC Turkish */
    case 28599: /* ISO-8859-9  */
      assert (p32_private_aswprintf (&localeStringW, heap, L"tr-TR.%u", codePage) != -1);
      break;
#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    case 10017: /* MAC Ukrainian */
    case 21866: /* KOI8-U        */
      assert (p32_private_aswprintf (&localeStringW, heap, L"uk-UA.%u", codePage) != -1);
      break;
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    case 1258: /* CP1258 */
      assert (p32_private_aswprintf (&localeStringW, heap, L"vi-VN.%u", codePage) != -1);
      break;
#endif
    case 936:   /* GBK                             */
    case 10008: /* MAC Chinese Simplified (EUC-CN) */
    case 20936: /* EUC-CN                          */
      assert (p32_private_aswprintf (&localeStringW, heap, L"zh-CN.%u", codePage) != -1);
      break;
    case 950:   /* BIG5                           */
    case 10002: /* MAC Chinese Traditional (BIG5) */
    case 20000: /* ???                            */
    case 20001: /* ???                            */
    case 20002: /* ???                            */
    case 20003: /* ???                            */
    case 20004: /* ???                            */
    case 20005: /* ???                            */
      assert (p32_private_aswprintf (&localeStringW, heap, L"zh-TW.%u", codePage) != -1);
      break;
    /**
     * Following code pages do not work with intended languages.
     */
    case 1147:  /* IBM France            */
    case 20297: /* IBM France (+ Euro)   */
    case 20290: /* IBM Japanese Katakana */
    case 20833: /* IBM Korean Extended   */
    case 10010: /* MAC Romanian          */
    case 864:   /* OEM Arabic            */
    default:
      assert (p32_private_aswprintf (&localeStringW, heap, L"en-US.%u", codePage) != -1);
      break;
  }

  assert (p32_private_wcstombs (&localeStringA, localeStringW, heap, P32_CODEPAGE_ASCII, false) != -1);

  locale_t locale = newlocale (LC_ALL_MASK, localeStringA, NULL);

  if (locale == NULL) {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"FAIL: %s\n", localeStringW);
  } else {
    fwprintf (stdout, L"PASS: %s\n", localeStringW);
    freelocale (locale);
  }

  assert (HeapFree (heapHandle, 0, localeStringA));
  assert (HeapFree (heapHandle, 0, localeStringW));

  return TRUE;
}

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, "en-US") != NULL);

  /**
   * Test all supported code pages.
   */
  p32_locale_test_func3 (Test);

  return exit_code;
}
