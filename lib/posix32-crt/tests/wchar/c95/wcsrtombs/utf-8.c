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
 * Test `wcsrtombs` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static void DoTest (void) {
  mbstate_t state = {0};
  char      buffer[BUFSIZ];

  const wchar_t *original_text = NULL;
  const wchar_t *text          = NULL;
  size_t         text_length   = 0;

  /**
   * Test Unicode input
   */
  original_text = UnicodeText.W;
  text_length   = UnicodeText.LengthA;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (NULL, &text, 0, &state) == text_length);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert 3 wide characters from `text`
   *
   * - return value must be 9
   * - `text` must be `original_text + 3`
   * - `buffer` must not be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (buffer, &text, 11, &state) == 9);
  assert (text == original_text + 3);
  assert (buffer[9] == EOF);
  assert (strncmp (buffer, UnicodeText.A, 9) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 12
   * - `text` must be NULL
   * - `buffer` must be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (&buffer[9], &text, BUFSIZ, &state) == 12);
  assert (text == NULL);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, UnicodeText.A) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test Unicode input which contains UTF-16 Surrogate Pairs
   */
  original_text = UnicodeTextWithSurrogatePairs.W;
  text_length   = UnicodeTextWithSurrogatePairs.LengthA;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (NULL, &text, 0, &state) == text_length);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert `text`
   *
   * - return value must be `text_length`
   * - `text` must be NULL
   * - `buffer` must be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (buffer, &text, BUFSIZ, &state) == text_length);
  assert (text == NULL);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, UnicodeTextWithSurrogatePairs.A) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Convert 2 wide characters (surrogate pair) from `text`
   *
   * - return value must be 4
   * - `text` must be `original_text + 3`
   * - `buffer` must not be terminated by '\0'
   * - `state` must contain high surrogate from `original_text[2]`
   * - `errno` must not change
   */
  assert (wcsrtombs (buffer, &text, 7, &state) == 4);
  assert (text == original_text + 3);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 4) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert 2 more wide characters (surrogate pair) from `text`
   *
   * - return value must be 4
   * - `text` must be `original_text + 4`
   * - `buffer` must not be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (&buffer[4], &text, 4, &state) == 4);
  assert (text == original_text + 4);
  assert (buffer[8] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 8) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test invalid Unicode input
   */
  wchar_t *InvalidUnicode = NULL;

  assert ((InvalidUnicode = wcsdup (UnicodeTextWithSurrogatePairs.W)) != NULL);
  InvalidUnicode[3] = 0x0;

  original_text = InvalidUnicode;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Attempt to get length of converted `text`
   *
   * - return value must be (size_t)-1
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must be set to EILSEQ
   */
  assert (wcsrtombs (NULL, &text, 0, &state) == (size_t) -1);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Attempt to convert `text`
   *
   * - return value must be (size_t)-1
   * - value of `text` must be `original_text + 3`
   * - `buffer` must not be terminated by '\0'
   * - `state` must be in the initial state
   * - `errno` must be set to EILSEQ
   */
  assert (wcsrtombs (buffer, &text, BUFSIZ, &state) == (size_t) -1);
  assert (text == original_text + 3);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 4) == 0);
  assert (mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Convert 2 wide characters (surrogate pair) from `text`
   *
   * - return value must be 4
   * - `text` must be `original_text + 2`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (buffer, &text, 4, &state) == 4);
  assert (text == original_text + 2);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 4) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

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
