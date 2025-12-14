/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: /usr/bin/gperf --output-file=F:/posix32/lib/posix32-crt/gperf/country_codes.c F:/posix32/lib/posix32-crt/gperf/country_codes.gperf  */
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

#line 1 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"

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
#line 44 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
struct country { const char *name; CountryIndex Index; };

#define TOTAL_KEYWORDS 496
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 3
#define MIN_HASH_VALUE 12
#define MAX_HASH_VALUE 1863
/* maximum key range = 1852, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
P32CountryCodeHash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,    5,
         0,    0, 1864, 1864,    5, 1864, 1864, 1864, 1864, 1864,
         0,    0, 1864, 1864, 1864, 1864,  271,   67,  373,  622,
       475,  153,    3,  105,  378,  273,  498,    0,   15,   95,
       235,  136,  434,    5,  505,  110,  123,  400,  230,   43,
       335,   10,   35,  215,  480,  413,  233,   55,  283,   63,
       490,   13,  246,  100,  193,  120,  360,   40,  435,   80,
       290,  882,  441,  175,    5,   60,   50, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864, 1864,
      1864, 1864
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[2]+12];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]+1];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]+26];
        break;
    }
  return hval;
}

struct country *
P32CountryCodeLookup (register const char *str, register size_t len)
{
  static struct country wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 60 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AL",  AL},
#line 77 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ALA", AX},
      {""},
#line 56 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AG",  AG},
      {""},
#line 66 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AR",  AR},
#line 274 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KG",  KG},
      {""},
#line 286 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KR",  KR},
      {""},
#line 78 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AZ",  AZ},
#line 67 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ARG", AR},
      {""},
#line 292 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KZ",  KZ},
#line 65 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AGO", AO},
#line 62 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AM",  AM},
#line 61 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ALB", AL},
      {""},
#line 280 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KM",  KM},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 98 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BL",  BL},
      {""}, {""},
#line 90 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BG",  BG},
      {""},
#line 108 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BR",  BR},
#line 109 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BRA", BR},
      {""}, {""}, {""},
#line 118 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BZ",  BZ},
      {""}, {""}, {""}, {""},
#line 100 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BM",  BM},
      {""}, {""},
#line 76 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AX",  AX},
      {""},
#line 213 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GL",  GL},
#line 83 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BRB", BB},
      {""},
#line 207 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GG",  GG},
#line 63 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ARM", AM},
#line 223 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GR",  GR},
      {""}, {""},
#line 247 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IL",  IL},
      {""},
#line 538 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ZM",  ZM},
      {""}, {""},
#line 257 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IR",  IR},
      {""},
#line 215 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GM",  GM},
      {""},
#line 208 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GGY", GG},
      {""}, {""}, {""}, {""}, {""},
#line 249 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IM",  IM},
#line 99 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BLM", BL},
#line 448 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SL",  SL},
#line 539 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ZMB", ZM},
      {""},
#line 438 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SG",  SG},
      {""},
#line 456 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SR",  SR},
#line 216 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GMB", GM},
      {""}, {""}, {""},
#line 468 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SZ",  SZ},
#line 220 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GLP", GP},
      {""}, {""}, {""},
#line 450 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SM",  SM},
#line 431 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SLB", SB},
      {""}, {""}, {""},
#line 332 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ML",  ML},
#line 423 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SRB", RS},
#line 82 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BB",  BB},
#line 326 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MG",  MG},
      {""},
#line 344 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MR",  MR},
#line 101 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BMU", BM},
      {""},
#line 282 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KN",  KN},
#line 283 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KNA", KN},
#line 360 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MZ",  MZ},
      {""}, {""}, {""}, {""},
#line 334 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MM",  MM},
      {""}, {""},
#line 276 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KH",  KH},
#line 439 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SGP", SG},
#line 70 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AT",  AT},
      {""},
#line 198 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GB",  GB},
#line 464 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SX",  SX},
      {""}, {""},
#line 57 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ATG", AG},
      {""}, {""}, {""},
#line 102 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BN",  BN},
      {""}, {""},
#line 72 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AU",  AU},
      {""},
#line 386 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"OM",  OM},
      {""}, {""}, {""}, {""},
#line 92 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BH",  BH},
#line 75 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ABW", AW},
      {""},
#line 356 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MX",  MX},
      {""},
#line 112 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BT",  BT},
      {""},
#line 430 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SB",  SB},
      {""},
#line 284 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KP",  KP},
#line 217 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GN",  GN},
      {""},
#line 53 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ARE", AE},
      {""}, {""}, {""},
#line 224 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GRC", GR},
#line 79 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AZE", AZ},
#line 251 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IN",  IN},
      {""},
#line 209 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GH",  GH},
#line 210 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GHA", GH},
#line 277 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KHM", KH},
#line 54 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AF",  AF},
      {""},
#line 225 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GT",  GT},
#line 230 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GNB", GW},
#line 465 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SXM", SX},
      {""},
#line 55 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AFG", AF},
#line 534 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"YT",  YT},
      {""}, {""},
#line 261 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IT",  IT},
#line 262 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ITA", IT},
#line 452 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SN",  SN},
      {""}, {""},
#line 227 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GU",  GU},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 440 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SH",  SH},
      {""}, {""},
#line 88 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BF",  BF},
#line 89 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BFA", BF},
#line 460 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ST",  ST},
#line 219 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GP",  GP},
#line 232 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GUY", GY},
#line 374 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NL",  NL},
      {""},
#line 336 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MN",  MN},
#line 370 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NG",  NG},
#line 371 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NGA", NG},
#line 380 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NR",  NR},
      {""}, {""},
#line 337 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MNG", MN},
      {""},
#line 384 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NZ",  NZ},
      {""},
#line 328 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MH",  MH},
      {""}, {""},
#line 205 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GF",  GF},
#line 226 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GTM", GT},
#line 348 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MT",  MT},
#line 333 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MLI", ML},
      {""}, {""}, {""},
#line 136 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CL",  CL},
      {""},
#line 449 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SLE", SL},
#line 128 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CG",  CG},
      {""},
#line 144 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CR",  CR},
      {""},
#line 228 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GUM", GU},
#line 350 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MU",  MU},
      {""},
#line 158 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CZ",  CZ},
#line 461 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"STP", ST},
      {""}, {""}, {""},
#line 138 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CM",  CM},
#line 341 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MNP", MP},
      {""}, {""}, {""}, {""},
#line 340 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MP",  MP},
      {""},
#line 194 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FR",  FR},
#line 195 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FRA", FR},
#line 74 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AW",  AW},
      {""}, {""},
#line 288 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KW",  KW},
      {""},
#line 64 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AO",  AO},
      {""}, {""},
#line 190 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FM",  FM},
#line 193 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FRO", FO},
      {""},
#line 304 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LR",  LR},
      {""},
#line 324 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MF",  MF},
#line 381 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NRU", NR},
      {""}, {""}, {""},
#line 154 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CX",  CX},
      {""},
#line 403 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SPM", PM},
      {""}, {""},
#line 275 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KGZ", KG},
      {""},
#line 114 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BW",  BW},
#line 115 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BWA", BW},
      {""}, {""}, {""},
#line 104 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BO",  BO},
      {""}, {""}, {""},
#line 85 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BGD", BD},
      {""}, {""}, {""}, {""},
#line 48 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"150", R_150},
#line 540 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ZW",  ZW},
      {""},
#line 119 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BLZ", BZ},
      {""},
#line 256 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IRQ", IQ},
#line 229 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GW",  GW},
      {""}, {""},
#line 237 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HR",  HR},
      {""},
#line 482 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TL",  TL},
#line 214 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GRL", GL},
      {""},
#line 474 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TG",  TG},
      {""},
#line 490 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TR",  TR},
#line 202 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GRD", GD},
      {""},
#line 253 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IO",  IO},
#line 246 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IRL", IE},
#line 498 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TZ",  TZ},
#line 499 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TZA", TZ},
      {""}, {""},
#line 475 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TGO", TG},
#line 484 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TM",  TM},
#line 80 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BA",  BA},
      {""},
#line 96 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BJ",  BJ},
      {""},
#line 140 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CN",  CN},
#line 416 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"QA",  QA},
      {""},
#line 296 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LB",  LB},
      {""},
#line 454 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SO",  SO},
#line 382 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NU",  NU},
      {""}, {""}, {""},
#line 130 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CH",  CH},
#line 536 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ZA",  ZA},
      {""}, {""}, {""}, {""},
#line 196 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GA",  GA},
#line 315 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LBY", LY},
#line 526 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"WF",  WF},
#line 378 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NP",  NP},
#line 354 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MW",  MW},
#line 145 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CRI", CR},
#line 323 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MNE", ME},
      {""},
#line 349 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MLT", MT},
#line 338 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MO",  MO},
      {""}, {""},
#line 148 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CU",  CU},
#line 345 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MRT", MR},
      {""},
#line 51 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AND", AD},
#line 197 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GAB", GA},
      {""}, {""}, {""},
#line 368 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NF",  NF},
      {""},
#line 290 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KY",  KY},
      {""}, {""},
#line 428 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SA",  SA},
      {""},
#line 444 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SJ",  SJ},
#line 149 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CUB", CU},
      {""},
#line 308 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LT",  LT},
      {""}, {""},
#line 455 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SOM", SO},
#line 400 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PL",  PL},
      {""},
#line 159 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CZE", CZ},
#line 394 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PG",  PG},
#line 463 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SLV", SV},
#line 406 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PR",  PR},
#line 222 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GNQ", GQ},
#line 71 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AUT", AT},
#line 126 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CF",  CF},
#line 310 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LU",  LU},
#line 116 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BY",  BY},
#line 316 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MA",  MA},
      {""}, {""}, {""},
#line 402 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PM",  PM},
#line 103 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BRN", BN},
      {""},
#line 235 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HN",  HN},
#line 415 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PRY", PY},
      {""}, {""}, {""}, {""},
#line 46 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"001", R_001},
#line 486 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TN",  TN},
      {""}, {""},
#line 58 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AI",  AI},
#line 59 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AIA", AI},
#line 231 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GY",  GY},
#line 278 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KI",  KI},
      {""},
#line 239 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HT",  HT},
#line 252 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IND", IN},
#line 476 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TH",  TH},
#line 477 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"THA", TH},
#line 445 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SJM", SJ},
      {""}, {""},
#line 492 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TT",  TT},
      {""},
#line 153 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CUW", CW},
      {""},
#line 258 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IRN", IR},
      {""},
#line 241 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HU",  HU},
#line 429 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SAU", SA},
      {""}, {""}, {""},
#line 493 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TTO", TT},
#line 309 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LTU", LT},
#line 94 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BI",  BI},
#line 250 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IMN", IM},
#line 466 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SY",  SY},
#line 176 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"EG",  EG},
#line 541 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ZWE", ZW},
#line 178 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ER",  ER},
      {""}, {""}, {""}, {""}, {""},
#line 413 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PLW", PW},
      {""},
#line 343 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MTQ", MQ},
      {""},
#line 376 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NO",  NO},
#line 375 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NLD", NL},
#line 177 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"EGY", EG},
      {""}, {""},
#line 211 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GI",  GI},
#line 385 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NZL", NZ},
#line 358 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MY",  MY},
#line 329 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MHL", MH},
      {""}, {""}, {""}, {""},
#line 355 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MWI", MW},
      {""}, {""},
#line 518 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VG",  VG},
#line 152 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CW",  CW},
      {""},
#line 437 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SWE", SE},
      {""},
#line 212 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GIB", GI},
#line 142 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CO",  CO},
      {""}, {""},
#line 432 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SC",  SC},
      {""},
#line 404 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PN",  PN},
#line 129 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"COG", CG},
#line 131 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CHE", CH},
#line 442 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SI",  SI},
#line 147 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SCG", CS},
#line 519 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VGB", VG},
#line 395 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PNG", PG},
      {""},
#line 263 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IV",  IV},
#line 362 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NA",  NA},
#line 396 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PH",  PH},
      {""},
#line 339 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MAC", MO},
#line 192 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FO",  FO},
#line 106 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BQ",  BQ},
#line 410 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PT",  PT},
#line 387 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"OMN", OM},
      {""},
#line 318 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MC",  MC},
      {""}, {""},
#line 407 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PRI", PR},
#line 289 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KWT", KW},
      {""}, {""},
#line 462 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SV",  SV},
#line 113 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BTN", BT},
      {""}, {""},
#line 319 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MCO", MC},
#line 52 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AE",  AE},
#line 120 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CA",  CA},
      {""},
#line 272 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KE",  KE},
#line 221 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GQ",  GQ},
#line 170 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DZ",  DZ},
#line 171 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DZA", DZ},
      {""}, {""},
#line 281 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"COM", KM},
#line 166 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DM",  DM},
#line 167 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DMA", DM},
#line 255 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IQ",  IQ},
      {""}, {""},
#line 352 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MV",  MV},
#line 105 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BOL", BO},
      {""},
#line 530 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"XK",  XK},
#line 240 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HTI", HT},
#line 266 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"JM",  JM},
#line 186 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FJ",  FJ},
      {""},
#line 363 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NAM", NA},
      {""},
#line 86 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BE",  BE},
#line 433 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SYC", SC},
      {""},
#line 392 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PF",  PF},
      {""},
#line 68 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AS",  AS},
#line 117 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BLR", BY},
#line 294 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LA",  LA},
      {""},
#line 91 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BGR", BG},
#line 496 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TW",  TW},
#line 441 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SHN", SH},
      {""},
#line 182 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ET",  ET},
      {""},
#line 488 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TO",  TO},
      {""}, {""},
#line 295 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LAO", LA},
#line 179 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ERI", ER},
#line 203 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GE",  GE},
#line 293 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KAZ", KZ},
#line 254 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IOT", IO},
      {""},
#line 342 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MQ",  MQ},
#line 532 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"YE",  YE},
#line 522 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VN",  VN},
      {""},
#line 245 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IE",  IE},
      {""},
#line 110 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BS",  BS},
#line 204 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GEO", GE},
      {""}, {""}, {""},
#line 417 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"QAT", QA},
      {""},
#line 73 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AUS", AU},
      {""}, {""},
#line 156 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CY",  CY},
#line 137 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CHL", CL},
      {""}, {""},
#line 111 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BHS", BS},
#line 436 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SE",  SE},
      {""},
#line 469 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SWZ", SZ},
#line 424 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"RU",  RU},
#line 69 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ASM", AS},
#line 379 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NPL", NP},
      {""}, {""},
#line 478 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TJ",  TJ},
#line 524 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VU",  VU},
      {""},
#line 364 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NC",  NC},
      {""},
#line 259 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IS",  IS},
      {""}, {""},
#line 372 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NI",  NI},
#line 238 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HRV", HR},
      {""}, {""},
#line 322 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ME",  ME},
#line 451 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SMR", SM},
      {""},
#line 446 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SK",  SK},
#line 533 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"YEM", YE},
#line 523 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VNM", VN},
#line 314 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LY",  LY},
#line 361 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MOZ", MZ},
      {""}, {""},
#line 458 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SS",  SS},
#line 157 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CYP", CY},
      {""},
#line 122 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CC",  CC},
      {""},
#line 412 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PW",  PW},
      {""}, {""},
#line 132 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CI",  CI},
#line 291 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CYM", KY},
      {""},
#line 335 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MMR", MM},
      {""},
#line 330 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MK",  MK},
#line 411 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PRT", PT},
      {""}, {""}, {""},
#line 199 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GBR", GB},
      {""},
#line 346 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MS",  MS},
      {""}, {""}, {""}, {""},
#line 527 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"WLF", WF},
#line 184 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FI",  FI},
      {""},
#line 206 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GUF", GF},
#line 236 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HND", HN},
#line 150 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CV",  CV},
      {""},
#line 187 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FJI", FJ},
      {""},
#line 298 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LC",  LC},
#line 299 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LCA", LC},
#line 93 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BHR", BH},
      {""}, {""},
#line 300 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LI",  LI},
      {""},
#line 270 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"JP",  JP},
#line 383 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NIU", NU},
      {""}, {""}, {""},
#line 388 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PA",  PA},
#line 50 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"AD",  AD},
      {""},
#line 189 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FLK", FK},
#line 151 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CPV", CV},
      {""},
#line 351 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MUS", MU},
#line 49 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"419", R_419},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 312 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LV",  LV},
#line 313 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LVA", LV},
      {""},
#line 297 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LBN", LB},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 141 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CHN", CN},
#line 84 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BD",  BD},
      {""}, {""}, {""}, {""}, {""},
#line 470 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TC",  TC},
#line 471 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TCA", TC},
#line 426 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"RW",  RW},
#line 427 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"RWA", RW},
#line 373 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NIC", NI},
#line 366 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NE",  NE},
#line 535 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MYT", YT},
#line 420 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"RO",  RO},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 201 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GD",  GD},
      {""}, {""},
#line 528 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"WS",  WS},
#line 143 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"COL", CO},
      {""}, {""},
#line 457 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SUR", SR},
#line 243 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ID",  ID},
#line 125 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"COD", CD},
      {""}, {""}, {""},
#line 494 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TV",  TV},
      {""}, {""}, {""}, {""},
#line 414 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PY",  PY},
#line 397 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PHL", PH},
#line 495 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TUV", TV},
      {""}, {""}, {""}, {""},
#line 434 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SD",  SD},
      {""},
#line 483 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TLS", TL},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 139 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CMR", CM},
#line 512 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VA",  VA},
#line 134 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CK",  CK},
      {""},
#line 168 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DO",  DO},
      {""}, {""}, {""}, {""},
#line 146 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CS",  CS},
      {""},
#line 320 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MD",  MD},
#line 321 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MDA", MD},
#line 529 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"WSM", WS},
#line 268 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"JO",  JO},
#line 421 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ROU", RO},
      {""},
#line 327 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MDG", MG},
#line 287 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KOR", KR},
      {""},
#line 188 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FK",  FK},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 155 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CXR", CX},
      {""},
#line 87 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BEL", BE},
#line 242 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HUN", HU},
      {""},
#line 302 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LK",  LK},
#line 303 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LKA", LK},
      {""},
#line 369 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NFK", NF},
      {""},
#line 487 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TUN", TN},
      {""},
#line 306 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LS",  LS},
      {""},
#line 162 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DJ",  DJ},
      {""}, {""},
#line 537 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ZAF", ZA},
      {""}, {""},
#line 169 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DOM", DO},
      {""},
#line 301 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LIE", LI},
#line 307 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LSO", LS},
      {""}, {""}, {""},
#line 285 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PRK", KP},
      {""},
#line 95 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BDI", BI},
#line 218 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"GIN", GN},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 233 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HK",  HK},
#line 191 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FSM", FM},
      {""}, {""}, {""},
#line 172 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"EC",  EC},
#line 234 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"HKG", HK},
#line 480 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TK",  TK},
      {""}, {""}, {""}, {""}, {""},
#line 305 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LBR", LR},
      {""}, {""},
#line 365 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NCL", NC},
#line 525 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VUT", VU},
#line 260 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ISL", IS},
      {""}, {""}, {""}, {""}, {""},
#line 267 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"JAM", JM},
#line 325 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MAF", MF},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 514 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VC",  VC},
      {""},
#line 443 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SVN", SI},
      {""}, {""},
#line 520 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VI",  VI},
      {""},
#line 459 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SSD", SS},
#line 121 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CAN", CA},
      {""},
#line 273 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KEN", KE},
      {""},
#line 401 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"POL", PL},
      {""}, {""}, {""}, {""}, {""},
#line 485 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TKM", TM},
      {""},
#line 331 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MKD", MK},
#line 390 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PE",  PE},
      {""},
#line 124 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CD",  CD},
      {""}, {""}, {""}, {""},
#line 173 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ECU", EC},
      {""}, {""}, {""},
#line 97 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BEN", BJ},
      {""}, {""},
#line 359 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MYS", MY},
      {""},
#line 135 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"COK", CK},
#line 317 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MAR", MA},
      {""}, {""}, {""},
#line 497 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TWN", TW},
      {""},
#line 398 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PK",  PK},
      {""},
#line 311 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"LUX", LU},
#line 489 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TON", TO},
      {""}, {""},
#line 163 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DJI", DJ},
#line 408 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PS",  PS},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 279 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"KIR", KI},
      {""}, {""}, {""}, {""},
#line 133 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CIV", CI},
      {""}, {""}, {""},
#line 447 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SVK", SK},
      {""}, {""}, {""},
#line 502 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UG",  UG},
#line 503 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UGA", UG},
      {""},
#line 174 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"EE",  EE},
      {""}, {""},
#line 453 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SEN", SN},
#line 510 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UZ",  UZ},
      {""},
#line 491 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TUR", TR},
      {""},
#line 467 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SYR", SY},
#line 504 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UM",  UM},
      {""},
#line 473 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TCD", TD},
      {""},
#line 509 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"URY", UY},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 511 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UZB", UZ},
#line 377 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NOR", NO},
#line 418 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"RE",  RE},
      {""},
#line 472 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TD",  TD},
      {""}, {""}, {""},
#line 516 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VE",  VE},
#line 531 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"XKS", XK},
#line 180 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ES",  ES},
#line 81 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BIH", BA},
      {""},
#line 127 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CAF", CF},
      {""}, {""},
#line 107 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"BES", BQ},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 422 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"RS",  RS},
      {""}, {""}, {""}, {""}, {""},
#line 513 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VAT", VA},
#line 185 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"FIN", FI},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 181 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ESP", ES},
#line 160 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DE",  DE},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 271 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"JPN", JP},
      {""},
#line 479 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TJK", TJ},
#line 264 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"JE",  JE},
#line 419 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"REU", RE},
#line 389 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PAN", PA},
      {""}, {""}, {""}, {""},
#line 425 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"RUS", RU},
      {""}, {""}, {""},
#line 165 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DNK", DK},
      {""},
#line 164 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DK",  DK},
#line 265 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"JEY", JE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 123 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"CCK", CC},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 47 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"029", R_029},
#line 409 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PSE", PS},
      {""}, {""}, {""},
#line 353 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MDV", MV},
      {""},
#line 505 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UMI", UM},
      {""}, {""},
#line 161 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"DEU", DE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 481 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"TKL", TK},
      {""},
#line 244 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"IDN", ID},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 183 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ETH", ET},
      {""}, {""},
#line 399 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PAK", PK},
      {""}, {""}, {""}, {""}, {""},
#line 435 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"SDN", SD},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 515 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VCT", VC},
#line 248 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"ISR", IL},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 357 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MEX", MX},
      {""}, {""},
#line 405 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PCN", PN},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 347 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"MSR", MS},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 393 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PYF", PF},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 367 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"NER", NE},
      {""}, {""},
#line 175 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"EST", EE},
#line 500 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UA",  UA},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 269 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"JOR", JO},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 508 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UY",  UY},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 517 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VEN", VE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 521 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"VIR", VI},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 391 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"PER", PE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 200 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UK",  GB},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 506 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"US",  US},
#line 507 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"USA", US},
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
      {""}, {""}, {""}, {""},
#line 501 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"
      {"UKR", UA}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = P32CountryCodeHash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 542 "F:/posix32/lib/posix32-crt/gperf/country_codes.gperf"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

bool p32_lookup_country_code (CountryInfo *info) {
  struct country *cc = P32CountryCodeLookup (info->String, info->Length);

  if (cc == NULL) {
    return false;
  }

  info->Country = cc->Index;

  return true;
}
