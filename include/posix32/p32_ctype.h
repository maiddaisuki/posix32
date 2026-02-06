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

#ifndef LIBPOSIX32_CTYPE_H_INCLUDED
#define LIBPOSIX32_CTYPE_H_INCLUDED

#include <ctype.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef isalnum
#undef isalpha
#undef isblank
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef tolower
#undef toupper
#undef isascii
#undef toascii

#if P32_REDIRECT
#define isalnum  p32_isalnum
#define isalpha  p32_isalpha
#define isblank  p32_isblank
#define iscntrl  p32_iscntrl
#define isdigit  p32_isdigit
#define isgraph  p32_isgraph
#define islower  p32_islower
#define isprint  p32_isprint
#define ispunct  p32_ispunct
#define isspace  p32_isspace
#define isupper  p32_isupper
#define isxdigit p32_isxdigit

#define tolower p32_tolower
#define toupper p32_toupper

#define isascii p32_isascii
#define toascii p32_toascii

#if P32_WANT_POSIX2008
#define isalnum_l  p32_isalnum_l
#define isalpha_l  p32_isalpha_l
#define isblank_l  p32_isblank_l
#define iscntrl_l  p32_iscntrl_l
#define isdigit_l  p32_isdigit_l
#define isgraph_l  p32_isgraph_l
#define islower_l  p32_islower_l
#define isprint_l  p32_isprint_l
#define ispunct_l  p32_ispunct_l
#define isspace_l  p32_isspace_l
#define isupper_l  p32_isupper_l
#define isxdigit_l p32_isxdigit_l

#define tolower_l p32_tolower_l
#define toupper_l p32_toupper_l
#endif
#endif

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;
#endif

P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isalnum) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isalpha) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isblank) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iscntrl) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isdigit) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isgraph) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (islower) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isprint) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (ispunct) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isspace) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isupper) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (isxdigit) (int);

P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (tolower) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (toupper) (int);

P32_CRT_DECL (P32_DECL) P32_DEPRECATE_POSIX2008 (isascii) int __cdecl P32_FUNCNAME (isascii) (int);
P32_CRT_DECL (P32_DECL) P32_DEPRECATE_POSIX2008 (toascii) int __cdecl P32_FUNCNAME (toascii) (int);

#ifdef __isascii
#undef isascii
#define isascii __isascii
#endif

#ifdef __toascii
#undef toascii
#define toascii __toascii
#endif

#if P32_WANT_POSIX2008
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isalnum_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isalpha_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isblank_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iscntrl_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isdigit_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isgraph_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (islower_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isprint_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (ispunct_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isspace_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isupper_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (isxdigit_l) (int, locale_t);

P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (tolower_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (toupper_l) (int, locale_t);
#endif /* POSIX.1-2008 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_CTYPE_H_INCLUDED */
