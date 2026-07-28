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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `p32_private_strlen_l` function with ASCII (code page 20127).
 */

#define C(c)        (char) (unsigned) (c)
#define C8(c)       C (0x80 | (c))
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.ASCII"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strlen_l`.
 */
#define strlen(s) p32_private_strlen_l (s, locale)

static void DoTest (void) {
  /**
   * Basic `strlen` usage.
   */
  assert (strlen ("") == 0);
  assert (strlen ("A") == 1);
  assert (strlen ("AB") == 2);
  assert (strlen ("ABC") == 3);

  /**
   * Test input which contains non-ASCII Code Points.
   */
  const char *Test1String = STRING ('A', 'B', C8 ('C'), C8 ('D'));

  assert (strlen (Test1String) == (size_t) -1);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
