/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf --output-file=F:/posix32/lib/posix32-crt/gperf/charset.c F:/posix32/lib/posix32-crt/gperf/charset.gperf  */
/* Computed positions: -k'1-7,$' */

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

#line 1 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"

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
#line 44 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
struct charset { const char *charset; uint32_t CodePage; };
#include <string.h>

#define TOTAL_KEYWORDS 420
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 17
#define MIN_HASH_VALUE 35
#define MAX_HASH_VALUE 2000
/* maximum key range = 1966, duplicates = 0 */

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
P32CharsetHash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001,   10,    5, 2001,   20,    5,
         0,  255,  140,   35,  135,  100,  265,  355,  405,  370,
       508,  278,  460, 2001, 2001,  140,   10,   25,   20,   45,
         0,    0,   15,    0,   10,  105,  320,   50,    0,    0,
         0,   15,  100,  475,   40,   90, 2001,   55,    0,   25,
        35, 2001, 2001, 2001, 2001,   20,    0,  140,   10,   25,
        20,   45,    0,    0,   15,    0,   10,  105,  320,   50,
         0,    0,    0,   15,  100,  475,   40,   90, 2001,   55,
         0,   25,   35, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001, 2001,
      2001
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[6]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]+5];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]+1];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
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
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct charset *
P32CharsetLookup (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct charset wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 236 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"JOHAB",                   1361},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 47 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"OCP",                     P32_CODEPAGE_OCP},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 284 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"12001",                   12001},
      {""}, {""}, {""}, {""},
#line 289 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20002",                   20002},
      {""}, {""}, {""},
#line 201 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1200",                    1200},
#line 242 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10002",                   10002},
      {""}, {""}, {""}, {""},
#line 288 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20001",                   20001},
      {""}, {""},
#line 390 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1025",                21025},
      {""},
#line 240 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10001",                   10001},
      {""}, {""},
#line 55 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"500",                     500},
#line 279 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-croatian",          10082},
#line 273 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-icelandic",         10079},
      {""},
#line 259 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-chinesesimp",       10008},
      {""}, {""},
#line 281 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"12000",                   12000},
      {""}, {""}, {""}, {""},
#line 267 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-thai",              10021},
      {""}, {""}, {""}, {""},
#line 152 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"BIG-5",                   950},
      {""}, {""}, {""},
#line 161 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1140",                    1140},
#line 287 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20000",                   20000},
      {""},
#line 243 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-chinesetrad",       10002},
      {""},
#line 256 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-cyrillic",          10007},
#line 237 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10000",                   10000},
      {""},
#line 58 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-500",                 500},
#line 276 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-turkish",           10081},
#line 241 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-japanese",          10001},
#line 447 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"55002",                   55002},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 270 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-ce",                10029},
#line 135 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"JIS_X0208-1990",          932},
#line 446 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"55001",                   55001},
      {""},
#line 250 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-hebrew",            10005},
#line 63 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"720",                     720},
      {""},
#line 293 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20105",                   20105},
      {""}, {""},
#line 62 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"710",                     710},
      {""},
#line 292 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20005",                   20005},
#line 388 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1025",                  21025},
      {""}, {""}, {""},
#line 249 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10005",                   10005},
      {""}, {""},
#line 394 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1027",                21027},
      {""},
#line 266 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10021",                   10021},
      {""}, {""}, {""},
#line 169 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1142",                    1142},
#line 264 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-ukrainian",         10017},
      {""}, {""},
#line 145 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UHC",                     949},
#line 204 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1201",                    1201},
#line 445 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"55000",                   55000},
      {""}, {""}, {""},
#line 261 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-romanian",          10010},
#line 377 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"EUCJP",                   20932},
#line 253 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-greek",             10006},
#line 245 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-korean",            10003},
      {""}, {""}, {""},
#line 214 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1252",                  1252},
      {""},
#line 172 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1142",                1142},
      {""}, {""},
#line 378 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"EUC-JP",                  20932},
      {""},
#line 168 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1141",                1141},
      {""},
