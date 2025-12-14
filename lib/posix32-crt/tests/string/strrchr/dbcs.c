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
 * Test `strrchr` function with a DBCS code page.
 */

#define LOCALE "ja_JP.ACP"

static void DoTest (void) {
  const char *text = NULL;

  /**
   * Test ASCII text.
   */
  text = AsciiText;

  assert (strrchr (text, 'A') == text);
  assert (strrchr (text, ' ') == text + 5);
  assert (strrchr (text, '.') == text + 10);
  assert (strrchr (text, '\0') == text + 11);
  assert (strrchr (text, '\n') == NULL);

  /**
   * Test ASCII list.
   */
  text = AsciiList;

  assert (strrchr (text, '|') == text + 7);
  assert (strrchr (text, '\0') == text + 13);
  assert (strrchr (text, '\n') == NULL);

  /**
   * Test DBCS text.
   */
  text = DBCS;

  for (int i = 1; i < 0x80; ++i) {
    assert (strrchr (text, i) == NULL);
  }

  assert (strrchr (text, '\0') == text + 16);
  assert (strrchr (text, '\n') == NULL);

  /**
   * Test DBCS list.
   */
  text = DBCSList;

  assert (strrchr (text, '|') == text + 5);

  assert (strrchr (text, '\0') == text + 8);
  assert (strrchr (text, '\n') == NULL);

  /**
   * Test invalid DBCS text.
   */
  char *invalid_text = NULL;

  assert ((invalid_text = strndup (DBCSList, 7)) != NULL);

  assert (strrchr (invalid_text, '|') == invalid_text + 5);
  assert (strrchr (invalid_text, '\0') == NULL);

  free (invalid_text);
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
