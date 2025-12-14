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
 * Test `mbsnrtowcs` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static void DoTest (void) {
  mbstate_t state     = {0};
  mbstate_t saveState = {0};
  wchar_t   buffer[BUFSIZ];

  const char *original_text = NULL;
  const char *text          = NULL;

  /**
   * Test UTF-8 input
   */
  original_text = UnicodeText.A;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Get length of at most 8 bytes converted from `text`
   *
   * - return value must be 2
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (NULL, &text, 8, 0, &state) == 2);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert at most 8 bytes from `text`
   *
   * - return value must be 2
   * - `text` must be `original_text + 8`
   * - `buffer` must not be terminated with '\0'
   * - `state` must contain two UTF-8 Code Units from `&original_text[5]`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 8, BUFSIZ, &state) == 2);
  assert (text == original_text + 8);
  assert (buffer[2] == WEOF);
  assert (wcsncmp (buffer, UnicodeText.W, 2) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Get length of at most 8 bytes converted from `text`
   *
   * - return value must be 3
   * - `text` must not change
   * - `state` must be equal to `saveState`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (NULL, &text, 8, 0, &state) == 3);
  assert (text == original_text + 8);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == 0);

  /**
   * Convert at most 8 bytes from `text`
   *
   * - return value must be 3
   * - value `text` must be `original_text + 16`
   * - `buffer` must not be terminated with '\0'
   * - `state` must contain UTF-8 Code Unit from `&original_text[15]`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (&buffer[2], &text, 8, BUFSIZ, &state) == 3);
  assert (text == original_text + 16);
  assert (buffer[5] == WEOF);
  assert (wcsncmp (buffer, UnicodeText.W, 5) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Get length of the rest of `text` converted
   *
   * - return value must be 2
   * - `text` must not change
   * - `state` must be equal to `saveState`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (NULL, &text, INT_MAX, 0, &state) == 2);
  assert (text == original_text + 16);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 2
   * - `text` must be NULL
   * - `buffer` must be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (&buffer[5], &text, INT_MAX, BUFSIZ, &state) == 2);
  assert (text == NULL);
  assert (buffer[7] == L'\0' && buffer[8] == WEOF);
  assert (wcscmp (buffer, UnicodeText.W) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test UTF-8 input which produces UTF-16 Surrogate Pairs in output.
   */
  original_text = UnicodeTextWithSurrogatePairs.A;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Convert at most 4 bytes from `text` while writing at most 1
   * wide character to `buffer`
   *
   * - return value must be 0
   * - `text` must be `original_text`
   * - nothing must be written to buffer
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 4, 0, &state) == 0);
  assert (text == original_text);
  assert (buffer[0] == WEOF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert at most 6 bytes from `text`
   *
   * - return value must be 2
   * - `text` must be `original_text + 6`
   * - `buffer` must not be terminated with '\0'
   * - `state` must contain two UTF-8 Code Units from `&original_text[4]`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 6, BUFSIZ, &state) == 2);
  assert (text == original_text + 6);
  assert (buffer[3] == WEOF);
  assert (wcsncmp (UnicodeTextWithSurrogatePairs.W, buffer, 2) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Convert the rest of `text` while writing at most 1 wide character
   * to `buffer`
   *
   * - return value must be 0
   * - `text` must be `original_text + 6`
   * - nothing must be written to `buffer`
   * - `state` must be equal to `saveState`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (&buffer[2], &text, INT_MAX, 1, &state) == 0);
  assert (text == original_text + 6);
  assert (buffer[3] == WEOF);
  assert (wcsncmp (UnicodeTextWithSurrogatePairs.W, buffer, 2) == 0);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 2
   * - `text` must be NULL
   * - `buffer` must be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (&buffer[2], &text, INT_MAX, BUFSIZ, &state) == 2);
  assert (text == NULL);
  assert (buffer[4] == L'\0' && buffer[5] == WEOF);
  assert (wcscmp (UnicodeTextWithSurrogatePairs.W, buffer) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test invalid UTF-8 input
   */
  char *InvalidUnicodeText = NULL;

  assert ((InvalidUnicodeText = strdup (UnicodeText.A)) != NULL);
  InvalidUnicodeText[5] = InvalidUnicodeText[0];

  original_text = InvalidUnicodeText;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Convert at most 5 bytes from `text`
   *
   * - return value must be 1
   * - `text` must be `original_text + 5`
   * - `buffer` must not be terminated with '\0'
   * - `state` must contain two UTF-8 Code Units from `&original_text[3]`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 5, BUFSIZ, &state) == 1);
  assert (text == original_text + 5);
  assert (buffer[0] == UnicodeText.W[0] && buffer[1] == WEOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Attempt to convert at most 5 bytes from `text`
   *
   * - return value must be (size_t)-1
   * - `text` must be `original_text + 5`
   * - nothing must be written to `buffer`
   * - `state` must be equal to `saveState`
   * - `errno` must be set to EILSEQ
   */
  assert (mbsnrtowcs (buffer, &text, 5, BUFSIZ, &state) == (size_t) -1);
  assert (text == original_text + 5);
  assert (buffer[0] == UnicodeText.W[0] && buffer[1] == WEOF);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  free (InvalidUnicodeText);
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
