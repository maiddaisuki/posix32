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
 * Test `mbsrtowcs` function with some DBCS code page.
 *
 * We test code page 932; this is the ANSI/OEM code page for `ja-JP` locale.
 */

#undef mbsrtowcs

/**
 * `Charset` structure with information about code page 932.
 */
static Charset cp932;

#undef MB_CUR_MAX
#define MB_CUR_MAX (cp932.MaxLength)

/**
 * Convenience macro to call `p32_private_mbsrtowcs_dbcs`.
 */
#define mbsrtowcs(wcs, mbs, size, state) p32_private_mbsrtowcs_dbcs (wcs, mbs, size, state, &cp932)

static void DoTest (void) {
  mbstate_t state = {0};
  wchar_t   buffer[BUFSIZ];

  const char *original_text = NULL;
  const char *text          = NULL;
  size_t      text_length   = 0;

  /**
   * Test DBCS input
   */
  original_text = DBCSText.A;
  text_length   = DBCSText.LengthA;

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Get length of converted `text`
   *
   * - return value must be 7
   * - `text` must not change
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (NULL, &text, 0, &state) == 7);
  assert (text == original_text);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert 3 multibyte characters in `text`
   *
   * - return value must be 3
   * - `text` must be `original_text + 6`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (buffer, &text, 3, &state) == 3);
  assert (text == original_text + 6);
  assert (buffer[3] == WEOF);
  assert (wcsncmp (buffer, DBCSText.W, 3) == 0);
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
  assert (buffer[7] == L'\0' && buffer[8] == WEOF);
  assert (wcscmp (buffer, DBCSText.W) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Test mixed input
   */
  original_text = MixedText.A;
  text_length   = MixedText.LengthW;

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
   * Converted 7 multibyte characters in `text`
   *
   * - return value must be 7
   * - `text` must be `original_text + 9`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (buffer, &text, 7, &state) == 7);
  assert (text == original_text + 9);
  assert (buffer[7] == WEOF);
  assert (wcsncmp (buffer, MixedText.W, 7) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Converted the rest of `text`
   *
   * - return value must be 4
   * - `text` must be NULL
   * - `buffer` must be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (&buffer[7], &text, BUFSIZ, &state) == 3);
  assert (text == NULL);
  assert (buffer[text_length] == L'\0' && buffer[text_length + 1] == WEOF);
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
   * Attempt get length of converted `text`
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
   * - `text` must be `original_text + 4`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must be set to EILSEQ
   */
  assert (mbsrtowcs (buffer, &text, BUFSIZ, &state) == (size_t) -1);
  assert (text == original_text + 4);
  assert (buffer[2] == WEOF);
  assert (wcsncmp (DBCSText.W, buffer, 2) == 0);
  assert (mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  wmemset (buffer, WEOF, BUFSIZ);
  text = original_text;

  /**
   * Convert 2 multibyte characters from `text`
   *
   * - return value must be 2
   * - `text` must be `original_text + 4`
   * - `buffer` must not be terminated with '\0'
   * - `state` must be in the initial state
   * - `errno` must not change
   */
  assert (mbsrtowcs (buffer, &text, 2, &state) == 2);
  assert (text == original_text + 4);
  assert (buffer[2] == WEOF);
  assert (wcsncmp (DBCSText.W, buffer, 2) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  free (InvalidDBCS);
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  if (!IsValidCodePage (932)) {
    return 77;
  }

  cp932.CodePage = 932;
  assert (p32_charset_info (&cp932));
  assert (MB_CUR_MAX == 2);

  DoTest ();

  return EXIT_SUCCESS;
}
