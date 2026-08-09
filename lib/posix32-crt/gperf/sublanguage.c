/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf --output-file=F:/posix32/lib/posix32-crt/gperf/sublanguage.c F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf  */
/* Computed positions: -k'1-5,7,9' */

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

#line 1 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"

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

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4267)
#endif
#line 45 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
struct sublanguage {const char *name; SublanguageIndex Index; };

#define TOTAL_KEYWORDS 503
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 18
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 1555
/* maximum key range = 1553, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
P32SublanguageHash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556,    0, 1556, 1556,   10, 1556,
      1556,    0,   35,    5, 1556, 1556, 1556,    0, 1556, 1556,
      1556, 1556, 1556, 1556,    0,   30,  150,    0,   15,  260,
       435,   55,   20,   20,  210,  465,   60,  230,   88,  353,
       150,    0,  138,  118,  440,   75,  230,  295,  218,   85,
       208,    5,  283,  258, 1556,  225, 1556,    5,   55,  175,
        10,  425,    0,  405,  455,  115,   10,  475,    0,   35,
        30,    1,  325,  130,   15,   20,  225,   45,    3,  430,
       400,   10,  270,  365,   85,    0,  320,  481,   70,  371,
       213,  385,    0,  421,   30,  190,  215,  350,    0,   60,
        65,   95, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556, 1556,
      1556, 1556, 1556, 1556, 1556
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[8]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 8:
      case 7:
        hval += asso_values[(unsigned char)str[6]];
#if (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]+3];
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
        hval += asso_values[(unsigned char)str[0]+19];
        break;
    }
  return hval;
}

struct sublanguage *
P32SublanguageLookup (register const char *str, register size_t len)
{
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static struct sublanguage wordlist[] =
    {
      {""}, {""}, {""},
#line 526 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"wo",             wo_SN},
      {""}, {""},
#line 109 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"co",             co_FR},
#line 268 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ja",             ja_JP},
      {""}, {""},
#line 102 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ca",             ca_ES},
      {""},
#line 183 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ff",             ff_Latn_SN},
#line 194 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fo",             fo_FO},
      {""},
#line 113 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"cy",             cy_GB},
      {""},
#line 180 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fa",             fa_IR},
      {""}, {""}, {""}, {""},
#line 212 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fy",             fy_NL},
      {""}, {""},
#line 111 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"cs",             cs_CZ},
      {""},
#line 196 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr",             fr_FR},
      {""}, {""}, {""}, {""},
#line 421 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sl",             sl_SI},
#line 433 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"so",             so_SO},
      {""},
#line 461 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sv",             sv_SE},
#line 184 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ff@fallback",    ff_Latn_NG},
#line 401 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sa",             sa_IN},
      {""}, {""}, {""}, {""},
#line 405 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sd",             sd_Arab_PK},
      {""}, {""},
#line 199 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-CA",          fr_CA},
#line 197 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-029",         fr_029},
#line 437 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr",               sr_Latn_CS},
#line 91 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bo",             bo_CN},
      {""}, {""},
#line 92 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bo-CN",          bo_CN},
#line 79 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ba",             ba_RU},
      {""}, {""}, {""}, {""}, {""},
#line 466 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"syr",            syr_SY},
      {""},
#line 402 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sa-IN",          sa_IN},
      {""},
#line 93 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"br",             br_FR},
      {""}, {""},
#line 407 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sd-IN",          sd_Deva_IN},
#line 406 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sd@fallback",    sd_Deva_IN},
#line 95 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bs",             bs_Latn_BA},
#line 532 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"yo",             yo_NG},
      {""},
#line 181 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fa-AF",          prs_AF},
#line 438 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr@fallback",      sr_Cyrl_CS},
#line 310 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ml",             ml_IN},
#line 423 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sma",            sma_SE},
      {""}, {""},
#line 424 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sma-NO",         sma_NO},
#line 87 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bn",             bn_BD},
#line 426 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"smj",            smj_SE},
      {""}, {""},
#line 427 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"smj-NO",         smj_NO},
#line 337 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"my",             my_MM},
      {""}, {""},
#line 200 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-CD",          fr_CD},
      {""},
#line 329 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mr",             mr_IN},
#line 431 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sms",            sms_FI},
      {""},
#line 202 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-CI",          fr_CI},
#line 96 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bs@fallback",    bs_Cyrl_BA},
#line 331 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ms",             ms_MY},
      {""}, {""},
#line 311 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ml-IN",          ml_IN},
      {""},
#line 250 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"id",             id_ID},
#line 429 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"smn",            smn_FI},
      {""},
#line 90 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bn-IN",          bn_IN},
#line 88 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bn@fallback",    bn_IN},
#line 312 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn",               mn_Cyrl_MN},
      {""}, {""},
#line 314 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-CN",            mn_Mong_CN},
      {""},
#line 256 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"is",             is_IS},
      {""}, {""},
#line 330 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mr-IN",          mr_IN},
#line 432 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sms-FI",         sms_FI},
      {""}, {""}, {""},
#line 182 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fa-IR",          fa_IR},
#line 332 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ms@fallback",    ms_BN},
#line 187 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ff-Latn",        ff_Latn_SN},
      {""}, {""}, {""},
#line 430 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"smn-FI",         smn_FI},
      {""}, {""},
#line 527 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"wo-SN",          wo_SN},
      {""},
#line 313 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn@fallback",      mn_Mong_CN},
#line 190 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fi",             fi_FI},
#line 192 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fil",            fil_PH},
      {""}, {""}, {""},
#line 261 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"iu",             iu_Latn_CA},
#line 186 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ff-SN",          ff_Latn_SN},
      {""}, {""}, {""},
#line 447 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Cyrl",          sr_Cyrl_RS},
      {""}, {""},
#line 450 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Cyrl-CS",       sr_Cyrl_CS},
      {""},
#line 548 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zu",             zu_ZA},
      {""}, {""}, {""},
#line 448 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Cyrl@fallback", sr_Cyrl_CS},
#line 417 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"si",             si_LK},
#line 211 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-SN",          fr_SN},
      {""},
#line 263 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"iu-CA",          iu_Latn_CA},
      {""},
#line 453 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Latn",          sr_Latn_RS},
      {""}, {""},
#line 456 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Latn-CS",       sr_Latn_CS},
#line 262 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"iu@fallback",    iu_Cans_CA},
#line 98 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bs-Cyrl",        bs_Cyrl_BA},
      {""}, {""}, {""},
#line 454 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Latn@fallback", sr_Latn_CS},
#line 435 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sq",             sq_AL},
      {""}, {""}, {""}, {""},
#line 264 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"iu-Cans",        iu_Cans_CA},
      {""}, {""},
#line 265 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"iu-Cans-CA",     iu_Cans_CA},
      {""},
#line 100 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bs-Latn",        bs_Latn_BA},
      {""}, {""},
#line 251 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"id-ID",          id_ID},
#line 463 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sv-SE",          sv_SE},
#line 409 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sd-Arab",        sd_Arab_PK},
      {""}, {""},
#line 203 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-CM",          fr_CM},
      {""},
#line 530 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"yi",             yi_001},
      {""}, {""}, {""}, {""},
#line 306 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mi",             mi_NZ},
#line 210 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-RE",          fr_RE},
      {""}, {""}, {""},
#line 317 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-Cyrl",          mn_Cyrl_MN},
#line 85 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bin",            bin_NG},
      {""}, {""},
#line 531 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"yi-001",         yi_001},
#line 468 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ta",             ta_IN},
#line 248 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ibb",            ibb_NG},
      {""},
#line 198 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-BE",          fr_BE},
#line 249 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ibb-NG",         ibb_NG},
#line 254 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ii",             ii_CN},
      {""}, {""},
#line 255 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ii-CN",          ii_CN},
      {""},
#line 490 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tr",             tr_TR},
#line 188 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ff-Latn-NG",     ff_Latn_NG},
      {""}, {""}, {""},
#line 492 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ts",             ts_ZA},
#line 422 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sl-SI",          sl_SI},
      {""},
#line 439 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-BA",            sr_Latn_BA},
      {""}, {""},
#line 323 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mni",            mni_Beng_IN},
      {""},
#line 470 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ta-IN",          ta_IN},
#line 469 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ta@fallback",    ta_LK},
#line 486 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tn",             tn_ZA},
#line 383 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"quc",            quc_Latn_GT},
#line 440 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-BA@fallback",   sr_Cyrl_BA},
      {""}, {""},
#line 47 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"af",             af_ZA},
      {""}, {""},
#line 355 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"or",             or_IN},
      {""},
#line 511 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ur",             ur_PK},
      {""}, {""},
#line 97 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bs-BA",          bs_Latn_BA},
      {""},
#line 266 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"iu-Latn",        iu_Latn_CA},
#line 189 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ff-Latn-SN",     ff_Latn_SN},
      {""},
#line 267 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"iu-Latn-CA",     iu_Latn_CA},
      {""},
#line 51 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar",             ar_SA},
      {""}, {""},
#line 333 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ms-BN",          ms_BN},
#line 487 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tn@fallback",    tn_BW},
#line 70 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"as",             as_IN},
      {""},
#line 193 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fil-PH",         fil_PH},
#line 353 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"om",             om_ET},
      {""}, {""},
#line 356 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"or-IN",          or_IN},
      {""},
#line 513 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ur-IN",          ur_IN},
#line 512 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ur@fallback",    ur_IN},
#line 459 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"st",             st_ZA},
      {""}, {""},
#line 63 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-QA",          ar_QA},
      {""},
#line 49 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"am",             am_ET},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 71 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"as-IN",          as_IN},
      {""},
#line 218 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gl",             gl_ES},
#line 68 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"arn",            arn_CL},
      {""},
#line 207 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-MA",          fr_MA},
      {""},
#line 214 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ga",             ga_IE},
#line 452 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Cyrl-RS",       sr_Cyrl_RS},
      {""},
#line 441 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-CS",            sr_Latn_CS},
#line 324 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mni-IN",         mni_Beng_IN},
#line 216 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gd",             gd_GB},
#line 408 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sd-PK",          sd_Arab_PK},
      {""},
#line 89 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bn-BD",          bn_BD},
#line 112 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"cs-CZ",          cs_CZ},
      {""}, {""},
#line 442 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-CS@fallback",   sr_Cyrl_CS},
#line 449 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Cyrl-BA",       sr_Cyrl_BA},
#line 467 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"syr-SY",         syr_SY},
#line 434 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"so-SO",          so_SO},
#line 458 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Latn-RS",       sr_Latn_RS},
      {""},
#line 52 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-AE",          ar_AE},
      {""},
#line 335 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mt",             mt_MT},
#line 418 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"si-LK",          si_LK},
      {""},
#line 443 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-ME",            sr_Latn_ME},
      {""},
#line 220 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gn",             gn_PY},
      {""}, {""},
#line 455 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Latn-BA",       sr_Latn_BA},
#line 425 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sma-SE",         sma_SE},
#line 480 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ti",             ti_ER},
#line 206 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-LU",          fr_LU},
#line 444 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-ME@fallback",   sr_Cyrl_ME},
#line 99 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bs-Cyrl-BA",     bs_Cyrl_BA},
#line 428 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"smj-SE",         smj_SE},
#line 258 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"it",             it_IT},
#line 185 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ff-NG",          ff_Latn_NG},
      {""}, {""}, {""},
#line 224 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gu",             gu_IN},
#line 387 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"quz",            quz_BO},
      {""},
#line 215 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ga-IE",          ga_IE},
      {""},
#line 274 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kl",             kl_GL},
#line 280 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ko",             ko_KR},
      {""},
#line 101 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bs-Latn-BA",     bs_Latn_BA},
      {""},
#line 270 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ka",             ka_GE},
      {""}, {""},
#line 410 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sd-Arab-PK",     sd_Arab_PK},
#line 481 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ti@fallback",    ti_ET},
#line 294 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ky",             ky_KG},
#line 114 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"cy-GB",          cy_GB},
      {""},
#line 315 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-MN",            mn_Cyrl_MN},
      {""},
#line 284 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kr",             kr_NG},
#line 269 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ja-JP",          ja_JP},
      {""},
#line 225 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gu-IN",          gu_IN},
#line 389 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"quz-EC",         quz_EC},
#line 286 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ks",             ks_Deva_IN},
      {""},
#line 316 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-MN@fallback",   mn_Mong_MN},
#line 205 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-HT",          fr_HT},
      {""}, {""},
#line 471 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ta-LK",          ta_LK},
      {""},
#line 67 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-YE",          ar_YE},
      {""},
#line 278 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kn",             kn_IN},
      {""}, {""}, {""}, {""},
#line 276 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"km",             km_KH},
      {""}, {""},
#line 257 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"is-IS",          is_IS},
      {""}, {""}, {""}, {""},
#line 287 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ks-IN",          ks_Deva_IN},
      {""},
#line 290 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ku",             ku_Arab_IQ},
#line 549 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zu-ZA",          zu_ZA},
#line 533 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"yo-NG",          yo_NG},
#line 451 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Cyrl-ME",       sr_Cyrl_ME},
      {""},
#line 226 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ha",             ha_Latn_NG},
#line 344 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nl",             nl_NL},
#line 377 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"qps-ploc",           SublanguageIndex_qps_ploc},
#line 279 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kn-IN",          kn_IN},
      {""},
#line 246 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hy",             hy_AM},
#line 64 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-SA",          ar_SA},
      {""},
#line 379 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"qps-ploca",          SublanguageIndex_qps_ploca},
      {""},
#line 238 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hr",             hr_Latn_HR},
#line 378 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"qps-ploc@fallback",  en_US},
      {""},
#line 457 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-Latn-ME",       sr_Latn_ME},
      {""},
#line 369 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pl",             pl_PL},
      {""},
#line 380 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"qps-ploca@fallback", ja_JP},
#line 351 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"oc",             oc_FR},
      {""},
#line 357 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pa",             pa_Guru_IN},
      {""}, {""},
#line 349 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nso",            nso_ZA},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 129 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"el",             el_GR},
#line 347 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nn",             nn_NO},
      {""},
#line 271 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ka-GE",          ka_GE},
#line 239 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hr@fallback",    hr_Latn_BA},
#line 371 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ps",             ps_AF},
      {""}, {""},
#line 381 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"qps-plocm",          SublanguageIndex_qps_plocm},
#line 80 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ba-RU",          ba_RU},
#line 244 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hu",             hu_HU},
      {""}, {""},
#line 359 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pa-IN",          pa_Guru_IN},
#line 358 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pa@fallback",    pa_Arab_PK},
#line 494 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tt",             tt_RU},
#line 445 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-RS",            sr_Latn_RS},
#line 382 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"qps-plocm@fallback", ar_SA},
      {""},
#line 86 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bin_NG",         bin_NG},
#line 152 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es",             es_ES_modern},
#line 367 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"prs",            prs_AF},
#line 393 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ro",             ro_RO},
#line 318 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-Cyrl-MN",       mn_Cyrl_MN},
      {""},
#line 446 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sr-RS@fallback",   sr_Cyrl_RS},
#line 339 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nb",             nb_NO},
      {""}, {""}, {""},
#line 131 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en",             en_US},
      {""}, {""},
#line 125 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"dv",             dv_MV},
#line 384 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"quc-GT",         quc_Latn_GT},
#line 115 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"da",             da_DK},
#line 493 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ts-ZA",          ts_ZA},
#line 390 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"quz-PE",         quz_PE},
      {""},
#line 368 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"prs-AF",         prs_AF},
      {""},
#line 242 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hsb",            hsb_DE},
      {""},
#line 165 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-HN",          es_HN},
#line 153 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es@fallback",    es_ES},
#line 178 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"eu",             eu_ES},
#line 489 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tn-ZA",          tn_ZA},
      {""},
#line 137 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-CA",          en_CA},
#line 132 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-029",         en_029},
#line 83 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bg",             bg_BG},
#line 48 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"af-ZA",          af_ZA},
      {""},
#line 142 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-IN",          en_IN},
      {""},
#line 413 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"se",             se_NO},
#line 391 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"rm",             rm_CH},
      {""},
#line 372 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ps-AF",          ps_AF},
      {""},
#line 464 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sw",             sw_KE},
      {""}, {""},
#line 338 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"my-MM",          my_MM},
#line 213 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fy-NL",          fy_NL},
#line 524 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"vi",             vi_VN},
#line 396 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ru",             ru_RU},
      {""}, {""}, {""},
#line 81 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"be",             be_BY},
#line 514 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ur-PK",          ur_PK},
      {""},
#line 141 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-IE",          en_IE},
#line 105 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"chr",            chr_Cher_US},
#line 228 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ha-Latn",        ha_Latn_NG},
      {""}, {""},
#line 240 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hr-HR",          hr_Latn_HR},
      {""},
#line 234 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hi",             hi_Deva_IN},
#line 460 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"st-ZA",          st_ZA},
#line 300 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"lo",             lo_LA},
#line 134 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-AE",          en_AE},
#line 304 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"lv",             lv_LV},
#line 515 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uz",             uz_Latn_UZ},
#line 296 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"la",             la_VA},
      {""},
#line 61 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-MA",          ar_MA},
      {""},
#line 252 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ig",             ig_NG},
#line 403 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sah",            sah_RU},
      {""},
#line 201 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-CH",          fr_CH},
#line 154 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-419",         es_419},
#line 72 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"az",             az_Latn_AZ},
#line 496 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm",             tzm_Latn_DZ},
      {""},
#line 159 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-CR",          es_CR},
#line 243 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hsb-DE",         hsb_DE},
#line 292 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ku-Arab",        ku_Arab_IQ},
#line 103 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ca-ES",          ca_ES},
      {""},
#line 235 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hi-IN",          hi_Deva_IN},
#line 133 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-419",         en_419},
#line 419 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sk",             sk_SK},
#line 123 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"dsb",            dsb_DE},
      {""},
#line 140 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-ID",          en_ID},
#line 516 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uz@fallback",    uz_Cyrl_UZ},
      {""},
#line 325 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mni-Beng",       mni_Beng_IN},
      {""},
#line 104 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ca-ES-valencia", ca_ES_valencia},
#line 326 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mni-Beng-IN",    mni_Beng_IN},
#line 528 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"xh",             xh_ZA},
#line 54 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-DZ",          ar_DZ},
#line 110 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"co-FR",          co_FR},
      {""},
#line 73 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"az@fallback",    az_Cyrl_AZ},
#line 411 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sd-Deva",        sd_Deva_IN},
#line 501 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-Arab",        tzm_Arab_MA},
      {""},
#line 293 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ku-Arab-IQ",     ku_Arab_IQ},
#line 502 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-Arab-MA",     tzm_Arab_MA},
#line 361 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pa-Arab",        pa_Arab_PK},
      {""},
#line 327 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"moh",            moh_CA},
#line 155 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-AR",          es_AR},
      {""}, {""},
#line 462 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sv-FI",          sv_FI},
      {""},
#line 130 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"el-GR",          el_GR},
#line 307 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mi-NZ",          mi_NZ},
      {""},
#line 298 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"lb",             lb_LU},
      {""},
#line 204 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-FR",          fr_FR},
      {""}, {""},
#line 139 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-HK",          en_HK},
      {""},
#line 412 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sd-Deva-IN",     sd_Deva_IN},
#line 345 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nl-BE",          nl_BE},
#line 308 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mk",             mk_MK},
#line 436 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sq-AL",          sq_AL},
      {""},
#line 241 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hr-BA",          hr_Latn_BA},
      {""},
#line 534 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh",             zh_Hans_CN},
      {""}, {""},
#line 536 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-CN",          zh_Hans_CN},
#line 498 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-DZ",          tzm_Latn_DZ},
      {""}, {""}, {""},
#line 247 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hy-AM",          hy_AM},
      {""},
#line 301 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"lo-LA",          lo_LA},
#line 158 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-CO",          es_CO},
      {""},
#line 59 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-LB",          ar_LB},
#line 124 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"dsb-DE",         dsb_DE},
      {""},
#line 229 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ha-Latn-NG",     ha_Latn_NG},
      {""}, {""},
#line 404 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sah-RU",         sah_RU},
      {""},
#line 167 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-NI",          es_NI},
      {""},
#line 94 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"br-FR",          br_FR},
#line 535 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh@fallback",    zh_Hant_TW},
#line 518 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uz-Cyrl",        uz_Cyrl_UZ},
#line 161 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-DO",          es_DO},
      {""}, {""},
#line 388 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"quz-BO",         quz_BO},
      {""},
#line 488 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tn-BW",          tn_BW},
      {""}, {""}, {""},
#line 75 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"az-Cyrl",        az_Cyrl_AZ},
#line 60 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-LY",          ar_LY},
#line 195 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fo-FO",          fo_FO},
#line 168 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-PA",          es_PA},
      {""},
#line 520 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uz-Latn",        uz_Latn_UZ},
#line 57 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-JO",          ar_JO},
      {""},
#line 169 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-PE",          es_PE},
      {""},
#line 474 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tg",             tg_Cyrl_TJ},
#line 416 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"se-SE",          se_SE},
      {""},
#line 217 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gd-GB",          gd_GB},
      {""},
#line 77 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"az-Latn",        az_Latn_AZ},
      {""}, {""}, {""}, {""},
#line 541 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-Hans",        zh_Hans_CN},
#line 334 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ms-MY",          ms_MY},
      {""},
#line 542 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-Hans-CN",     zh_Hans_CN},
      {""},
#line 373 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pt",             pt_PT},
#line 209 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-ML",          fr_ML},
      {""}, {""}, {""},
#line 472 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"te",             te_IN},
      {""}, {""},
#line 76 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"az-Cyrl-AZ",     az_Cyrl_AZ},
      {""},
#line 507 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ug",             ug_CN},
      {""}, {""},
#line 508 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ug-CN",          ug_CN},
      {""},
#line 176 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"et",             et_EE},
#line 172 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-SV",          es_SV},
      {""},
#line 260 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"it-IT",          it_IT},
      {""},
#line 348 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nn-NO",          nn_NO},
#line 360 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pa-PK",          pa_Arab_PK},
#line 305 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"lv-LV",          lv_LV},
#line 78 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"az-Latn-AZ",     az_Latn_AZ},
#line 374 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pt@fallback",    pt_BR},
      {""},
#line 285 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kr-NG",          kr_NG},
      {""},
#line 473 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"te-IN",          te_IN},
      {""}, {""},
#line 160 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-CU",          es_CU},
      {""},
#line 482 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ti-ER",          ti_ER},
#line 65 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-SY",          ar_SY},
#line 478 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"th",             th_TH},
      {""}, {""},
#line 170 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-PR",          es_PR},
#line 499 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-MA",          tzm_Arab_MA},
      {""},
#line 245 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hu-HU",          hu_HU},
      {""},
#line 519 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uz-Cyrl-UZ",     uz_Cyrl_UZ},
#line 350 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nso-ZA",         nso_ZA},
#line 340 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nb-NO",          nb_NO},
#line 150 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-ZA",          en_ZA},
      {""},
#line 166 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-MX",          es_MX},
      {""}, {""},
#line 537 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-HK",          zh_Hant_HK},
      {""},
#line 500 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-MA@fallback", tzm_Tfng_MA},
      {""},
#line 484 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tk",             tk_TM},
      {""}, {""},
#line 521 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uz-Latn-UZ",     uz_Latn_UZ},
      {""}, {""},
#line 227 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ha-NG",          ha_Latn_NG},
      {""},
#line 175 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-VE",          es_VE},
      {""},
#line 497 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm@fallback",    tzm_Arab_MA},
      {""}, {""},
#line 362 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pa-Arab-PK",     pa_Arab_PK},
      {""}, {""},
#line 135 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-AU",          en_AU},
      {""},
#line 66 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-TN",          ar_TN},
#line 415 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"se-NO",          se_NO},
#line 476 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tg-Cyrl",        tg_Cyrl_TJ},
      {""}, {""}, {""}, {""},
#line 509 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uk",             uk_UA},
      {""}, {""},
#line 208 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fr-MC",          fr_MC},
      {""},
#line 106 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"chr-US",         chr_Cher_US},
      {""}, {""},
#line 525 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"vi-VN",          vi_VN},
      {""},
#line 127 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"dz",             dz_BT},
#line 395 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ro-RO",          ro_RO},
      {""},
#line 56 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-IQ",          ar_IQ},
      {""}, {""},
#line 302 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"lt",             lt_LT},
      {""}, {""},
#line 69 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"arn-CL",         arn_CL},
#line 394 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ro-MD",          ro_MD},
#line 156 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-BO",          es_BO},
      {""},
#line 55 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-EG",          ar_EG},
#line 420 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sk-SK",          sk_SK},
      {""},
#line 365 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pap",            pap_029},
      {""}, {""}, {""},
#line 366 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pap-029",        pap_029},
#line 222 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gsw",            gsw_FR},
      {""},
#line 491 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tr-TR",          tr_TR},
      {""}, {""},
#line 543 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-Hans-SG",     zh_Hans_SG},
      {""},
#line 173 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-US",          es_US},
      {""}, {""},
#line 275 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kl-GL",          kl_GL},
      {""}, {""},
#line 297 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"la-VA",          la_VA},
      {""},
#line 221 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gn-PY",          gn_PY},
      {""},
#line 149 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-US",          en_US},
      {""}, {""},
#line 529 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"xh-ZA",          xh_ZA},
      {""}, {""},
#line 223 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gsw-FR",         gsw_FR},
      {""},
#line 62 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-OM",          ar_OM},
      {""},
#line 138 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-GB",          en_GB},
      {""}, {""},
#line 126 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"dv-MV",          dv_MV},
      {""}, {""},
#line 397 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ru-MD",          ru_MD},
#line 319 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-Mong",          mn_Mong_MN},
      {""}, {""},
#line 321 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-Mong-CN",       mn_Mong_CN},
      {""}, {""},
#line 147 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-SG",          en_SG},
      {""},
#line 219 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"gl-ES",          gl_ES},
#line 320 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-Mong@fallback", mn_Mong_CN},
      {""},
#line 116 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"da_DK",          da_DK},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 164 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-GT",          es_GT},
#line 495 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tt-RU",          tt_RU},
#line 522 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ve",             ve_ZA},
#line 157 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-CL",          es_CL},
      {""},
#line 259 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"it-CH",          it_CH},
#line 145 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-NZ",          en_NZ},
      {""},
#line 174 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-UY",          es_UY},
      {""},
#line 510 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uk-UA",          uk_UA},
      {""}, {""},
#line 74 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"az-AZ",          az_Latn_AZ},
      {""}, {""}, {""},
#line 232 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"he",             he_IL},
#line 253 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ig-NG",          ig_NG},
      {""},
#line 143 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-JM",          en_JM},
      {""},
#line 272 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kk",             kk_KZ},
#line 341 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ne",             ne_NP},
#line 282 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kok",            kok_IN},
      {""}, {""}, {""},
#line 230 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"haw",            haw_US},
      {""}, {""},
#line 385 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"quc-Latn",       quc_Latn_GT},
      {""},
#line 354 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"om-ET",          om_ET},
#line 386 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"quc-Latn-GT",    quc_Latn_GT},
      {""}, {""},
#line 544 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-Hant",        zh_Hant_TW},
      {""}, {""},
#line 84 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"bg-BG",          bg_BG},
      {""},
#line 346 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"nl-NL",          nl_NL},
      {""},
#line 299 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"lb-LU",          lb_LU},
#line 50 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"am-ET",          am_ET},
#line 342 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ne-IN",          ne_IN},
#line 288 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ks-Deva",        ks_Deva_IN},
#line 517 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"uz-UZ",          uz_Latn_UZ},
      {""},
#line 336 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mt-MT",          mt_MT},
      {""},
#line 398 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ru-RU",          ru_RU},
#line 505 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-Tfng",        tzm_Tfng_MA},
      {""},
#line 291 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ku-IQ",          ku_Arab_IQ},
#line 506 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-Tfng-MA",     tzm_Tfng_MA},
      {""}, {""}, {""},
#line 545 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-Hant-HK",     zh_Hant_HK},
      {""}, {""}, {""}, {""},
#line 375 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pt-BR",          pt_BR},
      {""}, {""},
#line 136 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-BZ",          en_BZ},
      {""},
#line 289 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ks-Deva-IN",     ks_Deva_IN},
      {""}, {""}, {""}, {""},
#line 191 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"fi_FI",          fi_FI},
#line 151 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-ZW",          en_ZW},
      {""},
#line 399 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"rw",             rw_RW},
      {""}, {""}, {""},
#line 117 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"de",             de_DE},
#line 171 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-PY",          es_PY},
      {""},
#line 53 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-BH",          ar_BH},
      {""},
#line 283 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kok-IN",         kok_IN},
      {""}, {""},
#line 483 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ti-ET",          ti_ET},
#line 281 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ko-KR",          ko_KR},
      {""},
#line 309 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mk-MK",          mk_MK},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 539 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-SG",          zh_Hans_SG},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 370 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pl-PL",          pl_PL},
      {""},
#line 120 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"de-DE",          de_DE},
      {""},
#line 363 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pa-Guru",        pa_Guru_IN},
      {""}, {""}, {""}, {""}, {""},
#line 352 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"oc-FR",          oc_FR},
      {""}, {""}, {""}, {""},
#line 82 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"be-BY",          be_BY},
      {""},
#line 177 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"et-EE",          et_EE},
      {""}, {""}, {""}, {""},
#line 163 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-ES",          es_ES_modern},
      {""}, {""},
#line 58 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ar-KW",          ar_KW},
      {""},
#line 364 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pa-Guru-IN",     pa_Guru_IN},
      {""}, {""},
#line 538 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-MO",          zh_Hant_MO},
      {""}, {""},
#line 392 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"rm-CH",          rm_CH},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 179 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"eu-ES",          eu_ES},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 144 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-MY",          en_MY},
      {""},
#line 465 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"sw-KE",          sw_KE},
      {""}, {""}, {""}, {""},
#line 414 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"se-FI",          se_FI},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 236 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hi-Deva",        hi_Deva_IN},
      {""}, {""}, {""}, {""},
#line 328 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"moh-CA",         moh_CA},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 121 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"de-LI",          de_LI},
      {""}, {""}, {""}, {""},
#line 237 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"hi-Deva-IN",     hi_Deva_IN},
      {""}, {""}, {""}, {""},
#line 322 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"mn-Mong-MN",       mn_Mong_MN},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 523 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ve-ZA",          ve_ZA},
      {""}, {""}, {""},
#line 343 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ne-NP",          ne_NP},
      {""}, {""},
#line 295 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"ky-KG",          ky_KG},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 231 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"haw-US",         haw_US},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 546 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-Hant-MO",     zh_Hant_MO},
      {""}, {""}, {""}, {""},
#line 146 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-PH",          en_PH},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 376 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"pt-PT",          pt_PT},
      {""}, {""}, {""}, {""}, {""},
#line 303 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"lt-LT",          lt_LT},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 503 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-Latn",        tzm_Latn_DZ},
      {""}, {""},
#line 504 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tzm-Latn-DZ",     tzm_Latn_DZ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 162 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"es-EC",          es_EC},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 477 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tg-Cyrl-TJ",     tg_Cyrl_TJ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 122 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"de-LU",          de_LU},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 128 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"dz-BT",          dz_BT},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 233 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"he-IL",          he_IL},
      {""},
#line 148 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"en-TT",          en_TT},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 118 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"de-AT",          de_AT},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 400 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"rw-RW",          rw_RW},
#line 540 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-TW",          zh_Hant_TW},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 547 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"zh-Hant-TW",     zh_Hant_TW},
      {""}, {""}, {""}, {""},
#line 485 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tk-TM",          tk_TM},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 475 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"tg-TJ",          tg_Cyrl_TJ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 277 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"km-KH",          km_KH},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 119 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"de-CH",          de_CH},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 107 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"chr-Cher",       chr_Cher_US},
      {""}, {""},
#line 108 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"chr-Cher-US",    chr_Cher_US},
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
#line 273 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"kk-KZ",          kk_KZ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 479 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"
      {"th-TH",          th_TH}
    };
#if (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = P32SublanguageHash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return (struct sublanguage *) 0;
}
#line 550 "F:/posix32/lib/posix32-crt/gperf/sublanguage.gperf"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

bool p32_lookup_sublanguage (SubLangInfo *info) {
  struct sublanguage *sublang = P32SublanguageLookup (info->String, info->Length);

  if (sublang == NULL) {
    return false;
  }

  info->SubLanguage = sublang->Index;

  return true;
}
#endif
