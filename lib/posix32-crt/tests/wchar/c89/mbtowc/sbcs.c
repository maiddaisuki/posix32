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
#include <errno.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-test.h"

/**
 * Test Summary:
 *
 * Test `mbtowc` function with single-byte character set.
 *
 * We use ISO-8859-1 (code page 28951) for testing, in which all bytes map
 * to the same Unicode Code Point values.
 *
 * Note that there are single-byte code pages where not all 256 bytes are valid
 * characters.
 */

#define LOCALE "en_US.ISO-8859-1"

static void DoTest (void) {
  wchar_t wc = WEOF;

  /**
   * When second argument to `mbtowc` is `NULL`, it must return non-zero for
   * state-dependant encodings. Otherwise it must return 0.
   */
  assert (mbtowc (&wc, NULL, 0) == 0);
  assert (wc == WEOF);
  assert (errno == 0);

  /**
   * When third argument to `mbtowc` is zero, it must not examine its second
   * argument.
   */
  assert (mbtowc (&wc, "", 0) == -1);
  assert (wc == WEOF);
  assert (errno == 0);

  /**
   * All bytes are valid characters.
   */
  for (uint8_t c = 0;; ++c) {
    wc = WEOF;

    assert (mbtowc (&wc, (char *) &c, MB_CUR_MAX) == !!c);
    assert (wc == c);
    assert (errno == 0);

    if (c == 0xFF) {
      break;
    }
  }

  /**
   * Probe internal conversion state.
   */
  wc = WEOF;

  assert (mbtowc (&wc, "", 1) == 0);
  assert (wc == '\0');
  assert (errno == 0);
}

static DWORD CALLBACK Thread (LPVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, LOCALE) != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

  assert (setlocale (LC_ALL, "C") != NULL);
  assert (MB_CUR_MAX == 1);

  HANDLE thread   = NULL;
  DWORD  exitCode = EXIT_FAILURE;

  assert ((thread = CreateThread (NULL, 0, Thread, LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (thread, INFINITE);
  GetExitCodeThread (thread, &exitCode);
  CloseHandle (thread);

  return EXIT_SUCCESS;
}