#line 380 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"EUCCN",                   20936},
#line 211 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1251",                  1251},
      {""},
#line 156 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1026",                1026},
      {""},
#line 450 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57002",                   57002},
      {""}, {""},
#line 46 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ACP",                     P32_CODEPAGE_ACP},
      {""},
#line 387 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"21025",                   21025},
#line 381 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"EUC-CN",                  20936},
      {""},
#line 164 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1140",                1140},
#line 165 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1141",                    1141},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 341 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-420",                 20420},
      {""},
#line 207 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1250",                    1250},
#line 338 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20420",                   20420},
      {""}, {""},
#line 184 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1145",                1145},
      {""},
#line 339 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP420",                   20420},
#line 208 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1250",                  1250},
      {""}, {""}, {""},
#line 260 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10010",                   10010},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 247 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"x-mac-arabic",            10004},
      {""}, {""},
#line 463 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"65001",                   65001},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 223 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1255",                  1255},
      {""}, {""}, {""},
#line 56 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP500",                   500},
      {""},
#line 389 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1025",                 21025},
      {""},
#line 213 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1252",                    1252},
      {""}, {""}, {""}, {""}, {""},
#line 453 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57005",                   57005},
      {""}, {""}, {""}, {""},
#line 460 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"65000",                   65000},
      {""}, {""}, {""}, {""},
#line 295 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20107",                   20107},
      {""},
#line 118 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"TIS-620",                 874},
      {""}, {""}, {""},
#line 392 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1027",                  21027},
      {""},
#line 192 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1147",                1147},
      {""},
#line 255 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10007",                   10007},
      {""}, {""}, {""},
#line 238 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACINTOSH",               10000},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 160 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1047",                1047},
#line 210 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1251",                    1251},
      {""}, {""}, {""}, {""}, {""},
#line 459 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57011",                   57011},
      {""}, {""}, {""}, {""},
#line 439 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"EUCTW",                   51950},
      {""},
#line 171 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1142",                 1142},
      {""}, {""}, {""}, {""}, {""},
#line 188 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1146",                1146},
      {""}, {""},
#line 170 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1142",                  1142},
#line 167 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1141",                 1141},
#line 180 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1144",                1144},
      {""}, {""},
#line 440 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"EUC-TW",                  51950},
      {""},
#line 278 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10082",                   10082},
      {""},
#line 391 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"21027",                   21027},
#line 166 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1141",                  1141},
#line 313 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-277",                 20277},
      {""}, {""},
#line 458 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57010",                   57010},
      {""},
#line 321 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-280",                 20280},
#line 275 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10081",                   10081},
      {""},
#line 297 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20127",                   20127},
      {""}, {""},
#line 70 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"850",                     850},
#line 75 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM852",                  852},
#line 294 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20106",                   20106},
      {""}, {""},
#line 206 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF-16BE",                1201},
      {""},
#line 462 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF-7",                   65000},
#line 154 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1026",                  1026},
#line 163 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1140",                 1140},
      {""}, {""},
#line 252 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10006",                   10006},
      {""}, {""}, {""}, {""},
#line 291 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20004",                   20004},
#line 162 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1140",                  1140},
      {""}, {""}, {""},
#line 246 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10004",                   10004},
      {""},
#line 329 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-285",                 20285},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 134 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"JIS_X0208-1983",          932},
#line 385 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"EUCKR",                   20949},
      {""}, {""}, {""},
#line 133 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"JIS_X0208",               932},
      {""},
#line 229 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1257",                  1257},
#line 183 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1145",                 1145},
      {""},
#line 72 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM850",                  850},
#line 263 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10017",                   10017},
#line 271 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACCENTRALEUROPE",        10029},
#line 393 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1027",                 21027},
#line 67 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"775",                     775},
      {""}, {""},
#line 182 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1145",                  1145},
      {""}, {""}, {""},
#line 455 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57007",                   57007},
      {""}, {""}, {""}, {""}, {""},
