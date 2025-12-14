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

#include "p32_wctype.h"

typedef struct {
  /**
   * Character class.
   *
   * This is one of `P32_CHARTYPE_*` constants.
   */
  uint16_t CharType;
  /**
   * Information bits which indicate that character belongs `CharType`.
   *
   * If any of these bits is set, character belongs to `CharType`.
   */
  uint16_t IncludeBits;
  /**
   * Information bits which are not compatible with `CharType`.
   *
   * If any of these bits is set, character does not belong to `CharType`.
   */
  uint16_t ExcludeBits;
} WcType1;

/**
 * Mapping between supported character classes and `CT_CTYPE1` information bits
 * returned by `GetStringType` function.
 */
static const WcType1 WcType1Map[] = {
  /**
   * `C1_ALPHA` implies `C1_LOWER` and `C1_UPPER`.
   */
  {P32_CHARTYPE_ALNUM,   (C1_ALPHA | C1_DIGIT),                       0         },
  /**
   * `C1_ALPHA` implies `C1_LOWER` and `C1_UPPER`.
   */
  {P32_CHARTYPE_ALPHA,   (C1_ALPHA),                                  0         },
  {P32_CHARTYPE_BLANK,   (C1_BLANK),                                  0         },
  {P32_CHARTYPE_CONTROL, (C1_CNTRL),                                  0         },
  {P32_CHARTYPE_DIGIT,   (C1_DIGIT),                                  0         },
  /**
   * Any character that has a glyph.
   */
  {P32_CHARTYPE_GRAPH,   (C1_ALPHA | C1_DIGIT | C1_PUNCT),            0         },
  {P32_CHARTYPE_LOWER,   (C1_LOWER),                                  0         },
  /**
   * Any `graph` or `blank` character, except those which also belong to
   * `cntrl` character class.
   */
  {P32_CHARTYPE_PRINT,   (C1_ALPHA | C1_BLANK | C1_DIGIT | C1_PUNCT), (C1_CNTRL)},
  {P32_CHARTYPE_PUNCT,   (C1_PUNCT),                                  0         },
  {P32_CHARTYPE_SPACE,   (C1_SPACE),                                  0         },
  {P32_CHARTYPE_UPPER,   (C1_UPPER),                                  0         },
  {P32_CHARTYPE_XDIGIT,  (C1_XDIGIT),                                 0         },
};

#define P32_IS_CHARTYPE(bits, wctype, chartype, map, bit)                                    \
  (wctype & chartype) && (bits & map[bit].IncludeBits) && (bits & map[bit].ExcludeBits) == 0

/**
 * Get character information of type `category` for single wide character `wc`.
 *
 * Note that this function uses GetStringTypeW instead of GetStringTypeExW.
 * These functions are locale-independant and are equivalent when used with
 * wide characters.
 *
 * ANSI version (GetStringTypeExA) uses passed LCID to convert its input
 * to Unicode using locale's default ANSI code page, but otherwise does not
 * depend on locale settings.
 */
static void P32GetCharType (uint16_t *bits, wchar_t wc, uint32_t category) {
  if (!GetStringTypeW (category, &wc, 1, bits)) {
    bits = 0;
  }
}

/**
 * Return subset of character classes from `wctype` which apply to single
 * wide character `wc`.
 */
static uint16_t P32IsCharType (wchar_t wc, wctype_t wctype, locale_t locale) {
  uint16_t charTypeBits = 0;

  /**
   * `CT_CTYPE1` information bits for wide character `wc`.
   */
  uint16_t charType1Bits = 0;

  P32GetCharType (&charType1Bits, wc, CT_CTYPE1);

  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_ALNUM, WcType1Map, P32_CHARTYPE_BIT_ALNUM)) {
    charTypeBits |= P32_CHARTYPE_ALNUM;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_ALPHA, WcType1Map, P32_CHARTYPE_BIT_ALPHA)) {
    charTypeBits |= P32_CHARTYPE_ALPHA;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_BLANK, WcType1Map, P32_CHARTYPE_BIT_BLANK)) {
    charTypeBits |= P32_CHARTYPE_BLANK;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_CONTROL, WcType1Map, P32_CHARTYPE_BIT_CONTROL)) {
    charTypeBits |= P32_CHARTYPE_CONTROL;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_DIGIT, WcType1Map, P32_CHARTYPE_BIT_DIGIT)) {
    charTypeBits |= P32_CHARTYPE_DIGIT;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_GRAPH, WcType1Map, P32_CHARTYPE_BIT_GRAPH)) {
    charTypeBits |= P32_CHARTYPE_GRAPH;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_LOWER, WcType1Map, P32_CHARTYPE_BIT_LOWER)) {
    charTypeBits |= P32_CHARTYPE_LOWER;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_PRINT, WcType1Map, P32_CHARTYPE_BIT_PRINT)) {
    charTypeBits |= P32_CHARTYPE_PRINT;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_PUNCT, WcType1Map, P32_CHARTYPE_BIT_PUNCT)) {
    charTypeBits |= P32_CHARTYPE_PUNCT;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_SPACE, WcType1Map, P32_CHARTYPE_BIT_SPACE)) {
    charTypeBits |= P32_CHARTYPE_SPACE;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_UPPER, WcType1Map, P32_CHARTYPE_BIT_UPPER)) {
    charTypeBits |= P32_CHARTYPE_UPPER;
  }
  if (P32_IS_CHARTYPE (charType1Bits, wctype, P32_CHARTYPE_XDIGIT, WcType1Map, P32_CHARTYPE_BIT_XDIGIT)) {
    charTypeBits |= P32_CHARTYPE_XDIGIT;
  }

  return charTypeBits;
  UNREFERENCED_PARAMETER (locale);
}

int p32_iswctype_l (wint_t wc, wctype_t wctype, locale_t locale) {
  return P32IsCharType (wc, wctype, locale);
}

int p32_iswctype (wint_t wc, wctype_t wctype) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return p32_iswctype_l (wc, wctype, activeLocale);
}
