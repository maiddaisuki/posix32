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
 * Test `strrchr` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

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
   * Test UTF-8 text.
   */
  text = UTF8;

  for (int i = 1; i < 0x100; ++i) {
    assert (strrchr (text, i) == 0);
  }

  assert (strrchr (text, '\0') == text + 24);

  /**
   * Test UTF-8 list.
   */
  text = UTF8Lits;

  assert (strrchr (text, '|') == text + 7);
  assert (strrchr (text, '\0') == text + 11);

  /**
   * Test invalid UTF-8 text.
   */
  char *invalid_text = NULL;

  assert ((invalid_text = strndup (UTF8Lits, 10)) != NULL);

  assert (strrchr (invalid_text, '|') == invalid_text + 7);
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

  assert (setlocale (LC_ALL, LOCALE) != NULL);

  DoTest ();

  assert (setlocale (LC_ALL, "POSIX") != NULL);

  DWORD  exitCode     = EXIT_FAILURE;
  HANDLE threadHandle = NULL;

  assert ((threadHandle = CreateThread (NULL, 0, Thread, LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (threadHandle, INFINITE);
  GetExitCodeThread (threadHandle, &exitCode);
  CloseHandle (threadHandle);

  return EXIT_SUCCESS;
}
