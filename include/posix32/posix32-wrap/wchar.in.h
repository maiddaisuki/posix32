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

#ifndef LIBPOSIX32_WCHAR_WRAP_INCLUDED
#define LIBPOSIX32_WCHAR_WRAP_INCLUDED

/* TODO: #define _NO_CRT_STDIO_INLINE  */
#ifdef __has_include_next
#if __has_include_next("wchar.h")
#include_next <wchar.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif
/* #undef _NO_CRT_STDIO_INLINE  */

#include "p32_wchar.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE wchar_t *wcsndup (const wchar_t *_W, size_t _N) {
  return p32_wcsndup (_W, _N);
}

P32_ALWAYS_INLINE int wcscoll_l (const wchar_t *_W1, const wchar_t *_W2, locale_t _L) {
  return p32_wcscoll_l (_W1, _W2, _L);
}
P32_ALWAYS_INLINE size_t wcsxfrm_l (wchar_t *_D, const wchar_t *_W, size_t _N, locale_t _L) {
  return p32_wcsxfrm_l (_D, _W, _N, _L);
}

P32_ALWAYS_INLINE int wcscasecmp (const wchar_t *_W1, const wchar_t *_W2) {
  return p32_wcscasecmp (_W1, _W2);
}
P32_ALWAYS_INLINE int wcscasecmp_l (const wchar_t *_W1, const wchar_t *_W2, locale_t _L) {
  return p32_wcscasecmp_l (_W1, _W2, _L);
}
P32_ALWAYS_INLINE int wcsncasecmp (const wchar_t *_W1, const wchar_t *_W2, size_t _N) {
  return p32_wcsncasecmp (_W1, _W2, _N);
}
P32_ALWAYS_INLINE int wcsncasecmp_l (const wchar_t *_W1, const wchar_t *_W2, size_t _N, locale_t _L) {
  return p32_wcsncasecmp_l (_W1, _W2, _N, _L);
}

P32_ALWAYS_INLINE wint_t btowc_l (int _C, locale_t _L) {
  return p32_btowc_l (_C, _L);
}
P32_ALWAYS_INLINE int wctob_l (wint_t _C, locale_t _L) {
  return p32_wctob_l (_C, _L);
}

P32_ALWAYS_INLINE size_t mbrlen_l (const char *_S, size_t _N, mbstate_t *_M, locale_t _L) {
  return p32_mbrlen_l (_S, _N, _M, _L);
}
P32_ALWAYS_INLINE size_t mbrtowc_l (wchar_t *_W, const char *_S, size_t _N, mbstate_t *_M, locale_t _L) {
  return p32_mbrtowc_l (_W, _S, _N, _M, _L);
}
P32_ALWAYS_INLINE size_t mbsrtowcs_l (wchar_t *_W, const char **_S, size_t _N, mbstate_t *_M, locale_t _L) {
  return p32_mbsrtowcs_l (_W, _S, _N, _M, _L);
}
P32_ALWAYS_INLINE size_t wcrtomb_l (char *_S, wchar_t _W, mbstate_t *_M, locale_t _L) {
  return p32_wcrtomb_l (_S, _W, _M, _L);
}
P32_ALWAYS_INLINE size_t wcsrtombs_l (char *_S, const wchar_t **_W, size_t _N, mbstate_t *_M, locale_t _L) {
  return p32_wcsrtombs_l (_S, _W, _N, _M, _L);
}
#endif

#if P32_WANT_SUSv4 || P32_WANT_POSIX2024
P32_ALWAYS_INLINE size_t mbsnrtowcs (wchar_t *_W, const char **_S, size_t _N, size_t _C, mbstate_t *_M) {
  return p32_mbsnrtowcs (_W, _S, _N, _C, _M);
}
P32_ALWAYS_INLINE size_t mbsnrtowcs_l (wchar_t *_W, const char **_S, size_t _N, size_t _C, mbstate_t *_M, locale_t _L) {
  return p32_mbsnrtowcs_l (_W, _S, _N, _C, _M, _L);
}
P32_ALWAYS_INLINE size_t wcsnrtombs (char *_S, const wchar_t **_W, size_t _N, size_t _C, mbstate_t *_M) {
  return p32_wcsnrtombs (_S, _W, _N, _C, _M);
}
P32_ALWAYS_INLINE size_t wcsnrtombs_l (char *_S, const wchar_t **_W, size_t _N, size_t _C, mbstate_t *_M, locale_t _L) {
  return p32_wcsnrtombs_l (_S, _W, _N, _C, _M, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2008
using posix32::btowc_l;
using posix32::mbrlen_l;
using posix32::mbrtowc_l;
using posix32::mbsrtowcs_l;
using posix32::wcrtomb_l;
using posix32::wcscasecmp;
using posix32::wcscasecmp_l;
using posix32::wcscoll_l;
using posix32::wcsncasecmp;
using posix32::wcsncasecmp_l;
using posix32::wcsndup;
using posix32::wcsrtombs_l;
using posix32::wcsxfrm_l;
using posix32::wctob_l;
#endif

#if P32_WANT_SUSv4 || P32_WANT_POSIX2024
using posix32::mbsnrtowcs;
using posix32::mbsnrtowcs_l;
using posix32::wcsnrtombs;
using posix32::wcsnrtombs_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_WCHAR_WRAP_INCLUDED */
