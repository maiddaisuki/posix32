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
 * Test `mbsnrtowcs` function with a single-byte code page.
 */

#define LOCALE "en_US.ACP"

static void DoTest (void) {
  mbstate_t state = {0};
  wchar_t   buffer[BUFSIZ];

  const char *original_text = NULL;
  const char *text          = NULL;

  /**
   * Test SBCS input
   */
  original_text = SBCSText.A;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Get length of 5 characters from `text` converted
   *
   * - return value must be 5
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (NULL, &text, 5, 0, &state) == 5);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert 5 characters from `text`
   *
   * - return value must be 5
   * - `text` must be `original_text + 5`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (buffer, &text, 5, BUFSIZ, &state) == 5);
  assert (text == original_text + 5);
  assert (buffer[5] == WEOF);
  assert (wcsncmp (buffer, SBCSText.W, 5) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert at most 10 characters from `text` while writing at most 5
   * wide characters to `buffer`
   *
   * - return value must be 5
   * - `text` must be `original_text + 10`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsnrtowcs (&buffer[5], &text, 10, 5, &state) == 5);
  assert (text == original_text + 10);
  assert (buffer[10] == WEOF);
  assert (wcsncmp (buffer, SBCSText.W, 10) == 0);
  assert (mbsinit (&state));
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
  assert (mbsnrtowcs (&buffer[10], &text, INT_MAX, BUFSIZ, &state) == 5);
  assert (text == NULL);
  assert (buffer[15] == L'\0' && buffer[16] == WEOF);
  assert (wcscmp (buffer, SBCSText.W) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);
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
