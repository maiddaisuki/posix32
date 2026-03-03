/**
 * Copyright 2026 Kirill Makurin
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

/**
 * File Summary:
 *
 * This file provides generic implementation for conversion functions declared
 * in wchar.h:
 *
 *  - mbrlen
 *  - mbrtowc
 *  - mbsrtowcs
 *  - mbsnrtowcs
 *  - wcrtomb
 *  - wcsrtombs
 *  - wcsnrtombs
 *
 * Macro `P32_WCHAR_IMPL` must be defined to one of the following values
 * before including this source file to choose particular implemetation:
 *
 *  - P32_WCHAR_IMPL_ISO_8859_1
 *  - P32_WCHAR_IMPL_ASCII
 *  - P32_WCHAR_IMPL_SBCS
 *  - P32_WCHAR_IMPL_DBCS
 *  - P32_WCHAR_IMPL_UTF8
 */

#ifndef P32_WCHAR_IMPL
#error P32_WCHAR_IMPL must be defined before including wchar-impl.c
#endif

#define P32_WCHAR_IMPL_ISO_8859_1 0
#define P32_WCHAR_IMPL_ASCII      1
#define P32_WCHAR_IMPL_SBCS       2
#define P32_WCHAR_IMPL_DBCS       3
#define P32_WCHAR_IMPL_UTF8       4

#if P32_WCHAR_IMPL == P32_WCHAR_IMPL_ISO_8859_1
#define mbrtoc16   p32_private_mbrtoc16_posix
#define c16rtomb   p32_private_c16rtomb_posix
#define mbrtoc32   p32_private_mbrtoc32_posix
#define c32rtomb   p32_private_c32rtomb_posix
#define mbrlen     p32_private_mbrlen_posix
#define mbrtowc    p32_private_mbrtowc_posix
#define mbsrtowcs  p32_private_mbsrtowcs_posix
#define mbsnrtowcs p32_private_mbsnrtowcs_posix
#define wcrtomb    p32_private_wcrtomb_posix
#define wcsrtombs  p32_private_wcsrtombs_posix
#define wcsnrtombs p32_private_wcsnrtombs_posix
#elif P32_WCHAR_IMPL == P32_WCHAR_IMPL_ASCII
#define mbrtoc16   p32_private_mbrtoc16_ascii
#define c16rtomb   p32_private_c16rtomb_ascii
#define mbrtoc32   p32_private_mbrtoc32_ascii
#define c32rtomb   p32_private_c32rtomb_ascii
#define mbrlen     p32_private_mbrlen_ascii
#define mbrtowc    p32_private_mbrtowc_ascii
#define mbsrtowcs  p32_private_mbsrtowcs_ascii
#define mbsnrtowcs p32_private_mbsnrtowcs_ascii
#define wcrtomb    p32_private_wcrtomb_ascii
#define wcsrtombs  p32_private_wcsrtombs_ascii
#define wcsnrtombs p32_private_wcsnrtombs_ascii
#elif P32_WCHAR_IMPL == P32_WCHAR_IMPL_SBCS
#define mbrtoc16   p32_private_mbrtoc16_sbcs
#define c16rtomb   p32_private_c16rtomb_sbcs
#define mbrtoc32   p32_private_mbrtoc32_sbcs
#define c32rtomb   p32_private_c32rtomb_sbcs
#define mbrlen     p32_private_mbrlen_sbcs
#define mbrtowc    p32_private_mbrtowc_sbcs
#define mbsrtowcs  p32_private_mbsrtowcs_sbcs
#define mbsnrtowcs p32_private_mbsnrtowcs_sbcs
#define wcrtomb    p32_private_wcrtomb_sbcs
#define wcsrtombs  p32_private_wcsrtombs_sbcs
#define wcsnrtombs p32_private_wcsnrtombs_sbcs
#elif P32_WCHAR_IMPL == P32_WCHAR_IMPL_DBCS
#define mbrtoc16   p32_private_mbrtoc16_dbcs
#define c16rtomb   p32_private_c16rtomb_dbcs
#define mbrtoc32   p32_private_mbrtoc32_dbcs
#define c32rtomb   p32_private_c32rtomb_dbcs
#define mbrlen     p32_private_mbrlen_dbcs
#define mbrtowc    p32_private_mbrtowc_dbcs
#define mbsrtowcs  p32_private_mbsrtowcs_dbcs
#define mbsnrtowcs p32_private_mbsnrtowcs_dbcs
#define wcrtomb    p32_private_wcrtomb_dbcs
#define wcsrtombs  p32_private_wcsrtombs_dbcs
#define wcsnrtombs p32_private_wcsnrtombs_dbcs
#elif P32_WCHAR_IMPL == P32_WCHAR_IMPL_UTF8
#define mbrtoc16   p32_private_mbrtoc16_utf8
#define c16rtomb   p32_private_c16rtomb_utf8
#define mbrtoc32   p32_private_mbrtoc32_utf8
#define c32rtomb   p32_private_c32rtomb_utf8
#define mbrlen     p32_private_mbrlen_utf8
#define mbrtowc    p32_private_mbrtowc_utf8
#define mbsrtowcs  p32_private_mbsrtowcs_utf8
#define mbsnrtowcs p32_private_mbsnrtowcs_utf8
#define wcrtomb    p32_private_wcrtomb_utf8
#define wcsrtombs  p32_private_wcsrtombs_utf8
#define wcsnrtombs p32_private_wcsnrtombs_utf8
#else
#error P32_WCHAR_IMPL is set to invalid value
#endif

#include "common/mbrlen.c"
#include "common/mbrtowc.c"
#include "common/mbsnrtowcs.c"
#include "common/mbsrtowcs.c"
#include "common/wcrtomb.c"
#include "common/wcsnrtombs.c"
#include "common/wcsrtombs.c"
