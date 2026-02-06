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
 * Test `wcsnrtombs` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static void DoTest (void) {
  mbstate_t state     = {0};
  mbstate_t saveState = {0};
  char      buffer[BUFSIZ];

  const wchar_t *original_text = NULL;
  const wchar_t *text          = NULL;

  /**
   * Test Unicode input
   */
  original_text = UnicodeText.W;

  text = original_text;
  memset (buffer, EOF, BUFSIZ);

  /**
   * Convert at most 4 wide characters from `text` while writing at most 11
   * bytes to `buffer`
   *
   * - return value must be 9
   * - `text` must be `original_text + 3`
   * - `buffer` must not be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (buffer, &text, 4, 11, &state) == 9);
  assert (text == original_text + 3);
  assert (buffer[9] == EOF);
  assert (strncmp (buffer, UnicodeText.A, 9) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert at most 4 wide characters from `text` while writing at most
   * 10 bytes to `buffer`
   *
   * - return value must be 9
   * - `text` must be `original_text + 6`
   * - `buffer must not be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[9], &text, 4, 10, &state) == 9);
  assert (text == original_text + 6);
  assert (buffer[18] == EOF);
  assert (strncmp (buffer, UnicodeText.A, 18) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 3
   * - `text` must be NULL
   * - `buffer` must be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[18], &text, INT_MAX, 4, &state) == 3);
  assert (text == NULL);
  assert (buffer[21] == '\0' && buffer[22] == EOF);
  assert (strcmp (buffer, UnicodeText.A) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test Unicode input which contains UTF-16 Surrogate Pairs
   */
  original_text = UnicodeTextWithSurrogatePairs.W;

  text = original_text;
  memset (buffer, EOF, BUFSIZ);

  /**
   * Convert at most 3 wide characters from `text` while writing at most
   * 3 bytes to `buffer`
   *
   * - return value must be 0
   * - `text` must be `original_text + 1`
   * - nothing must be written to `buffer`
   * - `state` must contain high surrogate from `original_text[0]`
   * - `errno` must not change
   */
  assert (wcsnrtombs (buffer, &text, 3, 3, &state) == 0);
  assert (text == original_text + 1);
  assert (buffer[0] == EOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Get length of 2 next wide characters in `text` converted
   *
   * - return value must be 4
   * - `text` must not change
   * - `state` must be equal to `saveState`
   * - `errno` must not change
   */
  assert (wcsnrtombs (NULL, &text, 2, 0, &state) == 4);
  assert (text == original_text + 1);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == 0);

  /**
   * Convert next 2 wide characters from `text`
   *
   * - return value must be 4
   * - `text` must be `original_text + 3`
   * - `buffer` must not be terminated by '\0'
   * - `state` must contain high surrogate from `original_text[2]`
   * - `errno` must not change
   */
  assert (wcsnrtombs (buffer, &text, 2, BUFSIZ, &state) == 4);
  assert (text == original_text + 3);
  assert (buffer[4] == EOF);
  assert (strncmp (UnicodeTextWithSurrogatePairs.A, buffer, 4) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Get length of the rest of `text` converted
   *
   * - return value must be 4
   * - value text` must not change
   * - `state` must be equal to `saveState`
   * - `errno` must not change
   */
  assert (wcsnrtombs (NULL, &text, INT_MAX, 0, &state) == 4);
  assert (text == original_text + 3);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 4
   * - `text` must be NULL
   * - `buffer` must be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsnrtombs (&buffer[4], &text, 2, BUFSIZ, &state) == 4);
  assert (text == NULL);
  assert (buffer[8] == '\0' && buffer[9] == EOF);
  assert (strcmp (UnicodeTextWithSurrogatePairs.A, buffer) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test invalid Unicode input
   */
  wchar_t *InvalidUnicode = NULL;

  assert ((InvalidUnicode = wcsdup (UnicodeTextWithSurrogatePairs.W)) != NULL);
  InvalidUnicode[3] = 0x100;

  original_text = InvalidUnicode;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Convert at most 3 wide characters from `text`
   *
   * - return value must be 4
   * - `text` must be `original_text + 3`
   * - `buffer` must not be terminated with '\0'
   * - `state` must contain high surrogate from `original_text[2]`
   * - `errno` must bot change
   */
  assert (wcsnrtombs (buffer, &text, 3, BUFSIZ, &state) == 4);
  assert (text == original_text + 3);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 4) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Attempt to convert the rest of `text`
   *
   * - return value must be (size_t)-1
   * - `text` must be `original_text + 3`
   * - nothing must be written to `buffer`
   * - `state` must be equal to `saveState`
   * - `errno` must be set to EILSEQ
   */
  assert (wcsnrtombs (&buffer[4], &text, INT_MAX, BUFSIZ, &state) == (size_t) -1);
  assert (text == original_text + 3);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 4) == 0);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  free (InvalidUnicode);
}

static DWORD CALLBACK Thread (LPVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);
  assert (MB_CUR_MAX == 4);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  assert (setlocale (LC_ALL, LOCALE) != NULL);
  assert (MB_CUR_MAX == 4);

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
