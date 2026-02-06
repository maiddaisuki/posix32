/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf --output-file=F:/posix32/lib/posix32-crt/gperf/script.c F:/posix32/lib/posix32-crt/gperf/script.gperf  */
/* Computed positions: -k'1-2,4' */

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

#line 1 "F:/posix32/lib/posix32-crt/gperf/script.gperf"

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

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4267)
#endif
#line 45 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
struct script { const char *name; ScriptIndex Index; };

#define TOTAL_KEYWORDS 60
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 21
#define MIN_HASH_VALUE 19
#define MAX_HASH_VALUE 164
/* maximum key range = 146, duplicates = 0 */

#ifndef GPERF_DOWNCASE
#define GPERF_DOWNCASE 1
static unsigned char gperf_downcase[256] =
  {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
     30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
    255
  };
#endif

#ifndef GPERF_CASE_STRCMP
#define GPERF_CASE_STRCMP 1
static int
gperf_case_strcmp (register const char *s1, register const char *s2)
{
  for (;;)
    {
      unsigned char c1 = gperf_downcase[(unsigned char)*s1++];
      unsigned char c2 = gperf_downcase[(unsigned char)*s2++];
      if (c1 != 0 && c1 == c2)
        continue;
      return (int)c1 - (int)c2;
    }
}
#endif

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
P32ScriptHash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165,  35,  15,  25,  46,  10,
        3,  50,   5,  47,   0,   6,  40,  35,  30,   6,
       16,   0,   0,  50,  15,  60,   2, 165, 165,  35,
      165, 165, 165, 165, 165, 165, 165,  35,  15,  25,
       46,  10,   3,  50,   5,  47,   0,   6,  40,  35,
       30,   6,  16,   0,   0,  50,  15,  60,   2, 165,
      165,  35, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
      165, 165, 165, 165, 165, 165, 165
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]+1];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct script *
P32ScriptLookup (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct script wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 83 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Kore",                  Kore},
      {""}, {""}, {""}, {""},
#line 101 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Thai",                  Thai},
#line 94 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Orya",                  Orya},
#line 79 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Jpan",                  Jpan},
#line 99 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Tfng",                  Tfng},
      {""},
#line 64 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Ethi",                  Ethi},
      {""},
#line 98 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Telu",                  Telu},
      {""}, {""},
#line 52 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Beng",                  Beng},
      {""},
#line 74 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Traditional",           Hant},
      {""}, {""},
#line 103 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Thaa",                  Thaa},
#line 105 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Vai",                   Vaii},
#line 104 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Vaii",                  Vaii},
      {""}, {""},
#line 70 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Hani",                  Hani},
#line 51 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Armn",                  Armn},
#line 89 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Traditional Mongolian", Mong},
      {""}, {""},
#line 69 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Hang",                  Hang},
#line 88 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Mong",                  Mong},
      {""}, {""}, {""},
#line 77 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Java",                  Java},
#line 82 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Knda",                  Knda},
#line 91 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Nkoo",                  Nkoo},
#line 66 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Gree",                  Gree},
#line 78 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Javanese",              Java},
#line 71 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Hans",                  Hans},
#line 80 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Kana",                  Kana},
#line 53 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Bangla",                Beng},
      {""}, {""},
#line 49 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Arab",                  Arab},
#line 81 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Khmr",                  Khmr},
#line 50 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Arabic",                Arab},
      {""},
#line 61 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Cyrillic",              Cyrl},
#line 75 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Hebr",                  Hebr},
#line 100 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Tifinagh",              Tfng},
#line 76 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Hira",                  Hira},
      {""}, {""},
#line 106 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Yaii",                  Yaii},
#line 62 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Deva",                  Deva},
#line 55 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Chakma",                Cakm},
      {""}, {""},
#line 56 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Cans",                  Cans},
#line 86 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Latin",                 Latn},
#line 63 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Devanagari",            Deva},
      {""}, {""},
#line 58 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Cher",                  Cher},
#line 85 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Latn",                  Latn},
      {""}, {""},
#line 59 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Cherokee",              Cher},
#line 97 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Taml",                  Taml},
#line 92 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Olck",                  Olck},
      {""}, {""}, {""},
#line 54 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Cakm",                  Cakm},
#line 84 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Laoo",                  Laoo},
      {""}, {""}, {""},
#line 60 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Cyrl",                  Cyrl},
#line 93 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Ol Chiki",              Olck},
#line 48 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Adlam",                 Adlm},
      {""}, {""},
#line 73 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Hant",                  Hant},
      {""}, {""},
#line 72 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Simplified",            Hans},
      {""},
#line 87 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Mlym",                  Mlym},
      {""}, {""}, {""}, {""},
#line 65 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Geor",                  Geor},
#line 47 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Adlm",                  Adlm},
#line 68 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Guru",                  Guru},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 90 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Mymr",                  Mymr},
      {""},
#line 102 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Tibt",                  Tibt},
      {""}, {""},
#line 57 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Syllabics",             Cans},
      {""}, {""}, {""}, {""}, {""},
#line 96 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Syrc",                  Syrc},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 95 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Sinh",                  Sinh},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 67 "F:/posix32/lib/posix32-crt/gperf/script.gperf"
      {"Gujr",                  Gujr}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = P32ScriptHash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
            return &wordlist[key];
        }
    }
  return (struct script *) 0;
}
#line 107 "F:/posix32/lib/posix32-crt/gperf/script.gperf"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

bool p32_lookup_script (ScriptInfo *info) {
  struct script *script = P32ScriptLookup (info->String, info->Length);

  if (script == NULL) {
    return false;
  }

  info->Script = script->Index;

  return true;
}
