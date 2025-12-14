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
 * Test `strtok_r` function with a SBCS code page.
 */

#define LOCALE "en_US.1250"

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
   * Test SBCS text.
   */
  original_text = SBCS;

  char *set6 = NULL;
  char *set7 = NULL;

  assert ((set6 = strndup (original_text, 4)) != NULL);
  assert ((set7 = strndup (original_text + 5, 4)) != NULL);

  /**
   * Use `SBCS` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, SBCS, &context) == NULL);
  assert (context == text + 10);

  assert (strtok_r (NULL, SBCS, &context) == NULL);
  assert (context == text + 10);

  free (text);

  /**
   * Use `set6` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, set6, &context) == text + 4);
  assert (context == text + 10);

  assert (strtok_r (NULL, set6, &context) == NULL);
  assert (context == text + 10);

  free (text);
  free (set6);

  /**
   * Use `set7` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, set7, &context) == text);
  assert (context == text + 6);

  assert (strtok_r (NULL, set7, &context) == text + 9);
  assert (context == text + 10);

  assert (strtok_r (NULL, set7, &context) == NULL);
  assert (context == text + 10);

  free (text);
  free (set7);

  /**
   * Test SBCS list.
   */
  original_text = SBCSList;

  /**
   * Use `SBCSList` as separator.
   */
  assert ((text = strdup (original_text)) != NULL);

  assert (strtok_r (text, SBCSList, &context) == NULL);
  assert (context == text + 13);

  assert (strtok_r (NULL, SBCSList, &context) == NULL);
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
