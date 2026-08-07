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
 * This test verifies conformance of UTF-8 conversion functions to
 * Unicode Standard.
 *
 * In particular, this test verifies that such functions will only consume
 * well-formed UTF-8 Code Unit Sequence as listed in "Table 3-7. Well-Formed
 * UTF-8 Byte Sequences"[1].
 *
 * Such functions must signal an error as soon as an ill-formed UTF-8 Code Unit
 * Sequence is encountered; in case of conversion functions such as `c8rtomb`,
 * this means returning `(size_t) -1` and setting `errno` to `EILSEQ`.
 *
 * Conformance to the above requirements allows to handle an ill-formed UTF-8
 * Code Unit Sequence as described in "3.9.6 U+FFFD Substitution of Maximal
 * Subparts"[2].
 *
 * [1] <https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G31703>
 * [2] <https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G66453>
 */

#undef c8rtomb

/**
 * `Charset` structure for code page 65001 (UTF-8).
 */
static Charset utf8;

/**
 * Convenience macro to call `p32_private_c8rtomb_cp65001`.
 */
#define c8rtomb(mb, c8, s) p32_private_c8rtomb_cp65001 (mb, c8, s, &utf8)

static void TestRangeInvalid (char8_t u1) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state1 = {0};
  assert (c8rtomb (buffer, u1, &state1) == (size_t) -1);
}

static void TestRange1 (char8_t u1) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state1 = {0};
  assert (c8rtomb (buffer, u1, &state1) == 1);
}

static void TestRange2 (char8_t u1) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state1 = {0};

  assert (c8rtomb (buffer, u1, &state1) == 0);

  for (char8_t u2 = 0x00;; ++u2) {
    mbstate_t state2 = state1;

    if (u2 >= 0x80 && u2 <= 0xBF) {
      assert (c8rtomb (buffer, u2, &state2) == 2);
    } else {
      assert (c8rtomb (buffer, u2, &state2) == (size_t) -1);
    }

    if (u2 == 0xFF) {
      break;
    }
  }
}

static void TestRange3 (char8_t u1, char8_t trailingStart, char8_t trailingEnd) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state1 = {0};

  assert (c8rtomb (buffer, u1, &state1) == 0);

  for (char8_t u2 = 0x00;; ++u2) {
    mbstate_t state2 = state1;

    if (u2 >= trailingStart && u2 <= trailingEnd) {
      assert (c8rtomb (buffer, u2, &state2) == 0);

      for (char8_t u3 = 0x00;; ++u3) {
        mbstate_t state3 = state2;

        if (u3 >= 0x80 && u3 <= 0xBF) {
          assert (c8rtomb (buffer, u3, &state3) == 3);
        } else {
          assert (c8rtomb (buffer, u3, &state3) == (size_t) -1);
        }

        if (u3) {
          break;
        }
      }
    } else {
      assert (c8rtomb (buffer, u2, &state2) == (size_t) -1);
    }

    if (u2 == 0xFF) {
      break;
    }
  }
}

static void TestRange4 (char8_t u1, char8_t trailingStart, char8_t trailingEnd) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state1 = {0};

  assert (c8rtomb (buffer, u1, &state1) == 0);

  for (char8_t u2 = 0x00;; ++u2) {
    mbstate_t state2 = state1;

    if (u2 >= trailingStart && u2 <= trailingEnd) {
      assert (c8rtomb (buffer, u2, &state2) == 0);

      for (char8_t u3 = 0x00;; ++u3) {
        mbstate_t state3 = state2;

        if (u3 >= 0x80 && u3 <= 0xBF) {
          assert (c8rtomb (buffer, u3, &state3) == 0);

          for (char8_t u4 = 0x00;; ++u4) {
            mbstate_t state4 = state3;

            if (u4 >= 0x80 && u4 <= 0xBF) {
              assert (c8rtomb (buffer, u4, &state4) == 4);
            } else {
              assert (c8rtomb (buffer, u4, &state4) == (size_t) -1);
            }

            if (u4 == 0xFF) {
              break;
            }
          }

        } else {
          assert (c8rtomb (buffer, u3, &state3) == (size_t) -1);
        }

        if (u3 == 0xFF) {
          break;
        }
      }
    } else {
      assert (c8rtomb (buffer, u2, &state2) == (size_t) -1);
    }

    if (u2 == 0xFF) {
      break;
    }
  }
}

