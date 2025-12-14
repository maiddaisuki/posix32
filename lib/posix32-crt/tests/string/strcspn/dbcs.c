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

#include "string-test.h"

/**
 * Test Summary:
 *
 * Test `strcspn` function with a DBCS code page.
 */

#define LOCALE "ja_JP.ACP"

static void DoTest (void) {
  const char *text = NULL;

  /**
   * Test ASCII text.
   */
  text = AsciiText;

  char *set1 = NULL;
  char *set2 = NULL;

  assert ((set1 = strndup (text, 5)) != NULL);
  assert ((set2 = strndup (text + 6, 5)) != NULL);

  assert (strcspn (text, set1) == 0);
  assert (strcspn (text, set2) == 6);

  free (set1);
  free (set2);

  /**
   * Test ASCII list.
   */
  text = AsciiList;

  char *set3 = NULL;
  char *set4 = NULL;
  char *set5 = NULL;

  assert ((set3 = strndup (text, 3)) != NULL);
  assert ((set4 = strndup (text + 4, 3)) != NULL);
  assert ((set5 = strndup (text + 8, 5)) != NULL);

  assert (strcspn (text, set3) == 0);
  assert (strcspn (text, "|") == 3);
  assert (strcspn (text, set4) == 4);
  assert (strcspn (text, set5) == 2);

  free (set3);
  free (set4);
  free (set5);

  /**
   * Test DBCS text.
   */
  text = DBCS;

  char *str6 = NULL;
  char *str7 = NULL;
  char *str8 = NULL;

  assert ((str6 = strndup (text, 6)) != NULL);
  assert ((str7 = strndup (text + 6, 8)) != NULL);
  assert ((str8 = strndup (text + 14, 2)) != NULL);

  assert (strcspn (text, str6) == 0);
  assert (strcspn (text, str7) == 6);
  assert (strcspn (text, str8) == 14);

  free (str6);
  free (str7);
  free (str8);

  /**
   * Test DBCS list.
   */
  text = DBCSList;

  char *str9 = NULL;
  char *str10 = NULL;
  char *str11 = NULL;

  assert ((str9 = strndup (text, 2)) != NULL);
  assert ((str10 = strndup (text + 3, 2)) != NULL);
  assert ((str11 = strndup (text + 6, 2)) != NULL);

  assert (strcspn (text, str9) == 0);
  assert (strcspn (text, "|") == 2);
  assert (strcspn (text, str10) == 3);
  assert (strcspn (text, str11) == 6);

  free (str9);
  free (str10);
  free (str11);

  /**
   * Test Invalid DBCS text.
   */
  char *invalid_text = NULL;

  assert ((invalid_text = strndup (DBCSList, 7)) != NULL);

  char *str12 = NULL;
  char *str13 = NULL;

  assert ((str12 = strndup (invalid_text + 3, 2)) != NULL);
  assert ((str13 = strndup (invalid_text, 1)) != NULL);

  assert (strcspn (invalid_text, "|") == 2);
  assert (strcspn (invalid_text, str12) == 3);
  assert (strcspn (invalid_text, str13) == 0);

  free (str12);
  free (str13);
}

static DWORD CALLBACK Thread (PVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();

#if P32_CRT == P32_MSVCRT10
  assert (setlocale (LC_ALL, LOCALE) == NULL);
#else
  assert (setlocale (LC_ALL, LOCALE) != NULL);

  DoTest ();
#endif

  assert (setlocale (LC_ALL, "POSIX") != NULL);

  DWORD  exitCode = EXIT_FAILURE;
  HANDLE threadHandle = NULL;

  assert ((threadHandle = CreateThread (NULL, 0, Thread, LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (threadHandle, INFINITE);
  GetExitCodeThread (threadHandle, &exitCode);
  CloseHandle (threadHandle);

  return EXIT_SUCCESS;
}
