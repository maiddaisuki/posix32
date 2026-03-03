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
 * Test `wcsrtombs` function with ASCII (code page 20127).
 */

#undef wcsrtombs

/**
 * `Charset` structure with information about code page 20127 (ASCII).
 */
static Charset ascii;

#undef MB_CUR_MAX
#define MB_CUR_MAX (ascii.MaxLength)

/**
 * Convenience macro to call `p32_private_wcsrtombs_cp20127`.
 */
#define wcsrtombs(mbs, wcs, size, state) p32_private_wcsrtombs_cp20127 (mbs, wcs, size, state, &ascii)

static void DoTest (void) {
  mbstate_t state = {0};
  char      buffer[BUFSIZ];

  const wchar_t *original_text = NULL;
  const wchar_t *text          = NULL;
  size_t         text_length   = 0;

  /**
   * Test ASCII input
   */
  original_text = AsciiText.W;
  text_length   = AsciiText.LengthA;

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
   * Convert 10 wide characters from `text`
   *
   * - return value must be 10
   * - `text` must be `original_text + 10`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (buffer, &text, 10, &state) == 10);
  assert (text == original_text + 10);
  assert (buffer[10] == EOF);
  assert (strncmp (buffer, AsciiText.A, 10) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert the rest of `text`
   *
   * - return value must be 10
   * - `text` must be NULL
   * - `buffer` must be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (&buffer[10], &text, BUFSIZ, &state) == 10);
  assert (text == NULL);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, AsciiText.A) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test SBCS input
   */
  original_text = SBCSText.W;
  text_length   = SBCSText.LengthA;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Attempt to get length of converted `text`
   *
   * - return value must be (size_t)-1
   * - `text` must be `original_text`
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
   * - `text` must be `original_text + 1`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must be set to EILSEQ
   */
  assert (wcsrtombs (buffer, &text, BUFSIZ, &state) == (size_t) -1);
  assert (text == original_text + 1);
  assert (buffer[1] == EOF);
  assert (strncmp (SBCSText.A, buffer, 1) == 0);
  assert (mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  text = original_text;
  memset (buffer, EOF, BUFSIZ);

  /**
   * Convert 1 wide characters from `text`
   *
   * - return value must be 1
   * - `text` must be `original_text + 1`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (buffer, &text, 1, &state) == 1);
  assert (text == original_text + 1);
  assert (buffer[0] == SBCSText.A[0] && buffer[1] == EOF);
  assert (mbsinit (&state));
  assert (errno == 0);
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  ascii.CodePage = P32_CODEPAGE_ASCII;
  assert (p32_charset_info (&ascii));
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
