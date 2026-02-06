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

#ifndef LIBPOSIX32_UCHAR_H_INCLUDED
#define LIBPOSIX32_UCHAR_H_INCLUDED

#include <stdint.h>
#include <uchar.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#if P32_REDIRECT
#define mbrtoc8  p32_mbrtoc8
#define c8rtomb  p32_c8rtomb
#define mbrtoc16 p32_mbrtoc16
#define c16rtomb p32_c16rtomb
#define mbrtoc32 p32_mbrtoc32
#define c32rtomb p32_c32rtomb

#if P32_WANT_POSIX2008
#define mbrtoc8_l  p32_mbrtoc8_l
#define c8rtomb_l  p32_c8rtomb_l
#define mbrtoc16_l p32_mbrtoc16_l
#define c16rtomb_l p32_c16rtomb_l
#define mbrtoc32_l p32_mbrtoc32_l
#define c32rtomb_l p32_c32rtomb_l
#endif
#endif

#if !defined(__cplusplus) || __cplusplus < 201811L
typedef uint_least8_t char8_t;
#endif

#ifndef __cplusplus
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
#endif

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;
#endif

P32_CRT_DECL (P32_DECL) size_t
  __cdecl P32_FUNCNAME (mbrtoc8) (char8_t *P32_RESTRICT, const char *P32_RESTRICT, size_t, mbstate_t *P32_RESTRICT);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (c8rtomb) (char *P32_RESTRICT, char8_t, mbstate_t *P32_RESTRICT);
P32_CRT_DECL (_CRTIMP) size_t __cdecl  P32_FUNCNAME_STD (mbrtoc16) (
  char16_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT
);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (c16rtomb) (
  char *P32_RESTRICT,
  char16_t,
  mbstate_t *P32_RESTRICT
);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (mbrtoc32) (
  char32_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT
);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (c32rtomb) (
  char *P32_RESTRICT,
  char32_t,
  mbstate_t *P32_RESTRICT
);

#if P32_WANT_POSIX2008
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbrtoc8_l) (
  char8_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (c8rtomb_l) (
  char *P32_RESTRICT,
  char8_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbrtoc16_l) (
  char16_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (c16rtomb_l) (
  char *P32_RESTRICT,
  char16_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbrtoc32_l) (
  char32_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (c32rtomb_l) (
  char *P32_RESTRICT,
  char32_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
#endif /* POSIX.1-2008 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_UCHAR_H_INCLUDED */
