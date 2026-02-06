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

#ifndef LIBPOSIX32_POSIX32_H_INCLUDED
#define LIBPOSIX32_POSIX32_H_INCLUDED

#ifdef LIBPOSIX32_NO_STATIC
#error This installation of posix32 library was configured without static library
#endif

/**
 * The following macros are used to check which CRT we compile against.
 */
#if defined(_MSC_VER)
#define P32_CRT_MSVC(msc_ver)           (_MSC_VER >= msc_ver)
#define P32_CRT_MINGW64(msvcrt_version) (0)
#elif defined(__MSVCRT_VERSION__)
#define P32_CRT_MSVC(msc_ver)           (0)
#define P32_CRT_MINGW64(msvcrt_version) (__MSVCRT_VERSION__ >= msvcrt_version)
#else
#error Unknown compilation environment: only mingw-w64 and MSVC are supported
#endif

/**
 * P32_CRT:
 *
 * This macro is defined to one of the values below to indicate which CRT
 * we are compiling against.
 *
 * This macro allows to have conditional code like
 *
 *  #if P32_CRT >= P32_MSVCR80
 *  ...
 *  #else
 *  ...
 *  #endif
 *
 * eliminating the need to test for toolchain-specific macros.
 */

#define P32_CRTDLL   0 /* crtdll.dll                     */
#define P32_MSVCRT10 1 /* msvcrt10.dll: Visual C++ 1.0   */
#define P32_MSVCRT20 2 /* msvcrt20.dll: Visual C++ 2.0   */
/**
 * There is no practical difference between `P32_MSVCRT40` and `P32_MSVCRT41`.
 */
#define P32_MSVCRT40 3 /* msvcrt40.dll: Visual C++ 4.0   */
#define P32_MSVCRT41 4 /* msvcrt40.dll: Visual C++ 4.1   */
#define P32_MSVCRT42 5 /* msvcrt.dll:   Visual C++ 4.2   */
#define P32_MSVCRT50 6 /* msvcrt.dll:   Visual C++ 5.0   */
#define P32_MSVCRT60 7 /* msvcrt.dll:   Visual C++ 6.0   */
/**
 * There is no practical difference between `P32_MSVCRT70` and `P32_MSVCRT71`.
 */
#define P32_MSVCR70  8  /* msvcr70.dll:  Visual C++ 7.0   */
#define P32_MSVCR71  9  /* msvcr71.dll:  Visual C++ 7.1   */
#define P32_MSVCR80  10 /* msvcr80.dll:  Visual C++ 8.0   */
#define P32_MSVCR90  11 /* msvcr90.dll:  Visual C++ 9.0   */
#define P32_MSVCR100 12 /* msvcr100.dll: Visual C++ 10.0  */
#define P32_MSVCR110 13 /* msvcr110.dll: Visual C++ 11.0  */
#define P32_MSVCR120 14 /* msvcr120.dll: Visual C++ 12.0  */
#define P32_UCRT     15 /* ucrtbase.dll                   */

#if P32_CRT_MSVC(1900) || P32_CRT_MINGW64(0x0E00)
#define P32_CRT P32_UCRT
#elif P32_CRT_MSVC(1800) || P32_CRT_MINGW64(0x0C00)
#define P32_CRT P32_MSVCR120
#elif P32_CRT_MSVC(1700) || P32_CRT_MINGW64(0x0B00)
#define P32_CRT P32_MSVCR110
#elif P32_CRT_MSVC(1600) || P32_CRT_MINGW64(0x0A00)
#define P32_CRT P32_MSVCR100
#elif P32_CRT_MSVC(1500) || P32_CRT_MINGW64(0x0900)
#define P32_CRT P32_MSVCR90
#elif P32_CRT_MSVC(1400) || P32_CRT_MINGW64(0x0800)
#define P32_CRT P32_MSVCR80
#elif P32_CRT_MSVC(1310) || P32_CRT_MINGW64(0x0701)
#define P32_CRT P32_MSVCR71
#elif P32_CRT_MSVC(1300) || P32_CRT_MINGW64(0x0700)
#define P32_CRT P32_MSVCR70
#elif P32_CRT_MSVC(1200) || P32_CRT_MINGW64(0x0600)
#define P32_CRT P32_MSVCRT60
#elif P32_CRT_MSVC(1100) || P32_CRT_MINGW64(0x0600)
#define P32_CRT P32_MSVCRT50
#elif P32_CRT_MSVC(1020) || P32_CRT_MINGW64(0x0600)
#define P32_CRT P32_MSVCRT42
#elif P32_CRT_MSVC(1010) || P32_CRT_MINGW64(0x0400)
#define P32_CRT P32_MSVCRT41
#elif P32_CRT_MSVC(1000) || P32_CRT_MINGW64(0x0400)
#define P32_CRT P32_MSVCRT40
#elif P32_CRT_MSVC(900) || P32_CRT_MINGW64(0x0200)
#define P32_CRT P32_MSVCRT20
#elif P32_CRT_MSVC(800) || P32_CRT_MINGW64(0x0100)
#define P32_CRT P32_MSVCRT10
#else
#define P32_CRT P32_CRTDLL
#endif

/**
 * Evaluates to non-zero if compiling for msvcrt.dll.
 */
#define P32_MSVCRT (P32_CRT >= P32_MSVCRT42 && P32_CRT <= P32_MSVCRT60)

/**
 * P32_ARCH: target architecture.
 *
 * Defined to one of macros below.
 */

#define P32_ARCH_X86   (1)
#define P32_ARCH_X64   (1 << 1)
#define P32_ARCH_ARM32 (1 << 2)
#define P32_ARCH_ARM64 (1 << 3)

