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

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `strxfrm` function.
 *
 * Two strings produced by `strxfrm` for a given locale, when passed to
 * `strcmp`, must compare as if origin strings were passed to `strcoll` for
 * that locale.
 */

static int exit_code = EXIT_SUCCESS;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  char *str1 = nl_langinfo_l (MON_1, locale);
  char *str2 = nl_langinfo_l (MON_2, locale);

  char *buffer1 = NULL;
  char *buffer2 = NULL;

  size_t bufferSize1 = 0;
  size_t bufferSize2 = 0;

  assert ((bufferSize1 = strxfrm_l (buffer1, str1, 0, locale)) != 0);
  bufferSize1 += 1;
  assert ((buffer1 = malloc (bufferSize1)) != NULL);
  assert ((strxfrm_l (buffer1, str1, bufferSize1, locale)) < bufferSize1);

  assert ((bufferSize2 = strxfrm_l (buffer2, str2, 0, locale)) != 0);
  bufferSize2 += 1;
  assert ((buffer2 = malloc (bufferSize2)) != NULL);
  assert ((strxfrm_l (buffer2, str2, bufferSize2, locale)) < bufferSize2);

  int cmpDiff  = strcmp (buffer1, buffer2);
  int collDiff = strcoll_l (str1, str2, locale);

  if (cmpDiff != collDiff) {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"%-24s: %d | %d\n", localeName, cmpDiff, collDiff);
  }

  assert (collDiff != _NLSCMPERROR);

  free (buffer1);
  free (buffer2);

  return true;
}

int main (void) {
  p32_test_init ();

  /**
   * Test all working locales
   */
  p32_locale_test_func2 (Test, P32_LOCALE_TEST_ALL);

  return exit_code;
}
