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

static bool P32LocaleTestFunc2 (Locale *locale, void *localeTestFuncData) {
  LocaleTestFuncData *data = (LocaleTestFuncData *) localeTestFuncData;

#if (P32_LOCALE_API & P32_LOCALE_API_LN)
  /**
   * Skip partial locale names which do not include country/region code.
   */
  switch (wcslen (locale->LocaleName)) {
    case 2:
    case 3:
      return TRUE;
    case 7:
      if (locale->LocaleName[2] == L'-') {
        return TRUE;
      }
      break;
    case 8:
      if (locale->LocaleName[3] == L'-') {
        return TRUE;
      }
      break;
  }
#endif

  bool flagSetlocale = !!(data->Flags & P32_LOCALE_TEST_SETLOCALE);
  bool flagUsableCp  = !!(data->Flags & P32_LOCALE_TEST_USABLE_CP);
  bool flagTestAcp   = !!(data->Flags & P32_LOCALE_TEST_ACP);
  bool flagTestOcp   = !!(data->Flags & P32_LOCALE_TEST_OCP);
  bool flagTestUtf8  = !!(data->Flags & P32_LOCALE_TEST_UTF8);
  bool flagOnce      = !!(data->Flags & P32_LOCALE_TEST_ONCE);

  uint32_t ansiCodePage = locale->CodePage.Ansi;
  uint32_t oemCodePage  = locale->CodePage.Oem;

  bool testAnsi = ansiCodePage != CP_ACP && flagTestAcp;
  bool testOem  = oemCodePage != CP_OEMCP && flagTestOcp;

#if P32_CRT >= P32_UCRT
  bool testUnicode = flagTestUtf8;
#else
  bool testUnicode = flagTestUtf8 && (!flagSetlocale || !flagUsableCp);
#endif

  if (flagUsableCp) {
    int rejectMask = 0;

    if (flagSetlocale) {
      rejectMask |= (P32_CHARSET_REJECT_CRT | P32_CHARSET_REJECT_GLOBAL);
    }

    if (testAnsi) {
      testAnsi &= p32_charset_usable (ansiCodePage, rejectMask, 0);
    }

    if (testOem) {
      testOem &= p32_charset_usable (oemCodePage, rejectMask, 0);
    }
  }

  bool keep_going = true;
  bool tested     = false;

  /**
   * Test locale with its default ANSI code page.
   */
  if (testAnsi) {
    LPSTR testLocaleString = NULL;

    assert (p32_private_asprintf (&testLocaleString, data->Heap, L"%s.%u", locale->LocaleName, ansiCodePage) != -1);
    locale_t testLocale = p32_newlocale (LC_ALL_MASK, testLocaleString, NULL);

    if (testLocale != NULL) {
      if (!flagSetlocale) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#if P32_CRT >= P32_MSVCRT20
      } else if (_wsetlocale (LC_ALL, testLocale->CrtLocaleStrings.W.LcAll) != NULL) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#endif
#if P32_CRT < P32_UCRT
      } else if (setlocale (LC_ALL, testLocale->CrtLocaleStrings.A.LcAll)) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#endif
      } else {
        fwprintf (stderr, L"%s: failed to set locale.\n", testLocale->CrtLocaleStrings.W.LcCtype);
      }

      p32_freelocale (testLocale);
    }

    assert (HeapFree (data->HeapHandle, 0, testLocaleString));
  }

  if (!keep_going || (flagOnce && tested)) {
    goto stop;
  }

  /**
   * Test locale with its default OEM code page.
   */
  if (testOem) {
    LPSTR testLocaleString = NULL;

    assert (p32_private_asprintf (&testLocaleString, data->Heap, L"%s.%u", locale->LocaleName, oemCodePage) != -1);
    locale_t testLocale = p32_newlocale (LC_ALL_MASK, testLocaleString, NULL);

    if (testLocale != NULL) {
      if (!flagSetlocale) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#if P32_CRT >= P32_MSVCRT20
      } else if (_wsetlocale (LC_ALL, testLocale->CrtLocaleStrings.W.LcAll)) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#endif
#if P32_CRT < P32_UCRT
      } else if (setlocale (LC_ALL, testLocale->CrtLocaleStrings.A.LcAll)) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#endif
      } else {
        fwprintf (stderr, L"%s: failed to set locale.\n", testLocale->CrtLocaleStrings.W.LcCtype);
      }

      p32_freelocale (testLocale);
    }

    assert (HeapFree (data->HeapHandle, 0, testLocaleString));
  }

  if (!keep_going || (flagOnce && tested)) {
    goto stop;
  }

  /**
   * Test locale with UTF-8.
   */
  if (testUnicode) {
    LPSTR testLocaleString = NULL;

    assert (p32_private_asprintf (&testLocaleString, data->Heap, L"%s.%u", locale->LocaleName, CP_UTF8) != -1);
    locale_t testLocale = p32_newlocale (LC_ALL_MASK, testLocaleString, NULL);

    if (testLocale != NULL) {
      if (!flagSetlocale) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#if P32_CRT >= P32_MSVCRT20
      } else if (_wsetlocale (LC_ALL, testLocale->CrtLocaleStrings.W.LcAll)) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#endif
#if P32_CRT < P32_UCRT
      } else if (setlocale (LC_ALL, testLocale->CrtLocaleStrings.A.LcAll)) {
        keep_going &= data->Callback2 (testLocale, testLocale->WindowsLocaleStrings.W.LcAll);
        tested      = true;
#endif
      } else {
        fwprintf (stderr, L"%s: failed to set locale.\n", testLocale->CrtLocaleStrings.W.LcAll);
      }

      p32_freelocale (testLocale);
    }

    assert (HeapFree (data->HeapHandle, 0, testLocaleString));
  }

  if (!keep_going || (flagOnce && tested)) {
    goto stop;
  }

