/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf --output-file=F:/posix32/lib/posix32-crt/gperf/country_names.c F:/posix32/lib/posix32-crt/gperf/country_names.gperf  */
/* Computed positions: -k'1-7' */

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

#line 1 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"

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
#line 45 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
struct country { const char *name; CountryIndex Index; };

#define TOTAL_KEYWORDS 284
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 41
#define MIN_HASH_VALUE 14
#define MAX_HASH_VALUE 1400
/* maximum key range = 1387, duplicates = 0 */

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
P32CountryNameHash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401,   20,  180, 1401, 1401, 1401, 1401, 1401,    0,
         0,    0, 1401, 1401, 1401,   85,    0, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401,    5,    5,  175,  270,   10,
       320,   15,  185,    0,  115,  250,    0,   90,    0,   55,
        45,    5,   25,   65,  115,    0,  230,  440,   10,  305,
       375, 1401, 1401, 1401, 1401, 1401, 1401,    5,    5,  175,
       270,   10,  320,   15,  185,    0,  115,  250,    0,   90,
         0,   55,   45,    5,   25,   65,  115,    0,  230,  440,
        10,  305,  375, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401, 1401,
      1401, 1401, 1401, 1401, 1401, 1401, 1401
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[6]+1];
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
        hval += asso_values[(unsigned char)str[3]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
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
  return hval;
}

