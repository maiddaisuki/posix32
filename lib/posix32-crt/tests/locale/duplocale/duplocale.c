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
 * Simple test for `duplocale`.
 */

int main (void) {
  p32_test_init ();

  locale_t locale = NULL;
  locale_t copy   = NULL;

  assert ((locale = newlocale (LC_ALL_MASK, "en-US", NULL)) != NULL);
  assert ((copy = duplocale (locale)) != NULL);

  assert (strcmp (getlocalename_l (LC_ALL, locale), getlocalename_l (LC_ALL, copy)) == 0);
  assert (strcmp (getlocalename_l (LC_COLLATE, locale), getlocalename_l (LC_COLLATE, copy)) == 0);
  assert (strcmp (getlocalename_l (LC_CTYPE, locale), getlocalename_l (LC_CTYPE, copy)) == 0);
  assert (strcmp (getlocalename_l (LC_MESSAGES, locale), getlocalename_l (LC_MESSAGES, copy)) == 0);
  assert (strcmp (getlocalename_l (LC_MONETARY, locale), getlocalename_l (LC_MONETARY, copy)) == 0);
  assert (strcmp (getlocalename_l (LC_NUMERIC, locale), getlocalename_l (LC_NUMERIC, copy)) == 0);
  assert (strcmp (getlocalename_l (LC_TIME, locale), getlocalename_l (LC_TIME, copy)) == 0);

  freelocale (copy);
  freelocale (locale);

  return EXIT_SUCCESS;
}
