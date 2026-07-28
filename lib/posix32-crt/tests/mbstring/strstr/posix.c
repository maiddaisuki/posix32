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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "string-test.h"

/**
 * Test Summary:
 *
 * Test `strstr` function with "POSIX" locale.
 */

static void DoTest (void) {
  const char *text = NULL;

  /**
   * Test ASCII text.
   */
  text = AsciiText;

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, "ASCII") == text);
  assert (strstr (text, "Text") == text + 6);

  /**
   * Test ASCII text.
   */
  text = AsciiList;

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, "One") == text);
  assert (strstr (text, "Two") == text + 4);
  assert (strstr (text, "Three") == text + 8);

  /**
   * Test SBCS text.
   */
  text = SBCS;

  char *substr1 = NULL;
  char *substr2 = NULL;

  assert ((substr1 = strndup (text, 4)) != NULL);
  assert ((substr2 = strndup (text + 5, 5)) != NULL);

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, substr1) == text);
  assert (strstr (text, substr2) == text + 5);

  free (substr1);
  free (substr2);

  /**
   * Test SBCS list.
   */
  text = SBCSList;

  char *item1 = NULL;
  char *item2 = NULL;
  char *item3 = NULL;

  assert ((item1 = strndup (text, 3)) != NULL);
  assert ((item2 = strndup (text + 4, 3)) != NULL);
  assert ((item3 = strndup (text + 8, 5)) != NULL);

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, item1) == text);
  assert (strstr (text, item2) == text + 4);
  assert (strstr (text, item3) == text + 8);

  free (item1);
  free (item2);
  free (item3);
}

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, "POSIX") != NULL);

  DoTest ();

  return EXIT_SUCCESS;
}
