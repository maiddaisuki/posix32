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
#include <errno.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-test.h"

/**
 * Test Summary:
 *
 * Test `wcsnrtombs` function with ASCII (code page 20127).
 */

#define LOCALE "en_US.ASCII"

static void DoTest (void) {
  mbstate_t state = {0};
  char      buffer[BUFSIZ];

  const wchar_t *original_text = NULL;
  const wchar_t *text          = NULL;

  /**
   * Test ASCII input
   */
  original_text = AsciiText.W;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Get length of 6 wide characters from `text` converted
   *
   * - return value must be 6
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (NULL, &text, 6, 0, &state) == 6);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert at most 6 wide characters from `text`
   *
   * - return value must be 6
   * - `text` must be `original_text + 6`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (buffer, &text, 6, BUFSIZ, &state) == 6);
  assert (text == original_text + 6);
  assert (buffer[6] == EOF);
  assert (strncmp (buffer, AsciiText.A, 6) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert at most 10 wide characters from `text` while writing at most 8
   * bytes to `buffer`
   *
   * - return value must be 8
   * - `text` must be `original_text + 14`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[6], &text, 10, 8, &state) == 8);
  assert (text == original_text + 14);
  assert (buffer[14] == EOF);
  assert (strncmp (buffer, AsciiText.A, 14) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of the rest of `text` converted
   *
   * - return value must be 6
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (NULL, &text, INT_MAX, 0, &state) == 6);
  assert (text == original_text + 14);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 6
   * - `text` must be NULL
   * - `buffer` must be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[14], &text, INT_MAX, BUFSIZ, &state) == 6);
  assert (text == NULL);
  assert (buffer[20] == '\0' && buffer[21] == EOF);
  assert (strcmp (buffer, AsciiText.A) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test SBCS input
   */
  original_text = SBCSText.W;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Convert at most 2 wide characters from `text` while writing at most 1
   * byte to `buffer`
   *
   * - return value must be 1
   * - `text` must be `origial_text + 1`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (buffer, &text, 2, 1, &state) == 1);
  assert (text == original_text + 1);
  assert (buffer[1] == EOF);
  assert (strncmp (SBCSText.A, buffer, 1) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Attempt to convert 1 more wide character from `text`
   *
   * - return value must be (size_t)-1
   * - `text` must be `original_text + 1`
   * - nothing must be written to buffer
   * - `state` must be in the initial state
   * - `errno` must be set to EILSEQ
   */
  assert (wcsnrtombs (&buffer[1], &text, 1, 0, &state) == (size_t) -1);
  assert (text == original_text + 1);
  assert (buffer[1] == EOF);
  assert (strncmp (SBCSText.A, buffer, 1) == 0);
  assert (mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);
}

static DWORD CALLBACK Thread (LPVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  assert (setlocale (LC_ALL, LOCALE) != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

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
