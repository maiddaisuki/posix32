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
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Simple test for `setlocale`.
 */

typedef struct TestStrings {
  /**
   * Locale to set.
   */
  char *Locale;
  /**
   * Locale string expected to be returned by CRT's `[_w]setlocale`.
   */
#if P32_CRT >= P32_MSVCRT20
  wchar_t *LocaleString;
#else
  char *LocaleString;
#endif
  /**
   * Locale string expected to be returned by `getlocalename_l`.
   */
  char *LocaleName;
} TestStrings;

#if P32_CRT >= P32_MSVCRT20

#define SETLOCALE(c, l) _wsetlocale (c, l)
#define CMP(s1, s2)     wcscmp (s1, s2)
#define STR(s)          TEXT (s)

/**
 * "en-US" with its default ANSI code page the safest locale we can test.
 */
#define LOCALE_STRING     "en-US.ACP"
#define LOCALE_NAME       "en-US.1252"
#define LOCALE_CRT_STRING L"English_United States.1252"
#define LOCALE_CRT_NAME   L"en-US"

#else /* crtdll.dll or msvcrt10.dll */
typedef const char *(*__cdecl setlocale_t) (int, const char *);

#define SETLOCALE(c, l)   crt_setlocale (c, l)
#define CMP(s1, s2)       strcmp (s1, s2)
#define STR(s)            s

/**
 * "en-US" with its default OEM code page the safest locale we can test.
 */
#define LOCALE_STRING     "en-US.OCP"
#define LOCALE_NAME       "en-US.437"
#define LOCALE_CRT_STRING "English_United States.437"
#define LOCALE_CRT_NAME   LOCALE_CRT_STRING

#endif

/**
 * Locale strings for Global Locale.
 */
static TestStrings TestLocale;

int main (void) {
  p32_test_init ();

#if P32_CRT <= P32_MSVCRT10
  setlocale_t crt_setlocale = NULL;

  HMODULE crt = (HMODULE) p32_crt_handle ();

  if (crt == NULL) {
    return 77;
  }

  assert ((crt_setlocale = (setlocale_t) (UINT_PTR) GetProcAddress (crt, "setlocale")) != NULL);
#endif

  TestLocale.Locale     = LOCALE_STRING;
  TestLocale.LocaleName = LOCALE_NAME;

  if (P32_CRT >= P32_MSVCR110 && P32_WINNT_CHECK (P32_WINNT_VISTA, WindowsNtVista)) {
    TestLocale.LocaleString = LOCALE_CRT_NAME;
  } else {
    TestLocale.LocaleString = LOCALE_CRT_STRING;
  }

  char *global_locale_c = NULL;
  char *global_locale   = NULL;

  /**
   * When process starts, Global Locale must be set to "C".
   */
  assert (setlocale (LC_ALL, NULL) != NULL);
  assert ((global_locale_c = strdup (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE))) != NULL);
  assert (strcmp (global_locale_c, "C") == 0);
  assert (CMP (SETLOCALE (LC_ALL, NULL), STR ("C")) == 0);

  /**
   * Set Global Locale.
   */
  assert (setlocale (LC_ALL, TestLocale.Locale) != NULL);
  assert ((global_locale = strdup (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE))) != NULL);
  assert (strcmp (global_locale, TestLocale.LocaleName) == 0);
  assert (CMP (SETLOCALE (LC_ALL, NULL), TestLocale.LocaleString) == 0);

  /**
   * Set Global Locale back to "C".
   */
  assert (setlocale (LC_ALL, global_locale_c) != NULL);
  assert (strcmp (global_locale_c, getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE)) == 0);
  assert (CMP (SETLOCALE (LC_ALL, NULL), STR ("C")) == 0);

  /**
   * Set Global Locale to the locale set earlier.
   */
  assert (setlocale (LC_ALL, global_locale) != NULL);
  assert (strcmp (global_locale, getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE)) == 0);
  assert (CMP (SETLOCALE (LC_ALL, NULL), TestLocale.LocaleString) == 0);

  free (global_locale);
  free (global_locale_c);

  return 0;
}
