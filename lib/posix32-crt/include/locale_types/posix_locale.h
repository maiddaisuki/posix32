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

#ifndef LIBPOSIX32_POSIX_LOCALE_H_INCLUDED
#define LIBPOSIX32_POSIX_LOCALE_H_INCLUDED

#include "locale_types/windows_locale.h"

/**
 * Use `nl_item` constants used by `nl_langinfo` instead of `LOCALE_*` and
 * `CAL_*` constants used by Windows NLS APIs.
 *
 * Any constant defined by POSIX can be used, except the following ones:
 *
 *  - CODESET
 *  - ERA
 *
 * Both `p32_posix_get_locale_info` and `p32_posix_get_calendar_info` support
 * this flag, and their behavior is the same when this flag is used.
 */
#define P32_POSIX_LOCALE_INFO_NL_ITEM  (1)
/**
 * Locale information for "POSIX" locale is being used as a fallback.
 *
 * Does not store locale information in `*request->OutputW` and only store
 * converted locale information in `*request->OutputA`.
 */
#define P32_POSIX_LOCALE_INFO_FALLBACK (1 << 1)

/**
 * This function is similar to `p32_winlocale_get_locale_info`, except it
 * retrieves locale information for "POSIX" locale.
 *
 * The `flags` can contain any combination of `P32_POSIX_LOCALE_INFO_*` flags.
 *
 * This function returns `0` on success and `-1` on failure.
 *
 * This functions returns `1` if information for `request->Info` is not
 * available in "POSIX" locale.
 */
P32_TEST_DECL int p32_posix_get_locale_info (LocaleInfoRequest *request, uintptr_t heap, int flags);

/**
 * This function is similar to `p32_winlocale_get_calendar_info`, except it
 * retrieves locale information for "POSIX" locale.
 *
 * The `flags` can contain any combination of `P32_POSIX_LOCALE_INFO_*` flags.
 *
 * This function returns `0` on success and `-1` on failure.
 *
 * This functions returns `1` if information for `request->Info` is not
 * available in "POSIX" locale.
 */
P32_TEST_DECL int p32_posix_get_calendar_info (CalendarInfoRequest *request, uintptr_t heap, int flags);

#endif /* LIBPOSIX32_POSIX_LOCALE_H_INCLUDED */
