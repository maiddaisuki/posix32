/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: /usr/bin/gperf --output-file=F:/posix32/lib/posix32-crt/gperf/language_names.c F:/posix32/lib/posix32-crt/gperf/language_names.gperf  */
/* Computed positions: -k'1-3,5,9-10' */

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

#line 1 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"

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
#line 45 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
struct language { const char *name; BaseLanguageIndex Index; };

#define TOTAL_KEYWORDS 396
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 38
#define MIN_HASH_VALUE 19
#define MAX_HASH_VALUE 2055
/* maximum key range = 2037, duplicates = 0 */

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
P32LanguageNameHash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056,  400, 2056, 2056, 2056, 2056, 2056, 2056,    0,
       260,    5, 2056, 2056, 2056,   95, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056,    0,  315,   45,  617,    0,
       440,  235,  145,   25,   10,  205,  328,  220,   90,   95,
         0,  133,   75,   45,    5,   15,  702,  465,   10,  480,
       500,    5,    0, 2056, 2056, 2056, 2056,    0,  315,   45,
       617,    0,  440,  235,  145,   25,   10,  205,  328,  220,
        90,   95,    0,  133,   75,   45,    5,   15,  702,  465,
        10,  480,  500,    5,    0, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056,
      2056, 2056, 2056, 2056, 2056, 2056, 2056, 2056
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
      case 7:
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]+2];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct language *
P32LanguageNameLookup (register const char *str, register size_t len)
{
  static struct language wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 354 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tachelhit",                              BaseLanguage_shi},
      {""}, {""},
#line 311 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Punjabi",                                BaseLanguage_pa},
      {""},
#line 385 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Teso",                                   BaseLanguage_teo},
      {""},
#line 319 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Pashto",                                 BaseLanguage_ps},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 163 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Persian",                                BaseLanguage_fa},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 351 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sena",                                   BaseLanguage_seh},
      {""}, {""},
#line 342 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Santali",                                BaseLanguage_sat},
      {""}, {""}, {""}, {""},
#line 394 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Turkish",                                BaseLanguage_tr},
      {""},
#line 373 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Saho",                                   BaseLanguage_ssy},
#line 390 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tigre",                                  BaseLanguage_tig},
      {""},
#line 136 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"us",                                     BaseLanguage_en_US},
      {""}, {""},
#line 344 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sangu",                                  BaseLanguage_sbp},
      {""}, {""}, {""},
#line 377 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sundanese",                              BaseLanguage_su},
      {""}, {""},
#line 374 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sesotho",                                BaseLanguage_st},
      {""}, {""}, {""}, {""},
#line 357 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sinhala",                                BaseLanguage_si},
      {""}, {""}, {""}, {""},
#line 368 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Serbian",                                BaseLanguage_sr},
      {""}, {""}, {""}, {""}, {""},
#line 389 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tigrinya",                               BaseLanguage_ti},
      {""}, {""}, {""},
#line 371 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"siSwati",                                BaseLanguage_ss},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 393 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tongan",                                 BaseLanguage_to},
#line 206 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Italian",                                BaseLanguage_it},
#line 137 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"usa",                                    BaseLanguage_en_US},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 214 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Javanese",                               BaseLanguage_jv},
      {""},
#line 332 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Rundi",                                  BaseLanguage_rn},
      {""},
#line 140 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Spanish",                                BaseLanguage_es},
      {""}, {""}, {""},
#line 303 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sesotho sa Leboa",                       BaseLanguage_nso},
#line 154 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-peru",                           BaseLanguage_es_PE},
      {""},
#line 153 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-panama",                         BaseLanguage_es_PA},
      {""},
#line 156 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-paraguay",                       BaseLanguage_es_PY},
      {""}, {""}, {""}, {""}, {""},
#line 335 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Russian",                                BaseLanguage_ru},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 353 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sango",                                  BaseLanguage_sg},
      {""},
#line 117 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"English",                                BaseLanguage_en},
#line 399 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Central Atlas Tamazight",                BaseLanguage_tzm},
#line 155 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-puerto rico",                    BaseLanguage_es_PR},
      {""},
#line 52 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Arabic",                                 BaseLanguage_ar},
#line 306 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Occitan",                                BaseLanguage_oc},
      {""}, {""}, {""},
#line 403 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Central Atlas Tamazight (Latin)",        BaseLanguage_tzm_Latn},
#line 401 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Central Atlas Tamazight (Arabic)",       BaseLanguage_tzm_Arab},
#line 67 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Haryanvi",                               BaseLanguage_bgc},
#line 405 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Central Atlas Tamazight (Tifinagh)",     BaseLanguage_tzm_Tfng},
      {""}, {""},
#line 341 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Samburu",                                BaseLanguage_saq},
      {""}, {""},
#line 127 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-jamaica",                        BaseLanguage_en_JM},
#line 120 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-aus",                            BaseLanguage_en_AU},
#line 309 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ossetic",                                BaseLanguage_os},
#line 310 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ossetian",                               BaseLanguage_os},
#line 426 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Soga",                                   BaseLanguage_xog},
#line 320 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Portuguese",                             BaseLanguage_pt},
      {""}, {""}, {""},
#line 345 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sardinian",                              BaseLanguage_sc},
#line 147 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-ecuador",                        BaseLanguage_es_EC},
      {""},
#line 247 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Cornish",                                BaseLanguage_kw},
#line 92 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Corsican",                               BaseLanguage_co},
#line 321 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Portuguese (Brazil)",                    BaseLanguage_pt_BR},
#line 322 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"portuguese-brazilian",                   BaseLanguage_pt_BR},
#line 323 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Portuguese (Portugal)",                  BaseLanguage_pt_PT},
      {""},
#line 211 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Japanese",                               BaseLanguage_ja},
      {""},
#line 313 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Papiamento",                             BaseLanguage_pap},
      {""}, {""}, {""},
#line 285 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Nama",                                   BaseLanguage_naq},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 123 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-can",                            BaseLanguage_en_CA},
      {""}, {""},
#line 388 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Thai",                                   BaseLanguage_th},
#line 194 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Hindi",                                  BaseLanguage_hi},
      {""},
#line 118 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-caribbean",                      BaseLanguage_en_029},
      {""}, {""},
#line 119 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"australian",                             BaseLanguage_en_AU},
      {""},
#line 141 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-argentina",                      BaseLanguage_es_AR,},
      {""}, {""},
#line 134 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-us",                             BaseLanguage_en_US},
#line 135 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-usa",                            BaseLanguage_en_US},
#line 232 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kannada",                                BaseLanguage_kn},
#line 195 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Hindi (Latin)",                          BaseLanguage_hi_Latn},
      {""},
#line 99 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Taita",                                  BaseLanguage_dav},
      {""}, {""}, {""}, {""},
#line 158 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-uruguay",                        BaseLanguage_es_UY},
#line 347 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sindhi",                                 BaseLanguage_sd},
      {""}, {""},
#line 189 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Manx",                                   BaseLanguage_gv},
      {""}, {""},
#line 213 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Machame",                                BaseLanguage_jmc},
#line 276 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Manipuri",                               BaseLanguage_mni},
#line 264 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Meru",                                   BaseLanguage_mer},
      {""}, {""}, {""}, {""}, {""},
#line 325 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kiche",                                  BaseLanguage_quc},
#line 316 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Polish",                                 BaseLanguage_pl},
#line 279 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Marathi",                                BaseLanguage_mr},
      {""}, {""},
#line 252 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ganda",                                  BaseLanguage_lg},
      {""},
#line 282 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Mundang",                                BaseLanguage_mua},
      {""}, {""},
#line 129 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-trinidad y tobago",              BaseLanguage_en_TT},
#line 100 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"German",                                 BaseLanguage_de},
#line 152 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-nicaragua",                      BaseLanguage_es_NI},
#line 327 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kiche (Latin)",                          BaseLanguage_quc_Latn},
      {""}, {""},
#line 125 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-ire",                            BaseLanguage_en_IE},
#line 391 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Turkmen",                                BaseLanguage_tk},
      {""},
#line 50 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Akan",                                   BaseLanguage_ak},
      {""}, {""},
#line 245 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kurdish",                                BaseLanguage_ku},
#line 338 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sanskrit",                               BaseLanguage_sa},
#line 198 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Hungarian",                              BaseLanguage_hu},
      {""}, {""}, {""}, {""}, {""},
#line 263 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Masai",                                  BaseLanguage_mas},
#line 196 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Croatian",                               BaseLanguage_hr},
      {""},
#line 199 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Armenian",                               BaseLanguage_hy},
      {""},
#line 339 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sakha",                                  BaseLanguage_sah},
#line 144 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-colombia",                       BaseLanguage_es_CO},
      {""},
#line 145 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-costa rica",                     BaseLanguage_es_CR},
#line 243 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ripuarian",                              BaseLanguage_ksh},
      {""}, {""}, {""}, {""},
#line 381 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kiswahili",                              BaseLanguage_sw},
      {""}, {""},
#line 105 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"german-luxembourg",                      BaseLanguage_de_LU},
      {""}, {""}, {""},
#line 235 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kanuri",                                 BaseLanguage_kr},
      {""},
#line 55 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Assamese",                               BaseLanguage_as},
#line 286 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Norwegian",                              BaseLanguage_nb},
      {""},
#line 312 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Punjabi (Arabic)",                       BaseLanguage_pa_Arab},
      {""},
#line 425 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Xhosa",                                  BaseLanguage_xh},
      {""},
#line 188 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Gusii",                                  BaseLanguage_guz},
      {""},
#line 441 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"isiZulu",                                BaseLanguage_zu},
      {""},
#line 139 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Esperanto",                              BaseLanguage_eo},
#line 138 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-south africa",                   BaseLanguage_en_ZA},
      {""},
#line 184 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Guarani",                                BaseLanguage_gn},
      {""}, {""},
#line 219 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kamba",                                  BaseLanguage_kam},
      {""},
#line 234 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Konkani",                                BaseLanguage_kok},
      {""}, {""}, {""},
#line 233 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Korean",                                 BaseLanguage_ko},
#line 51 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Amharic",                                BaseLanguage_am},
      {""},
#line 301 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"N'ko",                                   BaseLanguage_nqo},
#line 101 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"german-austrian",                        BaseLanguage_de_AT},
      {""},
#line 281 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Maltese",                                BaseLanguage_mt},
#line 143 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-chile",                          BaseLanguage_es_CL},
#line 65 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bena",                                   BaseLanguage_bez},
      {""}, {""},
#line 75 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bengali",                                BaseLanguage_bn},
      {""}, {""}, {""}, {""}, {""},
#line 364 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Shona",                                  BaseLanguage_sn},
#line 104 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"german-lichtenstein",                    BaseLanguage_de_LI},
      {""}, {""}, {""}, {""}, {""},
#line 62 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Basaa",                                  BaseLanguage_bas},
#line 384 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Telugu",                                 BaseLanguage_te},
      {""}, {""}, {""}, {""},
#line 365 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Shona (Latin)",                          BaseLanguage_sn_Latn},
#line 283 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Burmese",                                BaseLanguage_my},
      {""},
#line 304 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Nuer",                                   BaseLanguage_nus},
      {""}, {""},
#line 53 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Mapuche",                                BaseLanguage_arn},
      {""},
#line 408 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ukrainian",                              BaseLanguage_uk},
#line 151 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-mexican",                        BaseLanguage_es_MX},
#line 161 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Basque",                                 BaseLanguage_eu},
      {""},
#line 260 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Luyia",                                  BaseLanguage_luy},
      {""},
#line 244 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Central Kurdish",                        BaseLanguage_ku},
#line 186 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Alsatian",                               BaseLanguage_gsw},
      {""},
#line 250 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Langi",                                  BaseLanguage_lag},
#line 205 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Icelandic",                              BaseLanguage_is},
#line 254 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Lingala",                                BaseLanguage_ln},
      {""},
#line 331 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Romansh",                                BaseLanguage_rm},
#line 333 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Romanian",                               BaseLanguage_ro},
      {""},
#line 334 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Rombo",                                  BaseLanguage_rof},
#line 228 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kalaallisut",                            BaseLanguage_kl},
#line 224 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Koyra Chiini",                           BaseLanguage_khq},
      {""}, {""},
#line 124 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-uk",                             BaseLanguage_en_GB},
#line 150 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-honduras",                       BaseLanguage_es_HN},
      {""},
#line 265 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Morisyen",                               BaseLanguage_mfe},
      {""}, {""},
#line 133 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-american",                       BaseLanguage_en_US},
      {""},
#line 183 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Galician",                               BaseLanguage_gl},
      {""}, {""}, {""},
#line 149 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-guatemala",                      BaseLanguage_es_GT},
#line 270 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Maori",                                  BaseLanguage_mi},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 289 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"norwegian-bokmal",                       BaseLanguage_nb_NO},
#line 299 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"norwegian-nynorsk",                      BaseLanguage_nn_NO},
#line 230 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kalenjin",                               BaseLanguage_kln},
      {""}, {""}, {""}, {""},
#line 369 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Serbian (Cyrillic)",                     BaseLanguage_sr_Cyrl,},
      {""},
#line 90 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chiga",                                  BaseLanguage_cgg},
      {""},
#line 434 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chinese",                                BaseLanguage_zh},
#line 307 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Oromo",                                  BaseLanguage_om},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 284 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Mazanderani",                            BaseLanguage_mzn},
      {""}, {""}, {""},
#line 64 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bemba",                                  BaseLanguage_bem},
#line 226 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kazakh",                                 BaseLanguage_kk},
      {""},
#line 343 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Santali (Ol Chiki)",                     BaseLanguage_sat_Olck},
      {""}, {""}, {""}, {""}, {""},
#line 395 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tsonga",                                 BaseLanguage_ts},
      {""}, {""},
#line 355 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tachelhit (Latin)",                      BaseLanguage_shi_Latn},
#line 424 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"isiXhosa",                               BaseLanguage_xh},
#line 227 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kako",                                   BaseLanguage_kkj},
#line 356 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tachelhit (Tifinagh)",                   BaseLanguage_shi_Tfng},
      {""}, {""},
#line 383 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tamil",                                  BaseLanguage_ta},
      {""}, {""}, {""},
#line 294 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"belgian",                                BaseLanguage_nl_BE},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 148 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-modern",                         BaseLanguage_es_ES},
      {""}, {""}, {""},
#line 237 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kashmiri",                               BaseLanguage_ks},
      {""}, {""}, {""},
#line 172 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Faroese",                                BaseLanguage_fo},
      {""}, {""},
#line 181 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Irish",                                  BaseLanguage_ga},
      {""},
#line 79 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bosnian",                                BaseLanguage_bs},
      {""}, {""}, {""},
#line 85 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chakma",                                 BaseLanguage_ccp},
#line 157 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-el salvador",                    BaseLanguage_es_SV},
      {""},
#line 255 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Lao",                                    BaseLanguage_lo},
#line 241 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bafia",                                  BaseLanguage_ksf},
      {""}, {""}, {""},
#line 121 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-belize",                         BaseLanguage_en_BZ},
#line 49 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Aghem",                                  BaseLanguage_agq},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 378 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sundanese (Latin)",                      BaseLanguage_su_Latn},
      {""},
#line 259 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Luo",                                    BaseLanguage_luo},
      {""}, {""},
#line 437 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"chinese-singapore",                      BaseLanguage_zh_Hans_SG},
#line 436 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"chinese-simplified",                     BaseLanguage_zh_Hans_CN},
      {""}, {""}, {""}, {""},
#line 130 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"american",                               BaseLanguage_en_US},
#line 47 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Afar",                                   BaseLanguage_aa},
      {""}, {""},
#line 398 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tasawaq",                                BaseLanguage_twq},
      {""},
#line 440 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"chinese-traditional",                    BaseLanguage_zh_Hant_TW},
      {""}, {""},
#line 170 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Finnish",                                BaseLanguage_fi},
      {""},
#line 292 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Nepali",                                 BaseLanguage_ne},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 204 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Yi",                                     BaseLanguage_ii},
      {""},
#line 273 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Mongolian",                              BaseLanguage_mn},
#line 106 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Zarma",                                  BaseLanguage_dje},
      {""},
#line 358 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Slovak",                                 BaseLanguage_sk},
#line 56 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Asu",                                    BaseLanguage_asa},
      {""}, {""}, {""}, {""}, {""},
#line 66 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bulgarian",                              BaseLanguage_bg},
      {""}, {""}, {""}, {""}, {""},
#line 231 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Khmer",                                  BaseLanguage_km},
#line 246 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kurdish (Arabic)",                       BaseLanguage_ku_Arab},
      {""},
#line 91 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Cherokee",                               BaseLanguage_chr},
      {""}, {""}, {""}, {""}, {""},
#line 409 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Urdu",                                   BaseLanguage_ur},
#line 328 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Quechua",                                BaseLanguage_quz},
#line 382 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Syriac",                                 BaseLanguage_syr},
#line 220 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Makonde",                                BaseLanguage_kde},
      {""}, {""},
#line 142 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-bolivia",                        BaseLanguage_es_BO},
#line 407 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Uyghur",                                 BaseLanguage_ug},
#line 61 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bashkir",                                BaseLanguage_ba},
#line 386 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tajik",                                  BaseLanguage_tg},
      {""}, {""}, {""}, {""}, {""},
#line 164 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Fula",                                   BaseLanguage_ff},
      {""}, {""}, {""},
#line 266 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Malagasy",                               BaseLanguage_mg},
      {""}, {""}, {""}, {""},
#line 240 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Shambala",                               BaseLanguage_ksb},
#line 253 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Lakota",                                 BaseLanguage_lkt},
      {""},
#line 421 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Walser",                                 BaseLanguage_wae},
#line 324 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"K'iche'",                                BaseLanguage_quc},
#line 171 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Filipino",                               BaseLanguage_fil},
#line 242 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Colognian",                              BaseLanguage_ksh},
      {""}, {""}, {""},
#line 197 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Upper Sorbian",                          BaseLanguage_hsb},
#line 366 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Somali",                                 BaseLanguage_so},
      {""}, {""}, {""},
#line 329 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Rajasthani",                             BaseLanguage_raj},
      {""},
#line 360 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sami (Southern)",                        BaseLanguage_sma},
#line 201 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ibibio",                                 BaseLanguage_ibb},
#line 87 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chechen",                                BaseLanguage_ce},
#line 262 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Maithili",                               BaseLanguage_mai},
      {""},
#line 63 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Belarusian",                             BaseLanguage_be},
      {""},
#line 346 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sardinian (Latin)",                      BaseLanguage_sc_Latn},
      {""},
#line 430 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Nheengatu",                              BaseLanguage_yrl},
#line 372 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Swati",                                  BaseLanguage_ss},
#line 330 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Rajasthani (Devanagari)",                BaseLanguage_raj_Deva},
      {""},
#line 337 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Rwa",                                    BaseLanguage_rwk},
      {""}, {""},
#line 132 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"american-english",                       BaseLanguage_en_US},
      {""},
#line 317 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Prussian",                               BaseLanguage_prg},
      {""}, {""}, {""}, {""}, {""},
#line 208 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Inuktitut",                              BaseLanguage_iu},
#line 72 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bamanankan",                             BaseLanguage_bm},
#line 69 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bhojpuri",                               BaseLanguage_bho},
      {""}, {""}, {""},
#line 359 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Slovenian",                              BaseLanguage_sl},
      {""}, {""},
#line 73 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bamanankan (Latin)",                     BaseLanguage_bm_Latn},
#line 442 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Zulu",                                   BaseLanguage_zu},
      {""},
#line 217 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Georgian",                               BaseLanguage_ka},
      {""}, {""},
#line 400 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tamazight",                              BaseLanguage_tzm},
#line 190 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Hausa",                                  BaseLanguage_ha},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 236 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kanuri (Latin)",                         BaseLanguage_kr_Latn},
#line 207 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"italian-swiss",                          BaseLanguage_it_CH},
      {""}, {""},
#line 318 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Dari",                                   BaseLanguage_prs},
#line 362 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sami (Inari)",                           BaseLanguage_smn},
#line 191 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Hausa (Latin)",                          BaseLanguage_ha_Latn},
      {""},
#line 314 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Nigerian Pidgin",                        BaseLanguage_pcm},
      {""}, {""}, {""}, {""},
#line 349 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sami (Northern)",                        BaseLanguage_se},
      {""}, {""},
#line 315 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Nigerian Pidgin (Latin)",                BaseLanguage_pcm_Latn},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 74 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bangla",                                 BaseLanguage_bn},
      {""},
#line 439 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"chinese-hongkong",                       BaseLanguage_zh_Hant_HK},
      {""},
#line 222 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kaingang",                               BaseLanguage_kgp},
      {""}, {""}, {""}, {""},
#line 192 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Hawaiian",                               BaseLanguage_haw},
#line 76 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tibetan",                                BaseLanguage_bo},
#line 86 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chakma (Chakma)",                        BaseLanguage_ccp_Cakm},
#line 187 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Gujarati",                               BaseLanguage_gu},
#line 95 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chuvash",                                BaseLanguage_cv},
#line 290 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"North Ndebele",                          BaseLanguage_nd},
      {""}, {""}, {""}, {""},
#line 98 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Danish",                                 BaseLanguage_da},
#line 88 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Cebuano",                                BaseLanguage_ceb},
#line 122 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"canadian",                               BaseLanguage_en_CA},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 370 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Serbian (Latin)",                        BaseLanguage_sr_Latn},
      {""}, {""},
#line 438 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chinese (Traditional)",                  BaseLanguage_zh_Hant},
#line 110 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Duala",                                  BaseLanguage_dua},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 267 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Makhuwa-Meetto",                         BaseLanguage_mgh},
      {""}, {""}, {""}, {""}, {""},
#line 165 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Fulah",                                  BaseLanguage_ff},
#line 288 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Norwegian Bokmal",                       BaseLanguage_nb},
#line 298 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Norwegian Nynorsk",                      BaseLanguage_nn},
#line 287 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Norwegian (Bokmal)",                     BaseLanguage_nb},
#line 297 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Norwegian (Nynorsk)",                    BaseLanguage_nn},
      {""}, {""}, {""},
#line 115 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ewe",                                    BaseLanguage_ee},
#line 48 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Afrikaans",                              BaseLanguage_af},
#line 97 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Welsh",                                  BaseLanguage_cy},
      {""},
#line 417 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Venda",                                  BaseLanguage_ve},
#line 169 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Fulah (Latin)",                          BaseLanguage_ff_Latn},
      {""},
#line 340 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Yakut",                                  BaseLanguage_sah},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 435 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chinese (Simplified)",                   BaseLanguage_zh_Hans},
      {""}, {""}, {""}, {""}, {""},
#line 77 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Breton",                                 BaseLanguage_br},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 128 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"english-nz",                             BaseLanguage_en_NZ},
      {""},
#line 102 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"german-swiss",                           BaseLanguage_de_CH},
      {""}, {""},
#line 202 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Indonesian",                             BaseLanguage_id},
#line 296 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kwasio",                                 BaseLanguage_nmg},
      {""}, {""}, {""}, {""}, {""},
#line 179 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Frisian",                                BaseLanguage_fy},
#line 71 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Edo",                                    BaseLanguage_bin},
      {""},
#line 116 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Greek",                                  BaseLanguage_el},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 103 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"swiss",                                  BaseLanguage_de_CH},
      {""},
#line 107 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Dogri",                                  BaseLanguage_doi},
#line 193 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Hebrew",                                 BaseLanguage_he},
      {""}, {""}, {""}, {""},
#line 80 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bosnian (Cyrillic)",                     BaseLanguage_bs_Cyrl},
      {""}, {""}, {""},
#line 379 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Swedish",                                BaseLanguage_sv},
      {""},
#line 212 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ngomba",                                 BaseLanguage_jgo},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 216 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Javanese (Latin)",                       BaseLanguage_jv_Latn},
#line 397 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tatar",                                  BaseLanguage_tt},
      {""},
#line 215 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Javanese (Javanese)",                    BaseLanguage_jv_Java},
      {""}, {""}, {""},
#line 350 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Northern Sami",                          BaseLanguage_se},
      {""},
#line 280 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Malay",                                  BaseLanguage_ms},
#line 278 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Mohawk",                                 BaseLanguage_moh},
      {""}, {""}, {""}, {""},
#line 173 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"French",                                 BaseLanguage_fr},
#line 427 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Yangben",                                BaseLanguage_yav},
#line 111 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Divehi",                                 BaseLanguage_dv},
      {""},
#line 112 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Jola-Fonyi",                             BaseLanguage_dyo},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 352 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Koyraboro Senni",                        BaseLanguage_ses},
      {""},
#line 420 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Vunjo",                                  BaseLanguage_vun},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 305 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Nyankole",                               BaseLanguage_nyn},
      {""},
#line 57 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Asturian",                               BaseLanguage_ast},
#line 68 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Haryanvi (Devanagari)",                  BaseLanguage_bgc_Deva},
      {""}, {""},
#line 159 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-venezuela",                      BaseLanguage_es_VE},
      {""}, {""},
#line 177 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"french-luxembourg",                      BaseLanguage_fr_LU},
      {""}, {""},
#line 396 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Xitsonga",                               BaseLanguage_ts},
#line 114 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Embu",                                   BaseLanguage_ebu},
      {""}, {""}, {""},
#line 160 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Estonian",                               BaseLanguage_et},
      {""}, {""}, {""},
#line 300 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ngiemboon",                              BaseLanguage_nnh},
      {""},
#line 348 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sindhi (Devanagari)",                    BaseLanguage_sd_Deva},
#line 82 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Blin",                                   BaseLanguage_byn},
#line 146 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"spanish-dominican republic",             BaseLanguage_es_DO},
#line 78 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bodo",                                   BaseLanguage_brx},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 182 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Scottish Gaelic",                        BaseLanguage_gd},
#line 361 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sami (Lule)",                            BaseLanguage_smj},
      {""},
#line 203 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Igbo",                                   BaseLanguage_ig},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 419 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Volapuk",                                BaseLanguage_vo},
      {""},
#line 131 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"american english",                       BaseLanguage_en_US},
#line 277 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Manipuri (Bangla)",                      BaseLanguage_mni_Beng},
      {""}, {""},
#line 175 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"french-canadian",                        BaseLanguage_fr_CA},
#line 429 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Yoruba",                                 BaseLanguage_yo},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 414 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Vai",                                    BaseLanguage_vai},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 274 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Mongolian (Cyrillic)",                   BaseLanguage_mn_Cyrl},
#line 308 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Odia",                                   BaseLanguage_or},
      {""}, {""}, {""}, {""},
#line 269 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Meta",                                   BaseLanguage_mgo},
#line 268 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Meta'",                                  BaseLanguage_mgo},
      {""}, {""},
#line 93 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Czech",                                  BaseLanguage_cs},
      {""}, {""},
#line 275 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Mongolian (Traditional Mongolian)",      BaseLanguage_mn_Mong},
      {""}, {""},
#line 225 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kikuyu",                                 BaseLanguage_ki},
#line 271 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Macedonian",                             BaseLanguage_mk},
      {""}, {""}, {""}, {""},
#line 375 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sotho",                                  BaseLanguage_st},
      {""}, {""}, {""}, {""}, {""},
#line 336 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kinyarwanda",                            BaseLanguage_rw},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 363 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Sami (Skolt)",                           BaseLanguage_sms},
      {""},
#line 428 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Yiddish",                                BaseLanguage_yi},
#line 96 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Chuvash (Cyrillic)",                     BaseLanguage_cv_Cyrl},
      {""}, {""},
#line 84 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Valencian",                              BaseLanguage_ca_ES_valencia},
      {""}, {""}, {""}, {""}, {""},
#line 168 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Fula (Latin)",                           BaseLanguage_ff_Latn},
      {""}, {""}, {""}, {""},
#line 431 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Nheengatu (Latin)",                      BaseLanguage_yrl_Latn},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 210 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Inuktitut (Latin)",                      BaseLanguage_iu_Latn},
      {""}, {""},
#line 180 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Western Frisian",                        BaseLanguage_fy},
#line 209 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Inuktitut (Syllabics)",                  BaseLanguage_iu_Cans},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 272 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Malayalam",                              BaseLanguage_ml},
      {""}, {""},
#line 404 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tamazight (Latin)",                      BaseLanguage_tzm_Latn},
#line 402 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tamazight (Arabic)",                     BaseLanguage_tzm_Arab},
      {""},
#line 406 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tamazight (Tifinagh)",                   BaseLanguage_tzm_Tfng},
#line 126 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"irish-english",                          BaseLanguage_en_IE},
#line 54 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Mapudungun",                             BaseLanguage_arn},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 94 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Church Slavic",                          BaseLanguage_cu},
      {""},
#line 367 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Albanian",                               BaseLanguage_sq},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 81 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bosnian (Latin)",                        BaseLanguage_bs_Latn},
      {""}, {""},
#line 178 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Friulian",                               BaseLanguage_fur},
      {""}, {""}, {""}, {""}, {""},
#line 261 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Latvian",                                BaseLanguage_lv},
      {""}, {""}, {""}, {""},
#line 185 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Swiss German",                           BaseLanguage_gsw},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 256 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Northern Luri",                          BaseLanguage_lrc},
      {""},
#line 302 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"South Ndebele",                          BaseLanguage_nr},
      {""}, {""}, {""},
#line 83 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Catalan",                                BaseLanguage_ca},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 423 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Wolof",                                  BaseLanguage_wo},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 418 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Vietnamese",                             BaseLanguage_vi},
      {""},
#line 387 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Tajik (Cyrillic)",                       BaseLanguage_tg_Cyrl},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 238 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kashmiri (Arabic)",                      BaseLanguage_ks_Arab},
      {""}, {""}, {""},
#line 239 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kashmiri (Devanagari)",                  BaseLanguage_ks_Deva},
      {""}, {""}, {""},
#line 249 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Latin",                                  BaseLanguage_la},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 174 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"french-belgian",                         BaseLanguage_fr_BE},
#line 422 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Wolaytta",                               BaseLanguage_wal},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 58 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Azerbaijani",                            BaseLanguage_az},
      {""},
#line 200 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Interlingua",                            BaseLanguage_ia},
      {""}, {""},
#line 218 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kabyle",                                 BaseLanguage_kab},
#line 258 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Luba-Katanga",                           BaseLanguage_lu},
#line 326 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"K'iche' (Latin)",                        BaseLanguage_quc_Latn},
#line 60 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Azerbaijani (Latin)",                    BaseLanguage_az_Latn},
      {""}, {""},
#line 59 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Azerbaijani (Cyrillic)",                 BaseLanguage_az_Cyrl},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 257 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Lithuanian",                             BaseLanguage_lt},
      {""}, {""}, {""}, {""}, {""},
#line 248 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kyrgyz",                                 BaseLanguage_ky},
      {""}, {""}, {""}, {""},
#line 416 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Vai (Vai)",                              BaseLanguage_vai_Vaii},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 376 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Southern Sotho",                         BaseLanguage_st},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 392 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Setswana",                               BaseLanguage_tn},
#line 162 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Ewondo",                                 BaseLanguage_ewo},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 291 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Low German",                             BaseLanguage_nds},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 380 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"swedish-finland",                        BaseLanguage_sv_FI},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 89 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Cebuano (Latin)",                        BaseLanguage_ceb_Latn},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 70 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Bhojpuri (Devanagari)",                  BaseLanguage_bho_Deva},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 166 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Fula (Adlam)",                           BaseLanguage_ff_Adlm},
      {""}, {""},
#line 229 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Greenlandic",                            BaseLanguage_kl},
      {""}, {""}, {""}, {""},
#line 415 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Vai (Latin)",                            BaseLanguage_vai_Latn},
      {""}, {""}, {""},
#line 176 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"french-swiss",                           BaseLanguage_fr_CH},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 223 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kaingang (Latin)",                       BaseLanguage_kgp_Latn},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 410 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Uzbek",                                  BaseLanguage_uz},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 413 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Uzbek (Latin)",                          BaseLanguage_uz_Latn},
      {""}, {""}, {""},
#line 432 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Standard Moroccan Tamazight",            BaseLanguage_zgh},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 433 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Standard Moroccan Tamazight (Tifinagh)", BaseLanguage_zgh_Tfng},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 109 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Lower Sorbian",                          BaseLanguage_dsb},
      {""}, {""}, {""}, {""},
#line 251 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Luxembourgish",                          BaseLanguage_lb},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 411 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Uzbek (Arabic)",                         BaseLanguage_uz_Arab},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 108 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Dogri (Devanagari)",                     BaseLanguage_doi_Deva},
      {""},
#line 293 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Dutch",                                  BaseLanguage_nl},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 113 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Dzongkha",                               BaseLanguage_dz},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 221 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Kabuverdianu",                           BaseLanguage_kea},
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
#line 167 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Fulah (Adlam)",                          BaseLanguage_ff_Adlm},
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
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 412 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"Uzbek (Cyrillic)",                       BaseLanguage_uz_Cyrl},
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
#line 295 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"
      {"dutch-belgian",                          BaseLanguage_nl_BE}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = P32LanguageNameHash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 443 "F:/posix32/lib/posix32-crt/gperf/language_names.gperf"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

bool p32_lookup_language_name (LanguageInfo *info) {
  struct language *language = P32LanguageNameLookup (info->String, info->Length);

  if (language == NULL) {
    return false;
  }

  info->BaseLanguage = language->Index;

  return true;
}
