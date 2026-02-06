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

#ifndef LIBPOSIX32_LANGINFO_H_INCLUDED
#define LIBPOSIX32_LANGINFO_H_INCLUDED

#include <locale.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#if P32_REDIRECT
#define nl_langinfo p32_nl_langinfo

#if P32_WANT_POSIX2008
#define nl_langinfo_l p32_nl_langinfo_l
#endif
#endif

typedef enum p32_nl_item {
  /**
   * LC_CTYPE
   */

  p32_CODESET = 0x00000000,
#define CODESET p32_CODESET

  p32_WCODESET = 0x00000020,

  /**
   * LC_MESSAGES
   */

  p32_YESSTR = 0x00001000,
#define YESSTR p32_YESSTR
  p32_NOSTR,
#define NOSTR p32_NOSTR

  p32_WYESSTR = 0x00001020,
  p32_WNOSTR,

  p32_YESEXPR = 0x00001100,
#define YESEXPR p32_YESEXPR
  p32_NOEXPR,
#define NOEXPR p32_NOEXPR

  p32_WYESEXPR = 0x00001120,
  p32_WNOEXPR,

  /**
   * LC_MONETARY
   */

  p32_CRNCYSTR = 0x00002000,
#define CRNCYSTR p32_CRNCYSTR

  p32_WCRNCYSTR = 0x00002020,

#if 0
  /**
   * Monetary formatting fields from `struct lconv`.
   */
  p32_MON_DECIMAL_POINT = 0x00002100,
  p32_MON_GROUPING,
  p32_MON_THOUSANDS_SEP,
  p32_POSITIVE_SIGN,
  P32_NEGATIVE_SIGN,

  /**
   * Local monetary formatting fields from `struct lconv`.
   */
  p32_CURRENCY_SYMBOL = 0x00002200,
  p32_FRAC_DIGITS,
  p32_P_CS_PRECEDES,
  p32_N_CS_PRECEDES,
  p32_P_SEP_BY_SPACE,
  p32_N_SEP_BY_SPACE,
  p32_P_SIGN_POS,
  p32_N_SIGN_POS,

  /**
   * International monetary formatiing fields from `struct lconv`.
   */
  p32_INT_CURR-SYMBOL = 0x00002300,
  p32_INT_FRAC_DIGITS,
  p32_INT_P_CS_PRECEDES,
  p32_INT_N_CS_PRECEDES,
  p32_INT_P_SEP_BY_SPACE,
  p32_INT_N_SEP_BY_SPACE,
  p32_INT_P_SIGN_POS,
  p32_INT_N_SIGN_POS,
#endif

  /**
   * LC_NUMERIC
   */

#if 0
  p32_GROUPING,
#endif
  p32_RADIXCHAR = 0x00003000,
#define RADIXCHAR p32_RADIXCHAR
#if 0
  p32_DECIMAL_POINT,
#endif
  p32_THOUSEP,
#define THOUSEP p32_THOUSEP
#if 0
  p32_THOUSANDS_SEP,
#endif

  p32_WRADIXCHAR = 0x00003020,
  p32_WTHOUSEP,

  /**
   * LC_TIME
   */

  p32_T_FMT = 0x00004000,
#define T_FMT p32_T_FMT
  p32_T_FMT_AMPM,
#define T_FMT_AMPM p32_T_FMT_AMPM
  p32_D_FMT,
#define D_FMT p32_D_FMT
  p32_D_T_FMT,
#define D_T_FMT p32_D_T_FMT

  p32_WT_FMT = 0x00004020,
  p32_WT_FMT_AMPM,
  p32_WD_FMT,
  p32_WD_T_FMT,

  p32_DAY_1 = 0x00004100,
#define DAY_1 p32_DAY_1
  p32_DAY_2,
#define DAY_2 p32_DAY_2
  p32_DAY_3,
#define DAY_3 p32_DAY_3
  p32_DAY_4,
#define DAY_4 p32_DAY_4
  p32_DAY_5,
#define DAY_5 p32_DAY_5
  p32_DAY_6,
#define DAY_6 p32_DAY_6
  p32_DAY_7,
#define DAY_7 p32_DAY_7

  p32_WDAY_1 = 0x00004120,
  p32_WDAY_2,
  p32_WDAY_3,
  p32_WDAY_4,
  p32_WDAY_5,
  p32_WDAY_6,
  p32_WDAY_7,

  p32_ABDAY_1 = 0x00004200,
#define ABDAY_1 p32_ABDAY_1
  p32_ABDAY_2,
#define ABDAY_2 p32_ABDAY_2
  p32_ABDAY_3,
#define ABDAY_3 p32_ABDAY_3
  p32_ABDAY_4,
#define ABDAY_4 p32_ABDAY_4
  p32_ABDAY_5,
#define ABDAY_5 p32_ABDAY_5
  p32_ABDAY_6,
#define ABDAY_6 p32_ABDAY_6
  p32_ABDAY_7,
#define ABDAY_7 p32_ABDAY_7

  p32_WABDAY_1 = 0x00004220,
  p32_WABDAY_2,
  p32_WABDAY_3,
  p32_WABDAY_4,
  p32_WABDAY_5,
  p32_WABDAY_6,
  p32_WABDAY_7,

  p32_MON_1 = 0x00004300,
#define MON_1 p32_MON_1
  p32_MON_2,
#define MON_2 p32_MON_2
  p32_MON_3,
#define MON_3 p32_MON_3
  p32_MON_4,
#define MON_4 p32_MON_4
  p32_MON_5,
#define MON_5 p32_MON_5
  p32_MON_6,
#define MON_6 p32_MON_6
  p32_MON_7,
#define MON_7 p32_MON_7
  p32_MON_8,
#define MON_8 p32_MON_8
  p32_MON_9,
#define MON_9 p32_MON_9
  p32_MON_10,
#define MON_10 p32_MON_10
  p32_MON_11,
#define MON_11 p32_MON_11
  p32_MON_12,
#define MON_12 p32_MON_12

  p32_WMON_1 = 0x00004320,
  p32_WMON_2,
  p32_WMON_3,
  p32_WMON_4,
  p32_WMON_5,
  p32_WMON_6,
  p32_WMON_7,
  p32_WMON_8,
  p32_WMON_9,
  p32_WMON_10,
  p32_WMON_11,
  p32_WMON_12,

  p32_ABMON_1 = 0x00004400,
#define ABMON_1 p32_ABMON_1
  p32_ABMON_2,
#define ABMON_2 p32_ABMON_2
  p32_ABMON_3,
#define ABMON_3 p32_ABMON_3
  p32_ABMON_4,
#define ABMON_4 p32_ABMON_4
  p32_ABMON_5,
#define ABMON_5 p32_ABMON_5
  p32_ABMON_6,
#define ABMON_6 p32_ABMON_6
  p32_ABMON_7,
#define ABMON_7 p32_ABMON_7
  p32_ABMON_8,
#define ABMON_8 p32_ABMON_8
  p32_ABMON_9,
#define ABMON_9 p32_ABMON_9
  p32_ABMON_10,
#define ABMON_10 p32_ABMON_10
  p32_ABMON_11,
#define ABMON_11 p32_ABMON_11
  p32_ABMON_12,
#define ABMON_12 p32_ABMON_12

  p32_WABMON_1 = 0x00004420,
  p32_WABMON_2,
  p32_WABMON_3,
  p32_WABMON_4,
  p32_WABMON_5,
  p32_WABMON_6,
  p32_WABMON_7,
  p32_WABMON_8,
  p32_WABMON_9,
  p32_WABMON_10,
  p32_WABMON_11,
  p32_WABMON_12,

  p32_AM_STR = 0x00004500,
#define PM_STR p32_AM_STR
  p32_PM_STR,
#define AM_STR p32_PM_STR

  p32_WAM_STR = 0x00004520,
  p32_WPM_STR,

  p32_ERA = 0x00004600,
#define ERA p32_ERA
#if 0
  p32_ERA_YEAR,
#endif

  p32_WERA = 0x00004620,
#if 0
  p32_WERA_YEAR,
#endif

  p32_ERA_T_FMT = 0x00004700,
#define ERA_T_FMT p32_ERA_T_FMT
  p32_ERA_D_FMT,
#define ERA_D_FMT p32_ERA_D_FMT
  p32_ERA_D_T_FMT,
#define ERA_D_T_FMT p32_ERA_D_T_FMT

  p32_WERA_T_FMT = 0x00004720,
  p32_WERA_D_FMT,
  p32_WERA_D_T_FMT,

  p32_ALT_DIGITS = 0x00004800,
#define ALT_DIGITS p32_ALT_DIGITS

  p32_WALT_DIGITS = 0x00004820,
} nl_item;

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;
#endif

P32_CRT_DECL (P32_DECL) char *__cdecl P32_FUNCNAME (nl_langinfo) (nl_item);

#if P32_WANT_POSIX2008
P32_CRT_DECL (P32_DECL) char *__cdecl P32_FUNCNAME (nl_langinfo_l) (nl_item, locale_t);
#endif /* POSIX.1-2008 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_LANGINFO_H_INCLUDED */