#if defined(_M_IX86)
#define P32_ARCH P32_ARCH_X86
#elif defined(_M_X64) || defined(_M_ARM64EC)
#define P32_ARCH P32_ARCH_X64
#elif defined(_M_ARM)
#define P32_ARCH P32_ARCH_ARM32
#elif defined(_M_ARM64)
#define P32_ARCH P32_ARCH_ARM64
#else
#error Unknown architecture
#endif

/**
 * Evaluates to non-zero if compiling for `arch`, which can be any combination
 * of `P32_ARCH_*` macros defined above.
 */
#define P32_IS_ARCH(arch) (P32_ARCH & (arch))

/**
 * Handle _FILE_OFFSET_BITS and _TIME_BITS.
 **/

#ifdef _FILE_OFFSET_BITS
#if _FILE_OFFSET_BITS != 64 && _FILE_OFFSET_BITS != 32
#error Value of _FILE_OFFSET_BITS must be 32 or 64
#endif
#endif

#ifdef _TIME_BITS
#if _TIME_BITS != 64 && _TIME_BITS != 32
#error Value of _TIME_BITS must be 32 or 64
#endif
#endif

#ifdef _WIN64

#if defined(_USE_32BIT_TIME_T)
#error _USE_32BIT_TIME_T is not allowed when building for 64-bit windows
#endif
#if defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS != 64
#error _FILE_OFFSET_BITS != 64 is not allowed when building for 64-bit windows
#endif
#if defined(_TIME_BITS) && _TIME_BITS != 64
#error _TIME_BITS != 64 is not allowed when building for 64-bit windows
#endif

#define P32_FILE_BITS 64
#define P32_TIME_BITS 64

#else /* _WIN32 */

#if defined(_USE_32BIT_TIME_T) && defined(_TIME_BITS) && _TIME_BITS != 32
#error _USE_32BIT_TIME_T && _TIME_BITS != 32 are conflicting
#endif

#if defined(_FILE_BITS) && _TIME_BITS == 32 && !defined(_USE_32BIT_TIME_T)
#define _USE_32BIT_TIME_T
#endif

#ifdef _USE_32BIT_TIME_T
#define P32_TIME_BITS 32
#else
#define P32_TIME_BITS 64
#endif

#ifdef _FILE_OFFSET_BITS
#define P32_FILE_BITS _FILE_OFFSET_BITS
#else
#define P32_FILE_BITS P32_TIME_BITS
#endif

#endif /* _WIN64 */

/**
 * Handle _POSIX_C_SOURCE and _XOPEN_SOURCE.
 **/

#if defined(_XOPEN_SOURCE)
#define P32_WANT_SUSv5 (_XOPEN_SOURCE >= 800)
#define P32_WANT_SUSv4 (_XOPEN_SOURCE >= 700)
#define P32_WANT_SUSv3 (_XOPEN_SOURCE >= 600)
#define P32_WANT_SUSv2 (_XOPEN_SOURCE >= 500)
#if _XOPEN_SOURCE >= 500 || defined(_XOPEN_EXTENDED_SOURCE)
#define P32_WANT_SUSv1 (1)
#endif
#define P32_WANT_XPG (1)
#else /* !_XOPEN_SOURCE */
#define P32_WANT_SUSv4 (0)
#define P32_WANT_SUSv3 (0)
#define P32_WANT_SUSv2 (0)
#define P32_WANT_SUSv1 (0)
#define P32_WANT_XPG   (0)
#endif /* _XOPEN_SOURCE */

#if defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE)
#if P32_WANT_SUSv5
#define _POSIX_C_SOURCE 202405L
#elif P32_WANT_SUSv4
#define _POSIX_C_SOURCE 200809L
#elif P32_WANT_SUSv3
#define _POSIX_C_SOURCE 200112L
#elif P32_WANT_SUSv2
#define _POSIX_C_SOURCE 199506L
#elif P32_WANT_SUSv1
#define _POSIX_C_SOURCE 2
#elif P32_WANT_XPG
#define _POSIX_C_SOURCE 1
#endif
#endif /* _XOPEN_SOURCE && !_POSIX_C_SOURCE */

#if defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 1
#endif

#if defined(_POSIX_C_SOURCE)
#define P32_WANT_POSIX2024 (_POSIX_C_SOURCE >= 202405L || P32_WANT_SUSv5)
#define P32_WANT_POSIX2008 (_POSIX_C_SOURCE >= 200809L || P32_WANT_SUSv4)
#define P32_WANT_POSIX2001 (_POSIX_C_SOURCE >= 200112L || P32_WANT_SUSv3)
#define P32_WANT_POSIX1995 (_POSIX_C_SOURCE >= 199506L || P32_WANT_SUSv2)
#define P32_WANT_POSIX1993 (_POSIX_C_SOURCE >= 199309L || P32_WANT_SUSv2)
#define P32_WANT_POSIX2    (_POSIX_C_SOURCE >= 2 || P32_WANT_SUSv1)
#define P32_WANT_POSIX     (1)
#else /* !_POSIX_C_SOURCE */
#define P32_WANT_POSIX2008 (0)
#define P32_WANT_POSIX2001 (0)
#define P32_WANT_POSIX1995 (0)
#define P32_WANT_POSIX1993 (0)
#define P32_WANT_POSIX2    (0)
#define P32_WANT_POSIX     (0)
#endif

#include "posix32-decl.h"

#endif /* LIBPOSIX32_POSIX32_H_INCLUDED */
