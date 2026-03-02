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
 * Test `wcsrtombs` function with ISO-8859-1 (code page 28591).
 *
 * This code page is used with "POSIX" locale.
 */

#undef wcsrtombs

/**
 * `Charset` structure with information about code page 28591 (ISO-8859-1).
 */
static Charset iso_8859_1;

#undef MB_CUR_MAX
#define MB_CUR_MAX (iso_8859_1.MaxLength)

/**
 * Convenience macro to call `p32_private_wcsrtombs_posix`.
 */
#define wcsrtombs(mbs, wcs, size, state) p32_private_wcsrtombs_posix (mbs, wcs, size, state, &iso_8859_1)

static void DoTest (void) {
  mbstate_t state = {0};
  char      buffer[BUFSIZ];

  const wchar_t *original_text = NULL;
  const wchar_t *text          = NULL;
  size_t         text_length   = 0;

  /**
   * Test SBCS input
   */
  original_text = SBCSText.W;
  text_length   = SBCSText.LengthA;

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
   * Convert 5 wide characters from `text`
   *
   * - return value must be 5
   * - `text` must be `original_text + 5`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (wcsrtombs (buffer, &text, 5, &state) == 5);
  assert (text == original_text + 5);
  assert (buffer[5] == EOF);
  assert (strncmp (buffer, SBCSText.A, 5) == 0);
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
  assert (wcsrtombs (&buffer[5], &text, BUFSIZ, &state) == 10);
  assert (text == NULL);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, SBCSText.A) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test Unicode input
   */
  original_text = UnicodeText.W;

  memset (buffer, EOF, BUFSIZ);
  text = original_text;

  /**
   * Attempt to convert 1 wide character from `text`
   *
   * - return value must be (size_t)-1
   * - `text` must be `original_text`
   * - nothing must be written to `buffer`
   * - `state` must be in the initial state
   * - `errno` must be set to EILSEQ
   */
  assert (wcsrtombs (buffer, &text, 0, &state) == (size_t) -1);
  assert (text == original_text);
  assert (buffer[0] == EOF);
  assert (mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);
}

int main (void) {
  p32_test_init ();

  iso_8859_1.CodePage = P32_CODEPAGE_POSIX;
  assert (p32_charset_info (&iso_8859_1));
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
