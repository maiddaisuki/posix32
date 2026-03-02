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
 * Test `mbsrtowcs` function with UTF-8 (code page 65001).
 */

#undef mbsrtowcs

/**
 * `Charset` structure with information about code page 65001 (UTF-8).
 */
static Charset utf8;

#undef MB_CUR_MAX
#define MB_CUR_MAX (utf8.MaxLength)

/**
 * Convenience macro to call `p32_private_mbsrtowcs_utf8`.
 */
#define mbsrtowcs(wcs, mbs, size, state) p32_private_mbsrtowcs_utf8 (wcs, mbs, size, state, &utf8)

static void DoTest (void) {
  mbstate_t state = {0};
  wchar_t   buffer[BUFSIZ];

  const char *original_text = NULL;
  const char *text          = NULL;
  size_t      text_length   = 0;

  /**
   * Test UTF-8 input
   */
  original_text = UnicodeText.A;
  text_length   = UnicodeText.LengthW;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (NULL, &text, 0, &state) == text_length);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert 3 multibyte characters from `text`
   *
   * - return value must be 3
   * - `text` must be `original_text + 9`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (buffer, &text, 3, &state) == 3);
  assert (text == original_text + 9);
  assert (buffer[3] == WEOF);
  assert (wcsncmp (buffer, UnicodeText.W, 3) == 0);
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
  assert (mbsrtowcs (&buffer[3], &text, BUFSIZ, &state) == 4);
  assert (text == NULL);
  assert (buffer[text_length] == L'\0' && buffer[text_length + 1] == WEOF);
  assert (wcscmp (buffer, UnicodeText.W) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test UTF-8 input which produces UTF-16 Surrogate Pairs in output.
   */
  original_text = UnicodeTextWithSurrogatePairs.A;
  text_length   = UnicodeTextWithSurrogatePairs.LengthW;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (NULL, &text, 0, &state) == text_length);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert 1 multibyte character in UTF8SurrogatePair
   *
   * - return value must be 2
   * - `text` must be `original_text + 4`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (buffer, &text, 3, &state) == 2);
  assert (text == original_text + 4);
  assert (buffer[2] == WEOF);
  assert (wcsncmp (buffer, UnicodeTextWithSurrogatePairs.W, 2) == 0);
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
  assert (mbsrtowcs (&buffer[2], &text, BUFSIZ, &state) == 2);
  assert (text == NULL);
  assert (buffer[text_length] == L'\0' && buffer[text_length + 1] == WEOF);
  assert (wcscmp (buffer, UnicodeTextWithSurrogatePairs.W) == 0);
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
   * Attempt to get length of converted `text`
   *
   * - return value must be (size_t)-1
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must be set to EILSEQ
   */
  assert (mbsrtowcs (NULL, &text, 0, &state) == (size_t) -1);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Attempt to convert `text`
   *
   * - return value must be (size_t)-1
   * - `text` must be `original_text + 3`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must be set to EILSEQ
   */
  assert (mbsrtowcs (buffer, &text, BUFSIZ, &state) == (size_t) -1);
  assert (text == original_text + 3);
  assert (buffer[1] == WEOF);
  assert (wcsncmp (buffer, UnicodeText.W, 1) == 0);
  assert (mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Convert 1 multibyte character from `text`
   *
   * - return value must be 1
   * - `text` must be `original_text + 3`
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (buffer, &text, 1, &state) == 1);
  assert (text == original_text + 3);
  assert (buffer[1] == WEOF);
  assert (wcsncmp (buffer, UnicodeText.W, 1) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  utf8.CodePage = CP_UTF8;
  assert (p32_charset_info (&utf8));
  assert (MB_CUR_MAX == 4);

  DoTest ();

  return EXIT_SUCCESS;
}
