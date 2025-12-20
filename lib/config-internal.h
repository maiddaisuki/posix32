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

#ifndef LIBPOSIX32_CONFIG_INTERNAL_H_INCLUDED
#define LIBPOSIX32_CONFIG_INTERNAL_H_INCLUDED

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#endif

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 800
#endif

/**
 * When we are building against debug version of CRT, we want to make use of
 * debug functions declared in crtdbg.h.
 *
 * MSVC defines _DEBUG macro when compiling against debug version of CRT
 * (with /MDd or /MTd switches).
 *
 * With mingw-w64, we need to define _DEBUG ourselves.
 *
 * To help with this, config.h defines HAVE_CRTDBG_H if functions declared in
 * crtdbg.h are usable.
 */
#ifdef HAVE_CRTDBG_H
#ifndef _DEBUG
#define _DEBUG
#endif

/**
 * Instruct crtdbg.h to map memory allocation functions to their debug versions.
 */
#define _CRTDBG_MAP_ALLOC
#endif

/**
 * LIBPOSIX32_TEST:
 *
 * This macro is defined when we are building *test* version of the library
 * or compiling unit tests.
 *
 * P32_TEST_DECL:
 *
 * This macro is used to export library's internal functions so that they
 * can be used by unit tests.
 *
 * When we are building normal version of the library it expands to an empty
 * token, preventing internal function from being exported from the DLL.
 *
 * When we are buiding *test* version of the library, this will expand to
 * P32_DEFN, which will export library's internal functions from the DLL.
 */
#ifdef LIBPOSIX32_TEST
/**
 * Enable assertions.
 */
#ifdef NDEBUG
#undef NDEBUG
#endif
#endif /* LIBPOSIX32_TEST */

#define P32_DLLEXPORT __declspec (dllexport)
#define P32_DLLIMPORT __declspec (dllimport)

/**
 * DLL_EXPORT is defined by libtool when building DLL.
 **/
#ifdef DLL_EXPORT
#ifndef LIBPOSIX32_DLL
#define LIBPOSIX32_DLL
#endif
#endif

/**
 * P32_BUILD is defined when we're building a library.
 * It is not defined when we're compiling unit tests.
 *
 * Depending on whether we're building shared or static library:
 *
 *  P32_DECL: expands, respectively, to __declspec(dllimport) or empty token
 *  P32_DEFN: expands, respectively, to __declspec(dllexport) or empty token
 *
 * P32_DECL must be used in declaration of symbols in our own libraries we
 * want to link against.
 *
 * P32_DEFN must be used in declaration of symbols defined by the library
 * we are currently building.
 */
#ifdef P32_BUILD
/**
 * Prevent `posix32-decl.h` from defining these macros.
 * We need to have precise control over their values during the build.
 */
#define P32_REDIRECT           0
#define P32_FUNCNAME(name)     p32_##name
#define P32_FUNCNAME_STD(name) p32_##name

#ifdef LIBPOSIX32_DLL
#define P32_DECL P32_DLLIMPORT
#define P32_DEFN P32_DLLEXPORT
#else
#define P32_DECL
#define P32_DEFN
#endif

#ifdef P32_CRT_BUILD
#define P32_CRT_DECL(...) P32_DEFN
#endif

/**
 * This attribute macro is used to declare inline functions whose external
 * definiton is provided by the library.
 */
#if defined(__GNUC__) && !defined(__clang__)
#define P32_INLINE_DECL P32_INLINE
#elif defined(_MSC_VER) || defined(__clang__)
#define P32_INLINE_DECL P32_DEFN P32_INLINE
#endif

/**
 * This attribute macro is used to provide external definition for inline
 * functions which are exported from the DLL.
 */
#define P32_INLINE_DEFN extern P32_DEFN P32_INLINE

#ifdef LIBPOSIX32_TEST
#define P32_TEST_DECL P32_DEFN
#else
#define P32_TEST_DECL
#endif

#else /* !P32_BUILD */

#define P32_TEST_DECL P32_DECL

#endif /* P32_BUILD */

/**
 * This prevents `posix32-decl.h` from defining this macro.
 */
#define P32_DEPRECATED(msg)

#include <crtdbg.h>

#ifdef getcwd
#undef getcwd
#endif

#ifdef strdup
#undef strdup
#endif

#ifdef wcsdup
#undef wcsdup
#endif

#ifdef tempnam
#undef tempnam
#endif

#endif /* LIBPOSIX32_CONFIG_INTERNAL_H_INCLUDED */