stop:
  return keep_going;
}

void p32_locale_test_func2 (LocaleCallback2 callback, int flags) {
  bool keep_going = true;

  /**
   * Test "POSIX" locale.
   */
  locale_t posix = NULL;
  assert ((posix = p32_newlocale (LC_ALL_MASK, "POSIX", NULL)) != NULL);

  if (flags & P32_LOCALE_TEST_SETLOCALE) {
#if P32_CRT >= P32_MSVCRT20
    assert (_wsetlocale (LC_ALL, posix->CrtLocaleStrings.W.LcAll) != NULL);
#else
    assert (setlocale (LC_ALL, posix->CrtLocaleStrings.A.LcAll) != NULL);
#endif
  }

  keep_going = callback (posix, posix->WindowsLocaleStrings.W.LcAll);
  p32_freelocale (posix);

  if (!keep_going) {
    return;
  }

  /**
   * Test ASCII locale.
   */
  locale_t ascii = NULL;
  assert ((ascii = p32_newlocale (LC_ALL_MASK, "en-US.ASCII", NULL)) != NULL);

  if (flags & P32_LOCALE_TEST_SETLOCALE) {
#if P32_CRT >= P32_MSVCRT20
    assert (_wsetlocale (LC_ALL, ascii->CrtLocaleStrings.W.LcAll) != NULL);
#else
    assert (setlocale (LC_ALL, ascii->CrtLocaleStrings.A.LcAll) != NULL);
#endif
  }

  keep_going = callback (ascii, ascii->WindowsLocaleStrings.W.LcAll);
  p32_freelocale (ascii);

  if (!keep_going) {
    return;
  }

  LocaleTestFuncData localeTestFuncData = {0};

  localeTestFuncData.Flags      = flags;
  localeTestFuncData.HeapHandle = GetProcessHeap ();
  localeTestFuncData.Callback2  = callback;

  /**
   * Test all supported locales.
   */
  p32_winlocale_enum_system_locales (P32LocaleTestFunc2, localeTestFuncData.Heap, &localeTestFuncData);
}
