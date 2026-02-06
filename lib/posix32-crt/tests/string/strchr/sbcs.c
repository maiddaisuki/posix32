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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "string-test.h"

/**
 * Test Summary:
 *
 * Test `strchr` function with a SBCS code page.
 */

#define LOCALE "en-US.1250"

static void DoTest (void) {
  const char *text = NULL;

  /**
   * Test ASCII text.
   */
  text = AsciiText;

  assert (strchr (text, 'A') == text);
  assert (strchr (text, ' ') == text + 5);
  assert (strchr (text, '.') == text + 10);
  assert (strchr (text, '\0') == text + 11);
  assert (strchr (text, '\n') == NULL);

  /**
   * Test ASCII list.
   */
  text = AsciiList;

  assert (strchr (text, '|') == text + 3);
  assert (strchr (text, '\0') == text + 13);
  assert (strchr (text, '\n') == NULL);

  /**
   * Test SBCS text.
   */
  text = SBCS;

  for (size_t i = 0; i < _countof (SBCS); ++i) {
    assert (strchr (text, SBCS[i]) == text + i);
  }

  assert (strchr (text, '\0') == text + 10);
  assert (strchr (text, '\n') == NULL);

  /**
   * Test SBCS list.
   */
  text = SBCSList;

  assert (strchr (text, '|') == text + 3);
  assert (strchr (text, '\0') == text + 13);
  assert (strchr (text, '\n') == NULL);
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