int main (void) {
  p32_test_init ();

  utf8.CodePage = CP_UTF8;
  assert (p32_charset_info (&utf8));

  /**
   * Bytes [0x00-0x7F] are single-byte UTF-8 characters.
   */
  for (char8_t c8 = 0x00; c8 <= 0x7F; ++c8) {
    TestRange1 (c8);
  }

  /**
   * Bytes [0x80-0xC1] are not valid UTF-8 leading bytes.
   */
  for (char8_t c8 = 0x80; c8 <= 0xC1; ++c8) {
    TestRangeInvalid (c8);
  }

  /**
   * Bytes [0xC2-0xDB] are valid leading bytes for UTF-8 Code Unit Sequence
   * with length 2.
   */
  for (char8_t c8 = 0xC2; c8 <= 0xDF; ++c8) {
    TestRange2 (c8);
  }

  /**
   * Byte 0xE0 is a valid leading byte for UTF-8 Code Unit Sequence
   * with length 3.
   *
   * An UTF-8 Code Unit Sequence starting with 0xE0 is an exception to the rule
   * that valid trailing bytes are in range [0x80-0xBF]; second byte must be
   * in range [0xA0-0xBF] for a well-formed UTF-8 Code Unit Sequence.
   */
  TestRange3 (0xE0, 0xA0, 0xBF);

  for (char8_t c8 = 0xE1; c8 <= 0xEC; ++c8) {
    TestRange3 (c8, 0x80, 0xBF);
  }

  /**
   * Byte 0xED is a valid leading byte for UTF-8 Code Unit Sequence
   * with length 3.
   *
   * An UTF-8 Code Unit Sequence starting with 0xED is an exception to the rule
   * that valid trailing bytes are in range [0x80-0xBF]; second byte must be
   * in range [0x80-0x9F] for a well-formed UTF-8 Code Unit Sequence.
   */
  TestRange3 (0xED, 0x80, 0x9F);

  /**
   * Bytes [0xEE-0xEF] are valid leading bytes for UTF-8 Code Unit Sequence
   * with length 3.
   */
  for (char8_t c8 = 0xEE; c8 <= 0xEF; ++c8) {
    TestRange3 (c8, 0x80, 0xBF);
  }

  /**
   * Byte 0xF0 is a valid leading byte for UTF-8 Code Unit Sequence
   * with length 4.
   *
   * An UTF-8 Code Unit Sequence starting with 0xF0 is an exception to the rule
   * that valid trailing bytes are in range [0x80-0xBF]; second byte must be
   * in range [0x90-0xBF] for a well-formed UTF-8 Code Unit Sequence.
   */
  TestRange4 (0xF0, 0x90, 0xBF);

  /**
   * Bytes [0xF1-0xF3] are valid leading bytes for UTF-8 Code Unit Sequence
   * with length 4.
   */
  for (char8_t c8 = 0xF1; c8 <= 0xF3; ++c8) {
    TestRange4 (c8, 0x80, 0xBF);
  }

  /**
   * Byte 0xF4 is a valid leading byte for UTF-8 Code Unit Sequence
   * with length 4.
   *
   * An UTF-8 Code Unit Sequence starting with 0xF4 is an exception to the rule
   * that valid trailing bytes are in range [0x80-0xBF]; second byte must be
   * in range [0x80-0x8F] for a well-formed UTF-8 Code Unit Sequence.
   */
  TestRange4 (0xF4, 0x80, 0x8F);

  /**
   * Bytes [0xF5-0xFF] are not valid UTF-8 leading bytes.
   */
  for (char8_t c8 = 0xF5;; ++c8) {
    TestRangeInvalid (c8);

    if (c8 == 0xFF) {
      break;
    }
  }

  return EXIT_SUCCESS;
}
