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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"
#include "wchar-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to deal with locale's alternative digits.
 */

bool p32_alt_digits (AltDigits *altDigits, locale_t locale) {
  if (altDigits->A[0] == '\0') {
    return true;
  }

  const char *str = altDigits->A;

  /**
   * Conversion state for `str`.
   */
  mbstate_t state = {0};

  for (size_t i = 0; i < _countof (altDigits->Map); ++i) {
    size_t length = p32_private_mbrlen_l (str, locale->Charset.MaxLength, &state, locale);

    assert (length != 0);
    assert (length != (size_t) -1);
    assert (length != (size_t) -2);

    /**
     * If `length == 0`, then we encountered terminating NUL unexpectedly
     * early.
     */
    if (length == 0 || length == (size_t) -1 || length == (size_t) -2) {
      return false;
    }

    memcpy (altDigits->Map[i].Char, str, length);
    altDigits->Map[i].Length = (uint8_t) length;

    str += length;
  }

  assert (p32_private_mbrlen_l (str, locale->Charset.MaxLength, &state, locale) == 0);

  return true;
}
