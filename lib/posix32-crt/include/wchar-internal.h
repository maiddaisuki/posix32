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

#ifndef LIBPOSIX32_WCHAR_INTERNAL_H_INCLUDED
#define LIBPOSIX32_WCHAR_INTERNAL_H_INCLUDED

#include "p32_stdlib.h"
#include "p32_wchar.h"

#include "locale-internal.h"

/**
 * Internal versions which use `locale_t` objects.
 *
 * Code inside the library should use these versions instead of public ones.
 */
size_t p32_private_mbrlen_l (const char *, size_t, mbstate_t *, locale_t);
size_t p32_private_mbrtowc_l (wchar_t *, const char *, size_t, mbstate_t *, locale_t);
size_t p32_private_mbsrtowcs_l (wchar_t *, const char **, size_t, mbstate_t *, locale_t);
size_t p32_private_mbsnrtowcs_l (wchar_t *, const char **, size_t, size_t, mbstate_t *, locale_t);
size_t p32_private_wcrtomb_l (char *, wchar_t, mbstate_t *, locale_t);
size_t p32_private_wcsrtombs_l (char *, const wchar_t **, size_t, mbstate_t *, locale_t);
size_t p32_private_wcsnrtombs_l (char *, const wchar_t **, size_t, size_t, mbstate_t *, locale_t);

/**
 * The versions below use `Charset` structure instead of `locale_t` objects,
 * and can be used in context where `locale_t` can not.
 *
 * The `Charset` structure passed to these functions must contain information
 * obtained by calling `p32_charset_info`.
 */

/**
 * Internal versions which operate on ASCII (code page 20127).
 */
P32_TEST_DECL size_t p32_private_mbrlen_cp20127 (const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtowc_cp20127 (wchar_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsrtowcs_cp20127 (wchar_t *, const char **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsnrtowcs_cp20127 (wchar_t *, const char **, size_t, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcrtomb_cp20127 (char *, wchar_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsrtombs_cp20127 (char *, const wchar_t **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsnrtombs_cp20127 (char *, const wchar_t **, size_t, size_t, mbstate_t *, Charset *);

/**
 * Internal versions which operate on ISO-8859-1 (code page 28591).
 */
P32_TEST_DECL size_t p32_private_mbrlen_posix (const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtowc_posix (wchar_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsrtowcs_posix (wchar_t *, const char **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsnrtowcs_posix (wchar_t *, const char **, size_t, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcrtomb_posix (char *, wchar_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsrtombs_posix (char *, const wchar_t **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsnrtombs_posix (char *, const wchar_t **, size_t, size_t, mbstate_t *, Charset *);

/**
 * Internal versions which operate on SBCS code pages.
 */
P32_TEST_DECL size_t p32_private_mbrlen_sbcs (const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtowc_sbcs (wchar_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsrtowcs_sbcs (wchar_t *, const char **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsnrtowcs_sbcs (wchar_t *, const char **, size_t, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcrtomb_sbcs (char *, wchar_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsrtombs_sbcs (char *, const wchar_t **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsnrtombs_sbcs (char *, const wchar_t **, size_t, size_t, mbstate_t *, Charset *);

/**
 * Internal versions which operate on DBCS code pages.
 */
P32_TEST_DECL size_t p32_private_mbrlen_dbcs (const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtowc_dbcs (wchar_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsrtowcs_dbcs (wchar_t *, const char **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsnrtowcs_dbcs (wchar_t *, const char **, size_t, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcrtomb_dbcs (char *, wchar_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsrtombs_dbcs (char *, const wchar_t **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsnrtombs_dbcs (char *, const wchar_t **, size_t, size_t, mbstate_t *, Charset *);

/**
 * Internal versions which operate on UTF-8 (code page 65001).
 */
P32_TEST_DECL size_t p32_private_mbrlen_utf8 (const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtowc_utf8 (wchar_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsrtowcs_utf8 (wchar_t *, const char **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbsnrtowcs_utf8 (wchar_t *, const char **, size_t, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcrtomb_utf8 (char *, wchar_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsrtombs_utf8 (char *, const wchar_t **, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_wcsnrtombs_utf8 (char *, const wchar_t **, size_t, size_t, mbstate_t *, Charset *);

/**
 * Store locale-specific versions of wide character string functions declared
 * in wchar.h in `locale->Functions`.
 */
void p32_wchar_functions (locale_t locale);

/**
 * Store locale-specific versions of C89 conversion functions declared in
 * stdlib.h in `locale->Functions`.
 */
void p32_wchar_c89_functions (locale_t locale);

/**
 * Store locale-specific versions of C95 conversion functions declared in
 * wchar.h in `locale->Functions`.
 */
void p32_wchar_c95_functions (locale_t locale);

#endif /* LIBPOSIX32_WCHAR_INTERNAL_H_INCLUDED */
