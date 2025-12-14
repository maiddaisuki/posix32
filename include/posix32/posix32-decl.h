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

#ifndef LIBPOSIX32_POSIX32_DECL_H_INCLUDED
#define LIBPOSIX32_POSIX32_DECL_H_INCLUDED

/**
 * Function attributes.
 **/

#if defined(__GNUC__) || defined(__clang__)
#define P32_RESTRICT      __restrict__
#define P32_INLINE        __inline__
#define P32_ALWAYS_INLINE __inline__ __attribute__ ((__always_inline__))
#define P32_NORETURN      __attribute__ ((__noreturn__))
#elif defined(_MSC_VER)
#define P32_RESTRICT      __restrict
#define P32_INLINE        __inline
#define P32_ALWAYS_INLINE __inline __forceinline
#define P32_NORETURN      __declspec (noreturn)
#endif

#ifndef P32_DECL
#ifdef LIBPOSIX32_STATIC
#define P32_DECL
#else
#define P32_DECL __declspec (dllimport)
#endif
#endif

#if defined(LIBPOSIX32_STATIC) || defined(__cplusplus)
#define P32_FUNC_DECL(...) P32_DECL
#else
#define P32_FUNC_DECL(...) __VA_ARGS__
#endif

#ifndef P32_CRT_DECL
#define P32_CRT_DECL(...) P32_FUNC_DECL (__VA_ARGS__)
#endif

#ifndef P32_INLINE_DECL
#if defined(__GNUC__) || defined(__clang__)
#define P32_INLINE_DECL extern P32_INLINE __attribute__ ((__gnu_inline__))
#elif defined(_MSC_VER)
#define P32_INLINE_DECL P32_DECL P32_INLINE
#endif
#endif

#ifndef P32_DEPRECATED
#if defined(__GNUC__) || defined(__clang__)
#define P32_DEPRECATED(msg) __attribute__ ((__deprecated__ (msg)))
#elif defined(_MSC_VER)
#define P32_DEPRECATED(msg) __declspec (deprecated (msg))
#else
#define P32_DEPRECATED(msg)
#endif
#endif

#if P32_WANT_POSIX2001
#define P32_DEPRECATE_POSIX2001(func) P32_DEPRECATED (#func " has been deprecated or removed in POSIX.1-2001")
#else
#define P32_DEPRECATE_POSIX2001(func)
#endif

#if P32_WANT_POSIX2008
#define P32_DEPRECATE_POSIX2008(func) P32_DEPRECATED (#func " has been deprecated or removed in POSIX.1-2008")
#else
#define P32_DEPRECATE_POSIX2008(func)
#endif

#if P32_WANT_POSIX2024
#define P32_DEPRECATE_POSIX2024(func) P32_DEPRECATED (#func " has been deprecated or removed in POSIX.1-2024")
#else
#define P32_DEPRECATE_POSIX2024(func)
#endif

/**
 * Whether to define macros such as
 *
 * 	#define funcname p32_funcname
 *
 * This approach is only used for C compilation and only for static library.
 *
 * This approach cannot be used with C++ since such macros may and likely will
 * interfere with user's class members.
 */
#ifndef P32_REDIRECT
#if defined(LIBPOSIX32_STATIC) && !defined(__cplusplus)
#define P32_REDIRECT 1
#else
#define P32_REDIRECT 0
#endif
#endif

/**
 * How to declare replacements for CRT functions.
 *
 * Functions are declared with their standard names only for C compilation and
 * only for shared library. Since the library exports both names, this will
 * result in linking against posix32's functions while using standard names.
 *
 * FIXME: since functions are declared with their standard names, gcc and clang
 * 	may replace them with built-in functions, which may result in behavior
 * 	which differs from posix32's implementaion.
 *
 * Otherwise, they are declared with posix32 names.
 */
#ifndef P32_FUNCNAME_STD
#if defined(LIBPOSIX32_STATIC) || defined(__cplusplus)
#define P32_FUNCNAME_STD(name) p32_##name
#else
#define P32_FUNCNAME_STD(name) name
#endif
#endif

/**
 * How to declare POSIX functions provided by posix32 library.
 */
#ifndef P32_FUNCNAME
#if defined(LIBPOSIX32_STATIC)
#define P32_FUNCNAME(name) p32_##name
#else
#define P32_FUNCNAME(name) name
#endif
#endif

/**
 * FIXME: Some function declarations in mingw-w64's header files omit
 *  __declspec(dllimport) which causes clang to diagnose lots of
 *  -Wdll-attribute-on-redeclaration.
 */
#if defined(__clang__) && !defined(_MSC_VER)
#pragma clang diagnostic ignored "-Wdll-attribute-on-redeclaration"
#endif

/**
 * Our and Microsoft's header files disagree on usage of `restrict` in
 * declarations of some functions.
 *
 * This is harmless, but result in many annoying C4028 warnings.
 */
#ifdef _MSC_VER
#pragma warning(disable : 4028)
#endif

#endif /* LIBPOSIX32_POSIX32_DECL_H_INCLUDED */
