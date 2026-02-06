/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf --output-file=F:/posix32/lib/posix32-crt/gperf/sorting_id.c F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf  */
/* Computed positions: -k'1,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"

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

#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

#if !P32_LOCALE_NAMES
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4267)
#endif
#line 45 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
struct sorting_id { const char *name; SortingIdIndex Index; };

#define TOTAL_KEYWORDS 21
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 10
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 44
/* maximum key range = 43, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
P32SortingIdHash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45,  3, 45, 45, 45, 30, 45, 45, 25, 20,
      45, 45, 45, 45, 45, 45, 45,  0, 45, 45,
      45, 45, 45, 45, 45, 45, 45,  5, 30, 45,
       5,  0, 45, 45,  0, 45,  5, 10, 20, 45,
      15, 10, 45, 45,  5, 45, 45,  0, 45, 45,
      45, 45,  0, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
      45, 45, 45, 45, 45, 45
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

struct sorting_id *
P32SortingIdLookup (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct sorting_id wordlist[] =
    {
      {""}, {""},
#line 49 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"hu",         Sorting_hu_default},
      {""}, {""},
#line 62 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh-TW",      Sorting_zh_Hant_default},
      {""},
#line 47 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"de",         Sorting_de_default},
#line 61 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh-SG",      Sorting_zh_Hans_default},
#line 66 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh_stroke",  Sorting_zh_stroke},
#line 67 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh_unicode", Sorting_zh_unicode},
      {""},
#line 51 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"ja",         Sorting_ja_default},
      {""},
#line 65 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh_radstr",  Sorting_zh_radstr},
#line 53 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"ja_unicode", Sorting_ja_unicode},
      {""},
#line 54 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"ka",         Sorting_ka_default},
      {""},
#line 52 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"ja_radstr",  Sorting_ja_radstr},
#line 57 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"ko_unicode", Sorting_ko_unicode},
      {""},
#line 56 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"ko",         Sorting_ko_default},
      {""},
#line 64 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh_pronun",  Sorting_zh_pronun},
#line 60 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh-MO",      Sorting_zh_Hant_default},
      {""}, {""}, {""},
#line 50 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"hu_technl",  Sorting_hu_technl},
#line 58 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh-CN",      Sorting_zh_Hans_default},
      {""}, {""}, {""},
#line 55 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"ka_modern",  Sorting_ka_modern},
#line 59 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh-HK",      Sorting_zh_Hant_default},
      {""}, {""}, {""},
#line 63 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"zh_phoneb",  Sorting_zh_phoneb},
      {""}, {""}, {""}, {""},
#line 48 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"
      {"de_phoneb",  Sorting_de_phoneb}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = P32SortingIdHash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return (struct sorting_id *) 0;
}
#line 68 "F:/posix32/lib/posix32-crt/gperf/sorting_id.gperf"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

bool p32_lookup_sorting_id (SortingIdInfo *info) {
  struct sorting_id *ss = P32SortingIdLookup (info->String, info->Length);

  if (ss == NULL) {
    return false;
  }

  info->SortId = ss->Index;

  return true;
}
#endif
