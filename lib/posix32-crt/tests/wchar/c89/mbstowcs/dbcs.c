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

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

static locale_t locale;

static void DoTest (void) {
  wchar_t buffer[BUFSIZ];

  const char *text        = NULL;
  size_t      text_length = 0;

  /**
   * Test DBCS input
   */
  text        = DBCSText.A;
  text_length = DBCSText.LengthW;

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
  assert (wcscmp (buffer, DBCSText.W) == 0);
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
  assert (wcsncmp (buffer, DBCSText.W, 3) == 0);
  assert (errno == 0);

  /**
   * Test mixed input
   */
  text        = MixedText.A;
  text_length = MixedText.LengthW;

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
  assert (wcscmp (buffer, MixedText.W) == 0);
  assert (errno == 0);

  /**
   * Convert 7 multibyte characters in `text`
   *
   * - return value must be 7
   * - converted string must not be terminated with '\0'
   * - value of `errno` must not change
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, 7, locale) == 7);
  assert (buffer[7] == WEOF);
  assert (wcsncmp (buffer, MixedText.W, 7) == 0);
  assert (errno == 0);

  /**
   * Test bad DBCS input
   */
  char *InvalidDBCS = NULL;

  assert ((InvalidDBCS = (strdup (DBCSText.A))) != NULL);
  InvalidDBCS[5] = '\0';

  text = InvalidDBCS;

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
  assert (buffer[0] == DBCSText.W[0] && buffer[1] == DBCSText.W[1] && buffer[2] == WEOF);
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Convert 2 multibyte characters from `text`
   *
   * - return value must be 2
   * - value of `errno` must not change
   */
  wmemset (buffer, WEOF, BUFSIZ);

  assert (mbstowcs_l (buffer, text, 2, locale) == 2);
  assert (buffer[0] == DBCSText.W[0] && buffer[1] == DBCSText.W[1] && buffer[2] == WEOF);
  assert (errno == 0);
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
