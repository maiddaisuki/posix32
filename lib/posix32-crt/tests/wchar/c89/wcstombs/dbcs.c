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
 * Test `wcsrtombs` function with some DBCS code page.
 *
 * We test code page 932; this is the ANSI/OEM code page for `ja-JP` locale.
 */

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

static locale_t locale;

static void DoTest (void) {
  char buffer[BUFSIZ];

  const wchar_t *text        = NULL;
  size_t         text_length = 0;

  /**
   * Test DBCS input
   */
  text        = DBCSText.W;
  text_length = DBCSText.LengthA;

  /**
   * Get length of converted DBCSTextW
   *
   * - return value must be `text_length`
   * - value of `errno` must not change
   */
  assert (wcstombs_l (NULL, text, 0, locale) == text_length);
  assert (errno == 0);

  /**
   * Convert `text`
   *
   * - return value must be `text_length`
   * - converted string must be terminated with '\0'
   * - value of `errno` must not change
   */
  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, BUFSIZ, locale) == text_length);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, DBCSText.A) == 0);
  assert (errno == 0);

  /**
   * Convert 3 wide characters in `text`
   *
   * - return value must be 6
   * - converted string must not be terminated with '\0'
   * - value of `errno` must not change
   */
  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, 7, locale) == 6);
  assert (buffer[6] == EOF);
  assert (strncmp (buffer, DBCSText.A, 6) == 0);
  assert (errno == 0);

  /**
   * Test mixed input
   */
  text        = MixedText.W;
  text_length = MixedText.LengthA;

  /**
   * Get length of converted `text`
   *
   * - return value must be `text_length`
   * - value of `errno` must not change
   */
  assert (wcstombs_l (NULL, text, 0, locale) == text_length);
  assert (errno == 0);

  /**
   * Convert `text`
   *
   * - return value must be `text_length`
   * - converted string must be teminated with '\0'
   * - value of `errno` must not change
   */
  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, BUFSIZ, locale) == text_length);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, MixedText.A) == 0);
  assert (errno == 0);

  /**
   * Convert 7 wide characters from `text`
   *
   * - return value must be 9
   * - converted string must not be teminated with '\0'
   * - value of `errno` must not change
   */
  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, 10, locale) == 9);
  assert (buffer[9] == EOF);
  assert (strncmp (buffer, MixedText.A, 9) == 0);
  assert (errno == 0);

  /**
   * Test bad input
   */
  wchar_t *InvalidDBCS = NULL;

  assert ((InvalidDBCS = wcsdup (DBCSText.W)) != NULL);
  InvalidDBCS[3] = HIGH_SURROGATE_MIN;

  text = InvalidDBCS;

  /**
   * Attempt to get length of converted `text`
   *
   * - return value should be (size_t)-1
   * - value of `errno` must be EILSEQ
   */
  assert (wcstombs_l (NULL, text, 0, locale) == (size_t) -1);
  assert (errno == EILSEQ);

  /**
   * Attempt to convert DBCSInvalidTextW
   *
   * - return value should be (size_t)-1
   * - value of `errno` must be EILSEQ
   */
  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, BUFSIZ, locale) == (size_t) -1);
  assert (buffer[6] == EOF);
  assert (strncmp (buffer, DBCSText.A, 6) == 0);
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);
}

int main (void) {
  p32_test_init ();

  if (!IsValidCodePage (932)) {
    return 77;
  }

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 2);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
