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

#ifndef LIBPOSIX32_WCHAR_EXT_INCLUDED
#define LIBPOSIX32_WCHAR_EXT_INCLUDED

#include <wchar.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert Wide Character String to Multibyte Character String using
 * current thread's locale (LC_CTYPE).
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_wcstombs (char **address, const wchar_t *wcs);

/**
 * Convert Multibyte Character String to Wide Character String using
 * current thread's locale (LC_CTYPE).
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_mbstowcs (wchar_t **address, const char *mbs);

/**
 * Convert Wide Character String to Multibyte Character String using
 * specified code page.
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_wcstombs_cp (char **address, const wchar_t *wcs, unsigned cp);

/**
 * Convert Multibyte Character String to Wide Character String using
 * specified code page.
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_mbstowcs_cp (wchar_t **address, const char *mbs, unsigned cp);

/**
 * Convert Wide Character String to Multibyte Character String using
 * active ANSI code page.
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_wcstombs_ansi (char **address, const wchar_t *wcs);

/**
 * Convert Multibyte Character String to Wide Character String using
 * active ANSI code page.
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_mbstowcs_ansi (wchar_t **address, const char *mbs);

/**
 * Convert Wide Character String to Multibyte Character String using
 * active OEM code page.
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_wcstombs_oem (char **address, const wchar_t *wcs);

/**
 * Convert Multibyte Character String to Wide Character String using
 * active OEM code page.
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_mbstowcs_oem (wchar_t **address, const char *mbs);

/**
 * Convert Wide Character String to Multibyte Character String using
 * code page used by Windows File APIs.
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_wcstombs_fs (char **address, const wchar_t *wcs);

/**
 * Convert Multibyte Character String to Wide Character String using
 * code page used by Windows File APIs.
 */
P32_CRT_DECL (P32_DECL) int __cdecl p32_ext_mbstowcs_fs (wchar_t **address, const char *mbs);

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_WCHAR_EXT_INCLUDED */
