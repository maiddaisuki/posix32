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
 * Test `strtok_r` function with a DBCS code page.
 */

#define LOCALE "ja_JP.ACP"

static void DoTest (void) {
  const char *original_text = NULL;

  char *text = NULL;
  char *context;

  /**
   * Test ASCII text.
   */
  original_text = AsciiText;

  char *set1 = NULL;
  char *set2 = NULL;

  assert ((set1 = strndup (original_text, 5)) != NULL);
  assert ((set2 = strndup (original_text + 6, 4)) != NULL);

  /**
   * Use `AsciiText` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, original_text, &context) == NULL);
  assert (context == text + 11);

  assert (strtok_r (NULL, original_text, &context) == NULL);
  assert (context == text + 11);

  free (text);

  /**
   * Use `set1` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, set1, &context) == text + 5);
  assert (context == text + 11);

  assert (strtok_r (NULL, set1, &context) == NULL);
  assert (context == text + 11);

  free (text);
  free (set1);

  /**
   * Use `set2` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, set2, &context) == text);
  assert (context == text + 7);

  assert (strtok_r (NULL, set2, &context) == text + 10);
  assert (context == text + 11);

  assert (strtok_r (NULL, set2, &context) == NULL);
  assert (context == text + 11);

  free (text);
  free (set2);

  /**
   * Test ASCII list.
   */
  original_text = AsciiList;

  char *set3 = NULL;
  char *set4 = NULL;
  char *set5 = NULL;

  assert ((set3 = strndup (original_text, 3)) != NULL);
  assert ((set4 = strndup (original_text + 4, 3)) != NULL);
  assert ((set5 = strndup (original_text + 8, 5)) != NULL);

  /**
   * Use `AsciiList` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, AsciiList, &context) == NULL);
  assert (context == text + 13);

  assert (strtok_r (NULL, AsciiList, &context) == NULL);
  assert (context == text + 13);

  free (text);

  /**
   * Use "|" as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, "|", &context) == text);
  assert (context == text + 4);

  assert (strtok_r (NULL, "|", &context) == text + 4);
  assert (context == text + 8);

  assert (strtok_r (NULL, "|", &context) == text + 8);
  assert (context == text + 13);

  assert (strtok_r (NULL, "|", &context) == NULL);
  assert (context == text + 13);

  free (text);

  /**
   * Use `set3` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, set3, &context) == text + 3);
  assert (context == text + 12);

  assert (strtok_r (NULL, set3, &context) == NULL);
  assert (context == text + 13);

  assert (strtok_r (NULL, set3, &context) == NULL);
  assert (context == text + 13);

  free (text);
  free (set3);

  /**
   * Use `set4` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, set4, &context) == text);
  assert (context == text + 5);

  assert (strtok_r (NULL, set4, &context) == text + 7);
  assert (context == text + 9);

  assert (strtok_r (NULL, set4, &context) == text + 9);
  assert (context == text + 13);

  assert (strtok_r (NULL, set4, &context) == NULL);
  assert (context == text + 13);

  free (text);
  free (set4);

  /**
   * Use `set5` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, set5, &context) == text);
  assert (context == text + 3);

  assert (strtok_r (NULL, set5, &context) == text + 3);
  assert (context == text + 5);

  assert (strtok_r (NULL, set5, &context) == text + 5);
  assert (context == text + 9);

  assert (strtok_r (NULL, set5, &context) == NULL);
  assert (context == text + 13);

  free (text);
  free (set5);

  /**
   * Test DBCS text.
   */
  original_text = DBCS;

  char *str6 = NULL;
  char *str7 = NULL;
  char *str8 = NULL;

  assert ((str6 = strndup (original_text, 6)) != NULL);
  assert ((str7 = strndup (original_text + 6, 8)) != NULL);
  assert ((str8 = strndup (original_text + 14, 2)) != NULL);

  /**
   * Use `DBCS` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, DBCS, &context) == NULL);
  assert (context == text + 16);

  assert (strtok_r (NULL, DBCS, &context) == NULL);
  assert (context == text + 16);

  free (text);

  /**
   * Use `str6` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, str6, &context) == text + 6);
  assert (context == text + 16);

  assert (strtok_r (NULL, str6, &context) == NULL);
  assert (context == text + 16);

  free (text);
  free (str6);

  /**
   * Use `str7` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, str7, &context) == text);
  assert (context == text + 8);

  assert (strtok_r (NULL, str7, &context) == text + 14);
  assert (context == text + 16);

  assert (strtok_r (NULL, str7, &context) == NULL);
  assert (context == text + 16);

  free (text);
  free (str7);

  /**
   * Use `str8` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, str8, &context) == text);
  assert (context == text + 16);

  assert (strtok_r (NULL, str8, &context) == NULL);
  assert (context == text + 16);

  free (text);
  free (str8);

  /**
   * Test DBCS list.
   */
  original_text = DBCSList;

  /**
   * Use 'DBCSList' as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, DBCSList, &context) == NULL);
  assert (context == text + 8);

  assert (strtok_r (NULL, DBCSList, &context) == NULL);
  assert (context == text + 8);

  free (text);

  /**
   * Use "|" as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, "|", &context) == text);
  assert (context == text + 3);

  assert (strtok_r (NULL, "|", &context) == text + 3);
  assert (context == text + 6);

  assert (strtok_r (NULL, "|", &context) == text + 6);
  assert (context == text + 8);

  assert (strtok_r (NULL, "|", &context) == NULL);
  assert (context == text + 8);

  free (text);

  /**
   * Test Invalid DBCS text.
   */
  original_text = DBCSList;

  char *str12 = NULL;

  assert ((str12 = strndup (original_text, 4)) != NULL);

  /**
   * Use "|" as separator.
   */
  context = NULL;

  assert ((text = strndup (original_text, 7)) != NULL);

  assert (strtok_r (text, "|", &context) == text);
  assert (context == text + 3);

  assert (strtok_r (NULL, "|", &context) == text + 3);
  assert (context == text + 6);

  assert (strtok_r (NULL, "|", &context) == NULL);
  assert (context == text + 6);

  free (text);

  /**
   * Use `str12` as separator.
   */
  context = NULL;

  assert ((text = strndup (original_text, 7)) != NULL);

  assert (strtok_r (text, str12, &context) == NULL);
  assert (context == NULL);

  free (text);
  free (str12);
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

  DWORD  exitCode     = EXIT_FAILURE;
  HANDLE threadHandle = NULL;

  assert ((threadHandle = CreateThread (NULL, 0, Thread, LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (threadHandle, INFINITE);
  GetExitCodeThread (threadHandle, &exitCode);
  CloseHandle (threadHandle);

  return EXIT_SUCCESS;
}
