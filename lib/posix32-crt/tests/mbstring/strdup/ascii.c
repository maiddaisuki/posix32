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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `p32_private_strdup_l` function with ASCII (code page 20127).
 */

#define C(c)        (char) (unsigned) (c)
#define C8(c)       C (0x80 | (c))
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.ASCII"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strdup_l`.
 */
#define strdup(s) p32_private_strdup_l (s, locale)

static void DoTest (void) {
  char *str = NULL;

  /**
   * Basic `strdup` usage.
   */
  const char *Test1String = "";
  const char *Test2String = "ABC";

  _set_errno (0);
  assert ((str = strdup (Test1String)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, Test1String) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strdup (Test2String)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, Test2String) == 0);
  free (str);

  /**
   * Test input which contains non-ASCII Code Points.
   */
  const char *Test3String = STRING ('A', 'B', C8 ('C'), C8 ('D'));

  _set_errno (0);
  assert ((str = strdup (Test3String)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
