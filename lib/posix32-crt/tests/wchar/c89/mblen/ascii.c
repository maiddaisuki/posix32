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
#include <errno.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-test.h"

/**
 * Test Summary:
 *
 * Test `mblen` function with ASCII (code page 20127).
 */

#define LOCALE "en_US.ASCII"

static locale_t locale;

static void DoTest (void) {
  /**
   * When first argument to `mblen` is `NULL`, it must return non-zero for
   * state-dependant encodings. Otherwise it must return 0.
   */
  assert (mblen_l (NULL, MB_CUR_MAX_L (locale), locale) == 0);
  assert (errno == 0);

  /**
   * When second argument to `mblen` is zero, it must not examine its first
   * argument.
   */
  assert (mblen_l ("", 0, locale) == -1);
  assert (errno == 0);

  /**
   * All bytes in range [0,127] are valid ASCII characters.
   */
  for (uint8_t c = 0;; ++c) {
    assert (mblen_l ((char *) &c, MB_CUR_MAX_L (locale), locale) == !!c);
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  /**
   * All bytes in range [128,255] are invalid.
   */
  for (uint8_t c = 128;; ++c) {
    assert (mblen_l ((char *) &c, MB_CUR_MAX_L (locale), locale) == -1);
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);

    if (c == 0xFF) {
      break;
    }
  }
}

int main (void) {
  p32_test_init ();

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 1);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
