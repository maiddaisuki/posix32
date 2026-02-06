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
 * Test `mbsnrtowcs` function with a double-byte code page.
 */

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

static void DoTest (void) {
  mbstate_t state     = {0};
  mbstate_t saveState = {0};
  wchar_t   buffer[BUFSIZ];

  const char *original_text = NULL;
  const char *text          = NULL;

  /**
   * Test DBCS input
   */
  original_text = DBCSText.A;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Convert at most 1 byte from `text`
   *
   * - return value must be 0
   * - `text` must be `original_text + 1`
   * - nothing must be written to `buffer`
   * - `state` must contain DBCS lead byte
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 1, BUFSIZ, &state) == 0);
  assert (buffer[0] == WEOF);
  assert (text == original_text + 1);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Get length of at most next 4 bytes converted from `text`
   *
   * - return value must be 2
   * - `text` must not change
   * - `state` must not change
   * - `errno` must not change
   */
  assert (mbsnrtowcs (NULL, &text, 4, 0, &state) == 2);
  assert (text == original_text + 1);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == 0);

  /**
   * Convert at most 4 bytes from `text`
   *
   * - return value must be 2
   * - `text` must be `original_text + 5`
   * - `buffer` must not be terminated with '\0'
   * - `state` must not be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 4, BUFSIZ, &state) == 2);
  assert (text == original_text + 5);
  assert (buffer[2] == WEOF);
  assert (wcsncmp (buffer, DBCSText.W, 2) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Get langth of the rest of `text` converted
   *
   * - return value must be 5
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (NULL, &text, INT_MAX, 0, &state) == 5);
  assert (text == original_text + 5);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 5
   * - `text` must be NULL
   * - `buffer` must be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (&buffer[2], &text, INT_MAX, BUFSIZ, &state) == 5);
  assert (text == NULL);
  assert (buffer[7] == L'\0' && buffer[8] == WEOF);
  assert (wcscmp (buffer, DBCSText.W) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test mixed input
   */
  original_text = MixedText.A;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Converted at most 5 bytes from `text`
   *
   * - return value must be 3
   * - `text` must be `original_text + 5`
   * - `buffer` must not be terminated with '\0'
   * - `state` must contain DBCS lead byte from `original_text[4]`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 5, BUFSIZ, &state) == 3);
  assert (text == original_text + 5);
  assert (buffer[3] == WEOF);
  assert (wcsncmp (buffer, MixedText.W, 3) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  /**
   * Converted at most 5 bytes from `text`
   *
   * - return value must be 4
   * - `text` must be `original_text + 10`
   * - `buffer` must not be terminated with '\0'
   * - `state` must contain DBCS lead byte from `original_text[9]`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (&buffer[3], &text, 5, BUFSIZ, &state) == 4);
  assert (text == original_text + 10);
  assert (buffer[7] == WEOF);
  assert (wcsncmp (buffer, MixedText.W, 7) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  /**
   * Converted the rest of `text`
   *
   * - return value must be 3
   * - `text` must be NULL
   * - `buffer` must be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (&buffer[7], &text, INT_MAX, BUFSIZ, &state) == 3);
  assert (text == NULL);
  assert (buffer[10] == L'\0' && buffer[11] == WEOF);
  assert (wcscmp (buffer, MixedText.W) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test invalid DBCS input
   */
  char *InvalidDBCS = NULL;

  assert ((InvalidDBCS = (strdup (DBCSText.A))) != NULL);
  InvalidDBCS[5] = '\0';

  original_text = InvalidDBCS;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Convert at most 5 bytes from `text`
   *
   * - return value must be 2
   * - `text` must be `original_text + 5`
   * - `buffer` must not be terminated with '\0'
   * - `state` must contain DBCS lead byte from `original_text[4]`
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 5, BUFSIZ, &state) == 2);
  assert (text == original_text + 5);
  assert (buffer[2] == WEOF);
  assert (wcsncmp (DBCSText.W, buffer, 2) == 0);
  assert (!mbsinit (&state));
  assert (errno == 0);

  // save `state`
  saveState = state;

  /**
   * Attempt to convert at most 5 bytes from `text`
   *
   * - return value must be (size_t)-1
   * - `text` must be `original_text + 5`
   * - nothing must be written to buffer
   * - `state` must be equal to `saveState`
   * - `errno` must be set to EILSEQ
   */
  assert (mbsnrtowcs (&buffer[2], &text, 5, BUFSIZ, &state) == (size_t) -1);
  assert (text == original_text + 5);
  assert (buffer[2] == WEOF);
  assert (EqualConversionState (&state, &saveState));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  free (InvalidDBCS);
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
