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

#ifndef LIBPOSIX32_STRING_H_INCLUDED
#define LIBPOSIX32_STRING_H_INCLUDED

#include <string.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#if P32_REDIRECT
#if 0
#define strlen  p32_strlen
#define strnlen p32_strnlen
#define strcpy  p32_strcpy
#define strncpy p32_strncpy
#define strcat  p32_strcat
#define strncat p32_strncat
#define strcmp  p32_strcmp
#define strncmp p32_strncmp
#endif
#define strchr  p32_strchr
#define strrchr p32_strrchr
#define strstr  p32_strstr
#define strpbrk p32_strpbrk
#define strspn  p32_strspn
#define strcspn p32_strcspn
#define strtok  p32_strtok

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

P32_CRT_DECL (_CRTIMP) size_t __cdecl     p32_strlen (const char *);
P32_CRT_DECL (_CRTIMP) size_t __cdecl     p32_strnlen (const char *, size_t);
P32_CRT_DECL (/*blank*/) char *__cdecl    p32_strcpy (char *P32_RESTRICT, const char *P32_RESTRICT);
P32_CRT_DECL (/*blank*/) char *__cdecl    p32_strncpy (char *P32_RESTRICT, const char *P32_RESTRICT, size_t);
P32_CRT_DECL (/*blank*/) char *__cdecl    p32_strcat (char *P32_RESTRICT, const char *P32_RESTRICT);
P32_CRT_DECL (/*blank*/) char *__cdecl    p32_strncat (char *P32_RESTRICT, const char *P32_RESTRICT, size_t);
P32_CRT_DECL (/*blank*/) int __cdecl      p32_strcmp (const char *, const char *);
P32_CRT_DECL (/*blank*/) int __cdecl      p32_strncmp (const char *, const char *, size_t);
P32_CRT_DECL (/*_VCRTIMP*/) char *__cdecl P32_FUNCNAME_STD (strchr) (const char *, int);
P32_CRT_DECL (/*_VCRTIMP*/) char *__cdecl P32_FUNCNAME_STD (strrchr) (const char *, int);
P32_CRT_DECL (/*_VCRTIMP*/) char *__cdecl P32_FUNCNAME_STD (strstr) (const char *, const char *);
P32_CRT_DECL (_CRTIMP) char *__cdecl      P32_FUNCNAME_STD (strpbrk) (const char *, const char *);
P32_CRT_DECL (_CRTIMP) size_t __cdecl     P32_FUNCNAME_STD (strspn) (const char *, const char *);
P32_CRT_DECL (_CRTIMP) size_t __cdecl     P32_FUNCNAME_STD (strcspn) (const char *, const char *);
P32_CRT_DECL (_CRTIMP) char *__cdecl      P32_FUNCNAME_STD (strtok) (char *P32_RESTRICT, const char *P32_RESTRICT);

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
