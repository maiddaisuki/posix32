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
 * Test `wcstombs` function with ISO-8859-1 (code page 28591).
 *
 * This code page is used with "POSIX" locale.
 */

#define LOCALE "POSIX"

static locale_t locale;

static void DoTest (void) {
  char buffer[BUFSIZ];

  const wchar_t *text        = NULL;
  size_t         text_length = 0;

  /**
   * Test SBCS input
   */
  text        = SBCSText.W;
  text_length = SBCSText.LengthA;

  /**
   * Try get length of converted `text`
   *
   * - return value must be `text_length`
   * - value of `errno` must not change
   */

  assert (wcstombs_l (NULL, text, 0, locale) == text_length);
  assert (errno == 0);

  /**
   * Try convert `text`
   *
   * - return value must be `text_length`
   * - converted string must be terminated with '\0'
   * - value of `errno` must not change
   */
  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, BUFSIZ, locale) == text_length);
  assert (buffer[text_length] == '\0' && buffer[text_length + 1] == EOF);
  assert (strcmp (buffer, SBCSText.A) == 0);
  assert (errno == 0);

  /**
   * Convert 5 wide characters in `text`
   *
   * - return value must be 5
   * - converted string must not be terminated with '\0'
   * - value of `errno` must not change
   */
  memset (buffer, EOF, BUFSIZ);

  assert (wcstombs_l (buffer, text, 5, locale) == 5);
  assert (buffer[5] == EOF);
  assert (strncmp (buffer, SBCSText.A, 5) == 0);
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
