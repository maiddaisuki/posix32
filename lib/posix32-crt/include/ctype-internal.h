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

#ifndef LIBPOSIX32_CTYPE_INTERNAL_H_INCLUDED
#define LIBPOSIX32_CTYPE_INTERNAL_H_INCLUDED

#include "p32_ctype.h"
#include "p32_wctype.h"

#define P32_CHARTYPE_BIT_ALNUM   (0)
#define P32_CHARTYPE_BIT_ALPHA   (1)
#define P32_CHARTYPE_BIT_BLANK   (2)
#define P32_CHARTYPE_BIT_CONTROL (3)
#define P32_CHARTYPE_BIT_DIGIT   (4)
#define P32_CHARTYPE_BIT_GRAPH   (5)
#define P32_CHARTYPE_BIT_LOWER   (6)
#define P32_CHARTYPE_BIT_PRINT   (7)
#define P32_CHARTYPE_BIT_PUNCT   (8)
#define P32_CHARTYPE_BIT_SPACE   (9)
#define P32_CHARTYPE_BIT_UPPER   (10)
#define P32_CHARTYPE_BIT_XDIGIT  (11)

#define P32_CHARTYPE_ALNUM   (1 << P32_CHARTYPE_BIT_ALNUM)
#define P32_CHARTYPE_ALPHA   (1 << P32_CHARTYPE_BIT_ALPHA)
#define P32_CHARTYPE_BLANK   (1 << P32_CHARTYPE_BIT_BLANK)
#define P32_CHARTYPE_CONTROL (1 << P32_CHARTYPE_BIT_CONTROL)
#define P32_CHARTYPE_DIGIT   (1 << P32_CHARTYPE_BIT_DIGIT)
#define P32_CHARTYPE_GRAPH   (1 << P32_CHARTYPE_BIT_GRAPH)
#define P32_CHARTYPE_LOWER   (1 << P32_CHARTYPE_BIT_LOWER)
#define P32_CHARTYPE_PRINT   (1 << P32_CHARTYPE_BIT_PRINT)
#define P32_CHARTYPE_PUNCT   (1 << P32_CHARTYPE_BIT_PUNCT)
#define P32_CHARTYPE_SPACE   (1 << P32_CHARTYPE_BIT_SPACE)
#define P32_CHARTYPE_UPPER   (1 << P32_CHARTYPE_BIT_UPPER)
#define P32_CHARTYPE_XDIGIT  (1 << P32_CHARTYPE_BIT_XDIGIT)

#define P32_CHARTYPE_ALL                                                                                    \
  (P32_CHARTYPE_ALNUM | P32_CHARTYPE_ALPHA | P32_CHARTYPE_BLANK | P32_CHARTYPE_CONTROL | P32_CHARTYPE_DIGIT \
   | P32_CHARTYPE_GRAPH | P32_CHARTYPE_LOWER | P32_CHARTYPE_PRINT | P32_CHARTYPE_PUNCT | P32_CHARTYPE_SPACE \
   | P32_CHARTYPE_UPPER | P32_CHARTYPE_XDIGIT)

/**
 * Character mappings required by POSIX.
 */
#define P32_WCTRANS_POSIX_LOWER (1)
#define P32_WCTRANS_POSIX_UPPER (1 << 1)
#define P32_WCTRANS_POSIX_ALL   (P32_WCTRANS_POSIX_LOWER | P32_WCTRANS_POSIX_UPPER)

/**
 * Character mappings mostly relevant to Japanese locales.
 */
#define P32_WCTRANS_JA_HIRAGANA (1 << 2)
#define P32_WCTRANS_JA_KATAKANA (1 << 3)
#define P32_WCTRANS_JA_ALL      (P32_WCTRANS_JA_HIRAGANA | P32_WCTRANS_JA_KATAKANA)

/**
 * Character mappings mostly relevant to Chinese locales.
 */
#define P32_WCTRANS_ZH_SIMPLIFIED  (1 << 4)
#define P32_WCTRANS_ZH_TRADITIONAL (1 << 5)
#define P32_WCTRANS_ZH_ALL         (P32_WCTRANS_ZH_SIMPLIFIED | P32_WCTRANS_ZH_TRADITIONAL)

#endif /* LIBPOSIX32_CTYPE_INTERNAL_H_INCLUDED */
