/**
 * Copyright 2025 Kirill Makurin
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
 * Test `strrchr` function with "POSIX" locale.
 */

static void DoTest (void) {
  const char *text = NULL;

  /**
   * Test ASCII text.
   */
  text = AsciiText;

  assert (strrchr (text, 'A') == text);
  assert (strrchr (text, ' ') == text + 5);
  assert (strrchr (text, '.') == text + 10);
  assert (strrchr (text, '\0') == text + 11);
  assert (strrchr (text, '\n') == NULL);

  /**
   * Test ASCII list.
   */
  text = AsciiList;

  assert (strrchr (text, '|') == text + 7);
  assert (strrchr (text, '\0') == text + 13);
  assert (strrchr (text, '\n') == NULL);

  /**
   * Test SBCS text.
   */
  text = SBCS;

  for (size_t i = 0; i < _countof (SBCS); ++i) {
    assert (strrchr (text, SBCS[i]) == text + i);
  }

  assert (strrchr (text, '\0') == text + 10);
  assert (strrchr (text, '\n') == NULL);

  /**
   * Test SBCS list.
   */
  text = SBCSList;

  assert (strrchr (text, '|') == text + 7);
  assert (strrchr (text, '\0') == text + 13);
  assert (strrchr (text, '\n') == NULL);
}

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, "POSIX") != NULL);

  DoTest ();

  return EXIT_SUCCESS;
}
