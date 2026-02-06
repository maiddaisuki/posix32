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
#include <errno.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-test.h"

/**
 * Test Summary:
 *
 * Test `wcsnrtombs` function with a double-byte code page.
 */

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

static void DoTest (void) {
  mbstate_t state     = {0};
  mbstate_t saveState = {0};
  char      buffer[BUFSIZ];

  const wchar_t *original_text = NULL;
  const wchar_t *text          = NULL;

  /**
   * Test DBCS input
   */
  original_text = DBCSText.W;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Convert at most 5 wide characters from `text` while writing at most 7
   * bytes to `buffer`
   *
   * - return value must be 6
   * - `text` must be `original_text + 3`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (buffer, &text, 5, 7, &state) == 6);
  assert (text == original_text + 3);
  assert (buffer[6] == EOF);
  assert (strncmp (buffer, DBCSText.A, 6) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of next 2 wide characters from `text` converted
   *
   * - return value must be 4
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (NULL, &text, 2, 0, &state) == 4);
  assert (text == original_text + 3);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert next 2 wide characters from `text`
   *
   * - return value must be 4
   * - `text` must be `original_text + 5`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[6], &text, 2, BUFSIZ, &state) == 4);
  assert (text == original_text + 5);
  assert (buffer[10] == EOF);
  assert (strncmp (buffer, DBCSText.A, 10) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 4
   * - `text` must be NULL
   * - `buffer` must be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[10], &text, INT_MAX, 5, &state) == 4);
  assert (text == NULL);
  assert (buffer[14] == '\0' && buffer[15] == EOF);
  assert (strcmp (buffer, DBCSText.A) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test mixed input
   */
  original_text = MixedText.W;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Convert at most 4 wide characters from `text` while writing at most 5
   * bytes to `buffer`
   *
   * - return value must be 4
   * - `text` must be `original_text + 3`
   * - `buffer` must not be teminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (buffer, &text, 4, 5, &state) == 4);
  assert (text == original_text + 3);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, MixedText.A, 4) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert at most 5 wide characters from `text` while writing at most 6
   * bytes to `buffer`
   *
   * - return value must be 5
   * - `text` must be `original_text + 7`
   * - `buffer` must not be teminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[4], &text, 5, 6, &state) == 5);
  assert (text == original_text + 7);
  assert (buffer[9] == EOF);
  assert (strncmp (buffer, MixedText.A, 9) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 4
   * - `text` must be NULL
   * - `buffer` must be teminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[9], &text, INT_MAX, 5, &state) == 4);
  assert (text == NULL);
  assert (buffer[13] == '\0' && buffer[14] == EOF);
  assert (strcmp (buffer, MixedText.A) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test invalid input
   */
  wchar_t *InvalidDBCS = NULL;

  assert ((InvalidDBCS = wcsdup (DBCSText.W)) != NULL);
  InvalidDBCS[3] = HIGH_SURROGATE_MIN;

  original_text = InvalidDBCS;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Convert 2 wide characters from `text`
   *
   * - return value must be 4
   * - `text` must be `original_text + 2`
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (buffer, &text, 2, BUFSIZ, &state) == 4);
  assert (text == original_text + 2);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, DBCSText.A, 4) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert at most 2 next wide characters from `text` while writing at most
   * 3 bytes to `buffer`
   *
   * - return value must be 2
   * - `text` must be `original_text + 4`
   * - `state` must contain high surrogate from `InvalidDBCS[3]`
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[4], &text, 2, BUFSIZ, &state) == 2);
  assert (text == original_text + 4);
  assert (buffer[6] == EOF);
  assert (strncmp (buffer, DBCSText.A, 6) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Attempt to convert the rest of `text`
   *
   * - return value must be (size_t)-1
   * - `text` must not change
   * - nothing must be written to `buffer`
   * - `state` must be equal to `saveState`
   * - `errno` must be set to EILSEQ
   */
  assert (wcsnrtombs (&buffer[6], &text, INT_MAX, BUFSIZ, &state) == (size_t) -1);
  assert (text == original_text + 4);
  assert (buffer[6] == EOF);
  assert (strncmp (buffer, DBCSText.A, 6) == 0);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);
}

static DWORD CALLBACK Thread (LPVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);
  assert (MB_CUR_MAX == 2);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

#if P32_CRT == P32_MSVCRT10
  assert (setlocale (LC_ALL, LOCALE) == NULL);
#else
  assert (setlocale (LC_ALL, LOCALE) != NULL);
  assert (MB_CUR_MAX == 2);

  DoTest ();
#endif

  assert (setlocale (LC_ALL, "C") != NULL);
  assert (MB_CUR_MAX == 1);

  HANDLE thread   = NULL;
  DWORD  exitCode = EXIT_FAILURE;

  assert ((thread = CreateThread (NULL, 0, Thread, LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (thread, INFINITE);
  GetExitCodeThread (thread, &exitCode);
  CloseHandle (thread);

  return EXIT_SUCCESS;
}
