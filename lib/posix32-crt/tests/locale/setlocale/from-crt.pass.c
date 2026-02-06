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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * When `setlocale` is called for the first time, if Global Locale was not yet
 * initialized by another function, it will attempt to initialize it from
 * CRT's global locale.
 *
 * In most cases CRT's global locale will be "C" and this will succeed.
 *
 * However, it is possible that posix32's `setlocale` will be called after
 * CRT's global locale was set to another locale.
 *
 * This test verifies that `setlocale` is capable of handling this scenario.
 */

#if P32_CRT >= P32_MSVCRT20
#define SETLOCALE(c, l) _wsetlocale (c, l)
#define CMP(s1, s2)     wcscmp (s1, s2)
#define STR(s)          TEXT (s)
#else
typedef const char *(*__cdecl setlocale_t) (int, const char *);
#define SETLOCALE(c, l) crt_setlocale (c, l)
#define CMP(s1, s2)     strcmp (s1, s2)
#define STR(s)          s
#endif

#if P32_LOCALE_NAMES
#define LOCALE_LC_COLLATE  L"de-DE"
#define LOCALE_LC_CTYPE    L"en-US"
#define LOCALE_LC_MONETARY L"es-ES"
#define LOCALE_LC_NUMERIC  L"fr-FR"
#define LOCALE_LC_TIME     L"it-IT"
#else
#define LOCALE_LC_COLLATE  STR ("German_Germany")
#define LOCALE_LC_CTYPE    STR ("English_United States")
#define LOCALE_LC_MONETARY STR ("Spanish_Spain")
#define LOCALE_LC_NUMERIC  STR ("French_France")
#define LOCALE_LC_TIME     STR ("Italian_Italy")
#endif

#if P32_CRT >= P32_MSVCRT20
#define CP "1252"
#else
#define CP "437"
#endif

#define LOCALE_STRING         \
  "LC_COLLATE=de-DE." CP ";"  \
  "LC_CTYPE=en-US." CP ";"    \
  "LC_MONETARY=es-ES." CP ";" \
  "LC_NUMERIC=fr-FR." CP ";"  \
  "LC_TIME=it-IT." CP ";"     \
  "LC_MESSAGES=C"

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

  assert (SETLOCALE (LC_COLLATE, LOCALE_LC_COLLATE) != NULL);
  assert (SETLOCALE (LC_CTYPE, LOCALE_LC_CTYPE) != NULL);
  assert (SETLOCALE (LC_MONETARY, LOCALE_LC_MONETARY) != NULL);
  assert (SETLOCALE (LC_NUMERIC, LOCALE_LC_NUMERIC) != NULL);
  assert (SETLOCALE (LC_TIME, LOCALE_LC_TIME) != NULL);

  assert (setlocale (LC_ALL, NULL) != NULL);
  assert (strcmp (setlocale (LC_ALL, NULL), LOCALE_STRING) == 0);
  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), LOCALE_STRING) == 0);

  return 0;
}
