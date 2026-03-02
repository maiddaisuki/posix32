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
 * Test `mbstowcs` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static locale_t locale;

static void DoTest (void) {
  wchar_t buffer[BUFSIZ];

  const char *text        = NULL;
  size_t      text_length = 0;

  /**
   * Test UTF-8 input
   */
  text        = UnicodeText.A;
  text_length = UnicodeText.LengthW;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - value of `errno` must not change
   */
  assert (mbstowcs_l (NULL, text, 0, locale) == text_length);
  assert (errno == 0);

  /**
   * Convert `text`
   *
   * - return value must be `text_length`
   * - converted string must be terminated with '\0'
   * - value of `errno` must not change
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, BUFSIZ, locale) == text_length);
  assert (buffer[text_length] == L'\0' && buffer[text_length + 1] == WEOF);
  assert (wcscmp (buffer, UnicodeText.W) == 0);
  assert (errno == 0);

  /**
   * Convert 3 multibyte characters in `text`
   *
   * - return value must be 3
   * - converted string must not be terminated with '\0'
   * - value of `errno` must not change
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, 3, locale) == 3);
  assert (buffer[3] == WEOF);
  assert (wcsncmp (buffer, UnicodeText.W, 3) == 0);
  assert (errno == 0);

  /**
   * Test UTF-8 input which produces UTF-16 Surrogate Pairs in output.
   */
  text        = UnicodeTextWithSurrogatePairs.A;
  text_length = UnicodeTextWithSurrogatePairs.LengthW;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - value of `errno` must not change
   */

  assert (mbstowcs_l (NULL, text, 0, locale) == text_length);
  assert (errno == 0);

  /**
   * Convert `text`
   *
   * - return value must be `text_length`
   * - converted string must be terminated with '\0'
   * - value of `errno` must not change
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, BUFSIZ, locale) == text_length);
  assert (buffer[text_length] == L'\0' && buffer[text_length + 1] == WEOF);
  assert (wcscmp (buffer, UnicodeTextWithSurrogatePairs.W) == 0);
  assert (errno == 0);

  /**
   * Convert 1 multibyte character from `text`
   *
   * - return value must be 2
   * - converted string must not be terminated with '\0'
   * - value of `errno` must not change
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, 3, locale) == 2);
  assert (buffer[2] == WEOF);
  assert (wcsncmp (buffer, UnicodeTextWithSurrogatePairs.W, 2) == 0);
  assert (errno == 0);

  /**
   * Test invalid UTF-8 input
   */
  char *InvalidUnicodeText = NULL;

  assert ((InvalidUnicodeText = strdup (UnicodeText.A)) != NULL);
  InvalidUnicodeText[5] = InvalidUnicodeText[0];

  text = InvalidUnicodeText;

  /**
   * Attempt to get length of converted `text`
   *
   * - return value must be (size_t)-1
   * - value of `errno` must be EILSEQ
   */
  assert (mbstowcs_l (NULL, text, 0, locale) == (size_t) -1);
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Attempt to convert `text`
   *
   * - return value must be (size_t)-1
   * - value of `errno` must be EILSEQ
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, BUFSIZ, locale) == (size_t) -1);
  assert (buffer[0] == UnicodeText.W[0] && buffer[1] == WEOF);
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Convert 1 multibyte character from `text`
   *
   * - return value must be 1
   * - value of `errno` must not change
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, 1, locale) == 1);
  assert (buffer[0] == UnicodeText.W[0] && buffer[1] == WEOF);
  assert (errno == 0);
}

int main (void) {
  p32_test_init ();

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 4);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
