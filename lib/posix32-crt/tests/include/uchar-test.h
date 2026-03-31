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

#ifndef LIBPOSIX32_UCHAR_TEST_H_INCLUDED
#define LIBPOSIX32_UCHAR_TEST_H_INCLUDED

#include "tests-internal.h"

#define HIGH_SURROGATE_MIN 0xD800
#define HIGH_SURROGATE_MAX 0xDBFF

#define LOW_SURROGATE_MIN 0xDC00
#define LOW_SURROGATE_MAX 0xDFFF

#define C(c) (char) (unsigned) c

#define STRING_A(str) _countof (str) - 1, str
#define STRING_W(str) _countof (TEXT (str)) - 1, TEXT (str)
#define STRING(str)   STRING_A (str), STRING_W (str)

typedef struct {
  size_t   LengthA;
  char    *A;
  size_t   LengthW;
  wchar_t *W;
} Text;

/**
 * ASCII text.
 */
const Text AsciiText = {STRING ("Simple English text.")};

/**
 * SBCS text (code page 1252).
 */
const Text SBCSText = {
  15,
  (char[]) {
            C ('S'), C (0xF6), C ('m'), C (0xE8), C (' '), C ('f'), C (0xFB), C ('n'), C ('n'), C (0xFF), C (' '), C ('t'),
            C (0xEA), C ('x'), C ('t'), C (0x0)
  },
  STRING_W ("Sömè fûnnÿ têxt")
};

/**
 * DBCS text (code page 932).
 */
const Text DBCSText = {
  14,
  (char[]) {
            C (0x93), C (0xFA), C (0x96), C (0x7B), C (0x8C), C (0xEA), C (0x83), C (0x65), C (0x83), C (0x4E), C (0x83),
            C (0x58), C (0x83), C (0x67), C (0x0)
  },
  STRING_W ("日本語テクスト")
};

/**
 * Mix of single-byte and double-byte characters.
 */
const Text MixedText = {
  13,
  (char[]) {
            C (0x93), C (0xFA), C ('n'), C ('i'), C (0x96), C (0x7B), C ('h'), C ('o'), C ('n'), C (0x8C), C (0xEA), C ('g'),
            C ('o'), C (0x0)
  },
  STRING_W ("日ni本hon語go")
};

const struct CJK {
  const char *DBCS;
  const char *UTF8;
  wchar_t     Wc;
  char16_t    UTF16;
  char32_t    UTF32;
} CJK[] = {
  {(char[]) {C (0x93), C (0xFA), C (0x00)}, "日", L'日', u'日', U'日'},
  {(char[]) {C (0x96), C (0x7B), C (0x00)}, "本", L'本', u'本', U'本'},
  {(char[]) {C (0x8C), C (0xEA), C (0x00)}, "語", L'語', u'語', U'語'},
  {(char[]) {C (0x83), C (0x65), C (0x00)}, "テ", L'テ', u'テ', U'テ'},
  {(char[]) {C (0x83), C (0x4E), C (0x00)}, "ク", L'ク', u'ク', U'ク'},
  {(char[]) {C (0x83), C (0x58), C (0x00)}, "ス", L'ス', u'ス', U'ス'},
  {(char[]) {C (0x83), C (0x67), C (0x00)}, "ト", L'ト', u'ト', U'ト'},
  {(char[]) {C (0x81), C (0x42), C (0x00)}, "。", L'。', u'。', U'。'},
};

/**
 * Unicode Text.
 */
const Text UnicodeText = {STRING ("日本語テクスト")};

/**
 * Unicode Text which will produce UTF-16 surrogate pairs in output.
 */
const Text UnicodeTextWithSurrogatePairs = {STRING ("🧡🐔")};

/**
 * UTF-8 Code Unit Sequence with length 2
 */
const char UTF8Length2[] = "¥";

/**
 * UTF-8 Code Unit Sequence with length 3
 */
const char UTF8Length3[] = "語";

/**
 * UTF-8 Code Unit Sequence with length 4
 */
const char UTF8Length4[] = "🧡";

P32_ALWAYS_INLINE void TaintConversionState (mbstate_t *state) {
#if P32_CRT >= P32_UCRT
  state->_Byte  = (unsigned short) rand ();
  state->_State = (unsigned short) rand ();
  state->_Wchar = (rand () << 16) | rand ();
#else
  *state = (rand () << 16) | rand ();
#endif
}

P32_ALWAYS_INLINE void ResetConversionState (mbstate_t *state) {
  memset (state, 0, sizeof (mbstate_t));
}

P32_ALWAYS_INLINE bool EqualConversionState (mbstate_t *s1, mbstate_t *s2) {
  return memcmp (s1, s2, sizeof (mbstate_t)) == 0;
}

#endif /* LIBPOSIX32_UCHAR_TEST_H_INCLUDED */
