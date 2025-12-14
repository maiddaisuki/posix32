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
 * Test `strstr` function with a DBCS code page.
 */

#define LOCALE "ja_JP.ACP"

static void DoTest (void) {
  const char *text = NULL;

  /**
   * Test ASCII text.
   */
  text = AsciiText;

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, "ASCII") == text);
  assert (strstr (text, "Text") == text + 6);

  /**
   * Test |-separated list.
   */
  text = AsciiList;

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, "One") == text);
  assert (strstr (text, "Two") == text + 4);
  assert (strstr (text, "Three") == text + 8);

  /**
   * Test DBCS text.
   */
  text = DBCS;

  char *substr1 = NULL;
  char *substr2 = NULL;

  assert ((substr1 = strndup (text, 6)) != NULL);
  assert ((substr2 = strndup (text + 6, 8)) != NULL);

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, substr1) == text);
  assert (strstr (text, substr2) == text + 6);

  free (substr1);
  free (substr2);

  /**
   * Test DBCS text.
   */
  text = DBCSList;

  char *item1 = NULL;
  char *item2 = NULL;
  char *item3 = NULL;

  assert ((item1 = strndup (text, 2)) != NULL);
  assert ((item2 = strndup (text + 3, 2)) != NULL);
  assert ((item3 = strndup (text + 6, 2)) != NULL);

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, item1) == text);
  assert (strstr (text, item2) == text + 3);
  assert (strstr (text, item3) == text + 6);
  assert (strstr (text, "||") == NULL);

  free (item1);
  free (item2);
  free (item3);

  /**
   * Test Invalid DBCS text.
   */
  char *invalid_text = NULL;

  assert ((invalid_text = strndup (DBCSList, 7)) != NULL);

  char *item4 = NULL;
  char *item5 = NULL;

  assert ((item4 = strndup (invalid_text + 3, 2)) != NULL);
  assert ((item5 = strndup (invalid_text, 1)) != NULL);

  assert (strstr (invalid_text, "") == invalid_text);
  assert (strstr (invalid_text, invalid_text) == NULL);
  assert (strstr (invalid_text, item4) == invalid_text + 3);
  assert (strstr (invalid_text, item5) == NULL);

  free (item4);
  free (item5);
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
