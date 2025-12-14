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

#ifndef LIBPOSIX32_UCHAR_INTERNAL_H_INCLUDED
#define LIBPOSIX32_UCHAR_INTERNAL_H_INCLUDED

#include "p32_uchar.h"
#include "p32_wchar.h"

#include "locale-internal.h"

#ifdef P32_CRT_BUILD
#undef mbrtoc8
#undef mbrtoc8_l
#undef c8rtomb
#undef c8rtomb_l
#undef mbrtoc16
#undef mbrtoc16_l
#undef c16rtomb
#undef c16rtomb_l
#undef mbrtoc32
#undef mbrtoc32_l
#undef c32rtomb
#undef c32rtomb_l
#endif

size_t p32_private_mbrtoc8_posix (char8_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c8rtomb_posix (char *, char8_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc16_posix (char16_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c16rtomb_posix (char *, char16_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc32_posix (char32_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c32rtomb_posix (char *, char32_t, mbstate_t *, Charset *);

size_t p32_private_mbrtoc8_ascii (char8_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c8rtomb_ascii (char *, char8_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc16_ascii (char16_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c16rtomb_ascii (char *, char16_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc32_ascii (char32_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c32rtomb_ascii (char *, char32_t, mbstate_t *, Charset *);

size_t p32_private_mbrtoc8_sbcs (char8_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c8rtomb_sbcs (char *, char8_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc16_sbcs (char16_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c16rtomb_sbcs (char *, char16_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc32_sbcs (char32_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c32rtomb_sbcs (char *, char32_t, mbstate_t *, Charset *);

size_t p32_private_mbrtoc8_dbcs (char8_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c8rtomb_dbcs (char *, char8_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc16_dbcs (char16_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c16rtomb_dbcs (char *, char16_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc32_dbcs (char32_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c32rtomb_dbcs (char *, char32_t, mbstate_t *, Charset *);

size_t p32_private_mbrtoc8_utf8 (char8_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c8rtomb_utf8 (char *, char8_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc16_utf8 (char16_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c16rtomb_utf8 (char *, char16_t, mbstate_t *, Charset *);
size_t p32_private_mbrtoc32_utf8 (char32_t *, const char *, size_t, mbstate_t *, Charset *);
size_t p32_private_c32rtomb_utf8 (char *, char32_t, mbstate_t *, Charset *);

/**
 * Store locale-specific versions of uchar.h functions in `locale->Functions`.
 */
void p32_uchar_functions (locale_t locale);

#endif /* LIBPOSIX32_UCHAR_INTERNAL_H_INCLUDED */
