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
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-test.h"

/**
 * Test Summary:
 *
 * Test `c32rtomb` function with ASCII (code page 20127).
 */

#define LOCALE "en_US.ASCII"

static void DoTest (void) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state = {0};

  /**
   * Detect invalid conversion state
   */
  TaintConversionState (&state);
  memset (buffer, EOF, _countof (buffer));

  assert (c32rtomb (buffer, U'\0', &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  /**
   * Reset conversion state.
   */
  assert (c32rtomb (NULL, 0xFFFFFFFF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * All Code Points in range [0,127] are valid ASCII characters.
   */
  for (char32_t c32 = 0; c32 < 0x80; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == 1);
    assert (buffer[0] == (char) c32 && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * All Code Point outside of range [0,127] are invalid.
   */
  for (char32_t c32 = 0x80; c32 <= 0x0010FFFF; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == (size_t) -1);
    assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);
  }
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
  srand (0xBADF);

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
