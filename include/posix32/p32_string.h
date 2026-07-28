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

#ifndef LIBPOSIX32_STRING_H_INCLUDED
#define LIBPOSIX32_STRING_H_INCLUDED

#include <string.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#if P32_REDIRECT
#define strcoll p32_strcoll
#define strxfrm p32_strxfrm

#if P32_WANT_POSIX
#define strtok_r p32_strtok_r
#endif

#if P32_WANT_SUSv1 || P32_WANT_POSIX2001
#define strdup  p32_strdup
#define strndup p32_strndup
#endif

#if P32_WANT_POSIX2008
#define strcoll_l p32_strcoll_l
#define strxfrm_l p32_strxfrm_l
#endif
#endif

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;
#endif

P32_CRT_DECL (_CRTIMP) int __cdecl    P32_FUNCNAME_STD (strcoll) (const char *, const char *);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (strxfrm) (char *P32_RESTRICT, const char *P32_RESTRICT, size_t);

#if P32_WANT_POSIX
P32_CRT_DECL (P32_DECL) char
  *__cdecl P32_FUNCNAME (strtok_r) (char *P32_RESTRICT, const char *P32_RESTRICT, char **P32_RESTRICT);
#endif /* POSIX.1 */

#if P32_WANT_SUSv1 || P32_WANT_POSIX2001
P32_CRT_DECL (_CRTIMP) char *__cdecl  P32_FUNCNAME_STD (strdup) (const char *);
P32_CRT_DECL (P32_DECL) char *__cdecl P32_FUNCNAME (strndup) (const char *, size_t);
#endif /* SUSv1 || POSIX.1-2001 */

#if P32_WANT_POSIX2008
P32_CRT_DECL (P32_DECL) int __cdecl    P32_FUNCNAME (strcoll_l) (const char *, const char *, locale_t);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (strxfrm_l) (
  char *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  locale_t
);
#endif /* POSIX.1-2008 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_STRING_H_INCLUDED */
