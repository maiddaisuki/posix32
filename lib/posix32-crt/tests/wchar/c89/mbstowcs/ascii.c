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
 * Test `mbstowcs` function with ASCII (code page 20127).
 */

#define LOCALE "en_US.ASCII"

static locale_t locale;

static void DoTest (void) {
  wchar_t buffer[BUFSIZ];

  const char *text        = NULL;
  size_t      text_length = 0;

  /**
   * Test ASCII input
   */
  text        = AsciiText.A;
  text_length = AsciiText.LengthA;

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
  assert (buffer[text_length] == L'\0');
  assert (wcscmp (buffer, AsciiText.W) == 0);
  assert (errno == 0);

  /**
   * Convert 10 characters from `text`
   *
   * - return value must be 10
   * - converted string must not be terminated with '\0'
   * - value of `errno` must not change
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, 10, locale) == 10);
  assert (buffer[10] == WEOF);
  assert (wcsncmp (buffer, AsciiText.W, 10) == 0);
  assert (errno == 0);

  /**
   * Test SBCS input
   */
  text        = SBCSText.A;
  text_length = SBCSText.LengthA;

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
   * - value of `errno` must not EILSEQ
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, BUFSIZ, locale) == (size_t) -1);
  assert (buffer[0] == SBCSText.W[0] && buffer[1] == WEOF);
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Convert 1 byte from `text`
   *
   * - return value must be 1
   * - value of `errno` must not EILSEQ
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, 1, locale) == 1);
  assert (buffer[0] == SBCSText.W[0] && buffer[1] == WEOF);
  assert (errno == 0);
}

int main (void) {
  p32_test_init ();

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 1);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
