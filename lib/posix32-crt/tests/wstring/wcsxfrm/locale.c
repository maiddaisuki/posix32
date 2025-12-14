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

#include "posix32-ext/p32_wchar-ext.h"

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `wcsxfrm` function.
 *
 * Two strings produced by `wcsxfrm` for a given locale, when passed to
 * `wcscmp`, must compare as if origin strings were passed to `wcscoll` for
 * that locale.
 */

static int exit_code = EXIT_SUCCESS;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  wchar_t *wcs1 = (wchar_t *) nl_langinfo_l (p32_WMON_1, locale);
  wchar_t *wcs2 = (wchar_t *) nl_langinfo_l (p32_WMON_2, locale);

  wchar_t *buffer1 = NULL;
  wchar_t *buffer2 = NULL;

  size_t bufferSize1 = 0;
  size_t bufferSize2 = 0;

  assert ((bufferSize1 = wcsxfrm_l (buffer1, wcs1, 0, locale)) != 0);
  bufferSize1 += 1;
  assert ((buffer1 = malloc (bufferSize1 * sizeof (wchar_t))) != NULL);
  assert ((wcsxfrm_l (buffer1, wcs1, bufferSize1, locale)) < bufferSize1);

  assert ((bufferSize2 = wcsxfrm_l (buffer2, wcs2, 0, locale)) != 0);
  bufferSize2 += 1;
  assert ((buffer2 = malloc (bufferSize2 * sizeof (wchar_t))) != NULL);
  assert ((wcsxfrm_l (buffer2, wcs2, bufferSize2, locale)) < bufferSize2);

  int cmpDiff  = wcscmp (buffer1, buffer2);
  int collDiff = wcscoll_l (wcs1, wcs2, locale);

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
   * Test all supported locales.
   */
  p32_locale_test_func2 (Test, P32_LOCALE_TEST_ALL | P32_LOCALE_TEST_ONCE);

  return exit_code;
}