struct country *
P32CountryNameLookup (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct country wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 236 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Niue",                                        NU},
      {""}, {""}, {""}, {""}, {""},
#line 76 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Benin",                                       BJ},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 57 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Albania",                                     AL},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 168 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Iran",                                        IR},
#line 235 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Nauru",                                       NR},
#line 146 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Guinea",                                      GN},
#line 71 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Belgium",                                     BE},
      {""},
#line 167 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Iraq",                                        IQ},
      {""}, {""}, {""},
#line 152 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Guinea-Bissau",                               GW},
      {""},
#line 64 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Aruba",                                       AW},
#line 79 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Brunei",                                      BN},
#line 134 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"england",                                     GB},
      {""}, {""},
#line 70 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bangladesh",                                  BD},
      {""},
#line 195 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Liberia",                                     LR},
      {""}, {""},
#line 227 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Niger",                                       NE},
      {""},
#line 162 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Ireland",                                     IE},
      {""},
#line 143 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Gibraltar",                                   GI},
#line 254 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Palau",                                       PW},
      {""},
#line 229 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Nigeria",                                     NG},
      {""}, {""},
#line 234 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Nepal",                                       NP},
      {""},
#line 118 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Algeria",                                     DZ},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 144 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Greenland",                                   GL},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 241 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Peru",                                        PE},
#line 132 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Gabon",                                       GA},
#line 59 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Angola",                                      AO},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 209 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Mali",                                        ML},
      {""}, {""}, {""},
#line 278 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Suriname",                                    SR},
      {""}, {""}, {""}, {""},
#line 255 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Paraguay",                                    PY},
      {""}, {""},
#line 163 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Israel",                                      IL},
#line 225 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Namibia",                                     NA},
      {""},
#line 151 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Guam",                                        GU},
      {""},
#line 259 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Serbia",                                      RS},
      {""},
#line 56 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Anguilla",                                    AI},
      {""},
#line 123 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Spain",                                       ES},
      {""}, {""}, {""}, {""}, {""},
#line 145 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Gambia",                                      GM},
      {""}, {""},
#line 190 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Laos",                                        LA},
      {""}, {""},
#line 139 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Georgia",                                     GE},
      {""}, {""}, {""}, {""},
#line 191 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Lebanon",                                     LB},
      {""}, {""}, {""}, {""},
#line 58 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Armenia",                                     AM},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 49 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Europe",                                      R_150},
#line 257 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Reunion",                                     RE},
      {""},
#line 239 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Oman",                                        OM},
      {""},
#line 240 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Panama",                                      PA},
#line 54 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Antigua & Barbuda",                           AG},
#line 50 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Latin America",                               R_419},
#line 55 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Antigua and Barbuda",                         AG},
#line 256 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Qatar",                                       QA},
      {""}, {""}, {""}, {""}, {""},
#line 260 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Russia",                                      RU},
#line 86 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Belarus",                                     BY},
      {""}, {""},
#line 198 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Luxembourg",                                  LU},
      {""}, {""},
#line 73 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bulgaria",                                    BG},
      {""},
#line 175 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Japan",                                       JP},
#line 164 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Isle of Man",                                 IM},
      {""}, {""},
#line 194 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Sri Lanka",                                   LK},
      {""}, {""}, {""}, {""},
#line 267 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Singapore",                                   SG},
      {""}, {""},
#line 258 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Romania",                                     RO},
      {""},
#line 106 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cuba",                                        CU},
      {""},
#line 243 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Papua New Guinea",                            PG},
#line 295 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Tunisia",                                     TN},
      {""}, {""},
#line 296 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Tonga",                                       TO},
      {""},
#line 122 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Eritrea",                                     ER},
      {""}, {""}, {""}, {""},
#line 276 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Senegal",                                     SN},
      {""}, {""}, {""}, {""},
#line 169 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Iceland",                                     IS},
      {""}, {""}, {""}, {""},
#line 133 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"britain",                                     GB},
      {""}, {""},
#line 142 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Ghana",                                       GH},
      {""},
#line 205 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Martin",                                MF},
      {""}, {""},
#line 218 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Malta",                                       MT},
      {""},
#line 62 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Austria",                                     AT},
      {""},
#line 181 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Kitts & Nevis",                         KN},
#line 323 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Samoa",                                       WS},
#line 182 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Kitts and Nevis",                       KN},
#line 277 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Somalia",                                     SO},
      {""}, {""}, {""}, {""},
#line 317 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"British Virgin Islands",                      VG},
#line 247 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Pierre & Miquelon",                     PM},
      {""},
#line 248 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Pierre and Miquelon",                   PM},
      {""}, {""}, {""}, {""},
#line 166 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"British Indian Ocean Territory",              IO},
      {""}, {""}, {""},
#line 289 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Togo",                                        TG},
#line 307 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"US Minor Outlying Islands",                   UM},
#line 268 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Helena, Ascension, Tristan da Cunha", SH},
      {""}, {""},
#line 150 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Guatemala",                                   GT},
#line 216 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Mauritania",                                  MR},
#line 149 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Greece",                                      GR},
      {""}, {""}, {""}, {""}, {""},
#line 120 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Estonia",                                     EE},
      {""}, {""},
#line 275 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"San Marino",                                  SM},
#line 251 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"puerto-rico",                                 PR},
#line 232 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"holland",                                     NL},
      {""}, {""}, {""}, {""}, {""},
#line 253 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Portugal",                                    PT},
      {""}, {""}, {""},
#line 148 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Equatorial Guinea",                           GQ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 165 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"India",                                       IN},
      {""},
#line 171 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Invariant Country",                           IV},
      {""}, {""}, {""}, {""},
#line 74 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bahrain",                                     BH},
      {""}, {""}, {""},
#line 244 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Philippines",                                 PH},
      {""},
#line 179 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Kiribati",                                    KI},
#line 60 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Argentina",                                   AR},
      {""}, {""},
#line 72 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Burkina Faso",                                BF},
      {""}, {""}, {""},
#line 305 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Uganda",                                      UG},
#line 80 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bolivia",                                     BO},
      {""}, {""},
#line 94 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Congo",                                       CG},
#line 192 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Lucia",                                 LC},
#line 282 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Sint Maarten",                                SX},
      {""}, {""},
#line 159 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Haiti",                                       HT},
#line 105 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Serbia and Montenegro",                       CS},
      {""}, {""},
#line 63 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Australia",                                   AU},
      {""},
#line 84 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bhutan",                                      BT},
#line 309 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"america",                                     US},
      {""}, {""},
#line 200 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Libya",                                       LY},
      {""},
#line 274 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Sierra Leone",                                SL},
      {""},
#line 61 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"American Samoa",                              AS},
      {""}, {""}, {""}, {""}, {""},
#line 67 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bosnia & Herzegovina",                        BA},
#line 92 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Congo (DRC)",                                 CD},
#line 68 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bosnia and Herzegovina",                      BA},
      {""},
#line 280 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Sao Tome & Principe",                         ST},
#line 212 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Macao",                                       MO},
#line 153 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Guyana",                                      GY},
#line 138 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Grenada",                                     GD},
#line 211 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Mongolia",                                    MN},
      {""}, {""}, {""},
#line 126 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Finland",                                     FI},
      {""}, {""},
#line 265 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Sudan",                                       SD},
#line 222 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Mexico",                                      MX},
      {""}, {""}, {""},
#line 184 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Korea",                                       KR},
      {""}, {""}, {""}, {""},
#line 147 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Guadeloupe",                                  GP},
#line 302 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Tuvalu",                                      TV},
      {""}, {""},
#line 219 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Mauritius",                                   MU},
#line 215 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Martinique",                                  MQ},
#line 199 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Latvia",                                      LV},
      {""}, {""},
#line 306 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"US Outlying Islands",                         UM},
      {""}, {""},
#line 319 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Vietnam",                                     VN},
#line 69 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Barbados",                                    BB},
#line 197 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Lithuania",                                   LT},
#line 100 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"China",                                       CN},
      {""}, {""},
#line 290 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Thailand",                                    TH},
      {""},
#line 98 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Chile",                                       CL},
#line 252 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Palestinian Authority",                       PS},
#line 262 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saudi Arabia",                                SA},
#line 135 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"great britain",                               GB},
      {""}, {""},
#line 246 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Poland",                                      PL},
      {""}, {""}, {""}, {""},
#line 202 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Monaco",                                      MC},
#line 158 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Croatia",                                     HR},
      {""}, {""},
#line 263 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Solomon Islands",                             SB},
      {""},
#line 51 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Andorra",                                     AD},
      {""}, {""},
#line 270 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Svalbard & Jan Mayen",                        SJ},
#line 77 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Barthelemy",                            BL},
#line 271 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Svalbard and Jan Mayen",                      SJ},
      {""}, {""}, {""}, {""},
#line 173 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Jamaica",                                     JM},
      {""},
#line 230 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Nicaragua",                                   NI},
#line 283 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Syria",                                       SY},
#line 87 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Belize",                                      BZ},
#line 115 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Denmark",                                     DK},
#line 310 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"united-states",                               US},
#line 137 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"united-kingdom",                              GB},
#line 217 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Montserrat",                                  MS},
#line 183 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"North Korea",                                 KP},
#line 78 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bermuda",                                     BM},
#line 99 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cameroon",                                    CM},
      {""},
#line 208 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"North Macedonia",                             MK},
#line 82 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Brazil",                                      BR},
#line 83 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bahamas",                                     BS},
      {""}, {""},
#line 325 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Yemen",                                       YE},
#line 249 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Pitcairn Islands",                            PN},
#line 75 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Burundi",                                     BI},
      {""}, {""}, {""}, {""},
#line 311 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Uruguay",                                     UY},
#line 65 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Aland Islands",                               AX},
      {""},
#line 170 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Italy",                                       IT},
      {""},
#line 299 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Trinidad & Tobago",                           TT},
      {""},
#line 300 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Trinidad and Tobago",                         TT},
      {""}, {""},
#line 108 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Curacao",                                     CW},
      {""},
#line 127 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Fiji",                                        FJ},
      {""},
#line 250 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Puerto Rico",                                 PR},
#line 81 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Bonaire, Sint Eustatius and Saba",          BQ},
#line 141 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Guernsey",                                    GG},
#line 93 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Central African Republic",                    CF},
      {""},
#line 231 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Netherlands",                                 NL},
      {""}, {""}, {""}, {""},
#line 185 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"south korea",                                 KR},
      {""},
#line 114 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Djibouti",                                    DJ},
      {""}, {""}, {""},
#line 327 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"South Africa",                                ZA},
      {""},
#line 161 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Indonesia",                                   ID},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 88 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Canada",                                      CA},
      {""}, {""},
#line 228 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Norfolk Island",                              NF},
#line 91 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Keeling Islands",                             CC},
#line 293 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Timor-Leste",                                 TL},
      {""}, {""},
#line 213 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Macao SAR",                                   MO},
      {""},
#line 174 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Jordan",                                      JO},
      {""}, {""},
#line 214 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Northern Mariana Islands",                    MP},
      {""},
#line 207 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Marshall Islands",                            MH},
#line 196 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Lesotho",                                     LS},
#line 269 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Slovenia",                                    SI},
      {""}, {""},
#line 329 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Zambia",                                      ZM},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 245 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Pakistan",                                    PK},
      {""},
#line 121 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Egypt",                                       EG},
      {""},
#line 286 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Turks & Caicos Islands",                      TC},
#line 330 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Zimbabwe",                                    ZW},
      {""}, {""}, {""}, {""},
#line 103 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Colombia",                                    CO},
#line 287 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Turks and Caicos Islands",                    TC},
      {""}, {""}, {""}, {""}, {""},
#line 104 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Costa Rica",                                  CR},
      {""}, {""},
#line 101 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"pr china",                                    CN},
      {""},
#line 206 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Madagascar",                                  MG},
#line 186 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"south-korea",                                 KR},
      {""}, {""}, {""}, {""}, {""},
#line 328 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"south-africa",                                ZA},
      {""}, {""}, {""}, {""},
#line 113 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Germany",                                     DE},
      {""}, {""}, {""}, {""}, {""},
#line 125 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Ethiopia",                                    ET},
      {""}, {""},
#line 172 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Jersey",                                      JE},
      {""}, {""}, {""}, {""},
#line 131 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"France",                                      FR},
#line 318 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"US Virgin Islands",                           VI},
      {""},
#line 48 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Caribbean",                                   R_029},
      {""},
#line 221 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Malawi",                                      MW},
      {""},
#line 157 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Honduras",                                    HN},
      {""},
#line 66 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Azerbaijan",                                  AZ},
#line 281 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"El Salvador",                                 SV},
      {""}, {""}, {""}, {""}, {""},
#line 294 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Turkmenistan",                                TM},
      {""}, {""}, {""}, {""}, {""},
#line 130 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Faroe Islands",                               FO},
      {""}, {""},
#line 279 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"South Sudan",                                 SS},
#line 210 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Myanmar",                                     MM},
#line 90 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cocos (Keeling) Islands",                     CC},
      {""}, {""},
#line 301 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Taiwan",                                      TW},
      {""}, {""}, {""},
#line 176 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Kenya",                                       KE},
      {""},
#line 180 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Comoros",                                     KM},
#line 102 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"pr-china",                                    CN},
#line 154 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Hong Kong",                                   HK},
      {""},
#line 109 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Christmas Island",                            CX},
      {""},
#line 156 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Hong Kong SAR",                               HK},
      {""}, {""}, {""},
#line 119 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Ecuador",                                     EC},
#line 308 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"United States",                               US},
#line 136 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"United Kingdom",                              GB},
      {""}, {""},
#line 320 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Vanuatu",                                     VU},
#line 223 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Malaysia",                                    MY},
      {""},
#line 52 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"United Arab Emirates",                        AE},
      {""},
#line 313 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Vatican City",                                VA},
      {""}, {""},
#line 204 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Montenegro",                                  ME},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 304 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Ukraine",                                     UA},
      {""}, {""},
#line 291 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Tajikistan",                                  TJ},
#line 273 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"slovak",                                      SK},
#line 160 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Hungary",                                     HU},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 110 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cyprus",                                      CY},
      {""},
#line 303 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Tanzania",                                    TZ},
      {""}, {""}, {""},
#line 201 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Morocco",                                     MA},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 288 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Chad",                                        TD},
      {""}, {""}, {""}, {""},
#line 155 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"hong-kong",                                   HK},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 128 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Falkland Islands",                            FK},
      {""}, {""}, {""}, {""},
#line 53 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Afghanistan",                                 AF},
      {""},
#line 96 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cote d'Ivoire",                               CI},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 314 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Vincent & Grenadines",                  VC},
#line 292 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Tokelau",                                     TK},
#line 89 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cocos Islands",                               CC},
      {""},
#line 129 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Micronesia",                                  FM},
      {""},
#line 315 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Saint Vincent and the Grenadines",            VC},
      {""}, {""}, {""}, {""},
#line 97 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cook Islands",                                CK},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 116 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Dominica",                                    DM},
      {""}, {""}, {""}, {""},
#line 284 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Eswatini",                                    SZ},
      {""}, {""}, {""}, {""},
#line 117 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Dominican Republic",                          DO},
      {""},
#line 321 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Wallis & Futuna",                             WF},
      {""},
#line 322 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Wallis and Futuna",                           WF},
      {""}, {""}, {""},
#line 297 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Turkey",                                      TR},
#line 203 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Moldova",                                     MD},
      {""}, {""}, {""},
#line 324 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Kosovo",                                      XK},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 178 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cambodia",                                    KH},
      {""}, {""},
#line 124 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"ES-valencia",                                 ES_valencia},
      {""},
#line 272 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Slovakia",                                    SK},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 224 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Mozambique",                                  MZ},
      {""}, {""},
#line 85 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Botswana",                                    BW},
      {""}, {""},
#line 261 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Rwanda",                                      RW},
      {""},
#line 226 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"New Caledonia",                               NC},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 312 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Uzbekistan",                                  UZ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 188 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cayman Islands",                              KY},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 47 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"World",                                       R_001},
      {""}, {""}, {""}, {""}, {""},
#line 266 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Sweden",                                      SE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 187 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Kuwait",                                      KW},
      {""},
#line 193 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Liechtenstein",                               LI},
      {""},
#line 107 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Cabo Verde",                                  CV},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 233 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Norway",                                      NO},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 264 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Seychelles",                                  SC},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 237 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"New Zealand",                                 NZ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 285 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Swaziland",                                   SZ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 140 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"French Guiana",                               GF},
      {""}, {""},
#line 242 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"French Polynesia",                            PF},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 220 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Maldives",                                    MV},
      {""},
#line 112 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"czech",                                       CZ},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 111 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Czechia",                                     CZ},
      {""}, {""}, {""},
#line 238 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"new-zealand",                                 NZ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 316 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Venezuela",                                   VE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 326 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Mayotte",                                     YT},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 298 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Turkiye",                                     TR},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 95 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Switzerland",                                 CH},
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
      {""}, {""}, {""}, {""}, {""},
#line 189 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Kazakhstan",                                  KZ},
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
      {""}, {""}, {""}, {""}, {""}, {""},
#line 177 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"
      {"Kyrgyzstan",                                  KG}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = P32CountryNameHash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
            return &wordlist[key];
        }
    }
  return (struct country *) 0;
}
#line 331 "F:/posix32/lib/posix32-crt/gperf/country_names.gperf"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

bool p32_lookup_country_name (CountryInfo *info) {
  struct country *cc = P32CountryNameLookup (info->String, info->Length);

  if (cc == NULL) {
    return false;
  }

  info->Country = cc->Index;

  return true;
}
