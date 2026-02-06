/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf --output-file=F:/posix32/lib/posix32-crt/gperf/language_codes.c F:/posix32/lib/posix32-crt/gperf/language_codes.gperf  */
/* Computed positions: -k'1-3' */

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

#line 1 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"

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
#line 44 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
struct language { const char *name; BaseLanguageIndex Index; };

#define TOTAL_KEYWORDS 406
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 3
#define MIN_HASH_VALUE 25
#define MAX_HASH_VALUE 1258
/* maximum key range = 1234, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
P32LanguageCodeHash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259,   55,  328,  245,
       373,  130,  420,   53,  223,  115,   10,  278,  345,   83,
        23,   85,  205,  230,  170,  450,  215,   25,  355,  500,
       465,   75,  300,  133,   13,  370,    0,  485,  125,  200,
       103,  265,  138,  495,    5,   10,  165,  100,   20,  290,
       450,  250,   43,    3,  148,   30,  490,  130,    0,   15,
       225, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259, 1259,
      1259, 1259, 1259, 1259
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[2]+28];
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
        hval += asso_values[(unsigned char)str[0]+19];
        break;
    }
  return hval;
}

struct language *
P32LanguageCodeLookup (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct language wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 217 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kn",  BaseLanguage_kn},
      {""},
#line 229 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ku",  BaseLanguage_ku},
      {""}, {""},
#line 370 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sn",  BaseLanguage_sn},
      {""},
#line 383 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"su",  BaseLanguage_su},
      {""}, {""},
#line 404 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tn",  BaseLanguage_tn},
      {""}, {""}, {""}, {""},
#line 191 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"iu",  BaseLanguage_iu},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 403 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tuk", BaseLanguage_tk},
      {""},
#line 78 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bn",  BaseLanguage_bn},
      {""}, {""},
#line 384 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sun", BaseLanguage_su},
      {""}, {""}, {""},
#line 199 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ka",  BaseLanguage_ka},
#line 201 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kab", BaseLanguage_kab},
      {""},
#line 357 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sg",  BaseLanguage_sg},
#line 225 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kas", BaseLanguage_ks},
#line 343 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sa",  BaseLanguage_sa},
      {""}, {""},
#line 395 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tg",  BaseLanguage_tg},
      {""},
#line 390 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ta",  BaseLanguage_ta},
#line 183 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ig",  BaseLanguage_ig},
      {""},
#line 178 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ia",  BaseLanguage_ia},
#line 230 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kur", BaseLanguage_ku},
      {""}, {""}, {""}, {""},
#line 396 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tgk", BaseLanguage_tg},
#line 233 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ky",  BaseLanguage_ky},
#line 218 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kan", BaseLanguage_kn},
      {""},
#line 71 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bg",  BaseLanguage_bg},
#line 409 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tur", BaseLanguage_tr},
#line 64 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ba",  BaseLanguage_ba},
#line 344 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"san", BaseLanguage_sa},
      {""},
#line 215 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"km",  BaseLanguage_km},
#line 66 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bas", BaseLanguage_bas},
#line 219 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ko",  BaseLanguage_ko},
#line 223 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kau", BaseLanguage_kr},
      {""}, {""}, {""},
#line 372 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"so",  BaseLanguage_so},
#line 65 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bak", BaseLanguage_ba},
#line 369 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sms", BaseLanguage_sms},
      {""},
#line 367 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"smj", BaseLanguage_smj},
#line 406 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"to",  BaseLanguage_to},
#line 221 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kok", BaseLanguage_kok},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 434 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"wo",  BaseLanguage_wo},
      {""}, {""},
#line 76 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bm",  BaseLanguage_bm},
#line 368 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"smn", BaseLanguage_smn},
#line 80 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bo",  BaseLanguage_bo},
      {""}, {""}, {""},
#line 86 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bos", BaseLanguage_bs},
#line 207 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ki",  BaseLanguage_ki},
#line 407 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ton", BaseLanguage_to},
      {""}, {""}, {""},
#line 360 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"si",  BaseLanguage_si},
#line 87 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"byn", BaseLanguage_byn},
      {""}, {""},
#line 389 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"syr", BaseLanguage_syr},
#line 399 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ti",  BaseLanguage_ti},
#line 208 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kik", BaseLanguage_ki},
      {""},
#line 185 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ii",  BaseLanguage_ii},
#line 220 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kor", BaseLanguage_ko},
#line 136 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fa",  BaseLanguage_fa},
      {""}, {""}, {""},
#line 137 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fas", BaseLanguage_fa},
#line 353 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"se",  BaseLanguage_se},
#line 341 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kin", BaseLanguage_rw},
      {""}, {""},
#line 356 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ses", BaseLanguage_ses},
#line 392 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"te",  BaseLanguage_te},
#line 361 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sin", BaseLanguage_si},
      {""}, {""},
#line 147 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fur", BaseLanguage_fur},
      {""},
#line 431 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"vun", BaseLanguage_vun},
      {""},
#line 270 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mn",  BaseLanguage_mn},
      {""},
#line 148 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fy",  BaseLanguage_fy},
      {""}, {""}, {""},
#line 352 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"snd", BaseLanguage_sd},
#line 67 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"be",  BaseLanguage_be},
#line 202 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kam", BaseLanguage_kam},
      {""},
#line 174 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hu",  BaseLanguage_hu},
#line 234 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kir", BaseLanguage_ky},
#line 143 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fo",  BaseLanguage_fo},
#line 75 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bin", BaseLanguage_bin},
#line 182 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ind", BaseLanguage_id},
      {""},
#line 327 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"qub", BaseLanguage_quz_BO},
      {""},
#line 391 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tam", BaseLanguage_ta},
      {""}, {""},
#line 400 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tir", BaseLanguage_ti},
#line 222 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kr",  BaseLanguage_kr},
      {""}, {""}, {""}, {""},
#line 376 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sr",  BaseLanguage_sr},
#line 79 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ben", BaseLanguage_bn},
      {""},
#line 260 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mg",  BaseLanguage_mg},
#line 175 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hun", BaseLanguage_hu},
#line 408 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tr",  BaseLanguage_tr},
#line 77 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bam", BaseLanguage_bm},
      {""}, {""},
#line 257 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mas", BaseLanguage_mas},
#line 429 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"vo",  BaseLanguage_vo},
#line 307 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"om",  BaseLanguage_om},
      {""},
#line 164 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ha",  BaseLanguage_ha},
      {""},
#line 140 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fi",  BaseLanguage_fi},
#line 373 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"som", BaseLanguage_so},
      {""}, {""}, {""},
#line 82 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"br",  BaseLanguage_br},
#line 84 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"brx", BaseLanguage_brx},
      {""}, {""},
#line 345 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sah", BaseLanguage_sah},
#line 281 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"my",  BaseLanguage_my},
      {""}, {""}, {""},
#line 200 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kat", BaseLanguage_ka},
      {""}, {""}, {""},
#line 176 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hy",  BaseLanguage_hy},
#line 347 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sat", BaseLanguage_sat},
      {""},
#line 141 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fin", BaseLanguage_fi},
      {""}, {""},
#line 413 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tat", BaseLanguage_tt},
#line 427 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"vi",  BaseLanguage_vi},
#line 72 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bul", BaseLanguage_bg},
      {""},
#line 416 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ug",  BaseLanguage_ug},
#line 165 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hau", BaseLanguage_ha},
#line 381 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"st",  BaseLanguage_st},
#line 213 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kal", BaseLanguage_kl},
      {""},
#line 296 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nn",  BaseLanguage_nn},
#line 275 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mar", BaseLanguage_mr},
#line 412 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tt",  BaseLanguage_tt},
      {""}, {""},
#line 189 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"it",  BaseLanguage_it},
#line 303 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nus", BaseLanguage_nus},
#line 425 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ve",  BaseLanguage_ve},
#line 271 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mon", BaseLanguage_mn},
      {""},
#line 397 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"th",  BaseLanguage_th},
      {""},
#line 374 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sq",  BaseLanguage_sq},
#line 81 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bod", BaseLanguage_bo},
      {""}, {""},
#line 382 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sot", BaseLanguage_st},
#line 264 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mi",  BaseLanguage_mi},
#line 433 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"wal", BaseLanguage_wal},
      {""}, {""}, {""},
#line 145 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fr",  BaseLanguage_fr},
      {""}, {""},
#line 169 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hi",  BaseLanguage_hi},
      {""},
#line 349 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sc",  BaseLanguage_sc},
#line 426 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ven", BaseLanguage_ve},
      {""}, {""}, {""}, {""},
#line 69 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bem", BaseLanguage_bem},
      {""}, {""}, {""}, {""},
#line 149 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fry", BaseLanguage_fy},
      {""},
#line 167 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"he",  BaseLanguage_he},
#line 168 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"heb", BaseLanguage_he},
      {""},
#line 139 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ful", BaseLanguage_ff},
      {""}, {""},
#line 170 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hin", BaseLanguage_hi},
#line 46 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"aa",  BaseLanguage_aa},
#line 435 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"wol", BaseLanguage_wo},
      {""},
#line 309 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"or",  BaseLanguage_or},
#line 355 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"seh", BaseLanguage_seh},
#line 450 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"zu",  BaseLanguage_zu},
#line 432 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"wae", BaseLanguage_wae},
      {""},
#line 209 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kk",  BaseLanguage_kk},
      {""}, {""},
#line 210 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kaz", BaseLanguage_kk},
      {""},
#line 362 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sk",  BaseLanguage_sk},
#line 211 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kkj", BaseLanguage_kkj},
#line 285 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"no",  BaseLanguage_nb},
#line 287 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nob", BaseLanguage_nb},
      {""},
#line 402 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tk",  BaseLanguage_tk},
#line 354 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sme", BaseLanguage_se},
      {""}, {""}, {""}, {""},
#line 322 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"pus", BaseLanguage_ps},
#line 274 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mr",  BaseLanguage_mr},
#line 304 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nyn", BaseLanguage_nyn},
      {""},
#line 53 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"am",  BaseLanguage_am},
#line 258 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mer", BaseLanguage_mer},
      {""},
#line 350 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"srd", BaseLanguage_sc},
      {""},
#line 171 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hr",  BaseLanguage_hr},
      {""}, {""},
#line 393 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tel", BaseLanguage_te},
      {""}, {""}, {""}, {""},
#line 346 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"saq", BaseLanguage_saq},
      {""}, {""},
#line 47 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"aar", BaseLanguage_aa},
      {""}, {""},
#line 262 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mgh", BaseLanguage_mgh},
      {""},
#line 166 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"haw", BaseLanguage_haw},
#line 313 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"pa",  BaseLanguage_pa},
#line 68 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bel", BaseLanguage_be},
#line 192 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"iku", BaseLanguage_iu},
#line 156 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"gn",  BaseLanguage_gn},
#line 216 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"khm", BaseLanguage_km},
#line 159 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"gu",  BaseLanguage_gu},
#line 358 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sag", BaseLanguage_sg},
      {""}, {""}, {""},
#line 291 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ne",  BaseLanguage_ne},
#line 160 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"guj", BaseLanguage_gu},
      {""}, {""}, {""},
#line 420 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ur",  BaseLanguage_ur},
      {""}, {""}, {""}, {""},
#line 278 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mt",  BaseLanguage_mt},
#line 314 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"pan", BaseLanguage_pa},
#line 180 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ibb", BaseLanguage_ibb},
      {""}, {""},
#line 212 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kl",  BaseLanguage_kl},
#line 269 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mal", BaseLanguage_ml},
      {""},
#line 305 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"oc",  BaseLanguage_oc},
#line 273 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"moh", BaseLanguage_moh},
#line 364 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sl",  BaseLanguage_sl},
#line 430 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"vol", BaseLanguage_vo},
      {""}, {""}, {""},
#line 150 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ga",  BaseLanguage_ga},
#line 142 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fil", BaseLanguage_fil},
      {""}, {""}, {""},
#line 385 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sv",  BaseLanguage_sv},
#line 363 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"slk", BaseLanguage_sk},
#line 298 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nnh", BaseLanguage_nnh},
      {""},
#line 328 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"que", BaseLanguage_quz_EC},
      {""},
#line 214 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kln", BaseLanguage_kln},
      {""}, {""}, {""},
#line 300 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nr",  BaseLanguage_nr},
      {""}, {""}, {""},
#line 308 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"orm", BaseLanguage_om},
      {""}, {""}, {""},
#line 351 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sd",  BaseLanguage_sd},
      {""},
#line 100 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cu",  BaseLanguage_cu},
#line 70 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bez", BaseLanguage_bez},
      {""}, {""}, {""},
#line 55 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ar",  BaseLanguage_ar},
#line 181 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"id",  BaseLanguage_id},
      {""}, {""},
#line 326 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"quz", BaseLanguage_quz},
      {""},
#line 401 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tig", BaseLanguage_tig},
      {""}, {""},
#line 324 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"por", BaseLanguage_pt},
      {""},
#line 83 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bre", BaseLanguage_br},
      {""}, {""},
#line 371 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sna", BaseLanguage_sn},
      {""}, {""}, {""},
#line 266 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mk",  BaseLanguage_mk},
      {""}, {""},
#line 57 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"arn", BaseLanguage_arn},
#line 179 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ina", BaseLanguage_ia},
      {""},
#line 177 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hye", BaseLanguage_hy},
#line 88 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ca",  BaseLanguage_ca},
#line 415 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tzm", BaseLanguage_tzm},
      {""}, {""}, {""}, {""},
#line 428 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"vie", BaseLanguage_vi},
      {""}, {""}, {""}, {""},
#line 144 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fao", BaseLanguage_fo},
      {""}, {""}, {""},
#line 193 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ja",  BaseLanguage_ja},
      {""}, {""}, {""}, {""},
#line 104 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cy",  BaseLanguage_cy},
#line 394 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"teo", BaseLanguage_teo},
      {""}, {""}, {""}, {""}, {""},
#line 54 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"amh", BaseLanguage_am},
      {""},
#line 320 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"prs", BaseLanguage_prs},
#line 96 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"co",  BaseLanguage_co},
#line 451 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"zul", BaseLanguage_zu},
#line 449 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"zgh", BaseLanguage_zgh},
#line 418 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"uk",  BaseLanguage_uk},
#line 97 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cos", BaseLanguage_co},
      {""},
#line 283 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mzn", BaseLanguage_mzn},
      {""}, {""}, {""},
#line 224 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ks",  BaseLanguage_ks},
#line 226 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ksb", BaseLanguage_ksb},
      {""}, {""}, {""},
#line 378 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ss",  BaseLanguage_ss},
      {""}, {""}, {""},
#line 366 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sma", BaseLanguage_sma},
#line 410 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ts",  BaseLanguage_ts},
#line 421 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"urd", BaseLanguage_ur},
      {""},
#line 187 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"is",  BaseLanguage_is},
      {""},
#line 422 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"uz",  BaseLanguage_uz},
#line 423 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"uzb", BaseLanguage_uz},
      {""}, {""},
#line 263 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mgo", BaseLanguage_mgo},
#line 268 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ml",  BaseLanguage_ml},
#line 380 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ssy", BaseLanguage_ssy},
      {""},
#line 445 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"zh",  BaseLanguage_zh},
#line 206 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"khq", BaseLanguage_khq},
#line 85 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bs",  BaseLanguage_bs},
      {""}, {""}, {""}, {""},
#line 323 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"pt",  BaseLanguage_pt},
#line 405 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tsn", BaseLanguage_tn},
      {""}, {""},
#line 232 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cor", BaseLanguage_kw},
#line 91 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ce",  BaseLanguage_ce},
#line 93 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ceb", BaseLanguage_ceb},
#line 419 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ukr", BaseLanguage_uk},
#line 125 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"en",  BaseLanguage_en},
#line 99 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ces", BaseLanguage_cs},
#line 133 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"eu",  BaseLanguage_eu},
#line 157 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"grn", BaseLanguage_gn},
      {""},
#line 51 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ak",  BaseLanguage_ak},
#line 134 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"eus", BaseLanguage_eu},
#line 138 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ff",  BaseLanguage_ff},
      {""}, {""}, {""}, {""},
#line 231 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kw",  BaseLanguage_kw},
#line 204 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kea", BaseLanguage_kea},
      {""}, {""},
#line 205 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kgp", BaseLanguage_kgp},
#line 387 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sw",  BaseLanguage_sw},
#line 284 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"naq", BaseLanguage_naq},
      {""},
#line 243 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ln",  BaseLanguage_ln},
      {""},
#line 250 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lu",  BaseLanguage_lu},
#line 251 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lub", BaseLanguage_lu},
      {""},
#line 437 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"xh",  BaseLanguage_xh},
#line 297 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nno", BaseLanguage_nn},
#line 62 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"az",  BaseLanguage_az},
#line 318 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"pol", BaseLanguage_pl},
      {""},
#line 333 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"rn",  BaseLanguage_rn},
#line 50 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"agq", BaseLanguage_agq},
#line 338 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ru",  BaseLanguage_ru},
#line 280 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mua", BaseLanguage_mua},
      {""}, {""},
#line 339 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"rus", BaseLanguage_ru},
      {""},
#line 253 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"luy", BaseLanguage_luy},
      {""},
#line 286 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nb",  BaseLanguage_nb},
#line 267 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mkd", BaseLanguage_mk},
      {""},
#line 105 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cym", BaseLanguage_cy},
      {""}, {""}, {""},
#line 442 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"yo",  BaseLanguage_yo},
      {""}, {""},
#line 240 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lg",  BaseLanguage_lg},
#line 74 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bho", BaseLanguage_bho},
#line 235 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"la",  BaseLanguage_la},
#line 334 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"run", BaseLanguage_rn},
      {""}, {""}, {""},
#line 293 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nl",  BaseLanguage_nl},
#line 417 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"uig", BaseLanguage_ug},
      {""}, {""},
#line 295 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nmg", BaseLanguage_nmg},
#line 127 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"eo",  BaseLanguage_eo},
#line 161 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"guz", BaseLanguage_guz},
      {""},
#line 311 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"os",  BaseLanguage_os},
#line 227 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ksf", BaseLanguage_ksf},
#line 106 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"da",  BaseLanguage_da},
#line 330 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"raj", BaseLanguage_raj},
#line 312 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"oss", BaseLanguage_os},
      {""},
#line 89 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cat", BaseLanguage_ca},
      {""}, {""}, {""}, {""},
#line 73 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"bgc", BaseLanguage_bgc},
#line 440 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"yi",  BaseLanguage_yi},
      {""}, {""}, {""}, {""},
#line 245 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lo",  BaseLanguage_lo},
#line 282 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mya", BaseLanguage_my},
      {""},
#line 288 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nd",  BaseLanguage_nd},
#line 203 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"kde", BaseLanguage_kde},
#line 276 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ms",  BaseLanguage_ms},
#line 107 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dan", BaseLanguage_da},
#line 290 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nds", BaseLanguage_nds},
#line 331 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"rm",  BaseLanguage_rm},
#line 443 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"yor", BaseLanguage_yo},
#line 335 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ro",  BaseLanguage_ro},
#line 130 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"spa", BaseLanguage_es},
      {""}, {""},
#line 173 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hsb", BaseLanguage_hsb},
      {""},
#line 379 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ssw", BaseLanguage_ss},
      {""}, {""},
#line 228 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ksh", BaseLanguage_ksh},
      {""}, {""}, {""}, {""}, {""},
#line 121 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ee",  BaseLanguage_ee},
#line 194 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"jpn", BaseLanguage_ja},
      {""}, {""},
#line 190 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ita", BaseLanguage_it},
      {""},
#line 336 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ron", BaseLanguage_ro},
      {""}, {""},
#line 398 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tha", BaseLanguage_th},
      {""}, {""}, {""}, {""},
#line 101 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"chu", BaseLanguage_cu},
#line 317 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"pl",  BaseLanguage_pl},
#line 316 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"pcm", BaseLanguage_pcm},
      {""}, {""},
#line 329 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"qup", BaseLanguage_quz_PE},
      {""},
#line 146 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"fra", BaseLanguage_fr},
      {""}, {""},
#line 279 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mlt", BaseLanguage_mt},
      {""},
#line 244 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lin", BaseLanguage_ln},
#line 95 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"chr", BaseLanguage_chr},
      {""},
#line 186 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"iii", BaseLanguage_ii},
      {""},
#line 377 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"srp", BaseLanguage_sr},
      {""}, {""},
#line 188 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"isl", BaseLanguage_is},
#line 109 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"de",  BaseLanguage_de},
      {""},
#line 184 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ibo", BaseLanguage_ig},
      {""}, {""},
#line 48 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"af",  BaseLanguage_af},
      {""}, {""}, {""}, {""}, {""},
#line 436 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"xog", BaseLanguage_xog},
      {""}, {""},
#line 272 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mni", BaseLanguage_mni},
#line 154 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"gl",  BaseLanguage_gl},
      {""}, {""}, {""},
#line 325 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"quc", BaseLanguage_quc},
      {""},
#line 424 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"vai", BaseLanguage_vai},
      {""}, {""}, {""},
#line 162 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"gv",  BaseLanguage_gv},
      {""}, {""}, {""}, {""}, {""},
#line 110 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"deu", BaseLanguage_de},
      {""}, {""}, {""},
#line 58 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"as",  BaseLanguage_as},
      {""}, {""}, {""}, {""}, {""},
#line 294 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nld", BaseLanguage_nl},
      {""},
#line 152 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"gd",  BaseLanguage_gd},
#line 94 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cgg", BaseLanguage_cgg},
      {""},
#line 256 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mai", BaseLanguage_mai},
      {""}, {""},
#line 49 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"afr", BaseLanguage_af},
#line 131 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"et",  BaseLanguage_et},
      {""}, {""}, {""},
#line 337 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"rof", BaseLanguage_rof},
      {""}, {""}, {""}, {""},
#line 236 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lat", BaseLanguage_la},
      {""},
#line 441 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"yid", BaseLanguage_yi},
      {""}, {""},
#line 301 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nbl", BaseLanguage_nr},
      {""}, {""}, {""}, {""}, {""},
#line 248 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lt",  BaseLanguage_lt},
#line 319 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"prg", BaseLanguage_prg},
      {""}, {""}, {""}, {""},
#line 386 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"swe", BaseLanguage_sv},
      {""}, {""}, {""},
#line 102 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cv",  BaseLanguage_cv},
#line 111 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dje", BaseLanguage_dje},
      {""}, {""},
#line 195 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"jgo", BaseLanguage_jgo},
#line 321 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ps",  BaseLanguage_ps},
#line 63 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"aze", BaseLanguage_az},
      {""}, {""},
#line 332 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"roh", BaseLanguage_rm},
      {""},
#line 299 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nqo", BaseLanguage_nqo},
      {""}, {""},
#line 359 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"shi", BaseLanguage_shi},
#line 197 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"jv",  BaseLanguage_jv},
      {""}, {""}, {""}, {""}, {""},
#line 375 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sqi", BaseLanguage_sq},
      {""}, {""}, {""}, {""},
#line 261 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mlg", BaseLanguage_mg},
      {""}, {""},
#line 447 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cch", BaseLanguage_zh_Hant_HK},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 259 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mfe", BaseLanguage_mfe},
      {""}, {""},
#line 249 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lit", BaseLanguage_lt},
      {""},
#line 411 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"tso", BaseLanguage_ts},
      {""}, {""},
#line 126 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"eng", BaseLanguage_en},
      {""},
#line 56 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ara", BaseLanguage_ar},
      {""}, {""}, {""}, {""},
#line 414 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"twq", BaseLanguage_twq},
      {""}, {""},
#line 446 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"zho", BaseLanguage_zh},
      {""},
#line 59 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"asm", BaseLanguage_as},
      {""}, {""},
#line 310 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ori", BaseLanguage_or},
      {""},
#line 315 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"pap", BaseLanguage_pap},
      {""}, {""},
#line 289 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nde", BaseLanguage_nd},
      {""},
#line 241 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lug", BaseLanguage_lg},
      {""}, {""},
#line 92 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"che", BaseLanguage_ce},
      {""},
#line 292 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nep", BaseLanguage_ne},
      {""}, {""},
#line 348 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"sbp", BaseLanguage_sbp},
      {""},
#line 444 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"yrl", BaseLanguage_yrl},
      {""}, {""}, {""}, {""},
#line 265 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"mri", BaseLanguage_mi},
      {""}, {""},
#line 172 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"hrv", BaseLanguage_hr},
      {""}, {""}, {""}, {""}, {""},
#line 118 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dz",  BaseLanguage_dz},
#line 252 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"luo", BaseLanguage_luo},
      {""}, {""},
#line 438 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"xho", BaseLanguage_xh},
#line 98 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"cs",  BaseLanguage_cs},
#line 237 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lag", BaseLanguage_lag},
      {""}, {""}, {""},
#line 123 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"el",  BaseLanguage_el},
      {""}, {""},
#line 238 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lb",  BaseLanguage_lb},
#line 61 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ast", BaseLanguage_ast},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 120 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ebu", BaseLanguage_ebu},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 246 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lao", BaseLanguage_lo},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 254 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lv",  BaseLanguage_lv},
#line 365 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"slv", BaseLanguage_sl},
      {""}, {""},
#line 306 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"oci", BaseLanguage_oc},
      {""},
#line 151 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"gle", BaseLanguage_ga},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 115 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dv",  BaseLanguage_dv},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 52 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"aka", BaseLanguage_ak},
      {""},
#line 117 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dyo", BaseLanguage_dyo},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 388 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"swa", BaseLanguage_sw},
      {""}, {""}, {""}, {""},
#line 158 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"gsw", BaseLanguage_gsw},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 114 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dua", BaseLanguage_dua},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 155 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"glg", BaseLanguage_gl},
#line 242 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lkt", BaseLanguage_lkt},
      {""}, {""},
#line 129 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"es",  BaseLanguage_es},
#line 198 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"jav", BaseLanguage_jv},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 239 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ltz", BaseLanguage_lb},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 196 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"jmc", BaseLanguage_jmc},
      {""},
#line 302 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"nso", BaseLanguage_nso},
      {""}, {""}, {""}, {""},
#line 277 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"msa", BaseLanguage_ms},
      {""}, {""}, {""}, {""},
#line 113 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dsb", BaseLanguage_dsb},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 128 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"epo", BaseLanguage_eo},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 124 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ell", BaseLanguage_el},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 340 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"rw",  BaseLanguage_rw},
#line 439 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"yav", BaseLanguage_yav},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 342 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"rwk", BaseLanguage_rwk},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 153 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"gla", BaseLanguage_gd},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 255 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lav", BaseLanguage_lv},
      {""}, {""}, {""}, {""},
#line 60 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"asa", BaseLanguage_asa},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 108 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dav", BaseLanguage_dav},
      {""}, {""}, {""}, {""},
#line 90 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ccp", BaseLanguage_ccp},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 132 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"est", BaseLanguage_et},
      {""}, {""}, {""}, {""},
#line 103 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"chv", BaseLanguage_cv},
      {""}, {""}, {""}, {""},
#line 448 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"chi", BaseLanguage_zh_Hans_SG},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 112 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"doi", BaseLanguage_doi},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 119 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"dzo", BaseLanguage_dz},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 116 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"div", BaseLanguage_dv},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 163 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"glv", BaseLanguage_gv},
      {""}, {""}, {""}, {""},
#line 247 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"lrc", BaseLanguage_lrc},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 122 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ewe", BaseLanguage_ee},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 135 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"
      {"ewo", BaseLanguage_ewo}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = P32LanguageCodeHash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return (struct language *) 0;
}
#line 452 "F:/posix32/lib/posix32-crt/gperf/language_codes.gperf"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

bool p32_lookup_language_code (LanguageInfo *info) {
  struct language *language = P32LanguageCodeLookup (info->String, info->Length);

  if (language == NULL) {
    return false;
  }

  info->BaseLanguage = language->Index;

  return true;
}