#line 257 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACCYRILLIC",             10007},
      {""}, {""}, {""}, {""}, {""},
#line 363 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-871",                 20871},
#line 109 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"870",                     870},
      {""},
#line 449 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"55004",                   55004},
      {""},
#line 268 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACTHAI",                 10021},
      {""},
#line 78 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM855",                  855},
#line 383 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"GB_2312-80",              20936},
      {""}, {""}, {""},
#line 362 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM871",                  20871},
#line 442 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"HZ-GB-2312",              52936},
#line 158 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1047",                  1047},
      {""},
#line 88 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"861",                     861},
      {""},
#line 419 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"29001",                   29001},
#line 340 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM420",                  20420},
#line 333 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-290",                 20290},
      {""}, {""},
#line 424 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50222",                   50222},
#line 312 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM277",                  20277},
      {""},
#line 149 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"950",                     950},
      {""}, {""},
#line 320 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM280",                  20280},
#line 112 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-870",                 870},
#line 176 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1143",                1143},
      {""},
#line 423 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50221",                   50221},
      {""}, {""},
#line 85 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"860",                     860},
#line 93 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM862",                  862},
      {""}, {""}, {""},
#line 196 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1148",                1148},
#line 111 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM870",                  870},
      {""},
#line 226 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1256",                  1256},
#line 349 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-424",                 20424},
#line 140 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"GBK",                     936},
#line 90 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM861",                  861},
#line 346 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20424",                   20424},
#line 235 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1361",                  1361},
#line 155 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1026",                 1026},
      {""}, {""},
#line 347 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP424",                   20424},
#line 220 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1254",                  1254},
      {""},
#line 286 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF-32BE",                12001},
      {""},
#line 454 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57006",                   57006},
#line 328 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM285",                  20285},
#line 122 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-875",                 875},
      {""}, {""},
#line 422 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50220",                   50220},
      {""},
#line 48 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"37",                      37},
      {""}, {""},
#line 452 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57004",                   57004},
      {""},
#line 371 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-905",                 20905},
#line 239 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACROMAN",                10000},
#line 121 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM875",                  875},
      {""},
#line 386 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"EUC-KR",                  20949},
      {""}, {""},
#line 151 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"BIG5",                    950},
      {""}, {""}, {""},
#line 76 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"855",                     855},
      {""},
#line 417 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28605",                   28605},
#line 280 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACCROATIAN",             10082},
      {""}, {""},
#line 251 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACHEBREW",               10005},
      {""},
#line 57 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM500",                  500},
      {""}, {""}, {""},
#line 425 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50225",                   50225},
      {""}, {""}, {""},
#line 181 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1145",                    1145},
      {""}, {""},
#line 51 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-037",                 37},
      {""},
#line 248 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACARABIC",               10004},
      {""},
#line 117 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"TIS620",                  874},
#line 191 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1147",                 1147},
      {""},
#line 102 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM865",                  865},
#line 49 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP037",                   37},
      {""},
#line 285 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF32BE",                 12001},
      {""},
#line 234 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1361",                    1361},
      {""},
#line 190 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1147",                  1147},
#line 205 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF16BE",                 1201},
      {""}, {""}, {""},
#line 332 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM290",                  20290},
#line 159 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1047",                 1047},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 200 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-1149",                1149},
      {""}, {""}, {""}, {""}, {""},
#line 81 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM857",                  857},
#line 53 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP437",                   437},
      {""}, {""}, {""},
#line 87 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IMB860",                  860},
#line 368 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20905",                   20905},
      {""}, {""},
#line 119 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"875",                     875},
      {""},
#line 369 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP905",                   20905},
#line 382 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"GB2312",                  20936},
      {""},
#line 113 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"874",                     874},
#line 177 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1144",                    1144},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 185 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1146",                    1146},
#line 277 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACTURKISH",              10081},
      {""}, {""},
#line 73 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"852",                     852},
      {""}, {""}, {""},
#line 325 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-284",                 20284},
      {""}, {""}, {""},
