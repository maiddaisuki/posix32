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

#ifndef LIBPOSIX32_STRINGS_H_INCLUDED
#define LIBPOSIX32_STRINGS_H_INCLUDED

#include <string.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * mingw-w64 header files define macros.
 */
#ifdef strcasecmp
#undef strcasecmp
#endif
#ifdef strncasecmp
#undef strncasecmp
#endif

#if P32_REDIRECT
#if P32_WANT_POSIX2001
#define strcasecmp  p32_strcasecmp
#define strncasecmp p32_strncasecmp
#endif

#if P32_WANT_POSIX2008
#define strcasecmp_l  p32_strcasecmp_l
#define strncasecmp_l p32_strncasecmp_l
#endif
#endif

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;
#endif

#if P32_WANT_POSIX2001
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (strcasecmp) (const char *, const char *);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (strncasecmp) (const char *, const char *, size_t);
#endif /* POSIX.1-2001 */

#if P32_WANT_POSIX2008
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (strcasecmp_l) (const char *, const char *, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (strncasecmp_l) (const char *, const char *, size_t, locale_t);
#endif /* POSIX.1-2008 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_STRINGS_H_INCLUDED */
