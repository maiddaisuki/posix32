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

#ifndef LIBPOSIX32_UCHAR_INTERNAL_H_INCLUDED
#define LIBPOSIX32_UCHAR_INTERNAL_H_INCLUDED

#include "p32_uchar.h"
#include "p32_wchar.h"

#include "locale-internal.h"

/**
 * Internal versions which operate on ASCII (code page 20127).
 */
P32_TEST_DECL size_t p32_private_mbrtoc8_cp20127 (char8_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c8rtomb_cp20127 (char *, char8_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc16_cp20127 (char16_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c16rtomb_cp20127 (char *, char16_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc32_cp20127 (char32_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c32rtomb_cp20127 (char *, char32_t, mbstate_t *, Charset *);

/**
 * Internal versions which operate on ISO-8859-1 (code page 28591).
 */
P32_TEST_DECL size_t p32_private_mbrtoc8_cp28591 (char8_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c8rtomb_cp28591 (char *, char8_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc16_cp28591 (char16_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c16rtomb_cp28591 (char *, char16_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc32_cp28591 (char32_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c32rtomb_cp28591 (char *, char32_t, mbstate_t *, Charset *);

/**
 * Internal versions which operate on UTF-8 (code page 65001).
 */
P32_TEST_DECL size_t p32_private_mbrtoc8_cp65001 (char8_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c8rtomb_cp65001 (char *, char8_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc16_cp65001 (char16_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c16rtomb_cp65001 (char *, char16_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc32_cp65001 (char32_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c32rtomb_cp65001 (char *, char32_t, mbstate_t *, Charset *);

/**
 * Internal versions which operate on SBCS code pages.
 */
P32_TEST_DECL size_t p32_private_mbrtoc8_sbcs (char8_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c8rtomb_sbcs (char *, char8_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc16_sbcs (char16_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c16rtomb_sbcs (char *, char16_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc32_sbcs (char32_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c32rtomb_sbcs (char *, char32_t, mbstate_t *, Charset *);

/**
 * Internal versions which operate on DBCS code pages.
 */
P32_TEST_DECL size_t p32_private_mbrtoc8_dbcs (char8_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c8rtomb_dbcs (char *, char8_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc16_dbcs (char16_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c16rtomb_dbcs (char *, char16_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_mbrtoc32_dbcs (char32_t *, const char *, size_t, mbstate_t *, Charset *);
P32_TEST_DECL size_t p32_private_c32rtomb_dbcs (char *, char32_t, mbstate_t *, Charset *);

/**
 * Store locale-specific versions of uchar.h functions in `locale->Functions`.
 */
void p32_uchar_functions (locale_t locale);

#endif /* LIBPOSIX32_UCHAR_INTERNAL_H_INCLUDED */
