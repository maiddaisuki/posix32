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
 * Test `strcspn` function with a SBCS code page.
 */

#define LOCALE "en_US.1250"

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
  assert (strcspn (text, set4) == 4);
  assert (strcspn (text, set5) == 2);

  free (set3);
  free (set4);
  free (set5);

  /**
   * Test SBCS text.
   */
  text = SBCS;

  char *set6 = NULL;
  char *set7 = NULL;

  assert ((set6 = strndup (text, 4)) != NULL);
  assert ((set7 = strndup (text + 5, 5)) != NULL);

  assert (strcspn (text, set6) == 0);
  assert (strcspn (text, set7) == 5);

  free (set6);
  free (set7);

  /**
   * Test SBCS list.
   */
  text = SBCSList;

  char *set8 = NULL;
  char *set9 = NULL;
  char *set10 = NULL;

  assert ((set8 = strndup (text, 3)) != NULL);
  assert ((set9 = strndup (text + 4, 3)) != NULL);
  assert ((set10 = strndup (text + 8, 5)) != NULL);

  assert (strcspn (text, set8) == 0);
  assert (strcspn (text, "|") == 3);
  assert (strcspn (text, set9) == 4);
  assert (strcspn (text, set10) == 2);

  free (set8);
  free (set9);
  free (set10);
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

  DWORD  exitCode = EXIT_FAILURE;
  HANDLE threadHandle = NULL;

  assert ((threadHandle = CreateThread (NULL, 0, Thread, LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (threadHandle, INFINITE);
  GetExitCodeThread (threadHandle, &exitCode);
  CloseHandle (threadHandle);

  return EXIT_SUCCESS;
}