#line 370 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM905",                  20905},
#line 187 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1146",                 1146},
      {""},
#line 173 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1143",                    1143},
      {""},
#line 50 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IMB037",                  37},
#line 337 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-297",                 20297},
#line 100 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"865",                     865},
      {""},
#line 290 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20003",                   20003},
#line 186 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1146",                  1146},
#line 179 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1144",                 1144},
#line 97 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"864",                     864},
      {""},
#line 244 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10003",                   10003},
      {""}, {""}, {""},
#line 222 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1255",                    1255},
      {""},
#line 178 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1144",                  1144},
#line 367 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-880",                 20880},
      {""},
#line 396 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"KOI8-U",                  21866},
#line 296 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20108",                   20108},
      {""},
#line 193 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1148",                    1148},
      {""}, {""}, {""},
#line 127 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-932",             932},
#line 215 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1252",            1252},
#line 318 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20280",                   20280},
#line 366 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM880",                  20880},
#line 258 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10008",                   10008},
      {""},
#line 212 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1251",            1251},
#line 319 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP280",                   20280},
#line 299 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ANSI_X3.4-1986",          20127},
#line 262 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACROMANIA",              10010},
#line 126 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-31j",             932},
      {""}, {""},
#line 359 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"KOI8-R",                  20866},
#line 444 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"GB18030",                 54936},
      {""}, {""}, {""}, {""},
#line 426 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50227",                   50227},
      {""},
#line 209 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1250",            1250},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 448 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"55003",                   55003},
      {""},
#line 309 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-273",                 20273},
#line 326 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20285",                   20285},
      {""},
#line 274 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACICELAND",              10079},
      {""},
#line 224 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1255",            1255},
#line 327 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP285",                   20285},
      {""}, {""}, {""}, {""},
#line 103 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"866",                     866},
#line 219 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1254",                    1254},
#line 300 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ASCII",                   20127},
      {""},
#line 132 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"JIS0208",                 932},
#line 203 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF-16LE",                1200},
      {""},
#line 265 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACUKRAINE",              10017},
      {""},
#line 317 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-278",                 20278},
      {""},
#line 225 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1256",                    1256},
      {""},
#line 348 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM424",                  20424},
#line 375 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-924",                 20924},
      {""}, {""}, {""}, {""},
#line 189 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1147",                    1147},
#line 91 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"862",                     862},
      {""},
#line 302 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20261",                   20261},
#line 324 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM284",                  20284},
      {""}, {""},
#line 216 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1253",                    1253},
#line 465 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF-8",                   65001},
      {""},
#line 345 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-423",                 20423},
#line 79 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"857",                     857},
      {""},
#line 342 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20423",                   20423},
#line 336 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM297",                  20297},
#line 157 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1047",                    1047},
      {""}, {""},
#line 343 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP423",                   20423},
#line 217 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1253",                  1253},
#line 464 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF8",                    65001},
      {""}, {""}, {""}, {""}, {""},
#line 128 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MS_KANJI",                932},
      {""}, {""}, {""},
#line 231 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1258",                    1258},
#line 129 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MS-KANJI",                932},
      {""},
#line 451 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57003",                   57003},
      {""}, {""}, {""},
#line 69 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM775",                  775},
#line 376 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20932",                   20932},
#line 232 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1258",                  1258},
#line 230 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1257",            1257},
#line 364 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20880",                   20880},
#line 105 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM866",                  866},
#line 124 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP932",                   932},
      {""}, {""},
#line 365 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP880",                   20880},
      {""}, {""}, {""}, {""}, {""},
#line 99 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM864",                  864},
#line 456 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57008",                   57008},
      {""}, {""}, {""}, {""},
#line 435 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"51932",                   51932},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 64 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"737",                     737},
      {""}, {""},
#line 308 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM273",                  20273},
      {""}, {""}, {""},
#line 305 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-IR-156",              20269},
#line 139 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-936",             936},
#line 227 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1256",            1256},
      {""},
#line 298 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ANSI_X3.4-1968",          20127},
      {""},
