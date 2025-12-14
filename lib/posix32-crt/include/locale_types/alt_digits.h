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

#ifndef LIBPOSIX32_ALT_DIGITS_H_INCLUDED
#define LIBPOSIX32_ALT_DIGITS_H_INCLUDED

#include <limits.h>
#include <stdint.h>
#include <wchar.h>

/**
 * Alternative digits.
 */
typedef struct AltDigits {
  char    *A;
  wchar_t *W;
  /**
   * Multibyte representation of each alternative digit.
   */
  struct {
    /**
     * Length of character stored in `Char`.
     */
    uint8_t Length;
    char    Char[MB_LEN_MAX];
  } Map[10];
} AltDigits;

/**
 * Store broken-down `altDigits->A` in `altDigits->Map`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_alt_digits (AltDigits *altDigits, locale_t locale);

#endif /* LIBPOSIX32_ALT_DIGITS_H_INCLUDED */
