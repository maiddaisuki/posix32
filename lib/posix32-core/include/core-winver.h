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

#ifndef LIBPOSIX32_CORE_WINVER_H_INCLUDED
#define LIBPOSIX32_CORE_WINVER_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "posix32.h"

/**
 * Supported Windows platforms.
 */
typedef enum WindowsPlatform {
  WindowsPlatformInvalid = -1,
  /**
   * Windows NT.
   */
  WindowsPlatformNt,
  /**
   * Windows 9x.
   */
  WindowsPlatform9x,
} WindowsPlatform;

/**
 * Windows NT versions.
 */
typedef enum WindowsNtVersion {
  WindowsNtInvalid = -1,
  WindowsNt3_1,
  WindowsNt3_5,
  WindowsNt3_51,
  WindowsNt4,
  WindowsNt2000,
  WindowsNtXP,
  WindowsNtVista,
  WindowsNt7,
  WindowsNt8,
  WindowsNt8_1,
  WindowsNt10,
  WindowsNtLatest,
} WindowsNtVersion;

/**
 * Windows 9x versions.
 */
typedef enum Windows9xVersion {
  Windows9xInvalid = -1,
  Windows95,
  Windows98,
  WindowsMe,
} Windows9xVersion;

/**
 * Return `WindowsPlatform` value describing platform the code is running on.
 *
 * If library is configured without Win9x support, this function always
 * returns `WindowsPlatformNt`.
 */
P32_CORE_DECL WindowsPlatform p32_platform (void);

/**
 * If code is running on a Windows NT platform, returns some valid
 * `WindowsNtVersion` value.
 *
 * Otherwise, returns `WindowsNtInvalid`.
 */
P32_CORE_DECL WindowsNtVersion p32_winnt (void);

/**
 * If code is running on a Windows 9x platform, returns some valid
 * `Windows9xVersion` value.
 *
 * Otherwise, returns `Windows9xInvalid`.
 */
P32_CORE_DECL Windows9xVersion p32_win9x (void);

/**
 * Check Windows NT version the code is running on.
 *
 * The `winnt` must be one of `P32_WINNT_*` constants.
 * The `version` must be `WindowsNtVersion` value corresponing to `winnt`.
 *
 * Evaluates to non-zero if Windows NT version is greater or equal to
 * Windows NT version specified by `winnt` and `version`.
 */
#define P32_WINNT_COND(winnt, version) ((P32_WINNT >= winnt) || (p32_winnt () >= version))

/**
 * Same as `P32_WINNT_COND`, except that the check is performed only if
 * the code is running on Windows NT platform.
 *
 * This makes difference if library is configured with Win9x support;
 * `P32_WINNT_COND` may evaluate to true at compilation time, leading to
 * incorrect decisions at runtime if code is running on Windows 9x platform.
 */
#if P32_WIN9X
#define P32_WINNT_CHECK(winnt, version) (p32_platform () == WindowsPlatformNt && P32_WINNT_COND (winnt, version))
#else
#define P32_WINNT_CHECK(winnt, version) (P32_WINNT_COND (winnt, version))
#endif

/**
 * Check Windows 9x version the code is running on.
 *
 * The `win9x` must be one of `P32_WIN9X_*` constants.
 * The `version` must be `Windows9xVersion` value corresponing to `win9x`.
 *
 * Evaluates to non-zero if Windows 9x version is greater or equal to
 * Windows 9x version specified by `win9x` and `version`.
 *
 * If library is configured without Win9x support, it always evaluates to zero.
 */
#if P32_WIN9X
#define P32_WIN9X_COND(win9x, version) ((P32_WIN9X >= win9x) || (p32_win9x () >= version))
#else
#define P32_WIN9X_COND(win9x, version) (0)
#endif

/**
 * Same as `P32_WIN9X_COND`, except that the check is performed only if
 * the code is running on Windows 9x platform.
 *
 * This makes difference if library is configured with Win9x support;
 * `P32_WIN9X_COND` may evaluate to true at compilation time, leading to
 * incorrect decisions at runtime if code is running on Windows NT platform.
 *
 * If library is configured without Win9x support, it always evaluates to zero.
 */
#if P32_WIN9X
#define P32_WIN9X_CHECK(win9x, version) (p32_platform () == WindowsPlatform9x && P32_WIN9X_COND (win9x, version))
#else
#define P32_WIN9X_CHECK(win9x, version) (P32_WIN9X_COND (win9x, version))
#endif

/**
 * Check Windows version the code is running on.
 *
 * When configured with Win9x support, it evaluates either `P32_WINNT_COND`
 * or `P32_WIN9X_COND` depending on the platform the code is running on.
 *
 * Otherwise, this macro is equivalent to `P32_WINNT_CHECK`.
 */
#if P32_WIN9X
#define P32_PLATFORM_CHECK(winnt, vNt, win9x, v9x)                                                   \
  (p32_platform () == WindowsPlatformNt ? P32_WINNT_COND (winnt, vNt) : P32_WIN9X_COND (win9x, v9x))
#else
#define P32_PLATFORM_CHECK(winnt, vNt, win9x, v9x) (P32_WINNT_CHECK (winnt, vNt))
#endif

#endif /* LIBPOSIX32_CORE_WINVER_H_INCLUDED */
