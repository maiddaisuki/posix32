/**
 * Copyright 2026 Kirill Makurin
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
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `p32_private_strcpy_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strcpy_l`.
 */
#define strcpy(d, s) p32_private_strcpy_l (d, s, locale)

static void DoTest (void) {
  char buffer[BUFSIZ];

  /**
   * Basic `strcpy` usage.
   */
  const char *Test1String = "";
  const char *Test2String = "ABC";

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test1String) == buffer);
  assert (buffer[0] == '\0');
  assert (buffer[1] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test2String) == buffer);
  assert (strcmp (buffer, Test2String) == 0);
  assert (buffer[3] == '\0');
  assert (buffer[4] == C (0xFF));

  /**
   * Test input which contains well-formed UTF-8 Code Unit Sequence.
   */
  const char *Test3String = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF)
    // clang-format on
  );

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test3String) == buffer);
  assert (strcmp (buffer, Test3String) == 0);
  assert (buffer[9] == '\0');
  assert (buffer[10] == C (0xFF));

  /**
   * Test input which contains incomplete UTF-8 Code Unit Sequence.
   */
  const char *Test4String = STRING (C (0xCA));
  const char *Test5String = STRING (C (0xDF), C (0xBF), C (0xE0), C (0xA0));
  const char *Test6String = STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF), C (0xF0), C (0x90), C (0x80));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test4String) == NULL);
  assert (buffer[0] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test5String) == NULL);
  assert (buffer[0] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test6String) == NULL);
  assert (buffer[0] == C (0xFF));

  /**
   * Test input which contains ill-formed UTF-8 Code Unit Sequence.
   */
  const char *Test7String  = STRING (C (0xFF));
  const char *Test8String  = STRING (C (0xE0), C (0x80), C (0x80));
  const char *Test9String  = STRING (C (0xED), C (0xA0), C (0x80));
  const char *Test10String = STRING (C (0xF0), C (0x80), C (0x80), C (0x80));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test7String) == NULL);
  assert (buffer[0] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test8String) == NULL);
  assert (buffer[0] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test9String) == NULL);
  assert (buffer[0] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strcpy (buffer, Test10String) == NULL);
  assert (buffer[0] == C (0xFF));
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
