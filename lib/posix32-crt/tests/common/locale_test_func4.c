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

static LocaleCallback4 localeCallback4 = NULL;

static BOOL CALLBACK P32CallbackWrapper4 (wchar_t *codePageString) {
  assert (localeCallback4 != NULL);

  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  bool flagSetlocale = !!(P32LocaleTestFlags & P32_LOCALE_TEST_SETLOCALE);
  bool flagUsableCp  = !!(P32LocaleTestFlags & P32_LOCALE_TEST_USABLE_CP);

  uint32_t codePage = wcstoul (codePageString, NULL, 10);

  if (flagUsableCp) {
    int rejectMask = 0;

    if (flagSetlocale) {
      rejectMask |= (P32_CHARSET_REJECT_CRT | P32_CHARSET_REJECT_GLOBAL);
    }

    if (!p32_charset_usable (codePage, rejectMask, 0)) {
      return TRUE;
    }
  }

#if P32_CRT < P32_UCRT
  /**
   * We emulate UTF-8 locales for older CRTs.
   */
  if (codePage == CP_UTF8 && flagSetlocale && flagUsableCp) {
    return TRUE;
  }
#endif

  char    *localeStringA = NULL;
  wchar_t *localeStringW = NULL;

  assert (p32_private_aswprintf (&localeStringW, heap, L"en-US.%u", codePage) != -1);
  assert (p32_private_wcstombs (&localeStringA, localeStringW, heap, P32_CODEPAGE_ASCII, false) != -1);

  bool     keep_going = true;
  locale_t locale     = p32_newlocale (LC_ALL_MASK, localeStringA, NULL);

  if (locale != NULL) {
    if (!flagSetlocale) {
      keep_going = localeCallback4 (locale, localeStringW);
#if P32_CRT >= P32_MSVCRT20
    } else if (_wsetlocale (LC_ALL, locale->CrtLocaleStrings.W.LcAll) != NULL) {
      keep_going = localeCallback4 (locale, localeStringW);
#endif
#if P32_CRT < P32_UCRT
    } else if (setlocale (LC_ALL, locale->CrtLocaleStrings.A.LcAll) != NULL) {
      keep_going = localeCallback4 (locale, localeStringW);
#endif
    } else {
      fwprintf (stderr, L"%s: failed to set locale.\n", locale->CrtLocaleStrings.W.LcAll);
    }

    p32_freelocale (locale);
  }

  assert (HeapFree (heapHandle, 0, localeStringA));
  assert (HeapFree (heapHandle, 0, localeStringW));

  return keep_going;
}

void p32_locale_test_func4 (LocaleCallback4 callback, int flags) {
  localeCallback4    = callback;
  P32LocaleTestFlags = flags;

  bool keep_going = true;

  /**
   * Test "POSIX" locale.
   */
  locale_t posix = p32_newlocale (LC_ALL_MASK, "POSIX", NULL);
  assert (posix != NULL);

  if (flags & P32_LOCALE_TEST_SETLOCALE) {
#if P32_CRT >= P32_MSVCRT20
    assert (_wsetlocale (LC_ALL, posix->CrtLocaleStrings.W.LcAll) != NULL);
#else
    assert (setlocale (LC_ALL, posix->CrtLocaleStrings.A.LcAll) != NULL);
#endif
  }

  keep_going = localeCallback4 (posix, posix->WindowsLocaleStrings.W.LcAll);
  p32_freelocale (posix);

  if (!keep_going) {
    return;
  }

  /**
   * Test all supported code pages.
   */
  EnumSystemCodePagesW (P32CallbackWrapper4, CP_SUPPORTED);
}