#line 116 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-874",             874},
#line 221 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1254",            1254},
      {""}, {""},
#line 429 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50931",                   50931},
      {""},
#line 461 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF7",                    65000},
#line 59 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"708",                     708},
      {""}, {""},
#line 316 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM278",                  20278},
      {""}, {""}, {""}, {""}, {""},
#line 228 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1257",                    1257},
      {""}, {""},
#line 92 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP862",                   862},
#line 374 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM924",                  20924},
      {""},
#line 283 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF-32LE",                12000},
      {""},
#line 65 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP737",                   737},
      {""}, {""},
#line 52 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"437",                     437},
      {""},
#line 89 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP861",                   861},
      {""}, {""}, {""}, {""},
#line 428 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50930",                   50930},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 330 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20290",                   20290},
      {""}, {""}, {""}, {""},
#line 331 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP290",                   20290},
      {""}, {""}, {""},
#line 153 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1026",                    1026},
#line 74 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP852",                   852},
      {""}, {""}, {""}, {""},
#line 86 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP860",                   860},
      {""}, {""},
#line 94 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"863",                     863},
      {""},
#line 431 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50935",                   50935},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 175 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1143",                 1143},
      {""}, {""},
#line 372 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20924",                   20924},
      {""},
#line 282 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF32LE",                 12000},
      {""}, {""},
#line 373 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP924",                   20924},
#line 174 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1143",                  1143},
#line 202 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"UTF16LE",                 1200},
      {""}, {""}, {""}, {""}, {""},
#line 146 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"KSC_5601",                949},
      {""},
#line 101 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP865",                   865},
#line 54 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM437",                  437},
#line 195 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1148",                 1148},
      {""}, {""},
#line 71 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP850",                   850},
      {""}, {""},
#line 254 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MACGREEK",                10006},
      {""}, {""},
#line 194 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1148",                  1148},
      {""},
#line 322 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20284",                   20284},
      {""}, {""}, {""}, {""},
#line 323 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP284",                   20284},
      {""},
#line 269 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10029",                   10029},
      {""}, {""},
#line 136 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"936",                     936},
      {""},
#line 303 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-IR-103",              20261},
      {""},
#line 218 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1253",            1253},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 84 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM858",                  858},
#line 77 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP855",                   855},
      {""},
#line 233 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-1258",            1258},
      {""},
#line 197 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"1149",                    1149},
#line 399 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28592",                   28592},
      {""}, {""},
#line 123 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"932",                     932},
      {""}, {""}, {""}, {""},
#line 68 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP775",                   775},
      {""},
#line 397 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28591",                   28591},
      {""}, {""}, {""}, {""},
#line 150 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP950",                   950},
#line 344 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM423",                  20423},
      {""},
#line 61 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"709",                     709},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 457 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"57009",                   57009},
      {""}, {""}, {""}, {""},
#line 438 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"51950",                   51950},
      {""}, {""},
#line 360 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20871",                   20871},
#line 147 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"KS_C_5601-1987",          949},
      {""}, {""}, {""},
#line 361 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP871",                   20871},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 415 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28603",                   28603},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 82 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"858",                     858},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 433 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50937",                   50937},
      {""}, {""},
#line 110 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP870",                   870},
      {""},
#line 405 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28595",                   28595},
      {""}, {""}, {""},
#line 96 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM863",                  863},
#line 334 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20297",                   20297},
      {""}, {""}, {""}, {""},
#line 335 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP297",                   20297},
      {""}, {""}, {""}, {""}, {""},
#line 144 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"WINDOWS-949",             949},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 120 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP875",                   875},
      {""},
#line 379 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20936",                   20936},
      {""}, {""}, {""}, {""},
#line 137 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP936",                   936},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 66 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM737",                  737},
#line 441 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"52936",                   52936},
      {""}, {""}, {""}, {""},
#line 436 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"51936",                   51936},
      {""}, {""},
#line 310 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20277",                   20277},
      {""},
#line 80 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP857",                   857},
      {""}, {""},
