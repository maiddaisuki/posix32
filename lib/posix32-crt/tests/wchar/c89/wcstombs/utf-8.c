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
 * Test `wcstombs` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static locale_t locale;

static void DoTest (void) {
  char buffer[BUFSIZ];

  const wchar_t *original_text = NULL;
  const wchar_t *text          = NULL;
  size_t         text_length   = 0;

  /**
   * Test Unicode input
   */
  original_text = UnicodeText.W;
  text_length   = UnicodeText.LengthA;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - value of `errno` must not change
   */
  text = original_text;

  assert (wcstombs_l (NULL, text, 0, locale) == text_length);
  assert (errno == 0);

  /**
   * Convert `text`
   *
   * - return value must be `text_length`
   * - converted string must be terminated by '\0'
   * - value of `errno` must not change
   */
  text = original_text;

  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, BUFSIZ, locale) == text_length);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, UnicodeText.A) == 0);
  assert (errno == 0);

  /**
   * Convert 3 wide characters from `text`
   *
   * - return value must be 9
   * - converted string must not be terminated by '\0'
   * - value of `errno` must not change
   */
  text = original_text;

  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, 11, locale) == 9);
  assert (buffer[9] == EOF);
  assert (strncmp (buffer, UnicodeText.A, 9) == 0);
  assert (errno == 0);

  /**
   * Test Unicode input which contains UTF-16 Surrogate Pairs
   */
  original_text = UnicodeTextWithSurrogatePairs.W;
  text_length   = UnicodeTextWithSurrogatePairs.LengthA;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - value of `errno` must not change
   */
  text = original_text;

  assert (wcstombs_l (NULL, text, 0, locale) == text_length);
  assert (errno == 0);

  /**
   * Convert `text`
   *
   * - return value must be `text_length`
   * - converted string must be terminated by '\0'
   * - value of `errno` must not change
   */
  text = original_text;

  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, BUFSIZ, locale) == text_length);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, UnicodeTextWithSurrogatePairs.A) == 0);
  assert (errno == 0);

  /**
   * Convert 2 wide characters (surrogate pair) from `text`
   *
   * - return value must be 4
   * - converted string must not be terminated by '\0'
   * - value of `errno` must not change
   */
  text = original_text;

  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, 7, locale) == 4);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 4) == 0);
  assert (errno == 0);

  /**
   * Test invalid Unicode input
   */
  wchar_t *InvalidUnicode = NULL;

  assert ((InvalidUnicode = wcsdup (UnicodeTextWithSurrogatePairs.W)) != NULL);
  InvalidUnicode[3] = 0x0;

  original_text = InvalidUnicode;

  /**
   * Attempt to get length of converted `text`
   *
   * - return value must be (size_t)-1
   * - value of `errno` must be EILSEQ
   */
  text = original_text;

  assert (wcstombs_l (NULL, text, 0, locale) == (size_t) -1);
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Attempt to convert `text`
   *
   * - return value must be (size_t)-1
   * - value of `errno` must be EILSEQ
   */
  text = original_text;

  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, BUFSIZ, locale) == (size_t) -1);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 4) == 0);
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Convert 2 wide characters (surrogate pair) from `text`
   *
   * - return value must be 4
   * - converted string must not be terminated with '\0'
   * - value of `errno` must be EILSEQ
   */
  text = original_text;

  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, 4, locale) == 4);
  assert (buffer[4] == EOF);
  assert (strncmp (buffer, UnicodeTextWithSurrogatePairs.A, 4) == 0);
  assert (errno == 0);

  free (InvalidUnicode);
}

int main (void) {
  p32_test_init ();

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 4);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
