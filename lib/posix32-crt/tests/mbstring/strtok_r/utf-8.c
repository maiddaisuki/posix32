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

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `p32_private_strtok_r_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strtok_r_l`.
 */
#define strtok_r(s, d, c) p32_private_strtok_r_l (s, d, c, locale)

static void DoTest (void) {
  char *str     = NULL;
  char *token   = NULL;
  char *context = NULL;

  /**
   * Basic test 1.
   *
   * Use empty string as separator.
   */
  const char *Test1String = "ABC";
  const char *Test1Set    = "";

  assert ((str = strdup (Test1String)) != NULL);

  assert ((token = strtok_r (str, Test1Set, &context)) == str);
  assert (strcmp (token, "ABC") == 0);
  assert (context == str + 3);

  assert ((token = strtok_r (NULL, Test1Set, &context)) == NULL);
  assert (context == str + 3);

  free (str);

  str     = NULL;
  token   = NULL;
  context = NULL;

  /**
   * Basic test 2.
   *
   * Split string which consists entirely of characters in "separator".
   */
  const char *Test2String = "112233332211";
  const char *Test2Set    = "321";

  assert ((str = strdup (Test2String)) != NULL);

  assert ((token = strtok_r (str, Test2Set, &context)) == NULL);
  assert (context == str + 12);

  assert ((token = strtok_r (NULL, Test2Set, &context)) == NULL);
  assert (context == str + 12);

  free (str);

  str     = NULL;
  token   = NULL;
  context = NULL;

  /**
   * Split PATH-like string.
   */
  const char *Test3String = "C:\\Windows\\System32;C:\\Windows\\SysWOW64;C:\\Windows";
  const char *Test3Set    = ";";

  assert ((str = strdup (Test3String)) != NULL);

  assert ((token = strtok_r (str, Test3Set, &context)) == str);
  assert (strcmp (token, "C:\\Windows\\System32") == 0);
  assert (context == str + 20);

  assert ((token = strtok_r (NULL, Test3Set, &context)) == str + 20);
  assert (strcmp (token, "C:\\Windows\\SysWOW64") == 0);
  assert (context == str + 40);

  assert ((token = strtok_r (NULL, Test3Set, &context)) == str + 40);
  assert (strcmp (token, "C:\\Windows") == 0);
  assert (context == str + 50);

  assert ((token = strtok_r (NULL, Test3Set, &context)) == NULL);
  assert (context == str + 50);

  free (str);

  str     = NULL;
  token   = NULL;
  context = NULL;

  /**
   * Split words separated by punctuation and whitespace.
   *
   * This allows to tests that characters in "set" found at the start of the
   * "token" are skipped.
   */
  const char *Test4String = "Word1 word2, word3?! Word4: word5.";
  const char *Test4Set    = " ,.!?:;";

  assert ((str = strdup (Test4String)) != NULL);

  assert ((token = strtok_r (str, Test4Set, &context)) == str);
  assert (strcmp (token, "Word1") == 0);
  assert (context == str + 6);

  assert ((token = strtok_r (NULL, Test4Set, &context)) == str + 6);
  assert (strcmp (token, "word2") == 0);
  assert (context == str + 12);

  assert ((token = strtok_r (NULL, Test4Set, &context)) == str + 13);
  assert (strcmp (token, "word3") == 0);
  assert (context == str + 19);

  assert ((token = strtok_r (NULL, Test4Set, &context)) == str + 21);
  assert (strcmp (token, "Word4") == 0);
  assert (context == str + 27);

  assert ((token = strtok_r (NULL, Test4Set, &context)) == str + 28);
  assert (strcmp (token, "word5") == 0);
  assert (context == str + 34);

  assert ((token = strtok_r (NULL, Test4Set, &context)) == NULL);
  assert (context == str + 34);

  free (str);

  str     = NULL;
  token   = NULL;
  context = NULL;

  /**
   * Test input which contains multibyte characters.
   */
  const char *Test5String = STRING (
    // clang-format off
    C (0xC2), C (0x80),
    C (0xE0), C (0xA0), C (0x80),
    C (0xF0), C (0x90), C (0x80), C (0x80),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xDF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF),
    C (0xE0), C (0xA0), C (0xBF),
    C (0xF0), C (0x90), C (0xBF), C (0xBF),
    C (0xC2), C (0xBF)
    // clang-format on
  );

  const char *Test5Set = STRING (
    // clang-format off
    C (0xF0), C (0x90), C (0xBF), C (0xBF),
    C (0xDF), C (0xBF),
    C (0xE0), C (0xA0), C (0x80)
    // clang-format on
  );

  assert ((str = strdup (Test5String)) != NULL);

  assert ((token = strtok_r (str, Test5Set, &context)) == str);
  assert (strcmp (token, STRING (C (0xC2), C (0x80))) == 0);
  assert (context == str + 5);

  assert ((token = strtok_r (NULL, Test5Set, &context)) == str + 5);
  assert (strcmp (token, STRING (C (0xF0), C (0x90), C (0x80), C (0x80), C (0xEF), C (0xBF), C (0xBF))) == 0);
  assert (context == str + 14);

  assert ((token = strtok_r (NULL, Test5Set, &context)) == str + 14);
  assert (strcmp (token, STRING (C (0xF4), C (0x8F), C (0xBF), C (0xBF), C (0xE0), C (0xA0), C (0xBF))) == 0);
  assert (context == str + 25);

  assert ((token = strtok_r (NULL, Test5Set, &context)) == str + 25);
  assert (strcmp (token, STRING (C (0xC2), C (0xBF))) == 0);
  assert (context == str + 27);

  assert ((token = strtok_r (NULL, Test5Set, &context)) == NULL);
  assert (context == str + 27);

  free (str);

  str     = NULL;
  token   = NULL;
  context = NULL;
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