#line 311 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP277",                   20277},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 432 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50936",                   50936},
      {""}, {""}, {""}, {""},
#line 395 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"21866",                   21866},
      {""},
#line 199 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM1149",                 1149},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 198 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP1149",                  1149},
      {""}, {""}, {""},
#line 358 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20866",                   20866},
      {""}, {""}, {""}, {""},
#line 104 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP866",                   866},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 98 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP864",                   864},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 353 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-833",                 20833},
      {""}, {""},
#line 400 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-2",              28592},
      {""},
#line 421 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-8-I",            38598},
      {""}, {""},
#line 398 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-1",              28591},
      {""}, {""},
#line 60 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ASMO-708",                708},
#line 352 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM833",                  20833},
      {""}, {""}, {""}, {""}, {""},
#line 409 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28597",                   28597},
      {""},
#line 357 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM-838",                 20838},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 301 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"US-ASCII",                20127},
#line 356 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM838",                  20838},
      {""}, {""}, {""}, {""}, {""},
#line 406 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-5",              28595},
#line 418 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-15",             28605},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 443 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"54936",                   54936},
      {""}, {""}, {""}, {""},
#line 427 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50229",                   50229},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 407 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28596",                   28596},
      {""}, {""}, {""},
#line 108 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"IBM869",                  869},
      {""}, {""}, {""}, {""}, {""},
#line 403 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28594",                   28594},
      {""}, {""}, {""},
#line 131 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"SHIFT-JIS",               932},
#line 410 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-7",              28597},
      {""}, {""}, {""},
#line 148 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"KS_C_5601-1989",          949},
#line 350 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20833",                   20833},
      {""}, {""}, {""},
#line 130 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"SHIFT_JIS",               932},
#line 351 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP833",                   20833},
      {""}, {""},
#line 106 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"869",                     869},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 114 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP874",                   874},
      {""},
#line 354 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20838",                   20838},
      {""}, {""}, {""}, {""},
#line 355 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP838",                   20838},
      {""}, {""}, {""}, {""},
#line 408 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-6",              28596},
      {""}, {""}, {""}, {""},
#line 404 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-4",              28594},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 125 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MS932",                   932},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 430 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50933",                   50933},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 272 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"10079",                   10079},
      {""}, {""}, {""}, {""},
#line 141 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"949",                     949},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 95 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP863",                   863},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 306 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20273",                   20273},
      {""}, {""}, {""}, {""},
#line 307 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP273",                   20273},
      {""},
#line 402 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-3",              28593},
#line 416 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-13",             28603},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 412 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-8",              28598},
      {""}, {""},
#line 314 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20278",                   20278},
      {""},
#line 83 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP858",                   858},
      {""}, {""},
#line 315 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP278",                   20278},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 304 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20269",                   20269},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 401 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28593",                   28593},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 411 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28598",                   28598},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 414 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"ISO-8859-9",              28599},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 434 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"50939",                   50939},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 107 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP869",                   869},
      {""}, {""}, {""}, {""},
#line 138 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MS936",                   936},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 384 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"20949",                   20949},
      {""}, {""}, {""}, {""},
#line 142 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"CP949",                   949},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 437 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"51949",                   51949},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 413 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"28599",                   28599},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 420 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"38598",                   38598},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 115 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MS874",                   874},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 143 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"
      {"MS949",                   949}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = P32CharsetHash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].charset;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
            return &wordlist[key];
        }
    }
  return (struct charset *) 0;
}
#line 466 "F:/posix32/lib/posix32-crt/gperf/charset.gperf"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

bool p32_lookup_charset (CharsetInfo *info) {
  struct charset *charset = P32CharsetLookup (info->String, info->Length);

  if (charset == NULL) {
    return false;
  }

  if (charset->CodePage != P32_CODEPAGE_ACP && charset->CodePage != P32_CODEPAGE_OCP) {
    if (!IsValidCodePage (charset->CodePage)) {
      return false;
    }
  }

  info->CodePage = charset->CodePage;

  return true;
}
