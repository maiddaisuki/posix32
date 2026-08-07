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
 * Test `p32_private_strncpy_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strncpy_l`.
 */
#define strncpy(d, s, n) p32_private_strncpy_l (d, s, n, locale)

static void DoTest (void) {
  char buffer[BUFSIZ];

  /**
   * Basic `strncpy` usage.
   */
  const char *Test1String = "";
  const char *Test2String = "ABC";

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test1String, 0) == buffer);
  assert (buffer[0] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test1String, 1) == buffer);
  assert (buffer[0] == '\0');
  assert (buffer[1] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test1String, BUFSIZ) == buffer);
  assert (buffer[BUFSIZ - 1] == '\0');

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test2String, 0) == buffer);
  assert (buffer[0] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test2String, 1) == buffer);
  assert (strncmp (buffer, Test2String, 1) == 0);
  assert (buffer[1] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test2String, 2) == buffer);
  assert (strncmp (buffer, Test2String, 2) == 0);
  assert (buffer[2] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test2String, 3) == buffer);
  assert (strncmp (buffer, Test2String, 3) == 0);
  assert (buffer[3] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test2String, BUFSIZ) == buffer);
  assert (strcmp (buffer, Test2String) == 0);
  assert (buffer[BUFSIZ - 1] == 0x00);

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
  assert (strncpy (buffer, Test3String, 1) == buffer);
  assert (buffer[0] == '\0');
  assert (buffer[1] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, 2) == buffer);
  assert (strncmp (buffer, Test3String, 2) == 0);
  assert (buffer[2] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, 3) == buffer);
  assert (strncmp (buffer, Test3String, 2) == 0);
  assert (buffer[2] == '\0');
  assert (buffer[3] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, 4) == buffer);
  assert (strncmp (buffer, Test3String, 2) == 0);
  assert (buffer[2] == '\0' && buffer[3] == '\0');
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, 5) == buffer);
  assert (strncmp (buffer, Test3String, 5) == 0);
  assert (buffer[5] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, 6) == buffer);
  assert (strncmp (buffer, Test3String, 5) == 0);
  assert (buffer[5] == '\0');
  assert (buffer[6] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, 7) == buffer);
  assert (strncmp (buffer, Test3String, 5) == 0);
  assert (buffer[5] == '\0' && buffer[6] == '\0');
  assert (buffer[7] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, 8) == buffer);
  assert (strncmp (buffer, Test3String, 5) == 0);
  assert (buffer[5] == '\0' && buffer[6] == '\0' && buffer[7] == '\0');
  assert (buffer[8] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, 9) == buffer);
  assert (strncmp (buffer, Test3String, 9) == 0);
  assert (buffer[9] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test3String, BUFSIZ) == buffer);
  assert (strcmp (buffer, Test3String) == 0);
  assert (buffer[9] == '\0');
  assert (buffer[BUFSIZ - 1] == '\0');

  /**
   * Test input which contains incomplete UTF-8 Code Unit Sequence.
   */
  const char *Test4String = STRING (C (0xCA));
  const char *Test5String = STRING (C (0xDF), C (0xBF), C (0xE0), C (0xA0));
  const char *Test6String = STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF), C (0xF0), C (0x90), C (0x80));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test4String, 1) == buffer);
  assert (buffer[0] == '\0');
  assert (buffer[1] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test4String, BUFSIZ) == NULL);
  assert (buffer[0] == '\0');
  assert (buffer[BUFSIZ - 1] == '\0');

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test5String, 2) == buffer);
  assert (strncmp (buffer, Test5String, 2) == 0);
  assert (buffer[2] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test5String, 3) == buffer);
  assert (strncmp (buffer, Test5String, 2) == 0);
  assert (buffer[2] == '\0');
  assert (buffer[3] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test5String, 4) == buffer);
  assert (strncmp (buffer, Test5String, 2) == 0);
  assert (buffer[2] == '\0' && buffer[3] == '\0');
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test5String, BUFSIZ) == NULL);
  assert (buffer[0] == '\0');
  assert (buffer[BUFSIZ - 1] == '\0');

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test6String, 5) == buffer);
  assert (strncmp (buffer, Test6String, 5) == 0);
  assert (buffer[5] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test6String, 6) == buffer);
  assert (strncmp (buffer, Test6String, 5) == 0);
  assert (buffer[5] == '\0');
  assert (buffer[6] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test6String, 7) == buffer);
  assert (strncmp (buffer, Test6String, 5) == 0);
  assert (buffer[5] == '\0' && buffer[6] == '\0');
  assert (buffer[7] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test6String, 8) == buffer);
  assert (strncmp (buffer, Test6String, 5) == 0);
  assert (buffer[5] == '\0' && buffer[6] == '\0' && buffer[7] == '\0');
  assert (buffer[8] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test6String, BUFSIZ) == NULL);
  assert (buffer[0] == '\0');
  assert (buffer[BUFSIZ - 1] == '\0');

  /**
   * Test input which contains ill-formed UTF-8 Code Unit Sequence.
   */
  const char *Test7String  = STRING (C (0xFF));
  const char *Test8String  = STRING (C (0xE0), C (0x80), C (0x80));
  const char *Test9String  = STRING (C (0xED), C (0xA0), C (0x80));
  const char *Test10String = STRING (C (0xF0), C (0x80), C (0x80), C (0x80));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test7String, 1) == NULL);
  assert (buffer[0] == '\0');
  assert (buffer[1] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test7String, BUFSIZ) == NULL);
  assert (buffer[0] == '\0');
  assert (buffer[BUFSIZ - 1] == '\0');

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test8String, 2) == NULL);
  assert (buffer[0] == '\0' && buffer[1] == '\0');
  assert (buffer[2] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test8String, BUFSIZ) == NULL);
  assert (buffer[0] == '\0');
  assert (buffer[BUFSIZ - 1] == '\0');

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test9String, 2) == NULL);
  assert (buffer[0] == '\0' && buffer[1] == '\0');
  assert (buffer[2] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test9String, 3) == NULL);
  assert (buffer[0] == '\0' && buffer[1] == '\0' && buffer[2] == '\0');
  assert (buffer[3] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test9String, BUFSIZ) == NULL);
  assert (buffer[0] == '\0');
  assert (buffer[BUFSIZ - 1] == '\0');

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test10String, 2) == NULL);
  assert (buffer[0] == '\0' && buffer[1] == '\0');
  assert (buffer[2] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  assert (strncpy (buffer, Test10String, BUFSIZ) == NULL);
  assert (buffer[0] == '\0');
  assert (buffer[BUFSIZ - 1] == '\0');
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
