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

#ifndef LIBPOSIX32_LOCALE_H_INCLUDED
#define LIBPOSIX32_LOCALE_H_INCLUDED

#include <locale.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#if P32_REDIRECT
#define setlocale p32_setlocale

#if P32_WANT_POSIX2008
#define newlocale  p32_newlocale
#define freelocale p32_freelocale
#define duplocale  p32_duplocale
#define uselocale  p32_uselocale
#endif

#if P32_WANT_POSIX2024
#define getlocalename_l p32_getlocalename_l
#endif
#endif

/**
 * CRT's struct lconv is missing some fields added in C99.
 */

#define int_p_cs_precedes  p_cs_precedes
#define int_n_cs_precedes  n_cs_precedes
#define int_p_sep_by_space p_sep_by_space
#define int_n_sep_by_space n_sep_by_space
#define int_p_sign_posn    p_sign_posn
#define int_n_sign_posn    n_sign_posn

#ifdef LC_MESSAGES
#warning "LC_MESSAGES was defined prior to including p32_locale.h"
#undef LC_MESSAGES
#endif
#define LC_MESSAGES 16

P32_CRT_DECL (_CRTIMP) char *__cdecl P32_FUNCNAME_STD (setlocale) (int, const char *);

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;

#define LC_GLOBAL_LOCALE ((locale_t) (-1))

#define LC_COLLATE_MASK  (1 << LC_COLLATE)
#define LC_CTYPE_MASK    (1 << LC_CTYPE)
#define LC_MESSAGES_MASK (1 << LC_MESSAGES)
#define LC_MONETARY_MASK (1 << LC_MONETARY)
#define LC_NUMERIC_MASK  (1 << LC_NUMERIC)
#define LC_TIME_MASK     (1 << LC_TIME)

#define LC_ALL_MASK                                                                                        \
  (LC_COLLATE_MASK | LC_CTYPE_MASK | LC_MESSAGES_MASK | LC_MONETARY_MASK | LC_NUMERIC_MASK | LC_TIME_MASK)

P32_CRT_DECL (P32_DECL) locale_t __cdecl P32_FUNCNAME (newlocale) (int, const char *, locale_t);
P32_CRT_DECL (P32_DECL) void __cdecl     P32_FUNCNAME (freelocale) (locale_t);
P32_CRT_DECL (P32_DECL) locale_t __cdecl P32_FUNCNAME (duplocale) (locale_t);
P32_CRT_DECL (P32_DECL) locale_t __cdecl P32_FUNCNAME (uselocale) (locale_t);
#endif /* POSIX.1-2008 */

#if P32_WANT_POSIX2024
P32_CRT_DECL (P32_DECL) const char *__cdecl P32_FUNCNAME (getlocalename_l) (int, locale_t);
#endif /* POSIX.1-2024 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_LOCALE_H_INCLUDED */
