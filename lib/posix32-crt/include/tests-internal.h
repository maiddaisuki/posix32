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

#ifndef LIBPOSIX32_TESTS_INTERNAL_H_INCLUDED
#define LIBPOSIX32_TESTS_INTERNAL_H_INCLUDED

#include "crt-internal.h"
#include "ctype-internal.h"
#include "locale-internal.h"
#include "string-internal.h"
#include "time-internal.h"
#include "uchar-internal.h"
#include "wchar-internal.h"

/**
 * Initialize a test.
 */
P32_TEST_DECL void p32_test_init (void);

/**
 * Get CRT's handle. This handle can be passed to `GetProcAddress`.
 *
 * This function returns `0` if library was built against static CRT.
 */
P32_TEST_DECL uintptr_t p32_crt_handle (void);

/**
 * The following flags can be passed to `p32_locale_test_func2` and
 * `p32_locale_test_func4` functions to modify their behavior.
 */

/**
 * Set CRT's global locale before calling callback function.
 * If locale cannot be set, the callback is not called.
 */
#define P32_LOCALE_TEST_SETLOCALE (1)
/**
 * Affects behavior of `P32_LOCALE_TEST_SETLOCALE`: set CRT's global locale
 * only if locale's code page is supported by CRT.
 *
 * We emulate UTF-8 locales for CRTs which do not support them natively.
 */
#define P32_LOCALE_TEST_USABLE_CP (1 << 1)
/**
 * Call callback only once per each locale.
 */
#define P32_LOCALE_TEST_ONCE      (1 << 2)
/**
 * Specific to `p32_locale_test_func2`: unless locale requires Unicode,
 * attempt to create locale with it's default ANSI code page.
 */
#define P32_LOCALE_TEST_ACP       (1 << 8)
/**
 * Specific to `p32_locale_test_func2`: unless locale requires Unicode,
 * attempt to create locale with it's default OEM code page.
 */
#define P32_LOCALE_TEST_OCP       (1 << 9)
/**
 * Specific to `p32_locale_test_func2`: attempt to create locale using code
 * page 65001 (UTF-8).
 */
#define P32_LOCALE_TEST_UTF8      (1 << 10)

#define P32_LOCALE_TEST_SUPPORTED (P32_LOCALE_TEST_SETLOCALE | P32_LOCALE_TEST_USABLE_CP)
#define P32_LOCALE_TEST_ALL       (P32_LOCALE_TEST_ACP | P32_LOCALE_TEST_OCP | P32_LOCALE_TEST_UTF8)

typedef bool (__cdecl *LocaleCallback1) (Locale *);
typedef bool (__cdecl *LocaleCallback2) (locale_t, const wchar_t *);
typedef bool (__cdecl *LocaleCallback3) (uint32_t);
typedef bool (__cdecl *LocaleCallback4) (locale_t, const wchar_t *);

/**
 * This functions calls `callback` once for each locale supported by
 * the operating system, as long as `callback` returns `true`.
 *
 * The first argument passed to `callback` is `Locale` object which can be
 * used with `p32_winlocale_getinfo`.
 */
P32_TEST_DECL void p32_locale_test_func1 (LocaleCallback1 callback);

/**
 * This functions calls `callback` once for each locale supported by
 * the operating system, as long as `callback` returns `true`.
 *
 * The first argument passed to `callback` is `locale_t` object created for
 * that locale.
 */
P32_TEST_DECL void p32_locale_test_func2 (LocaleCallback2 callback, int flags);

/**
 * This functions calls `callback` once for each code page supported by
 * the operating system, as long as `callback` returns `true`.
 *
 * The first argument passed to `callback` is code page as an `uint32_t` value.
 */
P32_TEST_DECL void p32_locale_test_func3 (LocaleCallback3 callback);

/**
 * This functions calls `callback` once for each code page supported by
 * the operating system, as long as `callback` returns `true`.
 *
 * The first argument passed to `callback` is `locale_t` object created with
 * that code page.
 */
P32_TEST_DECL void p32_locale_test_func4 (LocaleCallback4 callback, int flags);

#endif /* LIBPOSIX32_TESTS_INTERNAL_H_INCLUDED */
